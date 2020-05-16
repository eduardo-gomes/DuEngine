// -lglut -lGL -lGLU -lm
#define _MAIN
#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <list>
#include "audio.hpp"
#include "graphics.hpp"
#include "assets.hpp"
#include "loadassets.hpp"
#include "basic_interaction.hpp"
#include "physics.hpp"
#include "elements.hpp"
//#define NO_VSYNC
namespace mouse {
	extern int x, y, pressed;
	position pos;
	void getcord() {
		//(x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
		pos.x = x * ((screen::camx + screen::viewx) - (screen::camx - screen::viewx)) / screen::width + (screen::camx - screen::viewx);
		pos.y = y * ((screen::camy - screen::viewy) - (screen::camy + screen::viewy)) / screen::height + (screen::camy + screen::viewy);
	}
}  // namespace mouse
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
std::list<coins> coins_list;
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
	for(auto it = coins_list.begin(); it != coins_list.end(); ++it)
		drawn_with_texture(it->getElement());
	//glDisable(GL_BLEND);//disable transparency
	if (mouse::pressed)
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
std::mutex game_state;

void render() {
	#ifdef NO_VSYNC
	static std::chrono::steady_clock::time_point last = std::chrono::steady_clock::now();
	static int times = 0;
	if(++times >= 1000){
		times = 0;
		printf("FPS: %lf\n", 1000*1e9/((double)std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - last).count()));
		last = std::chrono::steady_clock::now();
	}
	#endif
	game_state.lock();
	drawn_pointer();
	game_state.unlock();
	//glutPostRedisplay();
}

std::vector<vertex_with_text*> colision_static = {scene_box[0], &quad};//colision objects
double millis;
void logica() {
	static std::chrono::steady_clock::time_point text_anim = std::chrono::steady_clock::now(), clock = text_anim, logica_clock;
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
	std::this_thread::sleep_until(logica_clock);
	millis = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - clock).count()/13000000;
	//std::cout << 'M' << millis << std::endl;
	logica_clock = clock = std::chrono::steady_clock::now();
	logica_clock += std::chrono::milliseconds(1);
	game_state.lock();
	physics();//colision physics
	if(pers.getElement().pos.y < 0.5){
		//pers.onColision();
		//pers.onhorizontalColision();
		pers.respawn();
	}
	screen::camera_follow(pers.getElement().pos.x);
	mouse::getcord();
	game_state.unlock();
}

void logica_loop(){
	while(!window::quit)
		for(int i = 0; i < 16; ++i)
			logica();
}
std::thread logica_loop_thread;
void close() {
	window::quit = 1;
	if(logica_loop_thread.joinable())logica_loop_thread.join();
}
//bool &quit = window::quit;
int main(int argc, char** argv) {
	//Instrumentor::Get().BeginSession("Session");
	//start_gl(argc, argv);
	(void)argc;
	(void)argv;
	//std::thread music;
	if(!audio::init())
	if (window::init()) {
		//music = std::thread(play_sound);
		#ifdef NO_VSYNC
		SDL_GL_SetSwapInterval(0);
		#endif
		window::MainLoop();
	}
	audio::close();
	//music.join();
	close();
	//Instrumentor::Get().EndSession();
}

void Inicializa(void) {
	glClearColor(0.1f, 0.0f, 0.3f, 1.0f);
	screen::camx = 5.5;
	screen::camy = 5.0;
	//std::cout << "loading" << std::endl;
	loadassets();
	loadaudios();
	//std::cout << "loaded" << std::endl;
	logica_loop_thread = std::thread(logica_loop);
	coins_list.emplace_back(position(7.0, 5.0));
	coins_list.emplace_back(position(10.0, 5.0));
	for(int i = 0; i < 60; i+= 2)
		coins_list.emplace_back(position(15.0+i, 5.15));
}
