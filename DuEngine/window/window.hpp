#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>

#include <DuEngine/graphics/glmath.hpp>
namespace window {
//extern void OnWindowResize(double fovy, double aspect); //constant fov

//Drawn each frame
extern void render();
//update with delta in nanoseconds
extern void update(int64_t delta);
extern void Inicializa();
//Exit MainLoop and close window
extern bool quit;
//Set OpenGL Debug MessageCallback at init_window
extern bool OpenglDebugOutput;
//Enter or exit fullScreen
void toggle_fullscreen();
}  // namespace window

namespace screen {
//Window size pixels
extern int width, height;
//Cord window size
extern double aspect, &x, y;
//cam pos
extern double camx, camy, camz;
extern double bcgDist, bcgViewy, bcgViewx, bcg2Dist, bcg2Viewy, bcg2Viewx, bcg3Dist, bcg3Viewy, bcg3Viewx;
//cam FOV
extern double fovy;
//Cord size at z = 0.0
extern double viewx, viewy;
//Calc views cords
void calcview();
}  // namespace screen

namespace mouse {
//mouse cord
extern int x, y;
void motion_event(const SDL_Event& e);
void button_event(const SDL_Event& e);
}  // namespace mouse
void test_sound();
namespace keyboard {
//keys
extern bool w, a, s, d, backslash, space, F1, UP, DOWN, Repeat;
//mod keys bits
extern uint16_t mod;
void event(const SDL_Event& e);
}  // namespace keyboard
namespace window {
extern SDL_Window* window;
extern SDL_GLContext glcontext;

//Need to send new fov to renderer to make it work
void reshapeWindow();
//Need to send new fov to renderer to make it work
void reshapeWindow(int w, int h);

void toggle_fullscreen();

//Create Window with title WindowName
//Return false if fail
bool init_window(const char* windowName = "DuEngine Window");
//Destroy OpenGL context, delete window and Quit SDL
void close_window();

//Drawn called inside MainLoop
//Clear framebuffer and calls render
void Drawn(void);
//Window MainLoop
void MainLoop();

}  // namespace window

//Clear OpenGL errors buffer
bool ClearErrors();
//Print all errors from OpenGL buffer
void PrintAllErrors(const char*, const char*, int);
#define gltry(X) X;PrintAllErrors(#X, __FILE__, __LINE__);
#include <csignal>
#define ASSERT(X) if (!(X)) raise(SIGTRAP);
