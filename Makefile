all:
	g++ -c *.cpp -std=c++17 -g -Wall -m64 -I include && g++ *.o -o main -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
	