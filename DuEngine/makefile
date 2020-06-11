CXX?=g++
CXXFLAGS?=-std=c++17 -pedantic-errors -Wall -Wextra -Wsign-conversion -Werror -fPIC
DBG?=-g
OPTIMIZATION?=-O3 -march=native -mfpmath=sse

_INLCUDE_F=dependencies/include dependencies/imgui
INCLUDE_F=$(patsubst %, -I../%, $(_INLCUDE_F)) -I.

OBJS_DIR=objects
__AUDIO_OBJ=audio.o #vorbis_ogg.o
_AUDIO_OBJ=$(patsubst %,audio/%,$(__AUDIO_OBJ))
__WINDOW_OBJ=window.o
_WINDOW_OBJ=$(patsubst %,window/%,$(__WINDOW_OBJ))
__RENDER_OBJ=GLClasses.o glmath.o Renderer.o
_RENDER_OBJ=$(patsubst %,graphics/%,$(__RENDER_OBJ))
_ENGINE_OBJ=$(_AUDIO_OBJ) $(_WINDOW_OBJ) $(_RENDER_OBJ) loadfile.o random.o scenes.o
ENGINE_OBJ=$(patsubst %,$(OBJS_DIR)/%,$(_ENGINE_OBJ))

OBJ_SUB_DIRS=audio window graphics

LIBS_OBJ=../dependencies/include/glad.o dependencies/imgui/imgui.o

DuEngine.so: Prepare $(ENGINE_OBJ)
	gcc -o $@ -shared $(ENGINE_OBJ)


#objects/audio/audio.o: audio/audio.cpp audio/audio.hpp audio/vorbis_ogg.hpp audio/vorbis_ogg.cpp
#	$(CXX) $< -c -o $@ $(CXXFLAGS) $(DBG) $(OPTIMIZATION)

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
	rm $(ENGINE_OBJ) DuEngine.so
	rmdir objects/*
	rmdir objects
Prepare:
	mkdir -p objects
	cd objects && mkdir -p $(OBJ_SUB_DIRS)
