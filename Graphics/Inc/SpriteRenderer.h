#ifndef INCLUDED_GRAPHICS_SPRITERENDERER_H
#define INCLUDED_GRAPHICS_SPRITERENDERER_H

namespace DirectX {	class CommonStates; class SpriteBatch; }

namespace S {
namespace Graphics {

class Texture;

class SpriteRenderer
{
public:
	static void StaticInitialize();
	static void StaticTerminate();
	static SpriteRenderer* Get();

public:
	SpriteRenderer();
	~SpriteRenderer();

	SpriteRenderer(const SpriteRenderer&) = delete;
	SpriteRenderer& operator=(const SpriteRenderer&) = delete;

	void Initialize();
	void Terminate();

	void BeginRender();
	void EndRender();

	void Draw(const Texture& texture, const Math::Vector2& pos, float rotation = 0.0f, float scale = 1.0f);
	void Draw(const Texture& texture, const Geometry::Rect& sourceRect, const Math::Vector2& pos, float rotation = 0.0f, float scale = 1.0f);

private:
	DirectX::CommonStates* mCommonStates;
	DirectX::SpriteBatch* mSpriteBatch;
};

} // namespace Graphics
} // namespace S

#endif // #ifndef INCLUDED_GRAPHICS_SPRITERENDERER_H