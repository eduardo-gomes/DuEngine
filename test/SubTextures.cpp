#include <cstdint>
#include <DuEngine/DuEngine.hpp>

namespace scene {
class SubTextures : public BaseScene {
   private:
	std::shared_ptr<Texture> tex;
	SubTexture* subtex[16 * 16];

   public:
	SubTextures() {
		TextureParameters param(GL_LINEAR, GL_LINEAR);
		tex = std::make_shared<Texture>("test/sheet.bmp");
		for (int x = 0; x < 16; ++x)
			for (int y = 0; y < 16; ++y)
				subtex[x + y * 16] = new SubTexture(tex, x, y, 16, 16);
	}
	~SubTextures() {}

	void Update(int64_t){};
	void Render();
	void RenderGUI() {
		renderer->DispInfo();
	};
};

void SubTextures::Render() {
	constexpr vec3f position{0.0f, 0.0f, 0.0f};
	constexpr vec4f color{1.0f, 1.0f, 1.0f, 1.0f};
	constexpr vec2f size{1.0f, 1.0f};
	static int frame = 0;
	frame++;
	renderer->DrawnQuadRotate(position, color, size, (frame % 360), *subtex[frame % 256]);
	for (int x = 0; x < 16; ++x)
		for (int y = 0; y < 16; ++y) {
			vec3f position{((float)x / 8 - 1.0f) * 1.4f, (-(float)y / 8 + 1.0f), 0.0f};
			vec4f color{1.0f, 1.0f, 1.0f, 1.0f};
			vec2f size{0.12f, 0.12f};
			renderer->DrawnQuad(position, color, size, *subtex[x + y * 16]);
		}
	renderer->Drawn();
}

}  // namespace scene

#include <iostream>
int main() {
	//audio::musicReserve(1);
	Start("DuTest");
}

void Setup() {
	new scene::SubTextures;
}