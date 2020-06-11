#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>

#include <graphics/glmath.hpp>
extern void render();
extern void Inicializa();
//extern void OnWindowResize(double fovy, double aspect); //constant fov
namespace window {
extern bool quit;
void toggle_fullscreen();
}  // namespace window

namespace screen {
extern int width, height;
extern double aspect, &x, y;
extern double camx, camy, camz;
extern double bcgDist, bcgViewy, bcgViewx, bcg2Dist, bcg2Viewy, bcg2Viewx, bcg3Dist, bcg3Viewy, bcg3Viewx;
extern double fovy;
extern double viewx, viewy;
void calcview();
}  // namespace screen

namespace mouse {
extern int x, y, pressed;
void motion_event(const SDL_Event& e);
void button_event(const SDL_Event& e);
}  // namespace mouse
void test_sound();
namespace keyboard {
extern bool w, a, s, d, backslash, space, F1;
extern uint16_t mod;
void event(const SDL_Event& e);
}  // namespace keyboard
namespace window {
extern bool quit;
extern SDL_Window* window;
extern SDL_GLContext glcontext;

void reshapeWindow();
void reshapeWindow(int w, int h);

void toggle_fullscreen();

bool init_window();

extern SDL_Surface* rgbbmp;
void close_window();


void DesenhaNaTela(void);

void MainLoop();

extern int init();

}  // namespace window

bool ClearErrors();
void PrintAllErrors(const char*, const char*, int);
#define gltry(X) X;PrintAllErrors(#X, __FILE__, __LINE__);
#include <csignal>
#define ASSERT(X) if (!(X)) raise(SIGTRAP);
