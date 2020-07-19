export CXX?=g++ #x86_64-w64-mingw32-g++-posix
export LINKER?=ld #x86_64-w64-mingw32-ld
export CXXFLAGS=-std=c++17 -pedantic-errors -Wall -Wextra -Wsign-conversion -Werror -fvisibility=hidden -DDUENG_DLL -DDUENG_DLL_BUILD -Wno-pedantic-ms-format
export LIBS=-lSDL2 -lGL -lm -ldl -lpthread
export DBG?=-g
export OPTIMIZATION?=-O3 -march=native -mfpmath=sse

DUENGLIBS=-lSDL2 -lGL -lm -ldl -lpthread
DUENGLIBSW32=-lSDL2 -lm -lopengl32 -lpthread


_INLCUDE_F=dependencies/include dependencies/imgui .
INCLUDE_F=$(patsubst %, -I%, $(_INLCUDE_F))

LIBS_OBJ=libDuEngine.so libimgui.o libglad.o
LIBS_OBJW64=libDuEngine.dll libimgui.o libglad.o
copyshader:
	cp DuEngine/graphics/basic.glsl basic.glsl
	cp DuEngine/graphics/basic.glsl DuEngine/basic.glsl

build: $(LIBS_OBJ) copyshader
buildwin: $(LIBS_OBJW64) copyshader

.PHONY: clear Prepare clearAll test build buildwin copyshader libimgui.o DuEngine/DuEngine.o
clear:
	$(MAKE) -C DuEngine clear
	rm -f basic.glsl DuEngine/basic.glsl $(LIBS_OBJ) $(LIBS_OBJW64)
clearAll: clear
	$(MAKE) -C dependencies/imgui clear
Prepare:
	$(MAKE) -C DuEngine Prepare
	mkdir -p objects


DuEngine/DuEngine.o:
	$(MAKE) -C DuEngine build 
libDuEngine.so: DuEngine/DuEngine.o libimgui.o libglad.o
	$(CXX) -o $@ -shared $^ $(DUENGLIBS)
libDuEngine.dll: DuEngine/DuEngine.o libglad.o libimgui.o
	$(CXX) -o $@ -shared $^ $(DUENGLIBSW32) -Wl,--output-def,libDuEngine.def


libimgui.o:
	$(MAKE) -C dependencies/imgui libimgui.o
	cp dependencies/imgui/libimgui.o $@

libglad.o:
	$(CXX) dependencies/include/glad.c -c $(CXXFLAGS) -O3 -o $@ $(INCLUDE_F) -fPIC
#libglad.dll:
#	$(CXX) dependencies/include/glad.c -c -O3 -o $@ $(INCLUDE_F) -shared -fPIC -fvisibility=default -DGLAD_GLAPI_EXPORT -DGLAD_GLAPI_EXPORT_BUILD

TEST_SRC=$(wildcard test/*.cpp)
TEST_OBJ=$(patsubst %.cpp, %.o, $(TEST_SRC))
	
test: build $(TEST_OBJ)
test/%.o: test/%.cpp
	$(CXX) -o $@ $< $(CXXFLAGS) $(OPTIMIZATION) $(DBG) $(INCLUDE_F) -L. -lDuEngine -Wl,-rpath=DuEngine -Wl,-rpath=. $(LIBS)