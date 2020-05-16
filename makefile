CXX=g++
CXXFLAGS=-std=c++17 -pedantic-errors -Wall -Wextra -Wsign-conversion -Werror -lpthread
LIBS=-lSDL2 -lGL -lGLU -lm -lvorbis -lvorbisfile

#use Visual Studio Developer Command Prompt
MSVC_INIT=vcvars64.bat
MSVC_FLAGS=/W4 /EHsc /std:c++17 /Idependencies\include /Ox /link /LIBPATH:dependencies\lib\x64 /SUBSYSTEM:windows /ENTRY:mainCRTStartup

COLISION_OBJ=audio.o
colision_dbg: audio.o
	$(CXX) colision.cpp $(COLISION_OBJ) -g $(CXXFLAGS) $(LIBS) -O0
colision: audio.o
	$(CXX) colision.cpp $(COLISION_OBJ) $(CXXFLAGS) $(LIBS) -O3 -s
colision_win:
	cmd.exe /C "$(MSVC_INIT) && cl colision.cpp $(MSVC_FLAGS)"
audio.o: audio.cpp audio.hpp vorbis_ogg.hpp vorbis_ogg.cpp
	$(CXX) audio.cpp -c $(CXXFLAGS) -O3 -g


all: colision_win colision
win: colision_win