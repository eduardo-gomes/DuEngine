#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>

#include <DuEngine/graphics/glmath.hpp>
namespace window {
//extern void OnWindowResize(double fovy, double aspect); //constant fov

//Drawn each frame
extern DUENGINT void render();
//update with delta in nanoseconds
extern DUENGINT void update(int64_t delta);//defined in scenes.cpp
extern DUENGEXT void Inicializa();
//Exit MainLoop and close window
extern DUENGEXT bool quit;
//Set OpenGL Debug MessageCallback at init_window
extern DUENGINT bool OpenglDebugOutput;

}  // namespace window

namespace screen {
//Window size pixels
extern DUENGINT int width, height;
//Cord window size
extern DUENGINT double aspect, &x, y;
//cam pos
extern DUENGINT double camx, camy, camz;
//extern double bcgDist, bcgViewy, bcgViewx, bcg2Dist, bcg2Viewy, bcg2Viewx, bcg3Dist, bcg3Viewy, bcg3Viewx;
//cam FOV
extern DUENGINT double fovy;
//Cord size at z = 0.0
extern DUENGINT double viewx, viewy;
//Calc views cords
void DUENGINT calcview();
}  // namespace screen

namespace mouse {
//mouse cord
extern DUENGINT int x, y;
void DUENGINT motion_event(const SDL_Event& e);
void DUENGINT button_event(const SDL_Event& e);
}  // namespace mouse
namespace keyboard {
//keys
extern DUENGEXT bool w, a, s, d, backslash, space, F1, UP, DOWN, Repeat;
//mod keys bits
extern DUENGEXT uint16_t mod;
void DUENGINT event(const SDL_Event& e);
}  // namespace keyboard
namespace window {
extern DUENGINT SDL_Window* window;
extern DUENGINT SDL_GLContext glcontext;

//Need to send new fov to renderer to make it work
void DUENGINT reshapeWindow();
//Need to send new fov to renderer to make it work
void DUENGINT reshapeWindow(int w, int h);
//Enter or exit fullScreen
void DUENGINT toggle_fullscreen();

//Create Window with title WindowName
//Return false if fail
bool DUENGINT init_window(const char* windowName = "DuEngine Window");
//Destroy OpenGL context, delete window and Quit SDL
void DUENGINT close_window();

//Drawn called inside MainLoop
//Clear framebuffer and calls render
void DUENGINT Drawn(void);
//Window MainLoop
void DUENGINT MainLoop();

}  // namespace window

//Clear OpenGL errors buffer
bool DUENGINT ClearErrors();
//Print all errors from OpenGL buffer
void DUENGINT PrintAllErrors(const char*, const char*, int);
#define gltry(X) X;PrintAllErrors(#X, __FILE__, __LINE__);
#include <csignal>
#define ASSERT(X) if (!(X)) raise(SIGTRAP);
