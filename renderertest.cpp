#include "scenes.hpp"
#include "random.hpp"
#include <chrono>
#include <deque>

namespace scene {
renderertest::renderertest(){}
renderertest::~renderertest(){
}
static int elementsToDrawn = 10000;
void renderertest::Render(){
	vec3f pos = {0.0f, 0.0f, 0.0f};
	vec2f size = {0.5f, 0.5f};
	vec4f color = {0.0f, 1.0, 0.2f, 1.0f};
	//renderer.DrawnQuad(pos, color, size);
	for (int i = 0; i < elementsToDrawn; ++i) {
		pos = {(Random::getf() - 0.5f) * 4.0f, (Random::getf() - 0.5f) * 2.0f, 0.0f};
		size = {0.005f, 0.005f};
		color = {Random::getf(), Random::getf(), Random::getf(), 1.0f};
		renderer->DrawnQuad(pos, color, size);
	}
	renderer->Drawn();
}
void renderertest::RenderGUI(){
	ImGui::Begin("Renderertest");
	ImGui::SliderInt("Elements to drawn", &elementsToDrawn, 10, 300000);
	static bool Info = 1;
	ImGui::Checkbox("Info", &Info);
	if(ImGui::Button("Next"))
		new Tetris;
	ImGui::End();
	if(Info) renderer->DispInfo();
}

renderertestrotate::renderertestrotate() {}
renderertestrotate::~renderertestrotate() {
}
static int SizeCount = 290;
static double ms = 0;
static bool rotate = 1;
void renderertestrotate::Render() {
	static std::chrono::high_resolution_clock::time_point tp = std::chrono::high_resolution_clock::now();
	tp = std::chrono::high_resolution_clock::now();
	vec3f pos = {0.0f, 0.0f, 0.0f};
	vec2f size = {0.5f, 0.5f};
	vec4f color = {0.0f, 1.0, 0.2f, 1.0f};
	static float rotation = 0.0f;
	double increment = 2.0f/(SizeCount);
	rotation+= 1.75f;
	if(rotate) for(double x = -1.0f; x <= 1.0f; x+= increment){
		for (double y = -1.0f; y <= 1.0f; y += increment){
			pos = {(float)x, (float)y, 0.0f};
			size = {(float)increment*0.75f, (float)increment*0.75f};
			color = {0.0f, ((float)x + 1.0f) / 2.0f, ((float)y + 1.0f) / 2.0f, 1.0f};
			renderer->DrawnQuadRotate(pos, color, size, rotation + (float)(x+y)*10);
		}
	}else
		for (double x = -1.0f; x <= 1.0f; x += increment) {
			for (double y = -1.0f; y <= 1.0f; y += increment) {
				pos = {(float)x, (float)y, 0.0f};
				size = {(float)increment * 0.75f, (float)increment * 0.75f};
				color = {0.0f, ((float)x + 1.0f) / 2.0f, ((float)y + 1.0f) / 2.0f, 1.0f};
				renderer->DrawnQuad(pos, color, size);
			}
		}
	renderer->Drawn();
	ms = (double)(std::chrono::high_resolution_clock::now() - tp).count()/1000000;
	tp = std::chrono::high_resolution_clock::now();
}
void renderertestrotate::RenderGUI() {
	ImGui::Begin("Renderertest");
	static std::deque<double> ms120;
	ms120.push_back(ms);
	if(ms120.size() > 120) ms120.pop_front();
	double msmed = 0;
	for(auto x : ms120)
		msmed += x;
	msmed /= ms120.size();
	ImGui::Text("Duration of Render(): %.3lf, cpu max framerate: %.1lf", msmed, 1.0/(msmed/1000));
	ImGui::SliderInt("Elements per side", &SizeCount, 10, 1000);
	ImGui::Checkbox("Rotate quads?", &rotate);
	static bool Info = 1;
	ImGui::Checkbox("Info", &Info);
	if (ImGui::Button("Next"))
		new renderertest;
	ImGui::End();
	if (Info) renderer->DispInfo();
}
}