#include "graphics.hpp"
#define PI 3.1415926535897932
extern void render();
extern void Inicializa();
void GLAPIENTRY MessageCallback(GLenum source,
								GLenum type,
								GLuint id,
								GLenum severity,
								GLsizei length,
								const GLchar *message,
								const void *userParam) {
	(void) source;
	(void)id;
	(void)length;
	(void)userParam;
	if(type == GL_DEBUG_TYPE_ERROR){
		fprintf(stderr, "[GL CALLBACK]: %s type = 0x%x, severity = 0x%x, message = %s\n",
				(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
				type, severity, message);
		ASSERT(0);
	}
}

namespace window {
extern bool quit;
void toggle_fullscreen();
}  // namespace window

namespace screen {
int width, height;
double aspect, &x = aspect, y = 1.0;
double camx = 0.0, camy = 0.0, camz = 8.5;
double bcgDist = -15.0, bcgViewy, bcgViewx, bcg2Dist = -7.0, bcg2Viewy, bcg2Viewx, bcg3Dist = -3.0, bcg3Viewy, bcg3Viewx;
double fovy = 45.0;
double viewx, viewy;
void calcview() {
	viewy = camz * tan(fovy / 2 * PI / 180);
	viewx = aspect * viewy;
	bcgViewy = (camz - bcgDist) * tan(fovy / 2 * PI / 180);  // the bcg will be at z = bcgDist
	bcgViewx = aspect * bcgViewy;
	bcg2Viewy = (camz - bcg2Dist) * tan(fovy / 2 * PI / 180);  // the bcg2 will be at z = bcg2Dist
	bcg2Viewx = aspect * bcg2Viewy;
	bcg3Viewy = (camz - bcg3Dist) * tan(fovy / 2 * PI / 180);  // the bcg3 will be at z = bcg3Dist
	bcg3Viewx = aspect * bcg3Viewy;
}
}  // namespace screen

namespace mouse {
int x = 0, y = 0, pressed;
void motion_event(const SDL_Event &e) {
	const SDL_MouseMotionEvent &motion = e.motion;
	x = motion.x;
	y = motion.y;
}
void button_event(const SDL_Event &e) {
	const SDL_MouseButtonEvent &button = e.button;
	pressed = button.state == SDL_PRESSED;
	//printf("Mouse button %hhu, state %hhu, x %d, y %d\n", button.button, button.state, button.x, button.y);
	x = button.x;
	y = button.y;
}
}  // namespace mouse
void test_sound();
namespace keyboard {
bool w = 0, a = 0, s = 0, d = 0, backslash = 0, space = 0, F1 = 0;
uint16_t mod = KMOD_NONE;
void event(const SDL_Event &e) {  // Keyboard event handler
								  /*
		Uint32 	type		the event type; SDL_KEYDOWN or SDL_KEYUP
		Uint32	timestamp	timestamp of the event
		Uint32	windowID	the window with keyboard focus, if any
		Uint8	state		the state of the key; SDL_PRESSED or SDL_RELEASED
		Uint8	repeat		non-zero if this is a key repeat
		SDL_Keysym	keysym	the SDL_Keysym representing the key that was pressed or released
			SDL_Scancode	scancode	SDL physical key code; see SDL_Scancode for details
			SDL_Keycode		sym			SDL virtual key code; see SDL_Keycode for details
			Uint16			mod			current key modifiers; see SDL_Keymod for details
		*/
	const SDL_KeyboardEvent &key = e.key;
	//printf("KEY type %u, timestamp %u, windowID %u, state %hhu, repeat %hhu, keycode %d, mod %hu\n", key.type, key.timestamp, key.windowID, key.state, key.repeat, key.keysym.sym, key.keysym.mod);
	bool state = key.state == SDL_PRESSED;
	mod = key.keysym.mod;
	if (!key.repeat) switch (key.keysym.sym) {
			case SDLK_ESCAPE:
				window::quit = true;
				break;
			case SDLK_a:
				a = state;
				break;
			case SDLK_d:
				d = state;
				break;
			case SDLK_s:
				s = state;
				break;
			case SDLK_w:
				w = state;
				break;
			case SDLK_BACKSLASH:
				backslash = state;
				break;
			case SDLK_SPACE:
				space = state;
				break;
			case SDLK_F1:
				F1 = state;
				break;
			case SDLK_F11:
				if (state) window::toggle_fullscreen();
				break;
		}
}
}  // namespace keyboard
namespace window {
// Screen dimension constants
const int dfwidth = 720 * 16 / 9, dfheight = 720;
const char window_name[] = "GL game";
bool quit = false;
// The window we'll be rendering to
SDL_Window *window = NULL;
// Event handler
SDL_Event event;

SDL_GLContext glcontext;
void reshapeWindow() {
	SDL_GL_MakeCurrent(window, glcontext);
	/*if (h == 0)
		h = 1;*/
	// screen::height = h;
	// screen::width = w;
	screen::aspect = (double)screen::width / screen::height;
	glViewport(0, 0, screen::width, screen::height);
	//OnWindowResize(screen::fovy, screen::aspect); //constant fov
}
void reshapeWindow(int w, int h) {
	screen::height = h;
	screen::width = w;
	reshapeWindow();
}

void toggle_fullscreen() {
	static bool windowed = 1;
	windowed = !windowed;
	int i = SDL_GetWindowDisplayIndex(window);
	if (windowed) {
		screen::width = dfwidth;
		screen::height = dfwidth;
		if (SDL_SetWindowFullscreen(window, 0))
			printf("Window could not be resized! SDL_Error: %s\n", SDL_GetError());
	} else {
		SDL_Rect j;
		SDL_GetDisplayBounds(i, &j);
		screen::width = j.w;
		screen::height = j.h;
		if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP))
			printf("Window could not be resized! SDL_Error: %s\n", SDL_GetError());
	}
	// glinit_reshape();
}

