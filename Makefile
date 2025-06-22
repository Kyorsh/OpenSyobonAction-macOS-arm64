SyobonAction: SyobonAction_arm64 SyobonAction_x86_64
	lipo -create -output SyobonAction SyobonAction_arm64 SyobonAction_x86_64

SyobonAction_arm64: main.cpp loadg.cpp DxLib.cpp
	clang++ -std=c++98 -Wall -O2 -I/opt/homebrew/include/SDL2 -arch arm64 main.cpp loadg.cpp DxLib.cpp -o SyobonAction_arm64 -L/opt/homebrew/lib -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2_gfx -lm

SyobonAction_x86_64: main.cpp loadg.cpp DxLib.cpp
	clang++ -std=c++98 -Wall -O2 -I/usr/local/include/SDL2 -arch x86_64 main.cpp loadg.cpp DxLib.cpp -o SyobonAction_x86_64 -L/usr/local/lib -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2_gfx -lm

clean:
	rm -f *.o SyobonAction SyobonAction_arm64 SyobonAction_x86_64

rebuild: clean SyobonAction
