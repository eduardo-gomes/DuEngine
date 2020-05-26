#include "scenes.hpp"

namespace scene {
renderertest::renderertest() {
	float vertices[] = {
		-1.0f, -1.0f,
		 1.0f, -1.0f,
		 1.0f,  1.0f,
		-1.0f,  1.0f,
	};
	unsigned int index[]={
		0, 1, 2,
		2, 3, 0,
	};
	VA = std::make_unique<VertexArray>();
	VB = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
	VertexBufferLayout VBL;
	VBL.Push(GL_FLOAT, 2);
	IB = std::make_unique<IndexBuffer>(index, sizeof(index)/sizeof(unsigned int));
	VA->AddBuffer(*VB, VBL);
	shader = std::make_unique<Shader>("basic.glsl");
}
renderertest::~renderertest(){
}
void renderertest::Render(){
	ViewMatrix = mat4f::GenView(screen::camx, screen::camy, 3.0f, screen::camx, screen::camy, -1.0f, 0.0f, 1.0f, 0.0f);
	ProjectionMatrix = mat4f::GenPerspective(screen::fovy, screen::aspect, 0.01f, 100.0f);
	ModelMatrix = mat4f::GenRotate(0.0, 0.0, 0.0);
	MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
	shader->Bind();
	shader->SetUniformMat4f("u_MVP", MVP);
	renderer.Drawn(*VA, *IB, *shader);
}
void renderertest::RenderGUI(){

}
}