=========================
Variadic templates in C++
=========================

Depuis C++11, les templates ont acquis la capacité d'accepter un nombre variable de paramètres.
Est-ce un outil de tous les jours, ou préfèrera-t-on le considérer comme un outil bas niveau servant à mettre en oeuvre des fonctionalités plus haut niveau ?
Au delà des sentiers battus, notre imagination trouvera-t-elle de nombreuses applications à cette nouveauté du langage ?

Cette présentation met le focus sur des exemples concrets.

Elle commence par montrer comment cette évolution s'intègre bien mieux dans l'esprit du langage
que les fonctions et macros variadiques héritées du C, notamment pour la sûreté du typage.
La première série d'exemples décline les trois façons d'écrire une fonction variadique "max",
en commençant par les anciens mécanismes hérités du langage C, et en terminant avec du C++ moderne.

Nous ouvrons ensuite notre horizon sur une des possilibités d'utilisation simple que la bibliothèque standard n'exploite pas encore :
la construction d'une fonction de formatage style "printf" garantissant la sûreté du typage.

Pour terminer, nous abordons la notion de "perfect forwarding", compagnion essentiel des templates variadiques.
Le premier exemple montre un cas d'utilisation haut-niveau, transparent pour l'utilisateur,
à travers les fonctions optimisées "emplace" mis à disposition par la bibliothèque standard sur les conteneurs.
Nous terminons sur un exemple détaillé d'utilisation de "perfect forwarding" pour l'héritage de constructeurs.

Note : Les exemples ont été testés avec clang++ -std=c++1z (version 3.8.0)
