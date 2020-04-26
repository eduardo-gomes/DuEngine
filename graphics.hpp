// antes : sudo apt - get install freeglut3 - dev
// g++ (arquivo) -lglut -lGL -lGLU -lm
// g++ opengl_space_invader.cpp -o jogo.exe -lfreeglut -lopengl32 -lglu32 -Wl,-subsystem,windows
#include <GL/freeglut.h>

#include <cmath>
#define PI 3.1415926535897932
namespace screen{
	int width, height;
	double aspect, &x = aspect, y = 1.0;
	double camx = 0.0, camy = 0.0, camz = 7.5;
	double fovy = 45.0;
	double viewx, viewy;
	void calcview(){
		viewy = camz * tan(fovy / 2 * PI / 180);
		viewx = aspect * viewy;
	}
}
	const int dfwidth = 1366,
			  dfheight = 768;
bool fullscreen = 0;
void render();
void logica();

void Restaura() {
	glLoadIdentity();
	gluLookAt(screen::camx, screen::camy, screen::camz,
			  screen::camx, screen::camy, 0.0f,
			  0.0f, 1.0f, 0.0f);
}

void DesenhaNaTela(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	Restaura();
	render();
	glFinish();
	glutSwapBuffers();
}

void fscr_toggle() {
	fullscreen = !fullscreen;
	if (fullscreen) {
		glutFullScreen();
	} else {
		glutPositionWindow(20, 20);
		glutReshapeWindow(dfwidth, dfheight);
	}
}
void Teclado_press_up(unsigned char key, int, int);
void Teclado_press(unsigned char key, int, int);
void Teclado_spec(int key, int, int);
void mouse_click(int, int, int, int);
void mouse_move(int, int);

// Inicializa parâmetros de rendering
void Inicializa(void) ;/*{
	glClearColor(0.1f, 0.0f, 0.3f, 1.0f);
}*/

void AlteraTamanhoTela(int w, int h) {
	if (h == 0)
		h = 1;
	screen::aspect = 16.0 / 9;
	screen::height = h;
	screen::width = w;
	if(!fullscreen){
		if (screen::aspect - (w * 1.0 / h) > 0.01) {
			screen::height = h;
			screen::width = (int)(h * screen::aspect);
			glutReshapeWindow(screen::width, screen::height);
		} else if (screen::aspect - (w * 1.0 / h) < -0.01) {
			screen::height = (int)(w / screen::aspect);
			screen::width = w;
			glutReshapeWindow(screen::width, screen::height);
		}
	}/*else*/
	std::cout << screen::width << ' ' << screen::height << ' ' << screen::aspect << std::endl;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, screen::width, screen::height);
	gluPerspective(screen::fovy, screen::aspect, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	
}

void close() {
	glutLeaveMainLoop();
}

void start_gl(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(dfwidth, dfheight);
	screen::aspect = (double)dfwidth / dfheight;
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Game");
	glutDisplayFunc(DesenhaNaTela);
	glutIdleFunc(logica);
	glutKeyboardFunc(Teclado_press);
	glutKeyboardUpFunc(Teclado_press_up);
	glutIgnoreKeyRepeat(1);
	glutSpecialFunc(Teclado_spec);
	glutMouseFunc(mouse_click);
	glutPassiveMotionFunc(mouse_move);
	glutMotionFunc(mouse_move);
	glutReshapeFunc(AlteraTamanhoTela);
	Inicializa();
	if (fullscreen) glutFullScreen();
	glutMainLoop();
}