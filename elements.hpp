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


namespace phy{
	extern const double gravity, jumpVel;
}
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
	bool has_jumped = 0;
	public:
	void gravity(){
		jump_vel -= phy::gravity*millis;
	}
	void jump(){if(!has_jumped)jump_vel += phy::jumpVel; has_jumped = 1;}
	void apply_jump(){element.pos.y += jump_vel*millis;}
	void onhorizontalColision(){if(jump_vel < 0) jump_vel = 0; has_jumped = 0;}
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

void drawn_quad_with_texture(const vector_quad_text& vector) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[vector.text_index]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//repeat texture on x DISABLED because is the defaut
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//repeat texture on y
	glColor3d(1.0, 1.0, 1.0);
	glBegin(GL_QUADS);
	glTexCoord2d(vector.vector[2], vector.vector[3]);
	glVertex2d(vector.pos.x + vector.vector[0], vector.pos.y + vector.vector[1]);
	glTexCoord2d(vector.vector[6], vector.vector[7]);
	glVertex2d(vector.pos.x + vector.vector[4], vector.pos.y + vector.vector[5]);
	glTexCoord2d(vector.vector[10], vector.vector[11]);
	glVertex2d(vector.pos.x + vector.vector[8], vector.pos.y + vector.vector[9]);
	glTexCoord2d(vector.vector[14], vector.vector[15]);
	glVertex2d(vector.pos.x + vector.vector[12], vector.pos.y + vector.vector[13]);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void drawn_triang_with_texture(const vector_with_text& vector) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[vector.text_index]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//repeat texture on x DISABLED because is the defaut
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//repeat texture on y
	glColor3d(1.0, 1.0, 1.0);
	glBegin(vector.mode);
	for (long unsigned int i = 0; i < vector.cords.size() / 4; ++i) {
		//printf("%lu\n", i);
		glTexCoord2d(vector.cords[2 + i * 4], vector.cords[3 + i * 4]);
		glVertex2d(vector.pos.x + vector.cords[0 + i * 4], vector.pos.y + vector.cords[1 + i * 4]);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}