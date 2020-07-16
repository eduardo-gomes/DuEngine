#include "Init.hpp"

#include "audio/audio.hpp"
#include "manager/AssetsManager.hpp"
#include "scenes.hpp"
//void OnWindowResize(double fovy, double aspect){} //constant fov
Renderer *renderer;
//called when window is created
void window::Inicializa() {
	logger::info("StartImGui");
	scene::StartImGui();
	logger::info("StartedImGui");
	renderer = new Renderer;
}
//update with delta in nanoseconds
void window::update(int64_t delta) {
	scene::BaseScene::GetInstance()->Update(delta);
}
void window::render() {	 //called by MainLoop
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
bool Start(const std::string &WindowName, int AUDIO) {
	Man::Manager::Insatance = std::make_unique<Man::Manager>();
	window::OpenglDebugOutput = false;
	if (window::init_window(WindowName.c_str())) {
		if (AUDIO) audio::audioOut = std::make_unique<audio::audio>();
		Setup();
		window::MainLoop();
	} else
		return 0;
	return 1;
}
void CleanUp() {  //MainLoop CallBack
	delete scene::BaseScene::GetInstance();
	scene::StopImGui();
	delete renderer;
	window::close_window();
	audio::audioOut.reset();
	Man::Manager::Insatance.reset();
	Man::log::close();
}
void Stop() {
	window::quit = true;
}
