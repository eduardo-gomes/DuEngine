#include <iostream>

#include "GLClasses.hpp"
#include "graphics.hpp"
#include "scenes.hpp"
//void OnWindowResize(double fovy, double aspect){} //constant fov
void Inicializa() {
	//SDL_GL_SetSwapInterval(0);
	glClearColor(0.1f, 0.0f, 0.3f, 1.0f);
	glEnable(GL_BLEND);									// to use transparency
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// to use transparency
	glEnable(GL_DEPTH_TEST);
	scene::StartImGui();
	new scene::perstest;
}
#define max(A, B) (A > B ? A : B)
void render() {
	scene::BaseScene::GetInstance()->Render();
	scene::BaseScene::GetInstance()->RenderGUI();
}
int main() {
	if (!window::init()) {
		window::MainLoop();
	}
	delete scene::BaseScene::GetInstance();
	scene::StopImGui();
	window::close_window();
}