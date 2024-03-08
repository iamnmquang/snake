all:
	g++ -I include -L lib -o main main.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer
	