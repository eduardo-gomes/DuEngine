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
Renderer renderer;

BaseScene* BaseScene::instance = nullptr;
BaseScene::BaseScene() {
	if (instance != nullptr) {
		delete instance;
	}
	instance = this;
}
BaseScene::~BaseScene() {
	instance = nullptr;
}
BaseScene* BaseScene::GetInstance() {
	if (instance == nullptr) instance = new BaseScene;
	return instance;
}

const float perstest::positions[16] = {
	-1.0f, -1.0f, 0.0f, 0.0f,
	 1.0f, -1.0f, 1.0f, 0.0f,
	 1.0f,  1.0f, 1.0f, 1.0f,
	-1.0f,  1.0f, 0.0f, 1.0f,
};
const unsigned int perstest::index[6] = {
	0, 1, 2,
	2, 3, 0,
};
perstest::perstest() {
	TexVA2D = new VertexArray;

	QuadVB = new VertexBuffer(positions, sizeof(positions));

	Quad2DLayout = new VertexBufferLayout;
	Quad2DLayout->Push(GL_FLOAT, 2);  //position
	Quad2DLayout->Push(GL_FLOAT, 2);  //texture cord
	TexVA2D->AddBuffer(*QuadVB, *Quad2DLayout);

	QuadPersIB = new IndexBuffer(index, 6);

	shader = new Shader("basic.glsl");

	persquadtex = new Texture("assets/pers02.bmp");

	ViewMatrix = mat4f::GenView(screen::camx, screen::camy, 3.0f, screen::camx, screen::camy, -1.0f, 0.0f, 1.0f, 0.0f);
	ProjectionMatrix = mat4f::GenPerspective(screen::fovy, screen::aspect, 0.01f, 100.0f);

	io = &ImGui::GetIO();
}
perstest::~perstest() {
	delete TexVA2D;
	delete Quad2DLayout;
	delete QuadVB;
	delete QuadPersIB;
	delete shader;
	delete persquadtex;
}
void perstest::Render() {
	persquadtex->Bind();
	shader->Bind();
	shader->SetUniform1i("u_Texture", 0);

	ModelMatrix = mat4f::GenRotate(rotate.v0, rotate.v1, rotate.v2);  //mat4f::GenRotate(rotate.v0, rotate.v0, 0.0f, 0.0f) * mat4f::GenRotate(rotate.v1, 0.0f, rotate.v1, 0.0f) * mat4f::GenRotate(rotate.v2, 0.0f, 0.0f, rotate.v2);
	MVP = ProjectionMatrix * ViewMatrix * mat4f::Translate(ModelMatrix, position.v0, position.v1, position.v2);
	shader->SetUniformMat4f("u_MVP", MVP);

	renderer.Drawn(*TexVA2D, *QuadPersIB, *shader);
}
void perstest::RenderGUI() {
	ImGui::Begin("Pers");
	ImGui::SliderFloat3("Position", &position.v0, -2.0f, 2.0f);
	ImGui::SliderFloat3("Rotation", &rotate.v0, 0.0f, 360.0f);

	ImGui::End();
}
}  // namespace scene