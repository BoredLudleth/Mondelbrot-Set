all: link compile

link: main.cpp
	gcc -c main.cpp -o main.o

compile: main.o
	g++ main.o -o main -lsfml-graphics -lsfml-window -lsfml-system

clean:
	rm *.o *.out *.lst main
