#ifndef INCLUDED_GRAPHICS_SPRITECOMMAND_H
#define INCLUDED_GRAPHICS_SPRITECOMMAND_H

namespace S {
namespace Graphics {

struct SpriteCommand
{
	SpriteCommand(S::Graphics::TextureId inTextureId, const S::Math::Vector2& inPosiiton, float inRotation, float inScale)
		: textureId(inTextureId)
		, sourceRect({ 0.0f, 0.0f, 0.0f, 0.0f })
		, position(inPosiiton)
		, rotation(inRotation)
		, scale(inScale)
	{}

	SpriteCommand(S::Graphics::TextureId inTextureId, const S::Geometry::Rect& inSourceRect, const S::Math::Vector2& inPosiiton, float inRotation, float inScale)
		: textureId(inTextureId)
		, sourceRect(inSourceRect)
		, position(inPosiiton)
		, rotation(inRotation)
		, scale(inScale)
	{}

	S::Graphics::TextureId textureId;
	S::Geometry::Rect sourceRect;
	S::Math::Vector2 position;
	float rotation;
	float scale;
};

} // namespace Graphics
} // namespace S

#endif // !INCLUDED_GRAPHICS_SPRITECOMMAND_H
