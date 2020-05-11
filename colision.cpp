// -lglut -lGL -lGLU -lm
#define _MAIN
#include <iostream>
#include <chrono>
#include "graphics.hpp"
#include "assets.hpp"
#include "loadassets.hpp"
#include "basic_interaction.hpp"
#include "physics.hpp"
#include "elements.hpp"
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
	bool w = 0, a = 0, s = 0, d = 0, space = 0, F1 = 0;
}
namespace screen{
	void camera_follow(double px){
		if(px < camx - viewx * 0.4){
			camx = px + (viewx * 0.4);
		}
		if(px > camx + viewx * 0.4){
			camx = px - (viewx * 0.4);
		}
		calcview();
		return;
	}
}

std::vector<vertex_with_text *> scene_box = {
	&levelone::scene_box_one
};//vector
extern double millis;
player &pers = player::get();
vertex_with_text quad = {
	//texture
	sprites::rgba,
	GL_QUADS,
	//pos			text pos
	{
		0.0, 0.0,		0.0, 0.0,
		1.0, 0.0,		1.0, 0.0,
		1.0, 1.0,		1.0, 1.0,
		0.0, 1.0,		0.0, 1.0,
	}
};
void drawn_pointer() {
	glEnable(GL_BLEND);// to use transparency
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);// to use transparency
	drawn_bcg();
	drawn_bcg2();
	drawn_bcg3();
	for(auto it = scene_box.begin(); it != scene_box.end(); ++it)
		drawn_with_texture(**it);
	drawn_with_texture(pers.getElement());//global var
	//remove quad
	quad.pos = mouse::pos; 
	for(auto it = scene_box.begin(); it != scene_box.end(); ++it)
	if(colide(quad, **it) || colide(quad, pers.getElement()))//quad colision
		quad.text_index = sprites::brick;
	else
		quad.text_index = sprites::rgba;
	drawn_with_texture(quad);
	//glDisable(GL_BLEND);//disable transparency
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
	if(keyboard::F1){
		static text_params help_params(GL_LINEAR, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
		static vertex_with_text help{
			sprites::help,
			GL_QUADS,
			{
				-screen::viewy, -screen::viewy, 0.0, 1.0 - 1024.0/303.0,
				 screen::viewy, -screen::viewy, 1.0, 1.0 - 1024.0/303.0,//image aspect 1024/303
				 screen::viewy,  screen::viewy, 1.0, 1.0,
				-screen::viewy,  screen::viewy, 0.0, 1.0
			},
			{screen::camx, screen::camy},
			&help_params
		};
		help.pos.x = screen::camx;
		help.pos.y = screen::camy;
		drawn_with_texture(help);
	}
}
void render() {
	drawn_pointer();
}

std::vector<vertex_with_text*> colision_static = {scene_box[0], &quad};//colision objects
double millis;
void logica() {
	static std::chrono::steady_clock::time_point text_anim = std::chrono::steady_clock::now(), clock = text_anim, frame_clock;
	if(text_anim < std::chrono::steady_clock::now()){
		text_anim += std::chrono::milliseconds(100);
		GLuint& pers_tex = pers.texture();
		pers_tex++;
		if(pers_tex > spritesname::persandando04){
			pers_tex = spritesname::persparado01;
		}
		//std::cout << 'x' << pers.getElement().pos.x << 'y' << pers.getElement().pos.y << std::endl;
	}
	if (keyboard::a) pers.move(-phy::moveVel * millis);
	if (keyboard::d) pers.move( phy::moveVel * millis);
	if (keyboard::space) pers.jump();
	millis = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - clock).count()/13000000;
	//std::cout << 'M' << millis << std::endl;
	clock = std::chrono::steady_clock::now();
	physics();//colision physics
	if(pers.getElement().pos.y < 0.5){
		//pers.onColision();
		//pers.onhorizontalColision();
		pers.respawn();
	}
	screen::camera_follow(pers.getElement().pos.x);
	mouse::getcord();
	glutPostRedisplay();
}

void Teclado_press(unsigned char key, int x, int y) {
	mouse_move(x, y);
	if (key == 27) close();
	else if(key == 'w') screen::camz -= 0.5;
	else if(key == 's') screen::camz += 0.5;
	else if (key == 'a') keyboard::a = 1;
	else if (key == 'd') keyboard::d = 1;
	else if (key == ' ') keyboard::space = 1;
}
void Teclado_press_up(unsigned char key, int x, int y) {
	mouse_move(x, y);
	if (key == 'a') keyboard::a = 0;
	else if (key == 'd') keyboard::d = 0;
	else if (key == ' ') keyboard::space = 0;
}
void Teclado_spec(int key, int x, int y) {
	mouse_move(x, y);
	if(key == GLUT_KEY_F11) fscr_toggle();
	else if(key == GLUT_KEY_F1) keyboard::F1 ^= 1;
	else if(key == GLUT_KEY_UP) screen::camy += 0.5;
	else if(key == GLUT_KEY_DOWN) screen::camy -= 0.5;
	else if(key == GLUT_KEY_RIGHT) screen::camx += 0.5;
	else if(key == GLUT_KEY_LEFT) screen::camx -= 0.5;
	//std::cout << "Key " << key << std::endl;
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