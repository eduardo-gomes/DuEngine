// -lglut -lGL -lGLU -lm
#define _MAIN
#include <iostream>
#include <chrono>
#include "graphics.hpp"
#include "loadassets.hpp"
bool pressed_mouse = 0;
struct position {
	double x, y;
};
namespace mouse {
int x = -50, y = -50;
position pos;
void getcord() {
	pos.x = (x * (screen::x * 2)) / (screen::width - 1) - screen::x;
	pos.y = (y * (-screen::y * 2)) / (screen::height - 1) + screen::y;
}
}  // namespace mouse
static vector_quad_text pers = {
		0.5,  0.5,		1.0, 1.0,
		0.5, -0.5,		1.0, 0.0,
		-0.5,-0.5,		0.0, 0.0,
		-0.5, 0.5,		0.0, 1.0,
		spritesname::persparado01
};
void drawn_pointer() {
	glEnable(GL_BLEND);// to use transparency
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);// to use transparency
	
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
	/*glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[8]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glColor3d(1.0, 1.0, 1.0);
	glBegin(GL_QUADS);
		glTexCoord2d(1.0, 1.0);glVertex2d( 0.25, -0.5);
		glTexCoord2d(1.0, 0.0);glVertex2d( 0.25, -1.0);
		glTexCoord2d(0.0, 0.0);glVertex2d(-0.25, -1.0);
		glTexCoord2d(0.0, 1.0);glVertex2d(-0.25, -0.5);
	glEnd();
	//pers texture
	glDisable(GL_TEXTURE_2D);*/
	
	/*static vector_quad_text pers = {
		0.5,  0.5,		1.0, 1.0,
		0.5, -0.5,		1.0, 0.0,
		-0.5,-0.5,		0.0, 0.0,
		-0.5, 0.5,		0.0, 1.0,
		spritesname::persparado01
	};*/
	drawn_quad_with_texture(pers);//global var
	/*glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[texture_index % 5 + 1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//texture configured
	glBegin(GL_QUADS);
		glTexCoord2d(1.0, 1.0);glVertex2d( 0.5,  0.5);
		glTexCoord2d(1.0, 0.0);glVertex2d( 0.5, -0.5);
		glTexCoord2d(0.0, 0.0);glVertex2d(-0.5, -0.5);
		glTexCoord2d(0.0, 1.0);glVertex2d(-0.5,  0.5);

		//brick

	glEnd();
	glDisable(GL_TEXTURE_2D);*/

	glDisable(GL_BLEND);
	if (pressed_mouse)
		glColor4d(1.0, 0.0, 0.0, 1.0);
	else
		glColor4d(1.0, 1.0, 1.0, 1.0);
	glPointSize(10.0f);
	glBegin(GL_POINTS);
	mouse::getcord();
	glVertex2d(mouse::pos.x, mouse::pos.y);
	glEnd();
}
void render() {
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
	//std::cout << "Teclado_press function" << key << std::endl;
}
void Teclado_spec(int key, int x, int y) {
	(void)x;
	(void)y;
	std::cout << "Teclado_spec function" << key << std::endl;
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
	std::cout << "loading" << std::endl;
	loadassets();
	std::cout << "loaded" << std::endl;
}