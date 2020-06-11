#include "Init.hpp"

#include "scenes.hpp"
//void OnWindowResize(double fovy, double aspect){} //constant fov
Renderer *renderer;
void window::Inicializa() {//called when window is created
	//SDL_GL_SetSwapInterval(0);
	glClearColor(0.1f, 0.0f, 0.3f, 1.0f);
	glEnable(GL_BLEND);									// to use transparency
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// to use transparency
	//glEnable(GL_DEPTH_TEST);
	scene::StartImGui();
	renderer = new Renderer;
}
void window::render() {//called by MainLoop
	scene::BaseScene::GetInstance()->Render();

	//New frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(window::window);
	ImGui::NewFrame();
	//Call scene
	scene::BaseScene::GetInstance()->RenderGUI();
	//Render Frame
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
extern void Setup();//called Before MainLoop
bool Start(const std::string &WindowName){
	if (!window::init_window(WindowName.c_str())) {
		Setup();
		window::MainLoop();
	}else return 0;
	return 1;
}
void CleanUp(){//MainLoop CallBack
	delete scene::BaseScene::GetInstance();
	scene::StopImGui();
	delete renderer;
	window::close_window();
}
void Stop(){
	window::quit = true;
}
/*int main() {
	if (!window::init_window("Exec")) {
		window::MainLoop();
	}
	delete scene::BaseScene::GetInstance();
	//printf("BaseScene deleted\n");
	scene::StopImGui();
	delete renderer;
	window::close_window();
}*/