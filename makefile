Linux :
	g++ main.cpp glad.c graphics.cpp engineinit.cpp -o Build/jackal -Bstatic -lglfw -lGL -lGLU -lm -static-libstdc++ -static-libgcc -std=c++17
Windows :
	x86_64-w64-mingw32-g++ main.cpp glad.c graphics.cpp engineinit.cpp -o Build/jackal.exe -Bstatic -L -static -lglfw3 -lglu32 -lwinmm -lopengl32 -mwindows -static-libstdc++ -static-libgcc -std=c++17 -Wl,--subsystem,windows