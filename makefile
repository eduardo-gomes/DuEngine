CXX=g++
CXXFLAGS=-std=c++17 -pedantic-errors -Wall -Wextra -Wsign-conversion -Werror -lpthread
LIBS=-lglut -lGL -lGLU -lm

demo_dbg: graphics.hpp demo.cpp loadassets.hpp
	$(CXX) demo.cpp -g $(CXXFLAGS) $(LIBS)
demo: graphics.hpp demo.cpp loadassets.hpp
	$(CXX) demo.cpp $(CXXFLAGS) $(LIBS) -O3
