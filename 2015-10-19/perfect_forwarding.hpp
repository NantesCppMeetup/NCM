#pragma once

#include <vector>
#include <utility>
#include <cassert>
#include <iostream>

namespace perfect_forwarding {
	/*********************************************************************
	 *
	 * Cet exemple montre plus en détail la notion de perfect forwarding
	 *
	 *********************************************************************/

	// Ce template prend en argument un type de conteneur servant de type de base pour l'héritage,
	// Ce type de conteneur, comme std::vector, étant lui-même un template, on doit introduire l'utilisation
	// de "template de template" : le paramètre Base ci-dessous est un template variadique.
	// Le paquet de types T constitue les types passés au template variadique Base, c'est-à-dire qu'on utilise Base<T...>.
	//
	// Dans notre exemple, on redéfinit ensuite l'opérateur subsript [] pour y ajouter une vérification de la borne supérieure,
	// permettant ainsi de détecter les accès hors plage "légale".
	template<
			template<typename...> typename Base, // le type de base est lui-même un template variadique
			typename... T // ce paquet de type constitue les types passés au template Base : c-à-d Base<T...>
	>
	class checked_random_access_container : public Base<T...> { // héritage d'un template variadique!
		public:
			#if 0
				// Possibilité 1 : on hérite de tous les constructeurs de la classe de base.
				using Base<T...>::Base;
			#else
				// Possibilité 2 : perfect forwarding : on redéfinit l'ensemble des constructeurs
				// en déclarant un template qui forward les arguments au constructeur de la classe de base.
				// Ceci permettrait par exemple d'initialiser des données propres à la classe dérivée.
				// L'utilisation de '&&' combinée avec std::forward permet de gérer, sans avoir à se soucier des détails,
				// les subtilités des différentes natures de passage d'arguments (par valeur, par référence, par move-référence)
				// et donc des constructeurs "copy" et "move" entre autres.
				template<typename... Args>
				checked_random_access_container(Args &&... args) // A noter la déclaration des arguments avec '&&'
						// Appel au constructeur de la classe de base
						// A noter l'utilisation de std::forward, ainsi que le fait qu'on unpack une expression!
						: Base<T...>(std::forward<Args>(args)...) {
					std::cerr << "checked_random_access_container ctor" << std::endl;
				}

				// Pour supporter l'initialisation uniforme avec la syntaxe '{}', il tout de même nécessaire
				// de déclarer un constructeur qui prend explicitement un type std::initializer_list.
				// Autrement, le compilateur ne peut pas détecter qu'un tel constructeur existe.
				checked_random_access_container(std::initializer_list<typename Base<T...>::value_type> il)
				// Appel au constructeur de la classe de base
						: Base<T...>(il) {
					std::cerr << "checked_random_access_container ctor initializer_list" << std::endl;
				}

				// Apparement, la déclaration du template ci-dessous ne permet pas au compilateur
				// de détecter le support de std::initializer_list.
				// C'est uniquement la fonction non-template déclarée au dessus qui permet la détection!
				// Ce point reste donc à éclaircir. Est-ce un bug du compilateur ou est-ce normal ?
				template<typename... Args>
				checked_random_access_container(std::initializer_list<typename Base<T...>::value_type> il, Args &&... args)
						// Appel au constructeur de la classe de base
						: Base<T...>(il, std::forward<Args>(args)...) {
					std::cerr << "checked_random_access_container ctor initializer_list" << std::endl;
				}
			#endif

			// Dans notre exemple, on redéfinit l'opérateur subscript []
			// pour y ajouter une vérification de la borne supérieure et
			// ainsi détecter les accès hors plage "légale".
			typename Base<T...>::reference &operator[](std::size_t i) {
				std::cerr << "checked operator[]" << std::endl;
				if (this->size() <= i && i < this->capacity()) {
					// L'accès ne plante pas car on est dans la plage d'adresse réservée par le vecteur,
					// mais on est au delà de la taille => c'est un bug.
					std::cerr << "reading garbage" << std::endl;
					// Appel à l'opérateur de la classe de base
					return Base<T...>::operator[](i);
				}
				return this->at(i); // la fonction membre at() vérifie l'indice.
			}
	};

	// La technique ci-dessous permet de redéfinir localement "std::vector"
	// pour utiliser notre classe dérivée à la place.
	namespace std {
		// On réutilise tout le namespace racine ::std ...
		using namespace ::std;

		// ... sauf std::vector, qu'on remplace par notre classe dérivée.
		// On utilise la nouvelle syntaxe 'using' pour déclarer un alias de template.
		// C'est équivalent à un typedef, mais ça supporte la déclaration d'un template.
		template<typename... T>
		using vector = checked_random_access_container<vector, T...>;
	}

	void main() {
		{
			std::cerr << "~~~~~~~~~0~~~~~~~~~~~" << std::endl;
			std::vector<int> v0{2, 1, 6, 4}; // appel au ctor avec std::initializer_list
			std::cerr << v0[2] << std::endl;
			// => Affiche "6"
			std::cerr << "~~~~~~~~/0~~~~~~~~~~~" << std::endl;
		}

		{
			std::cerr << "~~~~~~~~~1~~~~~~~~~~~" << std::endl;
			std::vector<int> v1{{2, 1, 6, 4}}; // appel au ctor avec std::initializer_list, plus explicite avec les doubles {{}}
			std::cerr << v1[2] << std::endl;
			// => Affiche "6"
			std::cerr << "~~~~~~~~/1~~~~~~~~~~~" << std::endl;
		}

		{
			std::cerr << "~~~~~~~~~2~~~~~~~~~~~" << std::endl;
			std::vector<int> v2;
			v2.reserve(100); // allocation d'un espace mémoire pour 100 éléments, mémoire potentiellement non initialisée
			std::cerr << v2[99] << std::endl; // accès dans la plage d'adresse réservée, mais non initialisée!
			// => Affiche notre warning "reading garbage", puis un nombre quelconque!
			std::cerr << "~~~~~~~~/2~~~~~~~~~~~" << std::endl;
		}

		{
			std::cerr << "~~~~~~~~~3~~~~~~~~~~~" << std::endl;
			std::vector<int> v3;
			v3.reserve(100); // allocation d'un espace mémoire pour 100 éléments, mémoire potentiellement non initialisée
			std::cerr << v3[100] << std::endl; // accès en dehors de la plage d'adresse réservée!
			// => La fonction membre at() lève une exception std::out_of_range
			std::cerr << "~~~~~~~~/3#~~~~~~~~~~" << std::endl;
		}
	}
}
