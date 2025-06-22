SyobonAction:main.o loadg.o DxLib.o
	clang++ main.o loadg.o DxLib.o -o SyobonAction -L/opt/homebrew/lib -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lm
main.o:main.cpp
	clang++ -std=c++98 -Wall -O2 -I/opt/homebrew/include/SDL2 -c main.cpp -o main.o
loadg.o:loadg.cpp
	clang++ -std=c++98 -Wall -O2 -I/opt/homebrew/include/SDL2 -c loadg.cpp -o loadg.o
DxLib.o:DxLib.cpp
	clang++ -std=c++98 -Wall -O2 -I/opt/homebrew/include/SDL2 -c DxLib.cpp -o DxLib.o
