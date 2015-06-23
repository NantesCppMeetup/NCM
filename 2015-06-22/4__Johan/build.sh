#! /bin/sh

set -x &&

cd "$(dirname "$0")" &&

cxx() {
	clang++ -fmodules -x objective-c++ -Dimport=@import -std=c++1z "$@"
} &&

cxx "$@" bar.cpp -I . -fmodule-name=bar -fPIC -shared -o libbar.so &&

cxx "$@" foo.cpp -I . -fmodule-name=foo -L . -l bar -fPIC -shared -o libfoo.so &&

cxx "$@" prog.cpp -I . -L . -Wl,-rpath-link=. -Wl,-rpath=\$ORIGIN -l foo -o prog &&

./prog

