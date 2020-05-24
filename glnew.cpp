#include <iostream>

#include "GLClasses.hpp"
#include "graphics.hpp"
struct vertex2d {
	float x, y, z, s, t;
};
Render renderer;
unsigned int buffer;
int location;
mat4f ProjectionMatrix;	 // = GenPerspective(screen::fovy, screen::aspect, 0.1, 100.0);
mat4f ViewMatrix, ModelMatrix;
Shader *shader;
VertexArray *va;
IndexBuffer *ib;
void Inicializa() {
	//SDL_GL_SetSwapInterval(0);
	glClearColor(0.1f, 0.0f, 0.3f, 1.0f);
	vertex2d positions[8] = {
		-0.5f,-0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f,-0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

		-0.5f,-0.5f, -1.0f, 1.0f, 0.0f,
		 0.5f,-0.5f, -1.0f, 0.0f, 0.0f,
		 0.5f, 0.5f, -1.0f, 0.0f, 1.0f,
		-0.5f, 0.5f, -1.0f, 1.0f, 1.0f,
	};
	unsigned int indices[] = {
		0, 1, 2,  //front
		2, 3, 0,

		1, 5, 6,  //right
		6, 2, 1,

		0, 4, 3,  //bottom
		3, 1, 0,

		3, 2, 6,  //top
		6, 7, 3,

		4, 0, 3,  //left
		3, 7, 4,

		5, 4, 7,  //back
		7, 6, 5
	};
	va = new VertexArray;
	va->Bind();

	static VertexBuffer vb(positions, 8 * sizeof(vertex2d));
	static VertexBufferLayout layout;
	layout.Push(GL_FLOAT, 3);  //pos
	layout.Push(GL_FLOAT, 2);  //text pos
	va->AddBuffer(vb, layout);

	ib = new IndexBuffer(indices, 36);
	shader = new Shader("basic.shader");
	shader->Bind();
	ModelMatrix = mat4f::GenRotate(0.0f, 1.0f, 1.0f, 1.0f);
	//shader->SetUniformMat4f("u_ModelMatrix", ModelMatrix);
	ViewMatrix = mat4f::GenView(screen::camx, screen::camy, 3.0f, screen::camx, screen::camy, -1.0f, 0.0f, 1.0f, 0.0f);
	//shader->SetUniformMat4f("u_ViewMatrix", ViewMatrix);
	// std::cout << ViewMatrix.matrix[0][0] << " " << ViewMatrix.matrix[0][1] << " " << ViewMatrix.matrix[0][2] << " " << ProjectionMatrix.matrix[0][3] << std::endl;
	// std::cout << ViewMatrix.matrix[1][0] << " " << ViewMatrix.matrix[1][1] << " " << ViewMatrix.matrix[1][2] << " " << ProjectionMatrix.matrix[1][3] << std::endl;
	// std::cout << ViewMatrix.matrix[2][0] << " " << ViewMatrix.matrix[2][1] << " " << ViewMatrix.matrix[2][2] << " " << ProjectionMatrix.matrix[2][3] << std::endl;
	// std::cout << ViewMatrix.matrix[3][0] << " " << ViewMatrix.matrix[3][1] << " " << ViewMatrix.matrix[3][2] << " " << ProjectionMatrix.matrix[3][3] << std::endl;

	ProjectionMatrix = mat4f::GenPerspective(screen::fovy, screen::aspect, 0.01f, 100.0f);
	//shader->SetUniformMat4f("u_ProjectionMatrix", ProjectionMatrix);
	// std::cout << ProjectionMatrix.matrix[0][0] << " " << ProjectionMatrix.matrix[0][1] << " " << ProjectionMatrix.matrix[0][2] << " " << ProjectionMatrix.matrix[0][3] << std::endl;
	// std::cout << ProjectionMatrix.matrix[1][0] << " " << ProjectionMatrix.matrix[1][1] << " " << ProjectionMatrix.matrix[1][2] << " " << ProjectionMatrix.matrix[1][3] << std::endl;
	// std::cout << ProjectionMatrix.matrix[2][0] << " " << ProjectionMatrix.matrix[2][1] << " " << ProjectionMatrix.matrix[2][2] << " " << ProjectionMatrix.matrix[2][3] << std::endl;
	// std::cout << ProjectionMatrix.matrix[3][0] << " " << ProjectionMatrix.matrix[3][1] << " " << ProjectionMatrix.matrix[3][2] << " " << ProjectionMatrix.matrix[3][3] << std::endl;

	static Texture texture("assets/rgb.bmp");
	texture.Bind();
	shader->SetUniform1i("u_Texture", 0);
	glEnable(GL_BLEND);									// to use transparency
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// to use transparency
	glEnable(GL_DEPTH_TEST);
}
void render() {
	ClearErrors();
	static float green = 0.25f, add = 0.05f, rotate = 0.0f, rot = 1.0f;
	if (green <= 0.0 || green >= 1.0) add = -add;
	green += add;
	rotate += rot;
	ModelMatrix = mat4f::GenRotate(rotate, 1.0f, 0.25f, 0.0f);
	//shader->SetUniformMat4f("u_ModelMatrix", ModelMatrix);
	//shader->SetUniform4f("u_color", {0.0f, green, 0.95f, 1.0f});
	//shader->SetUniform4f("u_position", {0.0f, 0.0f, -green, 1.0f});
	static mat4f MVP;
	MVP = ProjectionMatrix * ViewMatrix * mat4f::Translate(ModelMatrix, 0.0f, 0.0f, green);
	shader->SetUniformMat4f("u_MVP", MVP);
	renderer.Drawn(*va, *ib, *shader);
}
int main() {
	if (!window::init()) {
		window::MainLoop();
	}
	window::close_window();
}