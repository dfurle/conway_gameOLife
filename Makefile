all: conway.cpp
	g++ -I/opt/local/include -L/opt/local/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio conway.cpp -o conway.exe
