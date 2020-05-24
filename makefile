CXX=g++
CXXFLAGS=-std=c++17 -pedantic-errors -Wall -Wextra -Wsign-conversion -Werror -lpthread
LIBS=-lSDL2 -lGL -lm -lvorbis -lvorbisfile -ldl
DBG?=-g
OPTIMIZATION?=-Og

##use Visual Studio Developer Command Prompt
#MSVC_INIT=vcvars64.bat
#MSVC_FLAGS=/W4 /EHsc /std:c++17 /Idependencies\include /Ox /link /LIBPATH:dependencies\lib\x64 /SUBSYSTEM:windows /ENTRY:mainCRTStartup

INCLUDE_F=-Idependencies/include -Idependencies/imgui
COLISION_OBJ=dependencies/include/glad.o audio.o graphics.o GLClasses.o loadfile.o glmath.o

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

clear:
	rm $(COLISION_OBJ)

all: colision_win colision
win: colision_win