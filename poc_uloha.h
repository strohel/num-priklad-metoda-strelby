/**
 * Struktura popisujici zadani obyc diferencialni rovnice druheho radu
 */
struct odr2 {
	char *jmeno;
	double (*f_1)(double x, double y_1); /* y' = f_1(x, z) */
	double (*f_2)(double x, double y, double y_1); /* y'' = z' = f_2(x, y, z) */
	double levy_kraj; /* levy okraj intervalu, na kterem chceme resit rovnici */
	double pravy_kraj; /* pravy okraj intervalu, na kterem chceme resit rovnici */
	double alfa; /* pozadovana hodnota hledane funkce na levem kraji */
	double beta; /* pozadovana hodnota hledane funkce na pravem kraji */
	double h; /* delka kroku pri diskretizaci */
	unsigned int N; /* pocet hodnot, musi byt nastaveno na (pravy_kraj-levy_kraj)/h + 1 */

	double *y; /* adresa pole, do ktereho se zapisuje reseni */
};

int euler2(const struct odr2 *odr, double y_1);
