#include <imgui.h>
#include <examples/imgui_impl_opengl3.h>
#include <examples/imgui_impl_sdl.h>

#include <DuEngine/graphics/Renderer.hpp>

extern DUENGEXT Renderer *renderer;
namespace scene {
//Create ImGui context
void DUENGINT StartImGui();
//Destroy ImGui context
void DUENGINT StopImGui();

//Class used to create instances
class DUENGEXT BaseScene {
   private:
	static BaseScene *instance;

   public:
	static BaseScene *GetInstance();
	BaseScene();
	virtual ~BaseScene();

	virtual void Update(int64_t) {}
	//Function to call renderer functions
	virtual void Render() {}
	virtual void RenderGUI() {}
};

}  // namespace scene
