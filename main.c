#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "poc_uloha.h"
#include "hledani_korenu.h"


/**
 * Vypise, jak se program pouziva.
 */
void vypis_pouziti(char **argv, const struct odr2 *odr)
{
	fprintf(stderr, "Pouziti: %s alfa beta\n"
		"\n"
		"Vyřeší ODR %s = 0 s okrajovými podmínkami.\n"
		"\n"
		"Parametry:\n"
		"    alfa: realne cislo; y(%f) = alfa\n"
		"    beta: realne cislo: y(%f) = beta\n",
		argv[0], odr->jmeno, odr->levy_kraj, odr->pravy_kraj);
}

/**
 * Precte cislo ze stringu a kdytak nahlasi chybu a ukonci program
 */
double precti_cislo(const struct odr2 *odr, char **argv, char *string, const char *nazev) {
	char *endptr; /* pro detekci spravneho precteni cisel */
	double ret;

	ret = strtod(string, &endptr);
	if(endptr == string) {
		fprintf(stderr, "Chyba pri cteni cisla %s.\n", nazev);
		vypis_pouziti(argv, odr);
		exit(1);
	}
	return ret;
}

/**
 * Implementace funkce f_1 ze zadani ODR
 */
double f_1_impl(double x, double y_1) {
	(void) x; /* toto je zde pouze abychom umlceli varovani kompilatoru */
	return y_1; /* y' = f_1(x, z) = z */
}

/**
 * Implementace funkce f_2 ze zadani ODR
 */
double f_2_impl(double x, double y, double y_1) {
	return -y_1 * tan(x) - y * pow(cos(x), 2.0);
}

/**
 * Funkce, ktera vraci jak daleko je reseni ODR od pozadovane hodnoty v pravem bode,
 * jako parametr bere derivaci hledane funkce v bode 0, jako data bere ukazatel na zadani
 * odr.
 */
double f_pro_newtona(double parametr, const void *data) {
	const struct odr2 *odr = (const struct odr2 *) data;
	int vysledek;

	vysledek = euler2(odr, parametr);
	if(vysledek != 0) {
		fprintf(stderr, "Chyba pri eulerovi.\n");
		exit(1);
	} else {
		/* vratime vzdalenost reseni v pravem bode od pozadovane hodnoty: */
		return odr->y[odr->N - 1] - odr->beta;
	}
}

/**
 * Vypise reseni odr ve formatu pro GnuPlot
 */
void vypis_reseni(const struct odr2 *odr) {
	unsigned int i;

	fprintf(stdout, "#    x		y\n");
	for(i = 0; i < odr->N; i++) {
		fprintf(stdout, "%f	%f\n", i*odr->h, odr->y[i]);
	}
}

int main(int argc, char **argv)
{
	double f_der_0; /* derivace f v bode 0 */
	double epsilon = 0.001; /* s jakou presnosti chceme urcit derivaci v bode 0 */
	struct odr2 odr;

	/* vyplnime pole struktury zadani ODR, ktera jsou ve vypsani pouziti */
	odr.jmeno = "y'' + y' * tan(x) - y * (cos(x))^2"; /* strink popisujici rovnici, bez "= 0" */
	odr.levy_kraj = 0; /* levy okraj intervalu, na kterem chceme resit rovnici */
	odr.pravy_kraj = 3.14159265358979323846 / 4.; /* pravy okraj intervalu */
	odr.h = 0.005; /* delka kroku pri diskretizaci */
	odr.N = lround((odr.pravy_kraj-odr.levy_kraj)/odr.h) + 1; /* nastavime spravny pocet kroku */

	if(argc != 3) {
		vypis_pouziti(argv, &odr);
		exit(1);
	}

	/* vyplnime zbytek strukturu popisujici zadani ODR: */
	odr.f_1 = f_1_impl; /* y' = f_1(x, z) */
	odr.f_2 = f_2_impl; /* y'' = z' = f_2(x, y, z) */
	/* pozadovana hodnota hledane funkce na levem kraji: */
	odr.alfa = precti_cislo(&odr, argv, argv[1], "alfa");
	/* pozadovana hodnota hledane funkce na pravem kraji: */
	odr.beta = precti_cislo(&odr, argv, argv[2], "beta");

	odr.y = malloc(odr.N * sizeof(double));
	if(odr.y == NULL) {
		fprintf(stderr, "Nepovedlo se naalokovat %d bytu pro reseni rovnice.\n", odr.N * sizeof(double));
		exit(1);
	}

	fprintf(stderr, "Hledam reseni ulohy %s = 0;  y \\in (%f, %f)\n",
			odr.jmeno, odr.levy_kraj, odr.pravy_kraj);
	fprintf(stderr, "  za podminek y(%f) = %f,  y(%f) = %f  a krokem diskretizace %f.\n",
			odr.levy_kraj, odr.alfa, odr.pravy_kraj, odr.beta, odr.h);

	f_der_0 = najdi_koren_pulenim(f_pro_newtona, (const void *) &odr,
								  -100., 100., epsilon); /* TODO: dynamicke okraje */
	fprintf(stderr, "Parametr nalezen: f'(%f) = %f  (+- %f)\n", odr.levy_kraj, f_der_0, epsilon);

	vypis_reseni(&odr);
	free(odr.y);

	return 0;
}
