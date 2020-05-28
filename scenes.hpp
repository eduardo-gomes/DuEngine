#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>

#include "Renderer.hpp"

namespace scene {
void StartImGui();
void StopImGui();

class BaseScene {
   private:
	static BaseScene *instance;

   public:
	static BaseScene *GetInstance();
	BaseScene();
	virtual ~BaseScene();

	virtual void Update(double) {}
	virtual void Render() {}
	virtual void RenderGUI() {}
};

class test : public BaseScene {
   public:
	test() {
	}
	~test(){} /*void RenderGUI() /{
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window::window);
		ImGui::NewFrame();
		ImGui::Begin("Hello, world!");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		//ImGui::SliderFloat3("Rotate", (float*)&rotate, 0.0f, 360.0f, "%.2f");
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}*/
	;
};

/*class perstest : public BaseScene {
   private:
	static const float positions[16];
	static const unsigned int index[6];
	VertexArray *TexVA2D;
	VertexBufferLayout *Quad2DLayout;
	VertexBuffer *QuadVB;
	IndexBuffer *QuadPersIB;
	Shader *shader;
	Texture *persquadtex;
	vec3f position = {0.0f, 0.0f, 0.0f};
	vec3f rotate = {0.0f, 0.0f, 0.0f};

   public:
	perstest();
	~perstest();
	void Render();
	void RenderGUI();
};*/

class renderertest : public BaseScene{
   private:
	/*std::unique_ptr<VertexArray> VA;
	std::unique_ptr<VertexBuffer> VB;
	std::unique_ptr<IndexBuffer> IB;
	std::unique_ptr<Shader> shader;*/
	Renderer renderer;

   public:
	renderertest();
	~renderertest();
	void Render();
	void RenderGUI();
};

}  // namespace scene