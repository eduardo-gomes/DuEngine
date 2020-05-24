CXX=g++
CXXFLAGS=-std=c++17 -pedantic-errors -Wall -Wextra -Wsign-conversion -Werror -lpthread
LIBS=-lSDL2 -lGL -lm -lvorbis -lvorbisfile -ldl
DBG?=-g
OPTIMIZATION?=-Og

##use Visual Studio Developer Command Prompt
#MSVC_INIT=vcvars64.bat
#MSVC_FLAGS=/W4 /EHsc /std:c++17 /Idependencies\include /Ox /link /LIBPATH:dependencies\lib\x64 /SUBSYSTEM:windows /ENTRY:mainCRTStartup

INCLUDE_F=-Idependencies/include -Idependencies/imgui
COLISION_OBJ=dependencies/include/glad.o audio.o graphics.o GLClasses.o loadfile.o glmath.o dependencies/imgui/imgui.o

glnew: $(COLISION_OBJ)
	$(CXX) glnew.cpp $(COLISION_OBJ) $(CXXFLAGS) $(LIBS) $(INCLUDE_F) $(DBG) $(OPTIMIZATION)
#colision_dbg: $(COLISION_OBJ)
#	export DBG="-g" &&	make colision
colision: $(COLISION_OBJ)
	$(CXX) colision.cpp $(COLISION_OBJ) $(CXXFLAGS) $(LIBS) $(INCLUDE_F) -O3 -s
#colision_win:
#	cmd.exe /C "$(MSVC_INIT) && cl colision.cpp $(MSVC_FLAGS)"


audio.o: audio.cpp audio.hpp vorbis_ogg.hpp vorbis_ogg.cpp
	$(CXX) audio.cpp -c $(CXXFLAGS) $(DBG) $(OPTIMIZATION)

graphics.o:	graphics.cpp
	$(CXX) graphics.cpp -c $(CXXFLAGS) $(INCLUDE_F) $(DBG) $(OPTIMIZATION)

GLClasses.o: GLClasses.cpp
	$(CXX) GLClasses.cpp -c $(CXXFLAGS) $(INCLUDE_F) $(DBG) $(OPTIMIZATION)

loadfile.o: loadfile.cpp
	$(CXX) loadfile.cpp -c $(CXXFLAGS) $(INCLUDE_F) $(DBG) $(OPTIMIZATION)

glmath.o: glmath.cpp
	$(CXX) glmath.cpp -c $(CXXFLAGS) $(INCLUDE_F) $(DBG) $(OPTIMIZATION)

dependencies/include/glad.o:
	$(CXX) dependencies/include/glad.c -c $(CXXFLAGS) -O3 -o dependencies/include/glad.o $(INCLUDE_F)

IMGUIDIR=dependencies/imgui
_IMGUISRC=imgui.cpp imgui_demo.cpp imgui_draw.cpp imgui_impl_opengl3.cpp imgui_impl_sdl.cpp imgui_widgets.cpp
IMGUISRC=$(patsubst %,$(IMGUIDIR)/%,$(_IMGUISRC))
dependencies/imgui/imgui.o: $(IMGUISRC)
	$(CXX) dependencies/imgui/all.cpp -c -O3 -o dependencies/imgui/imgui.o $(INCLUDE_F)

clear:
	rm $(COLISION_OBJ)

all: colision_win colision
win: colision_win