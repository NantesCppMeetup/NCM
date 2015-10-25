#pragma once

#include <iostream>
#include <string>
#include <typeinfo>
#include "demangle.hpp"

namespace max_variadic_template {
	/*********************************************************************
	 *
	 * Cet exemple montre comment implanter une fonction variadique max(...)
	 * en utilisant les templates variadiques du langage C++.
	 *
	 *********************************************************************/

	// Déclaration du cas terminal de la "récursivité" du template variadique où le nombre d'arguments est de 2
	// A noter que les cas terminaux doivent être déclarés en premier.
	template<typename T, typename U>
	auto max_impl(T t, U u) {
		return t > u ? t : u;
	};

	// Déclaration du cas variadique avec un premier argument simple 'First first',
	// suivi d'un nombre quelconque d'arguments supplémentaires 'Rest... rest'.
	// A noter que pour garder les choses au plus simple dans cet exemple, on passe tous les arguments par valeur.
	// Une version finalisée du code utiliserait le "perfect forwarding", notion qui sera expliquée plus tard.
	template<typename First, typename... Rest>
	auto max_impl(First first, Rest... rest) {
		return max_impl( // appel au cas terminal avec 2 arguments
				first,
				// appel "récursif" au template (avec un paramètre/argument de moins),
				// ou au cas terminal si 'rest' est de longueur 2
				max_impl(rest...)
		);
	};

	// Déclaration du cas particulier d'un nombre d'argument de 1
	template<typename T>
	auto max_impl(T t) {
		return t;
	}

	// Déclaration du point d'entrée. A noter qu'on peut très bien utiliser max_impl directement,
	// mais ce point d'entrée permet d'illustrer l'utilisation de l'opérateur "sizeof...".
	template<typename... T>
	auto max(T... args) {
		static_assert(sizeof...(args) > 0, "max needs at least 1 argument");
		return max_impl(args...);
	};

	void main() {
		#if 0
			// Utilisation incorrecte avec zéro argument
			std::cout << max() << std::endl;
			// => Ne compile pas : error: static_assert failed "max needs at least 1 argument"
		#endif

		#if 0
			// Utilisation incorrecte avec zéro argument (bypasse le point d'entrée et appel direct de l'implantation max_impl)
			std::cout << max_impl() << std::endl;
			// => Ne compile pas : error: no matching function for call to 'max_impl'
		#endif

		std::cout << max(2) << std::endl;
		// => Affiche "2"

		std::cout << max(2, 1) << std::endl;
		// => Affiche "2"

		std::cout << max(2, 1, 6) << std::endl;
		// => Affiche "6"

		// Fonctionne avec tout type qui implante l'opérateur >
		std::string s1{"def"}, s2{"abc"};
		std::cout << max(s1, s2) << std::endl;
		// => Affiche "def"

		// Attention tout de même! l'opérateur > ici compare les adresses
		std::cout << max(s1.c_str(), s2.c_str()) << std::endl;
		// => Affiche "abc" sur ma machine

		// Attention tout de même! l'opérateur > ici compare les adresses
		char a1[]{"abc"}, a2[]{"def"}, a3[]{"abc"};
		std::cout << max(a1, a2) << std::endl;
		// => Affiche "abc" sur ma machine
		std::cout << max(a2, a3) << std::endl;
		// => Affiche "def" sur ma machine

		// Fonctionne très bien avec des types hétérogènes
		std::cout << max(2, 1.0, 6.0f, 4ull) << std::endl;
		// => Affiche "6"

		// Voyons quel est le type du résultat ...
		auto result = max(2, 1.0, 6.0f, 4ull);
		std::cout << demangle(typeid(result).name()) << std::endl;
		// => Affiche "double"

		#if 0
			// Vérification statique (à la compilation) que les types sont compatibles
			std::cout << max(s1, 123) << std::endl;
			// => Ne compile pas : error: invalid operands to binary expression ('std::basic_string<char>' and 'int')
		#endif
	}
}
