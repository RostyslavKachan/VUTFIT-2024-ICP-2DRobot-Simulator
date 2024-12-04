.PHONY: all clean doxygen

all:
	cd src/Robots && cmake . && make

run:
	cd src/Robots && ./Robots

doxygen:
	cd doc && doxygen Doxyfile

pack: clean
	zip -r xjanec33-xkacha02.zip src/* doc/* examples/* Makefile README.txt uml.pdf

clean:
	cd src/Robots && rm -rf CMakeFiles CMakeCache.txt cmake_install.cmake Makefile Robots Robots_autogen
	cd doc && rm -rf html latex
