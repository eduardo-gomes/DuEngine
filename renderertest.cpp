#include "scenes.hpp"
#include "random.hpp"

namespace scene {
struct vertex{
	float position[2];
	float color[4];
	float texCord[2];
	float textureID;
};
struct index{
	unsigned int index[6];
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
#define N_TEST_QUADS 1000000
vertex quads[N_TEST_QUADS*4];
index inde [N_TEST_QUADS];
renderertest::renderertest() {
	VA = std::make_unique<VertexArray>();
	VB = std::make_unique<VertexBuffer>(sizeof(vertex)*4*N_TEST_QUADS);
	VertexBufferLayout VBL;
	VBL.Push(GL_FLOAT, 2, offsetof(vertex, position));
	VBL.Push(GL_FLOAT, 4, offsetof(vertex, color));
	VBL.Push(GL_FLOAT, 2, offsetof(vertex, texCord));
	VBL.Push(GL_FLOAT, 1, offsetof(vertex, textureID));
	VBL.Size(sizeof(vertex));
	VA->AddBuffer(*VB, VBL);
	shader = std::make_unique<Shader>("basic.glsl");
	for(unsigned int i = 0; i < N_TEST_QUADS; ++i){
		GenQuad(&quads[i*4], 0.005, 0.005, (Random::getf()-.5)*3.5, (Random::getf()-.5)*2.25);
		inde[i] = {0 + (i*4), 1 + (i*4), 2 + (i*4), 2 + (i*4), 3 + (i*4), 0 + (i*4)};
	}
	IB = std::make_unique<IndexBuffer>(inde[0].index, sizeof(inde) / sizeof(unsigned int));
	VB->SendData(0, sizeof(quads), quads);
	//delete[] quads;
	//delete [] inde;
	//quads = NULL;
	//inde = NULL;
}
renderertest::~renderertest(){
}
float pos[4], size[4] = {1, 1, 1, 1};
static bool SendEachFrame = 0;
void renderertest::Render(){
	Renderer::Perspective(screen::fovy, screen::aspect, 0.01f, 100.0f);
	Renderer::LookAt(screen::camx, screen::camy, 3.0f, screen::camx, screen::camy, -1.0f, 0.0f, 1.0f, 0.0f);
	Renderer::ModelMatrix = mat4f::GenRotate(0.0, 0.0, 0.0);
	Renderer::MVP = Renderer::ProjectionMatrix * Renderer::ViewMatrix * Renderer::ModelMatrix;
	static vertex quad[8];
	static int times = 5000;
	if (SendEachFrame) VB->SendData(0, sizeof(quads), quads);
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
	ImGui::SliderFloat2("Quad0 pos", &pos[0], -2.0, 2.0);
	ImGui::SliderFloat2("Quad1 pos", &pos[2], -2.0, 2.0);
	ImGui::SliderFloat2("Quad0 size", &size[0], 0.0, 2.0);
	ImGui::SliderFloat2("Quad1 size", &size[2], 0.0, 2.0);
	static bool Info = 0;
	ImGui::Checkbox("Info", &Info);
	ImGui::Checkbox("Send data each frame", &SendEachFrame);
	ImGui::Text("%sDrawing %u  quads each frame", SendEachFrame? "Sending and ":"", N_TEST_QUADS);
	ImGui::End();
	if(Info) Renderer::DispInfo();
}
}