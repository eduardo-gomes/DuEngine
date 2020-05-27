#include "scenes.hpp"

namespace scene {
struct vertex{
	float position[2];
	float color[4];
	float texCord[2];
	float textureID;
};
void GenQuad(vertex *ret, float xsize, float ysize, float xpos, float ypos, int textid = 0){
	xsize /= 2;
	ysize /= 2;
	ret[0] = {-xsize + xpos, -ysize + ypos, 1.0f, 1.0f, 0.2f, 1.0f, 0.0f, 0.0f, (float)textid};
	ret[1] = { xsize + xpos, -ysize + ypos, 0.0f, 1.0f, 1.2f, 1.0f, 1.0f, 0.0f, (float)textid};
	ret[2] = { xsize + xpos,  ysize + ypos, 0.0f, 1.0f, 1.2f, 1.0f, 1.0f, 1.0f, (float)textid};
	ret[3] = {-xsize + xpos,  ysize + ypos, 1.0f, 0.0f, 0.2f, 1.0f, 0.0f, 1.0f, (float)textid};
	return;
	
}
vertex vertices[4] = {
	{-1.0f, -1.0f, 1.0f, 1.0f, 0.2f, 1.0f, 0.0f, 0.0f, 0},
	{ 1.0f, -1.0f, 1.0f, 1.0f, 0.2f, 1.0f, 1.0f, 0.0f, 0},
	{ 1.0f,  1.0f, 1.0f, 1.0f, 0.2f, 1.0f, 1.0f, 1.0f, 0},
	{-1.0f,  1.0f, 1.0f, 1.0f, 0.2f, 1.0f, 0.0f, 1.0f, 0},
};
unsigned int index[]={
	0, 1, 2,
	2, 3, 0,
	4, 5, 6,
	6, 7, 4,
};
renderertest::renderertest() {
	VA = std::make_unique<VertexArray>();
	VB = std::make_unique<VertexBuffer>(sizeof(vertex)*1024);
	VertexBufferLayout VBL;
	VBL.Push(GL_FLOAT, 2, offsetof(vertex, position));
	VBL.Push(GL_FLOAT, 4, offsetof(vertex, color));
	VBL.Push(GL_FLOAT, 2, offsetof(vertex, texCord));
	VBL.Push(GL_FLOAT, 1, offsetof(vertex, textureID));
	VBL.Size(sizeof(vertex));
	IB = std::make_unique<IndexBuffer>(index, sizeof(index)/sizeof(unsigned int));
	VA->AddBuffer(*VB, VBL);
	shader = std::make_unique<Shader>("basic.glsl");
}
renderertest::~renderertest(){
}
float pos[4], size[4] = {1, 1, 1, 1};
void renderertest::Render(){
	Renderer::Perspective(screen::fovy, screen::aspect, 0.01f, 100.0f);
	Renderer::LookAt(screen::camx, screen::camy, 3.0f, screen::camx, screen::camy, -1.0f, 0.0f, 1.0f, 0.0f);
	Renderer::ModelMatrix = mat4f::GenRotate(0.0, 0.0, 0.0);
	Renderer::MVP = Renderer::ProjectionMatrix * Renderer::ViewMatrix * Renderer::ModelMatrix;
	static vertex quad[8];
	static int times = 5000;
	if(times > 0){
		GenQuad(&quad[0], size[0], size[1], pos[0], pos[1]);
		GenQuad(&quad[4], size[2], size[3], pos[2], pos[3]);
		VB->SendData(0, sizeof(quad), quad);
		--times;
	}
	shader->Bind();
	shader->SetUniformMat4f("u_MVP", Renderer::MVP);
	renderer.Drawn(*VA, *IB, *shader);
}
void renderertest::RenderGUI(){
	ImGui::Begin("Quads pos");
	ImGui::SliderFloat2("Quad1 pos", &pos[0], -2.0, 2.0);
	ImGui::SliderFloat2("Quad2 pos", &pos[2], -2.0, 2.0);
	ImGui::SliderFloat2("Quad1 size", &size[0], 0.0, 2.0);
	ImGui::SliderFloat2("Quad2 size", &size[2], 0.0, 2.0);
	static bool Info = 0;
	ImGui::Checkbox("Info", &Info);
	if(Info) Renderer::DispInfo();
	ImGui::End();
}
}