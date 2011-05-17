#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "poc_uloha.h"
#include "hledani_korenu.h"


 // toto musi byt v soucasne implementaci jako globalni promenne
struct odr2 odr; // struktura obsahujici zadani prikladu
double alfa, beta; // parametry hro hledani reseni
double h = 0.005; // velikost kroku
double epsilon = 0.001; // s jakou presnosti chceme urcit derivaci v bode 0
double delka = M_PI_4; // delka delka intervalu, na kterem budeme pocitat
unsigned int N; // pocet kroku


/**
 * Vypise, jak se program pouziva.
 */
void vypis_pouziti(char **argv)
{
	fprintf(stderr, "Pouziti: %s alfa beta\n"
		"\n"
		"Vyřeší ODR %s = 0 s okrajovými podmínkami.\n"
		"\n"
		"Parametry:\n"
		"    alfa: realne cislo; y(0) = alfa\n"
		"    beta: realne cislo: y(\\pi/4) = beta\n",
		argv[0], odr.jmeno);
}

/**
 * Precte cislo ze stringu a kdytak nahlasi chybu a ukonci program
 */
double precti_cislo(char **argv, char *string, const char *nazev) {
	char *endptr; // pro detekci spravneho precteni cisel
	double ret;

	ret = strtod(string, &endptr);
	if(endptr == string) {
		fprintf(stderr, "Chyba pri cteni cisla %s.\n", nazev);
		vypis_pouziti(argv);
		exit(1);
	}
	return ret;
}

/**
 * Implementace funkce f_1 ze zadani ODR
 */
double f_1_impl(double x, double y_1) {
	return y_1; // y' = f_1(x, z) = z
}

/**
 * Implementace funkce f_2 ze zadani ODR
 */
double f_2_impl(double x, double y, double y_1) {
	return -y_1 * tan(x) - y * pow(cos(x), 2.0);
}

/**
 * Funkce, ktera vraci jak daleko je reseni ODR od pozadovane hodnoty v pravem bode,
 * jako parametr bere derivaci hledane funkce v bode 0
 */
double f_pro_newtona(double parametr) {
	double *y; // posloupnost hodnot funkce y - reseni
	double y_vpravo;

	y = euler2(&odr, 0, alfa, parametr, h, N);
	if(y == NULL) {
		fprintf(stderr, "Chyba pri eulerovi\n");
		exit(1);
	} else {
		y_vpravo = y[N-1]; // kvuli volani free()
		free(y);
		return y_vpravo - beta; // vracime vzdalenost reseni v pravem bode od pozadovane hodnoty
	}
}

/**
 * Zkonstruuje reseni ODR pro zadany parametr a vypise ho ve formatu pro GnuPlot
 */
void vypis_reseni_pro_parametr(double parametr) {
	double *y; // posloupnost hodnot funkce y - reseni
	int i;

	y = euler2(&odr, 0, alfa, parametr, h, N);
	if(y == NULL) {
		fprintf(stderr, "Chyba pri eulerovi\n");
		exit(1);
	} else {
		fprintf(stdout, "#    x		y\n");
		for(i = 0; i < N; i++) {
			fprintf(stdout, "%f	%f\n", i*h, y[i]);
		}
		free(y);
	}
}

int main(int argc, char **argv)
{
	double f_der_0; // derivace f v bode 0

	// vyplnime strukturu popisujici zadani ODR:
	odr.jmeno = "y'' + y' * tan(x) - y * (cos(x))^2";
	odr.f_1 = f_1_impl;
	odr.f_2 = f_2_impl;
	N = lround(delka/h); // nastavime spravny pocet kroku

	if(argc != 3) {
		vypis_pouziti(argv);
		exit(1);
	}

	alfa = precti_cislo(argv, argv[1], "alfa");
	beta = precti_cislo(argv, argv[2], "beta");

	fprintf(stderr, "Hledam reseni ulohy %s = 0;  y \\in (0, \\pi/4)\n", odr.jmeno);
	fprintf(stderr, "  za podminek y(0) = %f,  y(\\pi/4) = %f\n", alfa, beta);

	f_der_0 = najdi_koren_pulenim(f_pro_newtona, -100., 100., epsilon); // TODO: dynamicke okraje
	fprintf(stderr, "Parametr nalezen: f'(0) = %f  (+- %f)\n", f_der_0, epsilon);

	vypis_reseni_pro_parametr(f_der_0);

	return 0;
}