bool init_window() {
	bool fail = 0;
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	} else {
		SDL_GL_LoadLibrary(NULL);
		// OpenGL 4.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		// Create window
		window = SDL_CreateWindow(window_name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, dfwidth, dfheight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		if (window == NULL) {
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			return fail = true;
		}

		// Create context
		glcontext = SDL_GL_CreateContext(window);
		if (glcontext == NULL) {
			printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
			return fail = true;
		}
		if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
			printf("Failed to initialize OpenGL context");
			return fail = true;
		}
		/*//Use Vsync
		if (SDL_GL_SetSwapInterval(1) < 0){
			printf("Warning: Unable to set VSync! SDL Error: %s\n",
		SDL_GetError());
		}*/
		// Initialize OpenGL
		reshapeWindow(dfwidth, dfheight);
		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {
			printf("Error initializing OpenGL! 0x%0X\n", error);
			return fail = true;
		}
		// printf("OpenGL %s\n", glGetString(GL_VERSION));
		printf("GPU Vendor: %s\n", glGetString(GL_VENDOR));
		printf("GPU       : %s\n", glGetString(GL_RENDERER));
		printf("GL Version: %s\n", glGetString(GL_VERSION));
		//printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);
		// During init, enable debug output ////////////////////////////////DEBUG OPENGL
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(MessageCallback, 0);

		/*if (GLAD_GL_EXT_framebuffer_multisample) {
			// GL_EXT_framebuffer_multisample is supported
		}
		if (GLAD_GL_VERSION_3_0) {
			// We support at least OpenGL version 3
		}*/
		Inicializa();
	}
	return fail;
}
SDL_Surface *rgbbmp = NULL;
void close_window() {
	SDL_FreeSurface(rgbbmp);
	rgbbmp = NULL;
	SDL_GL_DeleteContext(glcontext);
	glcontext = NULL;
	// Destroy window
	SDL_DestroyWindow(window);
	window = NULL;

	// Quit SDL subsystems
	SDL_Quit();
}

void DesenhaNaTela(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	render();
	glFinish();
	SDL_GL_SwapWindow(window);
}

void MainLoop() {
	while (!quit) {
		// Handle events on queue
		while (SDL_PollEvent(&event) != 0) {
			switch (event.type) {
				case SDL_QUIT:
					quit = true;
					break;
				// keyboard
				case SDL_KEYDOWN:
				case SDL_KEYUP:
					keyboard::event(event);
					break;
				case SDL_MOUSEMOTION:
					mouse::motion_event(event);
					break;
				case SDL_MOUSEBUTTONUP:
				case SDL_MOUSEBUTTONDOWN:
					mouse::button_event(event);
					break;
				case SDL_WINDOWEVENT:
					if (event.window.event == SDL_WINDOWEVENT_RESIZED)
						reshapeWindow(event.window.data1, event.window.data2);
					break;
			}
		}
		DesenhaNaTela();
	}
}

int init() { return init_window(); }
}  // namespace window

/*---------------------------Render----------------------------*/
#include <csignal>
bool ClearErrors() {
	bool ret = 0;
	while (glGetError() != GL_NO_ERROR) {
		ret = 1;
	};
	return ret;
}
void PrintAllErrors(const char *fun, const char *file, int line) {
	while (GLenum error = glGetError()) {
		printf("[GL Error]: %0X from: %s on file: %s on line %d\n", error, fun, file, line);
		raise(SIGTRAP);
	}
}
