#include <stdio.h>

#include "poc_uloha.h"


/**
 * Funkce pro reseni pocatecni ulohy pro ODR 2. radu eulerovou metodou. Pokud uspeje, zapise do
 * pole s adresou odr->y reseni (pole doublu, kterych je odr->N). Pole odr->y uz musi byt
 * naalokovano! Pokud neuspeje, neni obsah pole odr->y definovan.
 *
 * @param odr ukazatel na zadani diferencialni rovnice a parametru reseni
 * @param y_1 pocatecni y' = y'(odr->levy_kraj)
 *
 * @return 0 pri uspechu - tedy kdyz je v odr->y reseni, jinak nenulovou hodnotu idikujici chybu.
 */
int euler2(const struct odr2 *odr, double y_1) {
	unsigned int i;
	double x = odr->levy_kraj; /* nastavme prvni x na levy kraj */
	double y = odr->alfa; /* hodnota fce v levem kraji */

	if(odr->y == NULL) {
		fprintf(stderr, "euler2(): odr->y uz musi byt naalokovano!\n");
		return -1;
	}

	odr->y[0] = y; /* prvni bod reseni je znamy, vyplnme ho */
	for(i = 1; i < odr->N; i++) { /* iterativni konstrukce reseni */
		odr->y[i] = y + odr->h * odr->f_1(x, y_1);
		y_1 += odr->h * odr->f_2(x, y, y_1);

		y = odr->y[i];
		x += odr->h;
	}
	return 0;
}
