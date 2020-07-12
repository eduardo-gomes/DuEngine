#include "scenes.hpp"

namespace scene {
ImGuiIO* io;
void StartImGui() {
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(window::window, window::glcontext);
	ImGui_ImplOpenGL3_Init("#version 330");
}
void StopImGui() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

BaseScene* BaseScene::instance = nullptr;
BaseScene::BaseScene() {
	if (instance != nullptr) {
		delete instance;
	}
	instance = this;
	Renderer::LookAt(0.0f, 0.0f, 2.95f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
}
BaseScene::~BaseScene() {
	instance = nullptr;
}
BaseScene* BaseScene::GetInstance() {
	if (instance == nullptr) instance = new BaseScene;
	return instance;
}

}  // namespace scene