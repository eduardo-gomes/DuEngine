#include "Init.hpp"
#include "audio/audio.hpp"
#include "scenes.hpp"
//void OnWindowResize(double fovy, double aspect){} //constant fov
Renderer *renderer;
//called when window is created
void window::Inicializa() {
	scene::StartImGui();
	renderer = new Renderer;
}
//update with delta in nanoseconds
void window::update(int64_t delta){
	scene::BaseScene::GetInstance()->Update(delta);
}
void window::render() {//called by MainLoop
	scene::BaseScene::GetInstance()->Render();

	//New frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(window::window);
	ImGui::NewFrame();
	//Call ImGui scene
	scene::BaseScene::GetInstance()->RenderGUI();
	//Render ImGui Frame
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
bool Start(const std::string &WindowName){
	window::OpenglDebugOutput = false;
	if (window::init_window(WindowName.c_str())) {
		if(!audio::init()) printf("Cant initializate audio\n");
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
