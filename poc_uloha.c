#include <stdio.h>
#include <stdlib.h>

#include "poc_uloha.h"


/**
 * Funkce pro reseni pocatecni ulohy pro ODR 2. radu eulerovou metodou.
 *
 * @param odr ukazatel na zadani diferencialni rovnice
 * @param x pocatecni x
 * @param y pocatecni y = y(poc_x)
 * @param y_1 pocatecni y' = y'(poc_x)
 * @param h delka kroku
 * @param N pocet kroku
 *
 * @return pole N prvku typu double nebo NULL pri chybe. Volajici musi posleze zavolat free(),
 *         jinak to je memory leak. Jako prvni prvek obsahuje pocatecni y!
 */
double *euler2(const struct odr2 *odr, double x, double y, double y_1, double h, unsigned int N) {
	double *res; // posloupnost ypsilonu
	unsigned int i = 0;

	res = malloc(N * sizeof(double));
	if(res == NULL) {
		fprintf(stderr, "Nepovedlo se naalokovat %d bytu pro reseni rovnice.\n", N * sizeof(double));
		return NULL;
	}

	res[i] = y;
	for(i = 1; i < N; i++) {
		res[i] = y + h*odr->f_1(x, y_1);
		y_1 = y_1 + h*odr->f_2(x, y, y_1);

		y = res[i];
		x += h;
	}
	return res;
}
