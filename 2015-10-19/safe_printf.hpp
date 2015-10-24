#pragma once

#include <iostream>
#include <complex>

namespace safe_printf {
	/*********************************************************************
	 *
	 * Cet exemple montre que les templates variadiques nous donne la possibilité
	 * de définir une fonction printf garantissant la sûreté du typage des arguments.
	 * Ceci met en exergue le caractère hétérogène des types passés en paramètres.
	 *
	 *********************************************************************/

	// Déclaration du cas terminal de la "récursivité" du template variadique, avec zéro argument supplémentaire.
	// A noter que les cas terminaux doivent être déclarés en premier.
	void safe_printf(const char *format) {
		std::cout << format;
	}

	// Déclaration du cas variadique avec un premier argument simple 'First first',
	// suivi d'un nombre quelconque d'arguments supplémentaires 'Rest... rest'.
	// A noter que pour garder les choses au plus simple dans cet exemple, on passe tous les arguments par valeur.
	// Une version finalisée du code utiliserait le "perfect forwarding", notion qui sera expliquée plus tard.
	template<typename First, typename... Rest>
	void safe_printf(const char *format, First first, Rest... rest) {
		for (; *format != '\0'; ++format) {
			if (*format == '%') {
				// Le type de l'argument 'first' est connu du compilateur; il s'agit donc d'une surcharge classique.
				std::cout << first;

				// appel "récursif" au template (avec un paramètre/argument de moins),
				// ou au cas terminal si 'rest...' est vide
				safe_printf(format + 1, rest...);

				// Après l'appel récursif, tout a été traité, il faut sortir directement.
				return;
			}
			std::cout << *format;
		}
	}

	void main() {
		int i = 123;
		float f = 3.14f;
		std::complex<double> c{1.1, 2.2};

		std::cout << "int = " << i << ", float = " << f << ", complex = " << c << '\n';

		// A noter qu'on n'a pas besoin de préciser les types
		// dans la chaîne de formatage car le compilateur les connaît!
		safe_printf("int = %, float = %, complex = %\n", i, f, c);
	}
}
