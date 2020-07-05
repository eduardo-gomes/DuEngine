#include <map>
#include <memory>

#include "GLClasses.hpp"

class Renderer {
   private:
	VertexArray VA;
	VertexBuffer VB;
	IndexBuffer IB;
	Shader shader;
	class TextureBinder {
		std::map<unsigned int, int> TexturesIndexMap;
		static unsigned int MaxTexturesBinded;

	   public:
		unsigned int getMaxTexturesBinded() const;
		int Find(const Texture& tex) const;
		unsigned int UsedSlots() const;
		int Bind(const Texture& tex);
		int FindBind(const Texture& tex);
		void Clear();
		void Push(Shader& shader) const;
		TextureBinder();
		~TextureBinder();
	} Textures;
	struct StatsData;
	StatsData* Stats;
	static const unsigned int VB_MAX, IB_MAX;
	struct quadBuffer;
	quadBuffer* QBuffer;

   public:
	static mat4f MVP, ProjectionMatrix, ViewMatrix, ModelMatrix;
	void DispInfo();
	Renderer();
	~Renderer();
	void EnableDepth() const;
	void DisableDepth() const;
	void flush();
	void Drawn();
	void Drawn(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void DrawnQuad(const vec3f& position, const vec4f& color, const vec2f& size);
	void DrawnQuad(const vec3f& position, const vec4f& color, const vec2f& size, const Texture& Texture);
	void DrawnQuad(const vec3f& position, const vec4f& color, const vec2f& size, const SubTexture& Texture);
	void DrawnQuadRotate(const vec3f& position, const vec4f& color, const vec2f& size, float rotatex);
	void DrawnQuadRotate(const vec3f& position, const vec4f& color, const vec2f& size, float rotatex, const Texture& Texture);
	void DrawnQuadRotate(const vec3f& position, const vec4f& color, const vec2f& size, float rotatex, const SubTexture& SubTex);
	static mat4f& LookAt(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ);
	static mat4f& Perspective(float fovy, float aspect, float zNear, float zFar);
};