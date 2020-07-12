export CXX?=g++
export CXXFLAGS=-std=c++17 -pedantic-errors -Wall -Wextra -Wsign-conversion -Werror -fvisibility=hidden -DDUENG_DLL -DDUENG_DLL_BUILD
export LIBS=-lSDL2 -lGL -lm -ldl -lpthread
export DBG?=-g
export OPTIMIZATION?=-O3 -march=native -mfpmath=sse

LIBS=-lSDL2 -lGL -lm -ldl -lpthread


_INLCUDE_F=dependencies/include dependencies/imgui .
INCLUDE_F=$(patsubst %, -I%, $(_INLCUDE_F))

LIBS_OBJ=libglad.so libimgui.so libDuEngine.so
build: $(LIBS_OBJ)
	cp DuEngine/graphics/basic.glsl basic.glsl

.PHONY: clear Prepare clearAll DuEngine/DuEngine.o test
clear:
	$(MAKE) -C DuEngine clear
	rm -f basic.glsl
clearAll: clear
	rm -f $(LIBS_OBJ)
	$(MAKE) -C dependencies/imgui clear
Prepare:
	$(MAKE) -C DuEngine Prepare
	mkdir -p objects


DuEngine/DuEngine.o:
	$(MAKE) -C DuEngine build 
libDuEngine.so: DuEngine/DuEngine.o
	$(CXX) -o $@ -shared $<


libimgui.so:
	$(MAKE) -C dependencies/imgui imgui.so
	cp dependencies/imgui/imgui.so $@

libglad.so:
	$(CXX) dependencies/include/glad.c -c $(CXXFLAGS) -O3 -o $@ $(INCLUDE_F) -shared -fPIC

TEST_SRC=$(wildcard test/*.cpp)
TEST_OBJ=$(patsubst %.cpp, %.o, $(TEST_SRC))
	
test: build $(TEST_OBJ)
test/%.o: test/%.cpp
	$(CXX) -o $@ $< $(CXXFLAGS) $(DBG) $(INCLUDE_F) -L. -lglad -lDuEngine -limgui -Wl,-rpath=DuEngine -Wl,-rpath=. $(LIBS) $(OPTIMIZATION)