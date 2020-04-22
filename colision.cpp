// -lglut -lGL -lGLU -lm
#define _MAIN
#include <iostream>
#include <chrono>
#include "graphics.hpp"
#include "loadassets.hpp"
bool pressed_mouse = 0;
namespace mouse {
	int x = -50, y = -50;
	position pos;
	void getcord() {
		//(x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
		pos.x = x * ((screen::camx + screen::viewx) - (screen::camx - screen::viewx)) / screen::width + (screen::camx - screen::viewx);
		pos.y = y * ((screen::camy - screen::viewy) - (screen::camy + screen::viewy)) / screen::height + (screen::camy + screen::viewy);
	}
}  // namespace mouse
bool colide(const vector_quad_text &a, const vector_quad_text&b){
	const position &bPos = a.pos, &aPos = b.pos;
	const double &bWidth = a.vector[3*4],
				 &aWidth = b.vector[3*4],
				 &bHeight = a.vector[1*4 + 1],
				 &aHeight = b.vector[1*4 + 1];
	return (
		aPos.x < bPos.x + bWidth &&//
		aPos.x + aWidth > bPos.x &&
		aPos.y < bPos.y + bHeight &&
		aPos.y + aHeight > bPos.y);
}
bool colide(const vector_quad_text &a, const vector_with_text&b){
	const position &aPos = a.pos;
	const double &aWidth = a.vector[3*4],
				 &aHeight = a.vector[1 * 4 + 1];
	bool has_colided = 0;
	for(long unsigned int nQuad= 0; nQuad < b.cords.size()/(4*4); ++nQuad){
		const double &bX = b.cords[nQuad * 4 * 4],
					 &bY = b.cords[nQuad * 4 * 4 + 1],
					 &bWidth = b.cords[nQuad * 4 * 4 + 4 * 3],
					 &bHeigth = b.cords[nQuad * 4 * 4 + 4 * 1 + 1];
		if( //Test if there isn't a gap betwin any the two quads
			aPos.x < bX + (bWidth - bX) &&
			aPos.x + aWidth > bX &&
			aPos.y < bY + (bHeigth - bY) &&
			aPos.y + aHeight > bY) has_colided = 1;
	}
	return has_colided;
}

std::vector<vector_with_text> scene_box = {
	{//vector_with_text
		sprites::brick, GL_QUADS,
		{// clockwise from botom_left corner
			0.0, 0.0, 0.0, 0.0,
			0.0, 3.0, 0.0, 3.0,
			16.0,3.0, 16.0,3.0,
			16.0,0.0, 16.0,0.0,

			8.0, 3.0, 8.0, 3.0,
			8.0, 4.0, 8.0, 4.0,
			9.0, 4.0, 9.0, 4.0,
			9.0, 3.0, 9.0, 3.0
		}
	}// vector_with_text
};//vector

class entidade{
	vector_quad_text element = {
		0.0, 0.0,		0.0, 0.0,
		0.0, 1.0,		0.0, 1.0,
		1.0, 1.0,		1.0, 1.0,
		1.0, 0.0,		1.0, 0.0,
		spritesname::persparado01,
		{0.5, 3.0}
	};
	position lastpos = element.pos;

