#include "GLClasses.hpp"

class Renderer {
   private:
	VertexArray *m_VA;
	VertexBuffer *m_VB;
	IndexBuffer *m_IB;
	Shader *m_shader;
	
   public:
	static mat4f MVP, ProjectionMatrix, ViewMatrix, ModelMatrix;
	static void DispInfo();
	//void DrawnQuad();
	void Drawn(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void DrawnQuad(const vec4f& position, const vec4f& color, const vec3f& size);
	static mat4f& LookAt(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ);
	static mat4f& Perspective(float fovy, float aspect, float zNear, float zFar);
};