#include "GLClasses.hpp"
#include <memory>

class Renderer {
   private:
	VertexArray VA;
	VertexBuffer VB;
	IndexBuffer IB;
	Shader shader;
	static const unsigned int VB_MAX, IB_MAX, TEXTURES_MAX;
	struct quadBuffer;
	quadBuffer *QBuffer;

   public:
	static mat4f MVP, ProjectionMatrix, ViewMatrix, ModelMatrix;
	void DispInfo();
	Renderer();
	~Renderer();
	void flush();
	void Drawn();
	void Drawn(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void DrawnQuad(const vec3f& position, const vec4f& color, const vec2f& size);
	void DrawnQuadRotate(const vec3f& position, const vec4f& color, const vec2f& size, float rotatex);
	static mat4f& LookAt(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ);
	static mat4f& Perspective(float fovy, float aspect, float zNear, float zFar);
};