export CXX=g++
export CXXFLAGS=-std=c++17 -pedantic-errors -Wall -Wextra -Wsign-conversion -Werror
export LIBS=-lSDL2 -lGL -lm -lvorbis -lvorbisfile -ldl -lpthread
export DBG?=-g
export OPTIMIZATION?=-O3 -march=native -mfpmath=sse

LIBS=-lSDL2 -lGL -lm -lvorbis -lvorbisfile -ldl -lpthread


_INLCUDE_F=dependencies/include dependencies/imgui .
INCLUDE_F=$(patsubst %, -I%, $(_INLCUDE_F))

LIBS_OBJ=libglad.so libimgui.so libDuEngine.so
build: $(LIBS_OBJ)
	cp DuEngine/graphics/basic.glsl basic.glsl

.PHONY: clear Prepare clearAll DuEngine/DuEngine.so
clear:
	$(MAKE) -C DuEngine clear
	rm -f basic.glsl
clearAll: clear
	rm -f $(LIBS_OBJ)
Prepare:
	$(MAKE) -C DuEngine Prepare
	mkdir -p objects


DuEngine/DuEngine.so:
	$(MAKE) -C DuEngine build
libDuEngine.so: DuEngine/DuEngine.so
	cp $< $@


IMGUIDIR=dependencies/imgui
_IMGUISRC=imgui.cpp imgui_demo.cpp imgui_draw.cpp imgui_impl_opengl3.cpp imgui_impl_sdl.cpp imgui_widgets.cpp
IMGUISRC=$(patsubst %,$(IMGUIDIR)/%,$(_IMGUISRC))
libimgui.so: $(IMGUISRC)
	$(CXX) dependencies/imgui/all.cpp -c -O3 -o $@ $(INCLUDE_F) -shared -fPIC

libglad.so:
	$(CXX) dependencies/include/glad.c -c $(CXXFLAGS) -O3 -o $@ $(INCLUDE_F) -shared -fPIC
