export CXX=g++
export CXXFLAGS=-std=c++17 -pedantic-errors -Wall -Wextra -Wsign-conversion -Werror
export LIBS=-lSDL2 -lGL -lm -lvorbis -lvorbisfile -ldl -lpthread
export DBG?=-g
export OPTIMIZATION?=-O3 -march=native -mfpmath=sse

.PHONY: clear Prepare all build
build: glnew
clear:
	
Prepare:
	mkdir -p objects