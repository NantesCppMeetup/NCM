#pragma once

#include <vector>
#include <iostream>

namespace emplace_back {
	/*********************************************************************
	 *
	 * Cet exemple montre l'avantage de l'utilisation des templates variadiques
	 * exploitée par les fonctions "emplace" de la bibliothèque standard.
	 * Le mécanisme sous-jacent de perfect forwarding est détaillé dans le prochain exemple.
	 *
	 *********************************************************************/

	// On déclare un simple type de données qui va être stocké dans un std::vector.
	// On définit l'ensemble des constructeurs et opérateurs d'assignement standards
	// pour loguer lesquels sont appelés (copy, move ...).
	struct point {
			int x, y;

			point(int x, int y) : x(x), y(y) {
				std::cout << "ctor xy (" << x << ", " << y << ')' << std::endl;
			}

			point(const point &p) : x(p.x), y(p.y) {
				std::cout << "copy ctor (" << x << ", " << y << ')' << std::endl;
			}

			point(point &&p) : x(p.x), y(p.y) {
				std::cout << "move ctor (" << x << ", " << y << ')' << std::endl;
			}

			point &operator=(const point &p) {
				x = p.x;
				y = p.y;
				std::cout << "copy assignment (" << x << ", " << y << ')' << std::endl;
				return *this;
			}

			point &operator=(point &&p) {
				x = p.x;
				y = p.y;
				std::cout << "move assignment (" << x << ", " << y << ')' << std::endl;
				return *this;
			}

			~point() {
				std::cout << "dtor (" << x << ", " << y << ')' << std::endl;
			}
	};

	void main() {
		std::vector<point> v;
		v.reserve(3); // non nécessaire, mais permet de ne pas polluer les logs avec des réallocations de mémoire

		std::cout << "~~~~~~~~~0~~~~~~~~~~~" << std::endl;
		point p{1, 2}; // durée de vie jusqu'à la fin du bloc
		v.push_back(p); // inefficient : création d'une copie de l'objet dans le vecteur
		std::cout << "~~~~~~~~/0~~~~~~~~~~~" << std::endl;

		std::cout << "~~~~~~~~~1~~~~~~~~~~~" << std::endl;
		v.push_back(point{3, 4}); // mieux : déplacement de l'objet temporaire dans le vecteur
		std::cout << "~~~~~~~~/1~~~~~~~~~~~" << std::endl;

		std::cout << "~~~~~~~~~2~~~~~~~~~~~" << std::endl;
		v.emplace_back(5, 6); // parfait : création directe de l'objet dans le vecteur, à son emplacement mémoire final
		// => Deux mécanismes sous-jacents sont utilisés :
		// 1) Placement new est utilisé pour initialiser l'objet directement à l'adresse mémoire finale allouée par le vecteur,
		// 2) Le perfect forwarding, qui utilise les templates variadiques, permet de transporter les arguments (5, 6)
		// jusqu'au constructeur de la class point.
		std::cout << "~~~~~~~~/2~~~~~~~~~~~" << std::endl;

		std::cout << "~~~~~~~~~3~~~~~~~~~~~" << std::endl;
		// Affichage du contenu du vecteur
		for (auto &element : v) {
			std::cout << '(' << element.x << ", " << element.y << ')' << std::endl;
		}
		std::cout << "~~~~~~~~/3~~~~~~~~~~~" << std::endl;
	}
}
