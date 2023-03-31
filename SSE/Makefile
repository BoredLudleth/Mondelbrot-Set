all: link

link: main.cpp drawPic.cpp
	g++ main.cpp drawPic.cpp -o main -Ofast -mavx2 -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

clean:
	rm *.o *.out *.lst main
