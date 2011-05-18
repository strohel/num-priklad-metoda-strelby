#include <stdio.h>

#include "hledani_korenu.h"


/**
 * Najde koren funkce double f(double x) pulenim intervalu.
 *
 * @param f funkce, jejiz koren se ma najit
 * @param data ukazatel na libovolna data, ktera se maji navic predat funkce fs
 * @param levy levy kraj intervalu, na kterem se ma hledat reseni
 * @param pravy pravy kraj intervalu, na kterem se hleda reseni
 * @param epsilon skutecny koren se bude od vraceneho lisit max o epsilon
 *
 * Musi platit f(levy)*f(pravy) < 0 !!!
 *
 * @return koren funkce f z intervalu [levy, pravy] nebo NaN, pokud se
 * stala chyba.
 */
double najdi_koren_pulenim(double (*f)(double, const void *), const void *data,
						   double levy, double pravy, double epsilon) {
	double f_levy = f(levy, data);
	double f_pravy = f(pravy, data);
	double prostredni = 1./0.; /* schvalne NaN */
	double f_prostredni;

	if(levy >= pravy) {
		fprintf(stderr, "najdi_koren_pulenim(): musi platit  levy < pravy!\n");
		return 1./0.; /* NaN */
	}
	if(f_levy == 0) {
		return levy; /* specialni pripady */
	}
	if(f_pravy == 0) {
		return pravy; /* ditto specialni */
	}
	if(f_levy*f_pravy >= 0) {
		fprintf(stderr, "najdi_koren_pulenim(): musi platit  f(levy)*f(pravy) <= 0!  "
		        "(f(levy) = %f; f(pravy) = %f)\n", f_levy, f_pravy);
		return 1./0.; /* NaN */
	}

	while((pravy - levy) >= epsilon) {
		prostredni = (pravy + levy)/2.;
		f_prostredni = f(prostredni, data);
		if(f_prostredni == 0) /* pro extemni specialni pripady */
			return f_prostredni;

		if(f_levy*f_prostredni < 0) {
			pravy = prostredni; /* pokud je koren v leve pulce intervalu */
			f_pravy = f_prostredni;
		} else if(f_pravy*f_prostredni < 0) {
			levy = prostredni; /* pokud je koren v prave casti intervalu */
			f_levy = f_levy;
		} else {
			fprintf(stderr, "najdi_koren_pulenim(): ztratili jsme koren (jak?)!\n");
			return 1./0.; /* NaN */
		}
	}
	return prostredni;
}
