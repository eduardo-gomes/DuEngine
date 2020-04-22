CXX=g++
CXXFLAGS=-std=c++17 -pedantic-errors -Wall -Wextra -Wsign-conversion -Werror -lpthread
LIBS=-lglut -lGL -lGLU -lm

#use Visual Studio Developer Command Prompt
MSVC_INIT=vcvars64.bat
MSVC_FLAGS=/W4 /EHsc /std:c++17 /Idependencies\include /O2 /link /LIBPATH:dependencies\lib\x64

colision_dbg: graphics.hpp demo.cpp loadassets.hpp
	$(CXX) colision.cpp -g $(CXXFLAGS) $(LIBS)
colision: graphics.hpp demo.cpp loadassets.hpp
	$(CXX) colision.cpp $(CXXFLAGS) $(LIBS) -O3
colision_win:
	cmd.exe /C "$(MSVC_INIT) && cl colision.cpp $(MSVC_FLAGS)"

demo_dbg: graphics.hpp demo.cpp loadassets.hpp
	$(CXX) demo.cpp -g $(CXXFLAGS) $(LIBS)
demo: graphics.hpp demo.cpp loadassets.hpp
	$(CXX) demo.cpp $(CXXFLAGS) $(LIBS) -O3
