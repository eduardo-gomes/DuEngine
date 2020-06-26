#include "GLClasses.hpp"
#include <memory>
#include <map>

class Renderer {
   private:
	VertexArray VA;
	VertexBuffer VB;
	IndexBuffer IB;
	Shader shader;
	class TextureBinder{
		std::map<unsigned int, int> TexturesIndexMap;
		static unsigned int MAX_TEXTURES;
	   public:
		int Find(const Texture& tex) const;
		unsigned int UsedSlots() const;
		int Bind(const Texture& tex);
		int FindBind(const Texture& tex);
		void Clear();
		void Push(Shader& shader) const;
		TextureBinder();
		~TextureBinder();
	} Textures;
	static const unsigned int VB_MAX, IB_MAX;
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
	void DrawnQuadText(const vec3f& position, const vec4f& color, const vec2f& size, const Texture& Texture);
	void DrawnQuadRotate(const vec3f& position, const vec4f& color, const vec2f& size, float rotatex);
	static mat4f& LookAt(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ);
	static mat4f& Perspective(float fovy, float aspect, float zNear, float zFar);
};