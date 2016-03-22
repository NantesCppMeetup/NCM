import foo;
#include <iostream>

int main() {
	foo foo;
	std::cout << foo.compute(3) << std::endl;
}

#ifdef BAR
	#error bar was not imported as a module
#endif

