#pragma once
#include <vector>
#ifndef _MAIN
#include "colision.cpp"
#endif
#define _BASIC_STRUCTS
struct position {
	double x = 0.0, y = 0.0;
	position(double nx = 0.0, double ny = 0.0) : x(nx), y(ny){};
};
struct text_params{
	GLint mag_filter, min_filter, warp_s, warp_t;
	text_params(GLint mag_filter = GL_NEAREST, GLint min_filter = GL_LINEAR, GLint warp_s = GL_REPEAT, GLint warp_t = GL_REPEAT) :
	mag_filter(mag_filter), min_filter(min_filter), warp_s(warp_s), warp_t(warp_t){std::cout << "Text_params constructor" << std::endl;};
};
static const text_params def_text_params;
struct vertex_quad_text {
	double vector[16] = {
		//pos		text pos
		0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0};
	int text_index;
	position pos = {0.0, 0.0};
	const text_params *params = &def_text_params;
};
struct vertex_with_text {
	int text_index;
	GLenum mode;
	std::vector<double> cords = {};
	position pos = {0.0, 0.0};
	const text_params *params = &def_text_params;
};


namespace phy{
	extern const double gravity, jumpVel;
}
extern double millis;
class entidade{
	vertex_quad_text element = {
		0.0, 0.0,		0.0, 0.0,
		1.0, 0.0,		1.0, 0.0,
		1.0, 1.0,		1.0, 1.0,
		0.0, 1.0,		0.0, 1.0,
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
	void onhorizontalColision(){if(jump_vel < 0) has_jumped = 0; jump_vel = 0;}
	const double* getVector(){return element.vector;};
	const vertex_quad_text& getElement(){return element;}
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

void drawn_quad_with_texture(const vertex_quad_text& vertex) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[vertex.text_index]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, vertex.params->min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, vertex.params->mag_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, vertex.params->warp_s); //repeat texture on x DISABLED because is the defaut
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, vertex.params->warp_t); //repeat texture on y
	glColor3d(1.0, 1.0, 1.0);
	glBegin(GL_QUADS);
	glTexCoord2d(vertex.vector[2], vertex.vector[3]);
	glVertex2d(vertex.pos.x + vertex.vector[0], vertex.pos.y + vertex.vector[1]);
	glTexCoord2d(vertex.vector[6], vertex.vector[7]);
	glVertex2d(vertex.pos.x + vertex.vector[4], vertex.pos.y + vertex.vector[5]);
	glTexCoord2d(vertex.vector[10], vertex.vector[11]);
	glVertex2d(vertex.pos.x + vertex.vector[8], vertex.pos.y + vertex.vector[9]);
	glTexCoord2d(vertex.vector[14], vertex.vector[15]);
	glVertex2d(vertex.pos.x + vertex.vector[12], vertex.pos.y + vertex.vector[13]);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void drawn_triang_with_texture(const vertex_with_text& vertex) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[vertex.text_index]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, vertex.params->min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, vertex.params->mag_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, vertex.params->warp_s);//repeat texture on x DISABLED because is the defaut
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, vertex.params->warp_t);//repeat texture on y
	glColor3d(1.0, 1.0, 1.0);
	glBegin(vertex.mode);
	for (long unsigned int i = 0; i < vertex.cords.size() / 4; ++i) {
		//printf("%lu\n", i);
		glTexCoord2d(vertex.cords[2 + i * 4], vertex.cords[3 + i * 4]);
		glVertex2d(vertex.pos.x + vertex.cords[0 + i * 4], vertex.pos.y + vertex.cords[1 + i * 4]);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
void drawn_bcg(){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[sprites::funuv]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glColor3d(1.0, 1.0, 1.0);
	double offsetx = std::remainder((screen::camx * 1.5 *screen::aspect) / screen::bcgViewx, 1.0);
	glBegin(GL_QUADS);
		glTexCoord2d(0.0 + offsetx, 0.0);glVertex3d(screen::camx - screen::bcgViewx, screen::camy - screen::bcgViewy, screen::bcgDist);
		glTexCoord2d(screen::aspect * 2.0 + offsetx, 0.0);glVertex3d(screen::camx + screen::bcgViewx, screen::camy - screen::bcgViewy, screen::bcgDist);
		glTexCoord2d(screen::aspect * 2.0 + offsetx, 2.0);glVertex3d(screen::camx + screen::bcgViewx, screen::camy + screen::bcgViewy, screen::bcgDist);
		glTexCoord2d(0.0 + offsetx, 2.0);glVertex3d(screen::camx - screen::bcgViewx, screen::camy + screen::bcgViewy, screen::bcgDist);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
void drawn_bcg2(){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[sprites::nuvem]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glColor3d(1.0, 1.0, 1.0);
	double offsetx = std::remainder((screen::camx * 3 *screen::aspect) / (screen::bcg2Viewx * 2), 1.0);
	glBegin(GL_QUADS);
		glTexCoord2d(				  0.0 + offsetx, 0.0);glVertex3d(screen::camx - screen::bcg2Viewx, screen::camy - screen::bcg2Viewy, screen::bcg2Dist);
		glTexCoord2d(screen::aspect * 3.0 + offsetx, 0.0);glVertex3d(screen::camx + screen::bcg2Viewx, screen::camy - screen::bcg2Viewy, screen::bcg2Dist);
		glTexCoord2d(screen::aspect * 3.0 + offsetx, 3.0);glVertex3d(screen::camx + screen::bcg2Viewx, screen::camy + screen::bcg2Viewy, screen::bcg2Dist);
		glTexCoord2d(				  0.0 + offsetx, 3.0);glVertex3d(screen::camx - screen::bcg2Viewx, screen::camy + screen::bcg2Viewy, screen::bcg2Dist);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
void drawn_bcg3(){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[sprites::coque]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//repeat texture on x DISABLED because is the defaut
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);//clamp texture on y
	glColor3d(1.0, 1.0, 1.0);
	double offsetx = std::remainder((screen::camx * 3 *screen::aspect) / (screen::bcg3Viewx * 2), 1.0);
	double offsety = (screen::camy - 5) / screen::bcg3Viewy * 1.5;
	glBegin(GL_QUADS);
		glTexCoord2d(				  0.0 + offsetx, -0.5 + offsety);glVertex3d(screen::camx - screen::bcg3Viewx, screen::camy - screen::bcg3Viewy, screen::bcg3Dist);
		glTexCoord2d(screen::aspect * 3.0 + offsetx, -0.5 + offsety);glVertex3d(screen::camx + screen::bcg3Viewx, screen::camy - screen::bcg3Viewy, screen::bcg3Dist);
		glTexCoord2d(screen::aspect * 3.0 + offsetx,  2.5 + offsety);glVertex3d(screen::camx + screen::bcg3Viewx, screen::camy + screen::bcg3Viewy, screen::bcg3Dist);
		glTexCoord2d(				  0.0 + offsetx,  2.5 + offsety);glVertex3d(screen::camx - screen::bcg3Viewx, screen::camy + screen::bcg3Viewy, screen::bcg3Dist);
		/*glTexCoord2d(0.0, -0.5);glVertex3d(- screen::bcg3Viewx, - screen::bcg3Viewy, screen::bcg3Dist);
		glTexCoord2d(screen::aspect * 3.0, -0.5);glVertex3d(screen::bcg3Viewx, - screen::bcg3Viewy, screen::bcg3Dist);
		glTexCoord2d(screen::aspect * 3.0,  2.5);glVertex3d(screen::bcg3Viewx, + screen::bcg3Viewy, screen::bcg3Dist);
		glTexCoord2d(0.0, 2.5);glVertex3d(- screen::bcg3Viewx, + screen::bcg3Viewy, screen::bcg3Dist);*/
	glEnd();
	glDisable(GL_TEXTURE_2D);
}