	public:
	const double* getVector(){return element.vector;};
	const vector_quad_text& getElement(){return element;}
	int& texture(){return element.text_index;}
	void onColision(){element.pos = lastpos;}
	void move(double x = 0.0, double y = 0.0){
		lastpos = element.pos;
		element.pos.x += x;
		element.pos.y += y;
	}
	bool colide = 0;
	//void 
};
entidade pers;
vector_quad_text quad = {
	//pos			text pos
	0.0, 0.0,		0.0, 0.0,
	0.0, 1.0,		0.0, 1.0,
	1.0, 1.0,		1.0, 1.0,
	1.0, 0.0,		1.0, 0.0,
	//texture
	sprites::rgba
};
void drawn_pointer() {
	glEnable(GL_BLEND);// to use transparency
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);// to use transparency
	for(auto it = scene_box.begin(); it != scene_box.end(); ++it)
		drawn_triang_with_texture(*it);
	//brick texture
	/*static vector_quad_text brick = {
		//pos			text pos
		0.0, 0.0,		0.0, 0.0,
		0.0, 2.0,		0.0, 1.0,
		2.0, 2.0,		1.0, 1.0,
		2.0, 0.0,		1.0, 0.0,
		//texture
		sprites::brick
	};
	drawn_quad_with_texture(brick);*/
	
	drawn_quad_with_texture(pers.getElement());//global var

	quad.pos = mouse::pos;
	for(auto it = scene_box.begin(); it != scene_box.end(); ++it)
	if(colide(quad, *it) || colide(quad, pers.getElement()))
		quad.text_index = sprites::brick;
	else
		quad.text_index = sprites::rgba;
	drawn_quad_with_texture(quad);
	glDisable(GL_BLEND);
	if (pressed_mouse)
		glColor4d(1.0, 0.0, 0.0, 1.0);
	else
		glColor4d(1.0, 1.0, 1.0, 1.0);
	if(pers.colide)
		glColor4d(0.5, 0.0, 1.0, 1.0);
	glPointSize(10.0f);
	glBegin(GL_POINTS);
	glVertex2d(mouse::pos.x, mouse::pos.y);
	glEnd();
}
void render() {
	screen::calcview();
	mouse::getcord();
	drawn_pointer();
}

//std::chrono::steady_clock::time_point;
double millis;
unsigned char key_pressed;
void logica() {
	static std::chrono::steady_clock::time_point t = std::chrono::steady_clock::now(), clock = t;
	if(t < std::chrono::steady_clock::now()){
		t += std::chrono::milliseconds(100);
		int& pers_tex = pers.texture();
		pers_tex++;
		if(pers_tex > spritesname::persandando04){
			pers_tex = spritesname::persparado01;
		}
	}
	if(key_pressed == 'a') pers.move(-0.125*millis);
	else if(key_pressed == 'd') pers.move( 0.125*millis);
	key_pressed = 0;
	if(colide(pers.getElement(), scene_box[0]) | colide(pers.getElement(), quad)){
		pers.onColision();
	}
	millis = ((std::chrono::steady_clock::now() - clock).count()/10e5)/13;
	clock = std::chrono::steady_clock::now();
	glutPostRedisplay();
}
void Teclado_press(unsigned char key, int x, int y) {
	(void)x;
	(void)y;
	if (key == 27) exit(0);
	else if(key == 'w') screen::camz -= 0.5;
	else if(key == 's') screen::camz += 0.5;
	else/* if(key == 'a') pers.move(-0.125*millis);
	else if(key == 'd') pers.move( 0.125*millis);*/
	key_pressed = key;
	std::cout << "millis " << millis << std::endl;
}
void Teclado_spec(int key, int x, int y) {
	(void)x;
	(void)y;
	if(key == 11) fscr_toggle();
	else if(key == GLUT_KEY_UP) screen::camy += 0.5;
	else if(key == GLUT_KEY_DOWN) screen::camy -= 0.5;
	else if(key == GLUT_KEY_RIGHT) screen::camx += 0.5;
	else if(key == GLUT_KEY_LEFT) screen::camx -= 0.5;
	std::cout << "camx " << screen::camx << " camy " << screen::camy << std::endl;
}

void mouse_click(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			pressed_mouse = 1;
		} else if (state == GLUT_UP) {
			glColor4d(1.0, 1.0, 1.0, 1.0);
			pressed_mouse = 0;
		}
		mouse_move(x, y);
	}
}

void mouse_move(int x, int y) {
	mouse::x = x;
	mouse::y = y;
}

int main(int argc, char** argv) {
	start_gl(argc, argv);
}

void Inicializa(void) {
	glClearColor(0.1f, 0.0f, 0.3f, 1.0f);
	screen::camx = 5.5;
	screen::camy = 5.0;
	//std::cout << "loading" << std::endl;
	loadassets();
	//std::cout << "loaded" << std::endl;
}