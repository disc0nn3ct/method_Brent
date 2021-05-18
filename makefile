c=g++
all:
	@echo "================================================================="
	$(c) -g -O2 -std=c++11 -pthread -march=native main.cpp -o main.o -lntl -lgmp -lm 	
	./main.o
	@echo "================================================================="
clean: 
	rm -rf *.o