#include "Renderer.hpp"
#include <imgui.h>

mat4f Renderer::MVP, Renderer::ProjectionMatrix, Renderer::ViewMatrix, Renderer::ModelMatrix;
constexpr unsigned int QUADS_MAX = 5000;
constexpr unsigned int Renderer::VB_MAX = QUADS_MAX * 4, Renderer::IB_MAX = QUADS_MAX * 6;

struct Renderer::StatsData {
	unsigned int elementsLastFrame = 0;
	unsigned int elementsThisFrame;
	unsigned int indicesLastFrame;
	unsigned int indicesThisFrame;
	unsigned int DrawnCalls;
	unsigned int DrawnCallsLast;
	unsigned int TexturesBindsThisFrame = 0;
	unsigned int TexturesBindsLastFrame = 0;
};

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
	Stats = new StatsData();
	Perspective(screen::fovy, screen::aspect, 0.01f, 100.0f);
	LookAt(screen::camx, screen::camy, 3.0f, screen::camx, screen::camy, -1.0f, 0.0f, 1.0f, 0.0f);
	mat4f::GenRotate(ModelMatrix,0.0, 0.0, 0.0);
}
Renderer::~Renderer(){
	delete QBuffer;
	delete Stats;
}
void Renderer::flush() {
	VA.Bind();//contains VBL
	VB.SendData(0, QBuffer->vertexies * sizeof(vertex), QBuffer->firstVertex);
	IB.SendData(0, QBuffer->indexies, QBuffer->firstIndex);
	shader.Bind();
	MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
	shader.SetUniformMat4f("u_MVP", MVP);
	Textures.Push(shader);	//Set u_Texture uniform
	gltry(glDrawElements(GL_TRIANGLES, (int)QBuffer->indexies, GL_UNSIGNED_INT, NULL));	 //documentation
	Stats->elementsThisFrame += QBuffer->vertexies;
	Stats->indicesThisFrame += QBuffer->indexies;
	Stats->TexturesBindsThisFrame += Textures.UsedSlots();
	Stats->DrawnCalls++;
	QBuffer->clear();
	Textures.Clear();
}

void Renderer::DispInfo() {
	static bool vsync = 1, drawnDetail = 1;
	ImGui::Begin("Render Info");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	if (ImGui::Checkbox("Vsync", &vsync))
		SDL_GL_SetSwapInterval(vsync);
	ImGui::Checkbox("elementsInfo", &drawnDetail);
	ImGui::Text("Quads last drawn %u", Stats->elementsLastFrame/4);
	ImGui::End();
	if (drawnDetail){
		ImGui::Begin("Elements Drawn Info", &drawnDetail);
		ImGui::Text("Max Binded Textures %u", Textures.getMaxTexturesBinded());
		ImGui::Text("Vertex last drawn %u", Stats->elementsLastFrame);
		ImGui::Text("Indices last drawn %u", Stats->indicesLastFrame);
		ImGui::Text("Drawn calls last frame %u", Stats->DrawnCallsLast);
		ImGui::Text("Textures Binded last frame %u", Stats->TexturesBindsLastFrame);
		ImGui::Text("Vertex Buffer used size %lu", sizeof(vertex) * Stats->elementsLastFrame);
		ImGui::Text("Index Buffer used size %lu", sizeof(unsigned int) * Stats->indicesLastFrame);	 //TexturesBindsLastFrame
		double totalSize = sizeof(vertex) * Stats->elementsLastFrame + sizeof(unsigned int) * Stats->indicesLastFrame;
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
	int TextureIndex = Textures.FindBind(Texture);
	if(TextureIndex < 0){
		flush();
		TextureIndex = Textures.FindBind(Texture);
		if(TextureIndex < 0) throw std::runtime_error("Negative Texture Index after flush");
	}
	vertex quads[4];
	GenQuads(quads, position, color, size, ++TextureIndex);
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
	Stats->elementsLastFrame = Stats->elementsThisFrame;
	Stats->indicesLastFrame = Stats->indicesThisFrame;
	Stats->DrawnCallsLast = Stats->DrawnCalls;
	Stats->DrawnCalls = Stats->indicesThisFrame = Stats->elementsThisFrame = 0;
	Stats->TexturesBindsLastFrame = Stats->TexturesBindsThisFrame;
	Stats->TexturesBindsThisFrame = 0;
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


//std::vector<int> Renderer::TextureBinder::u_texture;
unsigned int Renderer::TextureBinder::MaxTexturesBinded = 16;	 //hardcoded minimum
inline int Renderer::TextureBinder::Find(const Texture& tex) const {
	std::map<unsigned int, int>::const_iterator search = TexturesIndexMap.find(tex.GetID());
	if(search != TexturesIndexMap.end()) return search->second;
	else return -1;
}
inline unsigned int Renderer::TextureBinder::UsedSlots() const{
	return TexturesIndexMap.size();
}
inline int Renderer::TextureBinder::Bind(const Texture& tex){
	const unsigned int NextIndex = UsedSlots();
	if(UsedSlots() >= MaxTexturesBinded) return -1;
	TexturesIndexMap.emplace(tex.GetID(), NextIndex);//TextureID, Slot
	tex.Bind(NextIndex);
	return (int)NextIndex;
}
inline int Renderer::TextureBinder::FindBind(const Texture& tex){
	int Index = Find(tex);
	if(Index < 0) return Bind(tex);
	return Index;
}
inline void Renderer::TextureBinder::Clear(){
	TexturesIndexMap.clear();
}
inline void Renderer::TextureBinder::Push(Shader& shader) const{//not needed (the slot num is the slot pos)
	//Create array with data;
	unsigned int *TexturesSlots = (unsigned int*)alloca(MaxTexturesBinded * sizeof(unsigned int));
	for(unsigned int i = 0; i < MaxTexturesBinded; ++i)
		TexturesSlots[i] = i;
	shader.Bind();
	shader.SetUniform1iv("u_Texture", UsedSlots(), (int*)TexturesSlots);
}
Renderer::TextureBinder::TextureBinder(){
	int NewMax;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &NewMax);
	if (NewMax < 0) throw std::range_error("GL_MAX_TEXTURE_IMAGE_UNITS less than zero :" + std::to_string(NewMax));
	MaxTexturesBinded = (unsigned int)NewMax;
}
Renderer::TextureBinder::~TextureBinder(){

}
unsigned int Renderer::TextureBinder::getMaxTexturesBinded() const {
	return MaxTexturesBinded;
}
