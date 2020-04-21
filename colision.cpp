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

vector_triangs_text vector_floor = {
	sprites::brick,
	{
		0.0, 0.0, 0.0, 0.0,
		0.0, 3.0, 0.0, 3.0,
		16.0,3.0, 16.0,3.0,

		8.0, 3.0, 8.0, 3.0,
		9.0, 3.0, 9.0, 3.0,
		9.0, 4.0, 9.0, 4.0,
		
		9.0, 4.0, 9.0, 4.0,
		8.0, 4.0, 8.0, 4.0,
		8.0, 3.0, 8.0, 3.0,
		
		0.0, 0.0, 0.0, 0.0,
		16.0,0.0, 16.0,0.0,
		16.0,3.0, 16.0,3.0
	}
};

static vector_quad_text pers = {
		0.5,  0.5,		1.0, 1.0,
		0.5, -0.5,		1.0, 0.0,
		-0.5,-0.5,		0.0, 0.0,
		-0.5, 0.5,		0.0, 1.0,
		spritesname::persparado01,
		{1.0, 3.5}
};
void drawn_pointer() {
	glEnable(GL_BLEND);// to use transparency
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);// to use transparency
	drawn_triang_with_texture(vector_floor);
	//brick texture
	static vector_quad_text brick = {
		//pos			text pos
		0.25, -0.5,		1.0, 1.0,
		0.25, -1.0,		1.0, 0.0,
		-0.25,-1.0,		0.0, 0.0,
		-0.25,-0.5,		0.0, 1.0,
		//texture
		sprites::brick
	};
	drawn_quad_with_texture(brick);
	
	drawn_quad_with_texture(pers);//global var

	glDisable(GL_BLEND);
	if (pressed_mouse)
		glColor4d(1.0, 0.0, 0.0, 1.0);
	else
		glColor4d(1.0, 1.0, 1.0, 1.0);
	glPointSize(10.0f);
	glBegin(GL_POINTS);
	mouse::getcord();
	glVertex2d(mouse::pos.x, mouse::pos.y);
	glVertex2d(0.0, 0.0);
	glVertex2d(screen::camx + screen::viewx, screen::camy + screen::viewy);
	glVertex2d(screen::camx + screen::viewx, screen::camy - screen::viewy);
	glVertex2d(screen::camx - screen::viewx, screen::camy - screen::viewy);
	glVertex2d(screen::camx - screen::viewx, screen::camy + screen::viewy);
	glEnd();
}
void render() {
	screen::calcview();
	drawn_pointer();
}
void logica() {
	static std::chrono::steady_clock::time_point t = std::chrono::steady_clock::now();
	if(t < std::chrono::steady_clock::now()){
		t += std::chrono::milliseconds(100);
		pers.text_index++;
		if(pers.text_index > spritesname::persandando04){
			pers.text_index = spritesname::persparado01;
		}
	}
	
	glutPostRedisplay();
}
void Teclado_press(unsigned char key, int x, int y) {
	(void)x;
	(void)y;
	if (key == 27) exit(0);
	else if(key == 'w') screen::camz -= 0.5;
	else if(key == 's') screen::camz += 0.5;
	else if(key == 'a') pers.pos.x -= 0.125;
	else if(key == 'd') pers.pos.x += 0.125;
	std::cout << "camz " << screen::camz << std::endl;
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
	std::cout << "loading" << std::endl;
	loadassets();
	std::cout << "loaded" << std::endl;
}