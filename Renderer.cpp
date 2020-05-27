#include "Renderer.hpp"
#include <imgui.h>

void Renderer::DispInfo() {
	static bool vsync = 1;
	ImGui::Begin("Render Info");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	if (ImGui::Checkbox("Vsync", &vsync))
		SDL_GL_SetSwapInterval(vsync);
	ImGui::End();
}

mat4f Renderer::MVP, Renderer::ProjectionMatrix, Renderer::ViewMatrix, Renderer::ModelMatrix;
void Renderer::Drawn(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
	shader.Bind();
	va.Bind();
	ib.Bind();
	gltry(glDrawElements(GL_TRIANGLES, (int)ib.GetCount(), GL_UNSIGNED_INT, NULL));	 //documentation
}
mat4f& Renderer::LookAt(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ){
	return mat4f::GenView(ViewMatrix, eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
}

mat4f& Renderer::Perspective(float fovy, float aspect, float zNear, float zFar) {
	return mat4f::GenPerspective(ProjectionMatrix, fovy, aspect, zNear, zFar);
}