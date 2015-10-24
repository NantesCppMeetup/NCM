#pragma once

#define BOOST_PP_VARIADICS 1 // force support for clang
#include <boost/preprocessor.hpp>
#include <iostream>

namespace max_variadic_macro {
	/*********************************************************************
	 *
	 * Cet exemple montre comment implanter une fonction variadique max(...)
	 * en utilisant le support des macros variadiques du préprocesseur.
	 *
	 *********************************************************************/

	// Pas de récursivité possible : la définition ci-dessous ne peut pas fonctionner
	#define MAX(a, b, ...) ((a) > (b) ? MAX((a), __VA_ARGS__) : MAX((b), __VA_ARGS__))
	#undef MAX

	// L'utilisation de la bibliothèque Boost.Preprocessor permet de s'en sortir avec un "overloading" répétitif de macros.
	// Le détail d'implantation des macros utilisées derrière est *très* complexe, impossible pour un non-expert.
	// Il y a aussi une limite sur le nombre d'arguments car l'implantation ne peut pas être entièrement variadique!
	#define MAX(...) BOOST_PP_SEQ_FOLD_LEFT(MAX_EACH, 0, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))
	#define MAX_EACH(s, x, y) ((x) > (y) ? (x) : (y)) // Attention aux parenthèses!

	void main() {
		std::cout << MAX(2, 1, 6, 4) << std::endl;
		// => OK, affiche "6"

		// Fonctionne bien avec des types hétérogènes
		std::cout << MAX(2, 1.0f, 6.5, 4ull) << std::endl;
		// => OK, affiche "6.5"

		{ // "Bien sûr", il ne faut pas utiliser d'expressions ...
			int i = 5;
			std::cout << MAX(2, 1, ++i, 4) << " (and 'i' is now " << i << ')' << std::endl;
			// => CAVEAT EMPTOR! ... affiche "9"
		}

		// L'appel ci-dessous, avec un plus grand nombre d'arguments, fait planter mon IDE! (CLion)
		//std::cout << MAX(2, 1, 6, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1) << std::endl;
	}
}
