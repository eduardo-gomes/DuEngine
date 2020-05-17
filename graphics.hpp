#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include <stdio.h>
#define PI 3.1415926535897932
extern void render();
extern void Inicializa();
namespace window{
	extern bool quit;
	void toggle_fullscreen();
}

namespace screen{
	extern int width, height;
	extern double aspect, &x, y;
	extern double camx, camy, camz;
	extern double bcgDist, bcgViewy, bcgViewx, bcg2Dist, bcg2Viewy, bcg2Viewx, bcg3Dist, bcg3Viewy, bcg3Viewx;
	extern double fovy;
	extern double viewx, viewy;
	void calcview();
}

namespace mouse {
	extern int x , y , pressed;
	void motion_event(const SDL_Event& e);
	void button_event(const SDL_Event& e);
}
void test_sound();
namespace keyboard{
	extern bool w, a, s, d, space, F1;
	extern uint16_t mod;
	void event(const SDL_Event &e);
}
namespace window{
extern bool quit;

void glinit_reshape();
void glinit_reshape(int w, int h);

void toggle_fullscreen();

bool init_window();

extern SDL_Surface *rgbbmp;
void close_window();

void Restaura();


void DesenhaNaTela(void);

void MainLoop();

extern int init();
}
