#include <DuEngine/graphics/LoadTexture.hpp>
#include <DuEngine/graphics/glmath.hpp>
#include <DuEngine/window/window.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class VertexBuffer {
   private:
	unsigned int RenderID;
	static unsigned int BindedRenderID;

   public:
	VertexBuffer(const void* data, unsigned int size);
	VertexBuffer(unsigned int size);  //Create dynamic buffer
	~VertexBuffer();

	void SendData(unsigned int offset, unsigned int size, const void* data);
	void Bind() const;
	void Unbind() const;
};
struct VertexBufferElement {
	unsigned int type;
	unsigned int count;
	unsigned char normalized;
	size_t offset;
	static unsigned int GetSizeOfType(unsigned int type);
};
class VertexBufferLayout {
   private:
	std::vector<VertexBufferElement> Elements;
	unsigned int Stride = 0;

   public:
	void Push(unsigned int type, unsigned int count, unsigned char normalized = GL_FALSE) {
		Elements.push_back({type, count, normalized, Stride});
		ASSERT(Stride += VertexBufferElement::GetSizeOfType(type) * count);
	}
	void Push(unsigned int type, unsigned int count, size_t offset, unsigned char normalized = GL_FALSE) {
		Elements.push_back({type, count, normalized, offset});
		ASSERT(Stride += VertexBufferElement::GetSizeOfType(type) * count);
	}
	void Size(unsigned int size) {
		Stride = size;
	}
	inline const std::vector<VertexBufferElement>& GetElements() const { return Elements; }
	inline unsigned int GetStride() const { return Stride; }
};

class IndexBuffer {
   private:
	unsigned int RenderID, m_count;
	static unsigned int BindedRenderID;

   public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	IndexBuffer(unsigned int count);  //Create dynamic buffer
	~IndexBuffer();

	void SendData(unsigned int offset, unsigned int count, const unsigned int* data);
	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_count; }
};

class VertexArray {
   private:
	unsigned int RenderID;
	static unsigned int BindedRenderID;

   public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void Bind() const;
	void Unbind() const;
};

class Shader {
   private:
	std::string File;
	unsigned int RenderID;
	static unsigned int BindedRenderID;
	struct ShaderProgramSource;
	std::unordered_map<std::string, int> UniformLocationCache;

   public:
	Shader(const std::string& Filename);
	~Shader();

	void Bind() const;
	void Unbind() const;
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1iv(const std::string& name, unsigned int count, int* data);
	void SetUniform2f(const std::string& name, const vec2f&);
	void SetUniform3f(const std::string& name, const vec3f&);
	void SetUniform4f(const std::string& name, const vec4f&);
	void SetUniformMat4f(const std::string& name, const mat4f& matrix);

   private:
	int GetUniformLocation(const std::string& name);
	unsigned int CreateShader(const std::string& vertexshader, const std::string& fragmentshader);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	ShaderProgramSource ParseShader(const std::string& filepath);
};

struct TextureParameters {
	GLint mag_filter, min_filter, warp_s, warp_t;
	TextureParameters(GLint mag_filter = GL_NEAREST, GLint min_filter = GL_LINEAR, GLint warp_s = GL_REPEAT, GLint warp_t = GL_REPEAT);
};
const TextureParameters TextureParametersDefault;

class Texture {
   private:
	unsigned int RenderID;
	std::string File;
	unsigned char* LocalBuffer;
	int Width, Height, size;

   public:
	Texture(const std::string& path, const TextureParameters& params = TextureParametersDefault);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;
	unsigned int GetID() const;

	inline int GetWidth() const { return Width; }
	inline int GetHeight() const { return Height; }
};

class SubTexture {
	std::shared_ptr<Texture> MainTexture;
	float Hpos, Vpos, Hsize, Vsize;

   public:
	inline float getHpos() const { return Hpos; };
	inline float getVpos() const { return Vpos; };
	inline float getHsize() const { return Hsize; };
	inline float getVsize() const { return Vsize; };
	Texture& getTexture() const;
	SubTexture(const std::shared_ptr<Texture>& Texture);
	SubTexture(const std::shared_ptr<Texture>& Texture, int Hindex, int Vindex, int Hnum, int Vnum);
	~SubTexture();
};
