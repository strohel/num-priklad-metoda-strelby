/**
 * Struktura popisujici zadani obyc diferencialni rovnice druheho radu
 */
struct odr2 {
	char *jmeno;
	double (*f_1)(double x, double y_1); // y' = f_1(x, z)
	double (*f_2)(double x, double y, double y_1); // y'' = f_1(x, y, z)
};

double *euler2(const struct odr2 *odr, double x, double y, double y_1, double h, unsigned int N);
