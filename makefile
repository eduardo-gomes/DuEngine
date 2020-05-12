CXX=g++
CXXFLAGS=-std=c++17 -pedantic-errors -Wall -Wextra -Wsign-conversion -Werror -lpthread
LIBS=-lSDL2 -lGL -lGLU -lm

#use Visual Studio Developer Command Prompt
MSVC_INIT=vcvars64.bat
MSVC_FLAGS=/W4 /EHsc /std:c++17 /Idependencies\include /Ox /link /LIBPATH:dependencies\lib\x64 /SUBSYSTEM:windows /ENTRY:mainCRTStartup

colision_dbg:
	$(CXX) colision.cpp -g $(CXXFLAGS) $(LIBS) -O0
colision:
	$(CXX) colision.cpp $(CXXFLAGS) $(LIBS) -O3
colision_win:
	cmd.exe /C "$(MSVC_INIT) && cl colision.cpp $(MSVC_FLAGS)"

all: colision_win colision
win: colision_win