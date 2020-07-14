export CXX?=g++ #x86_64-w64-mingw32-g++-posix
export LINKER?=ld
export CXXFLAGS=-std=c++17 -pedantic-errors -Wall -Wextra -Wsign-conversion -Werror -fvisibility=hidden -DDUENG_DLL -DDUENG_DLL_BUILD -Wno-pedantic-ms-format
export LIBS=-lSDL2 -lGL -lm -ldl -lpthread
export DBG?=-g
export OPTIMIZATION?=-O3 -march=native -mfpmath=sse

DUENGLIBS=-lSDL2 -lGL -lm -ldl -lpthread
DUENGLIBSW32=-lSDL2 -lm -lopengl32 -lpthread


_INLCUDE_F=dependencies/include dependencies/imgui .
INCLUDE_F=$(patsubst %, -I%, $(_INLCUDE_F))

LIBS_OBJ=libglad.so libimgui.so libDuEngine.so
build: $(LIBS_OBJ)
	cp DuEngine/graphics/basic.glsl basic.glsl
	cp DuEngine/graphics/basic.glsl DuEngine/basic.glsl

buildwin: libDuEngine.dll

.PHONY: clear Prepare clearAll DuEngine/DuEngine.o test build buildwin
clear:
	$(MAKE) -C DuEngine clear
	rm -f basic.glsl DuEngine/basic.glsllibDuEngine.dll libimgui.dll libglad.dll
clearAll: clear
	rm -f $(LIBS_OBJ)
	$(MAKE) -C dependencies/imgui clear
Prepare:
	$(MAKE) -C DuEngine Prepare
	mkdir -p objects


DuEngine/DuEngine.o:
	$(MAKE) -C DuEngine build 
libDuEngine.so: DuEngine/DuEngine.o
	$(CXX) -o $@ -shared $< $(DUENGLIBS)
libDuEngine.dll: DuEngine/DuEngine.o libglad.dll libimgui.dll
	$(CXX) -o $@ -shared $< $(DUENGLIBSW32) -Wl,--output-def,DuEngine.def,--out-implib,libDuEngine.a -L. -llibglad -llibimgui


libimgui.so:
	$(MAKE) -C dependencies/imgui libimgui.so
	cp dependencies/imgui/libimgui.so $@
libimgui.dll:
	$(MAKE) -C dependencies/imgui libimgui.dll
	cp dependencies/imgui/libimgui.dll $@

libglad.so:
	$(CXX) dependencies/include/glad.c -c $(CXXFLAGS) -O3 -o $@ $(INCLUDE_F) -shared -fPIC -fvisibility=default
libglad.dll:
	$(CXX) dependencies/include/glad.c -c $(CXXFLAGS) -O3 -o $@ $(INCLUDE_F) -shared -fPIC -fvisibility=default

TEST_SRC=$(wildcard test/*.cpp)
TEST_OBJ=$(patsubst %.cpp, %.o, $(TEST_SRC))
	
test: build $(TEST_OBJ)
test/%.o: test/%.cpp
	$(CXX) -o $@ $< $(CXXFLAGS) $(OPTIMIZATION) $(DBG) $(INCLUDE_F) -L. -lglad -lDuEngine -limgui -Wl,-rpath=DuEngine -Wl,-rpath=. $(LIBS)