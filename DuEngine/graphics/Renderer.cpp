#include "Renderer.hpp"
#include <imgui.h>

mat4f Renderer::MVP, Renderer::ProjectionMatrix, Renderer::ViewMatrix, Renderer::ModelMatrix;
constexpr unsigned int QUADS_MAX = 5000, TEXTURES_MAX = 256, TEXTURES_MAX_BINDED = 32;
constexpr unsigned int Renderer::VB_MAX = QUADS_MAX * 4, Renderer::IB_MAX = QUADS_MAX * 6, Renderer::TEXTURES_MAX = 16;

struct vertex {
	float position[3];
	float color[4];
	float texCord[2];
	float textureID;
};
struct quadIndex {
	unsigned int index[6];
};
struct Renderer::quadBuffer {
	const vertex *firstVertex;
	vertex *endVertex;
	const unsigned int *firstIndex;
	unsigned int *endIndex;
	unsigned int elements, indexies, vertexies;
	inline void insert(vertex *vertexPtr, unsigned int *index){
		memcpy(endVertex, vertexPtr, sizeof(vertex)*4);
		memcpy(endIndex, index, sizeof(unsigned int) * 6);
		endVertex += 4;
		endIndex += 6;
		elements++;
		indexies += 6;
		vertexies += 4;
	}
	inline void clear(){
		endVertex = (vertex*)firstVertex;
		endIndex = (unsigned int*)firstIndex;
		indexies = vertexies = elements = 0;
	}
	quadBuffer(unsigned int vertexSize, unsigned int indexSize) : firstVertex(new vertex[vertexSize]), firstIndex(new unsigned int[indexSize]), elements(0){
		clear();
	}
	~quadBuffer(){
		delete[] firstVertex;
		delete[] firstIndex;
	}
};
void GenQuads(vertex* ret, const vec3f& position, const vec4f& color, const vec2f& size, int textid = 0) {
	float xsize = size.v0/2;
	float ysize = size.v1/2;
	ret[0] = {position.v0 +-xsize, position.v1 + -ysize, position.v2, color.v0, color.v1, color.v2, color.v3, 0.0f, 0.0f, (float)textid};
	ret[1] = {position.v0 + xsize, position.v1 + -ysize, position.v2, color.v0, color.v1, color.v2, color.v3, 1.0f, 0.0f, (float)textid};
	ret[2] = {position.v0 + xsize, position.v1 +  ysize, position.v2, color.v0, color.v1, color.v2, color.v3, 1.0f, 1.0f, (float)textid};
	ret[3] = {position.v0 +-xsize, position.v1 +  ysize, position.v2, color.v0, color.v1, color.v2, color.v3, 0.0f, 1.0f, (float)textid};
	return;
}
void GenRotateQuads(vertex* ret, const vec3f& position, const vec4f& color, const vec2f& size, float anglex, int textid = 0) {
	mat4f rotate;
	mat4f::GenRotate(rotate, anglex, 0.0f, 0.0f);
	float xsize = size.v0 / 2;
	float ysize = size.v1 / 2;
	vec3f positions[4] = {{-xsize, -ysize, 0.0f}, {xsize, -ysize, 0.0f}, {xsize, ysize, 0.0f}, {-xsize, ysize, 0.0f}};
	positions[0] = rotate * positions[0];
	positions[1] = rotate * positions[1];
	positions[2] = rotate * positions[2];
	positions[3] = rotate * positions[3];
	ret[0] = {position.v0 + positions[0].v0, position.v1 + positions[0].v1, position.v2, color.v0, color.v1, color.v2, color.v3, 0.0f, 0.0f, (float)textid};
	ret[1] = {position.v0 + positions[1].v0, position.v1 + positions[1].v1, position.v2, color.v0, color.v1, color.v2, color.v3, 1.0f, 0.0f, (float)textid};
	ret[2] = {position.v0 + positions[2].v0, position.v1 + positions[2].v1, position.v2, color.v0, color.v1, color.v2, color.v3, 1.0f, 1.0f, (float)textid};
	ret[3] = {position.v0 + positions[3].v0, position.v1 + positions[3].v1, position.v2, color.v0, color.v1, color.v2, color.v3, 0.0f, 1.0f, (float)textid};
	return;
}
Renderer::Renderer() : VA(), VB(sizeof(vertex) * VB_MAX), IB(IB_MAX), shader("DuEngine/basic.glsl") {
	VertexBufferLayout VBL;
	VBL.Push(GL_FLOAT, 3, offsetof(vertex, position));
	VBL.Push(GL_FLOAT, 4, offsetof(vertex, color));
	VBL.Push(GL_FLOAT, 2, offsetof(vertex, texCord));
	VBL.Push(GL_FLOAT, 1, offsetof(vertex, textureID));
	VBL.Size(sizeof(vertex));
	VA.AddBuffer(VB, VBL);
	QBuffer = new quadBuffer(VB_MAX, IB_MAX);
	Perspective(screen::fovy, screen::aspect, 0.01f, 100.0f);
	LookAt(screen::camx, screen::camy, 3.0f, screen::camx, screen::camy, -1.0f, 0.0f, 1.0f, 0.0f);
	mat4f::GenRotate(ModelMatrix,0.0, 0.0, 0.0);
}
Renderer::~Renderer(){
	delete QBuffer;
}
unsigned int elementsLastFrame = 0, elementsThisFrame, indicesLastFrame, indicesThisFrame, DrawnCalls, DrawnCallsLast;
void Renderer::flush() {
	VA.Bind();//contains VBL
	VB.SendData(0, QBuffer->vertexies * sizeof(vertex), QBuffer->firstVertex);
	IB.SendData(0, QBuffer->indexies, QBuffer->firstIndex);
	shader.Bind();
	MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
	shader.SetUniformMat4f("u_MVP", MVP);
	shader.SetUniform1iv("u_Texture", 2, u_Texture);//Sampler uniform
	gltry(glDrawElements(GL_TRIANGLES, (int)QBuffer->indexies, GL_UNSIGNED_INT, NULL));	 //documentation
	elementsThisFrame += QBuffer->vertexies;
	indicesThisFrame += QBuffer->indexies;
	DrawnCalls++;
	QBuffer->clear();
}

