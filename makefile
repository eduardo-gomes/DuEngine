CXX=g++
CXXFLAGS=-std=c++17 -pedantic-errors -Wall -Wextra -Wsign-conversion -Werror -lpthread
LIBS=-lglut -lGL -lGLU -lm

colision_dbg: graphics.hpp demo.cpp loadassets.hpp
	$(CXX) colision.cpp -g $(CXXFLAGS) $(LIBS)
colision: graphics.hpp demo.cpp loadassets.hpp
	$(CXX) colision.cpp $(CXXFLAGS) $(LIBS) -O3

demo_dbg: graphics.hpp demo.cpp loadassets.hpp
	$(CXX) demo.cpp -g $(CXXFLAGS) $(LIBS)
demo: graphics.hpp demo.cpp loadassets.hpp
	$(CXX) demo.cpp $(CXXFLAGS) $(LIBS) -O3
