CXX=g++
CXXFLAGS=-std=c++17 -pedantic-errors -Wall -Wextra -Wsign-conversion -Werror -lpthread
LIBS=-lSDL2 -lGL -lGLU -lm -lvorbis -lvorbisfile -ldl
DBG?=-g
OPTIMIZATION?=-Og

##use Visual Studio Developer Command Prompt
#MSVC_INIT=vcvars64.bat
#MSVC_FLAGS=/W4 /EHsc /std:c++17 /Idependencies\include /Ox /link /LIBPATH:dependencies\lib\x64 /SUBSYSTEM:windows /ENTRY:mainCRTStartup

INCLUDE_F=-Idependencies/include
COLISION_OBJ=dependencies/include/glad.o audio.o graphics.o

glnew: $(COLISION_OBJ)
	$(CXX) glnew.cpp $(COLISION_OBJ) $(CXXFLAGS) $(LIBS) $(INCLUDE_F) $(DBG)
#colision_dbg: $(COLISION_OBJ)
#	export DBG="-g" &&	make colision
colision: $(COLISION_OBJ)
	$(CXX) colision.cpp $(COLISION_OBJ) $(CXXFLAGS) $(LIBS) $(INCLUDE_F) -O3 -s
#colision_win:
#	cmd.exe /C "$(MSVC_INIT) && cl colision.cpp $(MSVC_FLAGS)"


audio.o: audio.cpp audio.hpp vorbis_ogg.hpp vorbis_ogg.cpp
	$(CXX) audio.cpp -c $(CXXFLAGS) $(DBG) $(OPTIMIZATION)

graphics.o:	graphics.hpp graphics.cpp
	$(CXX) graphics.cpp -c $(CXXFLAGS) $(INCLUDE_F) $(DBG) $(OPTIMIZATION)


dependencies/include/glad.o:
	$(CXX) dependencies/include/glad.c -c $(CXXFLAGS) -O3 -o dependencies/include/glad.o $(INCLUDE_F)


all: colision_win colision
win: colision_win