#pragma once
#include <vector>
#ifndef _MAIN
#include "colision.cpp"
#endif
struct position {
	double x = 0.0, y = 0.0;
	position(double nx = 0.0, double ny = 0.0) : x(nx), y(ny){};
};
struct vector_quad_text {
	double vector[16] = {
		//pos		text pos
		0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0};
	int text_index;
	position pos = {0.0, 0.0};
};
struct vector_with_text {
	int text_index;
	GLenum mode;
	std::vector<double> cords;
	position pos = {0.0, 0.0};
};


#include "loadassets.hpp"
extern double millis;
class entidade{
	vector_quad_text element = {
		0.0, 0.0,		0.0, 0.0,
		0.0, 1.0,		0.0, 1.0,
		1.0, 1.0,		1.0, 1.0,
		1.0, 0.0,		1.0, 0.0,
		spritesname::persparado01,
		{0.5, 5.0}
	};
	double jump_vel;
	position lastpos = element.pos;

	public:
	void gravity(){jump_vel -= 0.035*millis;}
	void jump(){jump_vel += 0.25;}
	void apply_jump(){element.pos.y += jump_vel*millis;}
	void onhorizontalColision(){if(jump_vel < 0) jump_vel = 0;}
	const double* getVector(){return element.vector;};
	const vector_quad_text& getElement(){return element;}
	int& texture(){return element.text_index;}
	void onColision(){element.pos = lastpos;}
	void noColision(){lastpos = element.pos;}
	void move(double x = 0.0, double y = 0.0){
		lastpos = element.pos;
		element.pos.x += x;
		element.pos.y += y;
	}
	void move(position newpos){
		lastpos = element.pos;
		element.pos.x += newpos.x;
		element.pos.y += newpos.y;
	}
	bool colide = 0;
	//void 
};
