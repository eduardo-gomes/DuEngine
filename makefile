CXX=g++
CXXFLAGS=-std=c++17 -pedantic-errors -Wall -Wextra -Wsign-conversion -Werror
LIBS=-lSDL2 -lGL -lm -lvorbis -lvorbisfile -ldl -lpthread
DBG?=-g
OPTIMIZATION?=-O3 -march=native -mfpmath=sse

##use Visual Studio Developer Command Prompt
#MSVC_INIT=vcvars64.bat
#MSVC_FLAGS=/W4 /EHsc /std:c++17 /Idependencies\include /Ox /link /LIBPATH:dependencies\lib\x64 /SUBSYSTEM:windows /ENTRY:mainCRTStartup

INCLUDE_F=-Idependencies/include -Idependencies/imgui

OBJS_DIR=objects
_COLISION_OBJ=audio.o graphics.o GLClasses.o loadfile.o glmath.o scenes.o Renderer.o renderertest.o glnew.o random.o
COLISION_OBJ=$(patsubst %,$(OBJS_DIR)/%,$(_COLISION_OBJ))

LIBS_OBJ=dependencies/include/glad.o dependencies/imgui/imgui.o

glnew: $(COLISION_OBJ) $(LIBS_OBJ)
	$(CXX) $^ $(LIBS) -o $@
	cp $@ a.out
#colision_dbg: $(COLISION_OBJ)
#	export DBG="-g" &&	make colision
colision: $(COLISION_OBJ)
	$(CXX) colision.cpp $(COLISION_OBJ) $(CXXFLAGS) $(LIBS) $(INCLUDE_F) -O3 -s
#colision_win:
#	cmd.exe /C "$(MSVC_INIT) && cl colision.cpp $(MSVC_FLAGS)"


objects/audio.o: audio.cpp audio.hpp vorbis_ogg.hpp vorbis_ogg.cpp
	$(CXX) audio.cpp -c -o $@ $(CXXFLAGS) $(DBG) $(OPTIMIZATION)

objects/%.o: %.cpp
	$(CXX) $< -c -o $@ $(CXXFLAGS) $(INCLUDE_F) $(DBG) $(OPTIMIZATION)

IMGUIDIR=dependencies/imgui
_IMGUISRC=imgui.cpp imgui_demo.cpp imgui_draw.cpp imgui_impl_opengl3.cpp imgui_impl_sdl.cpp imgui_widgets.cpp
IMGUISRC=$(patsubst %,$(IMGUIDIR)/%,$(_IMGUISRC))
dependencies/imgui/imgui.o: $(IMGUISRC)
	$(CXX) dependencies/imgui/all.cpp -c -O3 -o $@ $(INCLUDE_F)

dependencies/include/glad.o:
	$(CXX) dependencies/include/glad.c -c $(CXXFLAGS) -O3 -o $@ $(INCLUDE_F)

.PHONY: clear Prepare all build
build: glnew
clear:
	rm $(COLISION_OBJ) glnew -f
Prepare:
	mkdir -p objects

all: colision_win colision
#win: colision_win