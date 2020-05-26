#include "GLClasses.hpp"

class Renderer {
   public:
	//void DrawnQuad();
	void Drawn(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};