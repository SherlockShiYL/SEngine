#ifndef INCLUDED_GRAPHICS_H
#define INCLUDED_GRAPHICS_H

#include "Common.h"

#include "Animation.h"
#include "AnimationBuilder.h"
#include "AnimationClip.h"
#include "AnimationIO.h"
#include "Bone.h"
#include "BoneAnimation.h"
#include "Camera.h"
#include "Config.h"
#include "ConstantBuffer.h"
#include "Font.h"
#include "GraphicsSystem.h"
#include "Gui.h"
#include "Keyframe.h"
#include "Light.h"
#include "Material.h"
#include "Mesh.h"
#include "MeshBuffer.h"
#include "MeshBuilder.h"
#include "Model.h"
#include "ModelLoader.h"
#include "PixelShader.h"
#include "RasterizerState.h"
#include "RenderTarget.h"
#include "Sampler.h"
#include "SimpleDraw.h"
#include "SpriteCommand.h"
#include "SpriteRenderer.h"
#include "TextCommand.h"
#include "Texture.h"
#include "TextureManager.h"
#include "VertexShader.h"
#include "VertexTypes.h"

#pragma comment(lib, "FW1FontWrapper.lib")

namespace S {
namespace Graphics {

static std::vector<SpriteCommand> sSpriteCommands;
static std::vector<TextCommand> sTextCommands;

inline uint32_t ToColor(const Math::Vector4& color)
{
	uint8_t r = (uint8_t)(color.x * 255);
	uint8_t g = (uint8_t)(color.y * 255);
	uint8_t b = (uint8_t)(color.z * 255);
	return 0xff000000 | (b << 16) | (g << 8) | r;
}


TextureId LoadTexture(const char* fileName);
void ClearAllTextures();

void DrawLine(float x0, float y0, float z0, float x1, float y1, float z1, const Math::Vector4& color);
void DrawAABB(const Geometry::AABB& aabb, const Math::Vector4& color);
void DrawAABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, const Math::Vector4& color);
void DrawOBB(const Geometry::OBB& obb, const Math::Vector4& color);
void DrawSphere(const Math::Vector3& center, float radius, const Math::Vector4& color, uint32_t slices = 8, uint32_t rings = 4);
void DrawSphere(float x, float y, float z, float radius, const Math::Vector4& color, uint32_t slices = 8, uint32_t rings = 4);
void DrawScreenLine(const Math::Vector2& v0, const Math::Vector2& v1, const Math::Vector4& color);
void DrawScreenLine(float x0, float y0, float x1, float y1, const Math::Vector4& color);
void DrawScreenLine(const Geometry::LineSegment2D& l, const Math::Vector4& color);
void DrawScreenRect(const Geometry::Rect& rect, const Math::Vector4& color);
void DrawScreenRect(float l, float t, float r, float b, const Math::Vector4& color);
void DrawScreenCircle(const Geometry::Circle& circle, const Math::Vector4& color);
void DrawScreenCircle(const Math::Vector2& center, float r, const Math::Vector4& color);
void DrawScreenCircle(float x, float y, float r, const Math::Vector4& color);
void DrawScreenDiamond(const Math::Vector2& center, float size, const Math::Vector4& color);
void DrawScreenDiamond(float x, float y, float size, const Math::Vector4& color);
void DrawScreenText(const char* str, Math::Vector2 p, float size, const Math::Vector4& color);
void DrawScreenText(const char* str, float x, float y, float size, const Math::Vector4& color);

void DrawSprite(TextureId textureId, const Math::Vector2& position);
void DrawSprite(TextureId textureId, const Math::Vector2& position, float rotation, float scale = 1.0f);
void DrawSprite(TextureId textureId, const Geometry::Rect& sourceRect, const Math::Vector2& position, float rotation = 0.0f, float scale = 1.0f);
uint32_t GetSpriteWidth(TextureId textureId);
uint32_t GetSpriteHeight(TextureId textureId);
uint32_t GetScreenWidth();
uint32_t GetScreenHeight();
Geometry::Rect GetScreenRect();

void Render();

} // namespace Graphics
} // namespace S

#endif // #ifndef INCLUDED_GRAPHICS_H