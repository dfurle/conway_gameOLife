all: conway.cpp
	g++ -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio conway.cpp -o conway.exe