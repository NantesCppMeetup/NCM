/*
~~~ Templates variadiques en C++ ~~~
Depuis C++11, les templates ont acquis la capacité d'accepter un nombre variable de paramètres.
Cette évolution s'intègre bien mieux dans l'esprit du langage que les fonctions et macros variadiques héritées du C, notamment pour la sûreté du typage.
Nous verrons quelques uns des usages que la bibliothèque standard fait des templates variadiques, parfois de manière transparente pour l'utilisateur.
Nous ouvrirons ensuite notre horizon sur quelques possilibités d'utilisation simples que la bibliothèque standard n'exploite pas encore.
Au delà des sentiers battus, notre imagination trouvera-t-elle de nombreuses applications à cette nouveauté du langage ?
Est-ce un outil de tous les jours, ou préfèrera-t-on le considérer comme un outil bas niveau servant à mettre en oeuvre des fonctionalités plus haut niveau ?
~~~

http://lbrandy.com/blog/2013/03/variadic_templates/
https://channel9.msdn.com/Events/GoingNative/GoingNative-2012/Variadic-Templates-are-Funadic
http://cpptruths.blogspot.fr/2012/06/perfect-forwarding-of-parameter-groups.html
https://parasol.tamu.edu/~jarvi/papers/n1704.pdf
	The “library improvements” special provides features that can improve the usability, clarity of imple-
	mentation, and compile-time performance of libraries in the C++ standard and upcoming Library TR,
	along with many third-party libraries.  In particular, it permits succinct, efficient implementations of
		reference_wrapper	[1,	§	2.1],
		result_of		[1,	§	3.1],
		mem_fn			[1,	§	3.1],
		bind			[1,	§	3.3],
		function		[1,	§	3.4],
		tuple			[1,	§	6.1],
	along with third-party libraries such as MPL [5] or Lamba [9] and improvements to the
	standard containers. These primitives can eliminate much of the need for template metaprogramming and
	preprocessor metaprogramming for these libraries, reducing the burden on compilers. However, the imple-
	mentation effort required to support variadic templates will be considerably increased due to the need to
	support arbitrary expansion and matching patterns
http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2215.pdf

{
	https://www.youtube.com/watch?v=R1G3P5SRXCw
	https://github.com/CppCon/CppCon2015/blob/master/Presentations/Variadic%20Templates%20-%20Guidelines,%20Examples%20and%20Compile-time%20computation/Variadic%20Templates%20-%20Guidelines,%20Examples%20and%20Compile-time%20computation%20-%20Peter%20Sommerlad%20-%20CppCon%202015.pdf
}

http://stackoverflow.com/questions/30018387/is-boost-optimized-to-use-variadic-templates-from-c11

joke : 6-dots http://lbrandy.com/blog/2013/02/c11s-six-dots/

*/

//#include "max_c_style_variadic_function.hpp"
//#include "max_variadic_macro.hpp"
#include "max_variadic_template.hpp"
//#include "safe_printf.hpp"
//#include "emplace_back.hpp"
//#include "perfect_forwarding.hpp"

int main() {
	//max_c_style_variadic_function::main();
	//max_variadic_macro::main();
	max_variadic_template::main();
	//safe_printf::main();
	//emplace_back::main();
	//perfect_forwarding::main();
	return 0;
}
