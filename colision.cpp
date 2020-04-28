// -lglut -lGL -lGLU -lm
#define _MAIN
#include <iostream>
#include <chrono>
#include "graphics.hpp"
#include "loadassets.hpp"
#include "elements.hpp"
#include "physics.hpp"
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
namespace keyboard{
	bool w = 0, a = 0, s = 0, d = 0, space = 0;
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
extern double millis;
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
	drawn_bcg();
	drawn_bcg2();
	drawn_bcg3();
	for(auto it = scene_box.begin(); it != scene_box.end(); ++it)
		drawn_triang_with_texture(*it);
	drawn_quad_with_texture(pers.getElement());//global var

	quad.pos = mouse::pos;
	for(auto it = scene_box.begin(); it != scene_box.end(); ++it)
	if(colide(quad, *it) || colide(quad, pers.getElement()))//quad colision
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
	//mouse::getcord(); //moved to before physics() call
	drawn_pointer();
}

//std::chrono::steady_clock::time_point;
std::vector<vector_with_text*> colision_static = {&scene_box[0]};//colision objects
std::vector<vector_quad_text*> colision_static_quad = {&quad};//colision objects
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
		//std::cout << 'x' << pers.getElement().pos.x << 'y' << pers.getElement().pos.y << std::endl;
	}
	if (keyboard::a) pers.move(-phy::moveVel * millis);
	if (keyboard::d) pers.move( phy::moveVel * millis);
	if (keyboard::space) pers.jump();
	key_pressed = 0;
	mouse::getcord();
	physics();//colision physics
	millis = ((std::chrono::steady_clock::now() - clock).count()/10e5)/13;
	clock = std::chrono::steady_clock::now();
	millis = 1.0;
	if(pers.getElement().pos.y < 0.5){
		pers.onColision();
		pers.onhorizontalColision();
	}
	glutPostRedisplay();
}
void Teclado_press(unsigned char key, int x, int y) {
	(void)x;
	(void)y;
	if (key == 27) exit(0);
	else if(key == 'w') screen::camz -= 0.5;
	else if(key == 's') screen::camz += 0.5;
	else if (key == 'a') keyboard::a = 1;
	else if (key == 'd') keyboard::d = 1;
	else if (key == ' ') keyboard::space = 1;
	else/* if(key == 'a') pers.move(-0.125*millis);
	else if(key == 'd') pers.move( 0.125*millis);*/
	key_pressed = key;
	//std::cout << "millis " << millis << std::endl;
}
void Teclado_press_up(unsigned char key, int x, int y) {
	(void)x;
	(void)y;
	if (key == 'a') keyboard::a = 0;
	else if (key == 'd') keyboard::d = 0;
	else if (key == ' ') keyboard::space = 0;
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