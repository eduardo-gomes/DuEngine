CXX?=g++
CXXFLAGS?=-std=c++17 -pedantic-errors -Wall -Wextra -Wsign-conversion -Werror -fPIC
DBG?=-g
OPTIMIZATION?=-O3 -march=native -mfpmath=sse

_INLCUDE_F=dependencies/include dependencies/imgui .
INCLUDE_F=$(patsubst %, -I../%, $(_INLCUDE_F)) -I.

OBJS_DIR=objects
__AUDIO_OBJ=audio.o vorbis_ogg.o
_AUDIO_OBJ=$(patsubst %,audio/%,$(__AUDIO_OBJ))
__WINDOW_OBJ=window.o
_WINDOW_OBJ=$(patsubst %,window/%,$(__WINDOW_OBJ))
__RENDER_OBJ=GLClasses.o glmath.o Renderer.o LoadTexture.o
_RENDER_OBJ=$(patsubst %,graphics/%,$(__RENDER_OBJ))
__MANAGER_OBJ=AssetsManager.o logger.o
_MANAGER_OBJ=$(patsubst %,manager/%,$(__MANAGER_OBJ))
_ENGINE_OBJ=$(_AUDIO_OBJ) $(_WINDOW_OBJ) $(_RENDER_OBJ) $(_MANAGER_OBJ) random.o scenes.o Init.o
ENGINE_OBJ=$(patsubst %,$(OBJS_DIR)/%,$(_ENGINE_OBJ))

OBJ_SUB_DIRS=audio window graphics manager


DuEngine.o: $(ENGINE_OBJ)
	ld -E -r $^ -o $@


objects/audio/audio.o: audio/audio.cpp audio/audio.hpp audio/vorbis_ogg.hpp audio/vorbis_ogg.cpp
	$(CXX) $< -c -o $@ $(CXXFLAGS) $(INCLUDE_F) $(OPTIMIZATION) $(DBG) -fPIC -Wno-pointer-arith

objects/%.o: %.cpp
	$(CXX) $< -c -o $@ $(CXXFLAGS) $(INCLUDE_F) $(OPTIMIZATION) $(DBG) -fPIC


.PHONY: clear Prepare all build
build: Prepare DuEngine.o
clear:
	rm $(ENGINE_OBJ) DuEngine.o -f
	-rmdir objects/* objects
Prepare:
	mkdir -p objects
	cd objects && mkdir -p $(OBJ_SUB_DIRS)
