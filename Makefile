all: main

main: payoff.cpp Option.cpp FDM.cpp PDE.cpp main.cpp payoff.hpp Option.hpp FDM.hpp PDE.hpp 
	clang++ -o main payoff.cpp Option.cpp FDM.cpp PDE.cpp main.cpp  -std=c++11

.PHONY: run clean remake

run: main
	./main
	python2 plot.py

clean:
	rm -f main

remake:
	make clean
	make all
