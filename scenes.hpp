#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>

#include "graphics.hpp"
extern vec3f rotate;
namespace scene {

class BaseScene {
   public:
	BaseScene() {}
	virtual ~BaseScene() {}

	virtual void Update(double) {}
	virtual void Render() {}
	virtual void RenderGUI() {}
};

class test : public BaseScene {
   public:
	test() {
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGui_ImplSDL2_InitForOpenGL(window::window, window::glcontext);
		ImGui_ImplOpenGL3_Init("#version 330");
	}
	~test() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}
	void RenderGUI() {
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window::window);
		ImGui::NewFrame();
		ImGui::Begin("Hello, world!");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::SliderFloat3("Rotate", (float*)&rotate, 0.0f, 360.0f, "%.2f");
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
};

}  // namespace scene