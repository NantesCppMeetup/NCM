#pragma once

#include <string>

#if defined __GNUG__
	#include <cxxabi.h>
	#include <cstdlib>
#endif

/// fonction utilitaire permettant de "démangler" le nom d'un type pour afficher qqc de lisible par un être humain.
std::string demangle(std::string const &mangled_symbol) {
	#if defined __GNUG__
		int status;
		char *c(abi::__cxa_demangle(mangled_symbol.c_str(), 0, 0, &status));
		std::string s(c);
		std::free(c);
		return s;
	#else
		return mangled_symbol;
	#endif
}
