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
libDuEngine.dll: DuEngine/DuEngine.o libglad.dll libimgui.o
	$(CXX) -o $@ -shared $< libimgui.o $(DUENGLIBSW32) -Wl,--output-def,libDuEngine.def,--out-implib,libDuEngine.a -L. -llibglad


libimgui.so:
	$(MAKE) -C dependencies/imgui libimgui.so
	cp dependencies/imgui/libimgui.so $@
libimgui.o:
	$(MAKE) -C dependencies/imgui libimgui.o
	cp dependencies/imgui/libimgui.o $@

libglad.so:
	$(CXX) dependencies/include/glad.c -c $(CXXFLAGS) -O3 -o $@ $(INCLUDE_F) -shared -fPIC -fvisibility=default
libglad.dll:
	$(CXX) dependencies/include/glad.c -c  -O3 -o $@ $(INCLUDE_F) -shared -fPIC -fvisibility=default -DGLAD_GLAPI_EXPORT -DGLAD_GLAPI_EXPORT_BUILD -Wl,--output-def,libDuEngine.def,--out-implib,libDuEngine.a

TEST_SRC=$(wildcard test/*.cpp)
TEST_OBJ=$(patsubst %.cpp, %.o, $(TEST_SRC))
	
test: build $(TEST_OBJ)
test/%.o: test/%.cpp
	$(CXX) -o $@ $< $(CXXFLAGS) $(OPTIMIZATION) $(DBG) $(INCLUDE_F) -L. -lglad -lDuEngine -limgui -Wl,-rpath=DuEngine -Wl,-rpath=. $(LIBS)