void Renderer::DispInfo() {
	static bool vsync = 1, drawnDetail = 1;
	ImGui::Begin("Render Info");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	if (ImGui::Checkbox("Vsync", &vsync))
		SDL_GL_SetSwapInterval(vsync);
	ImGui::Checkbox("elementsInfo", &drawnDetail);
	ImGui::Text("Quads last drawn %u", elementsLastFrame/4);
	ImGui::End();
	if (drawnDetail){
		ImGui::Begin("Elements Drawn Info", &drawnDetail);
		ImGui::Text("Vertex last drawn %u", elementsLastFrame);
		ImGui::Text("Indices last drawn %u", indicesLastFrame);
		ImGui::Text("Drawn calls last frame %u", DrawnCallsLast);
		ImGui::Text("Vertex Buffer used size %lu", sizeof(vertex) * elementsLastFrame);
		ImGui::Text("Index Buffer used size %lu", sizeof(unsigned int) * indicesLastFrame);
		double totalSize = sizeof(vertex) * elementsLastFrame + sizeof(unsigned int) * indicesLastFrame;
		if(totalSize > 1000000)
			ImGui::Text("Buffer size: %lf MB", totalSize/1000000);
		else
			ImGui::Text("Buffer size: %.0lf b", totalSize);
		ImGui::Text("Data transfer: %lf MB/s", totalSize/1000000*ImGui::GetIO().Framerate);
		ImGui::End();
	}
}
void Renderer::DrawnQuad(const vec3f& position, const vec4f& color, const vec2f& size) {
	if (QBuffer->elements == QUADS_MAX)
		flush();
	vertex quads[4];
	GenQuads(quads, position, color, size);
	unsigned int indexoffset = QBuffer->vertexies;
	quadIndex index = {0 + indexoffset, 1 + indexoffset, 2 + indexoffset, 2 + indexoffset, 3 + indexoffset, 0 + indexoffset};
	QBuffer->insert(quads, index.index);
}
void Renderer::DrawnQuadText(const vec3f& position, const vec4f& color, const vec2f& size, const Texture& Texture) {
	if (QBuffer->elements == QUADS_MAX)
		flush();
	vertex quads[4];
	Texture.Bind(0);
	u_Texture[0] = 0;
	GenQuads(quads, position, color, size, 2);
	unsigned int indexoffset = QBuffer->vertexies;
	quadIndex index = {0 + indexoffset, 1 + indexoffset, 2 + indexoffset, 2 + indexoffset, 3 + indexoffset, 0 + indexoffset};
	QBuffer->insert(quads, index.index);
}
void Renderer::DrawnQuadRotate(const vec3f& position, const vec4f& color, const vec2f& size, float rotatex) {
	if (QBuffer->elements == QUADS_MAX)
		flush();
	vertex quads[4];
	GenRotateQuads(quads, position, color, size, rotatex);
	unsigned int indexoffset = QBuffer->vertexies;
	quadIndex index = {0 + indexoffset, 1 + indexoffset, 2 + indexoffset, 2 + indexoffset, 3 + indexoffset, 0 + indexoffset};
	QBuffer->insert(quads, index.index);
}
void Renderer::Drawn(){
	flush();
	elementsLastFrame = elementsThisFrame;
	indicesLastFrame = indicesThisFrame;
	DrawnCallsLast = DrawnCalls;
	DrawnCalls = indicesThisFrame = elementsThisFrame = 0;
}
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