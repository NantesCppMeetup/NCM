#pragma once

#include <cstdarg>
#include <limits>
#include <iostream>

namespace max_c_style_variadic_function {
	/*********************************************************************
	 *
	 * Cet exemple montre comment implanter une fonction variadique max(...)
	 * en utilisant le système de fonctions variadiques hérité du langage C.
	 * Il montre aussi les dangers d'utilisation.
	 *
	 *********************************************************************/

	// Version double
	//
	// A noter qu'il faut passer le nombre d'arguments 'count' explicitement!
	// Le reste des arguments est juste déclaré par '...' et ne possède aucun type spéficié!
	double max_d(int count, ...) {
		double result = -std::numeric_limits<double>::max();
		va_list ap; // pointeur sur la pile
		va_start(ap, count);
		for (auto i = 0; i < count; ++i) {
			double arg = va_arg(ap, double); // récupère le prochain argument pointé par 'ap' sur la pile
			if (arg > result) result = arg;
		}
		va_end(ap);
		return result;
	}

	// Version float
	float max_f(int count, ...) {
		float result = -std::numeric_limits<float>::max();
		va_list ap;
		va_start(ap, count);
		for (auto i = 0; i < count; ++i) {
			#if 0 // Attention! Les types doivent être "self-promoted" : float ne fonctionne pas!
				float arg = va_arg(ap, float);
			#else
				float arg = va_arg(ap, double); // note : double et non float!
			#endif
			if (arg > result) result = arg;
		}
		va_end(ap);
		return result;
	}

	void main() {
		// Passage d'arguments corrects : 4 doubles
		std::cout << max_d(4, 2.0, 1.0, 6.0, 4.0) << std::endl;
		// => Affiche "6"

		// Passage d'arguments corrects : 4 floats
		std::cout << max_f(4, 2.0f, 1.0f, 6.0f, 4.0f) << std::endl;
		// => Affiche "6"

		// Passage de int au lieu de double!
		std::cout << max_d(4, 2, 1, 6, 4) << std::endl;
		// => Affiche n'importe quoi ou plante!

		// L'argument 'count' est inférieur au nombre d'arguments variadiques.
		std::cout << max_d(2, 2.0, 1.0, 6.0, 4.0) << std::endl;
		// => Affiche "2

		// L'argument 'count' est supérieur au nombre d'arguments variadiques!
		std::cout << max_d(1000, 2.0, 1.0, 6.0, 4.0) << std::endl;
		// => Affiche n'importe quoi ou plante!
	}
}
