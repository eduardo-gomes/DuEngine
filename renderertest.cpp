#include "scenes.hpp"
#include "random.hpp"

namespace scene {
renderertest::renderertest() : renderer(){}
renderertest::~renderertest(){
}
static int elementsToDrawn = 100;
void renderertest::Render(){
	vec3f pos = {0.0f, 0.0f, 0.0f};
	vec2f size = {0.5f, 0.5f};
	vec4f color = {0.0f, 1.0, 0.2f, 1.0f};
	//renderer.DrawnQuad(pos, color, size);
	for (int i = 0; i < elementsToDrawn; ++i) {
		pos = {(Random::getf() - 0.5f) * 4.0f, (Random::getf() - 0.5f) * 2.0f, 0.0f};
		size = {0.005f, 0.005f};
		color = {Random::getf(), Random::getf(), Random::getf(), 1.0f};
		renderer.DrawnQuad(pos, color, size);
	}
	renderer.Drawn();
}
void renderertest::RenderGUI(){
	ImGui::Begin("Renderertest");
	ImGui::SliderInt("Elements to drawn", &elementsToDrawn, 10, 100010);
	static bool Info = 1;
	ImGui::Checkbox("Info", &Info);
	ImGui::End();
	if(Info) renderer.DispInfo();
}
}