#include "Precompiled.h"
#include "Graphics.h"

namespace S {
namespace Graphics {

TextureId LoadTexture(const char* fileName)
{
	ASSERT(GraphicsSystem::Get(), "[SEngine] Engine not started.");
	return TextureManager::Get()->Load(fileName);
}

void ClearAllTextures()
{
	ASSERT(GraphicsSystem::Get(), "[SEngine] Engine not started.");
	return TextureManager::Get()->Clear();
}

void DrawLine(float x0, float y0, float z0, float x1, float y1, float z1, const Math::Vector4& color)
{
	ASSERT(GraphicsSystem::Get(), "[SEngine] Engine not started.");
	SimpleDraw::AddLine(x0, y0, z0, x1, y1, z1, color);
}

void DrawAABB(const Geometry::AABB& aabb, const Math::Vector4& color)
{
	ASSERT(GraphicsSystem::Get(), "[SEngine] Engine not started.");
	SimpleDraw::AddAABB(aabb, color);
}

void DrawAABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, const Math::Vector4& color)
{
	ASSERT(GraphicsSystem::Get(), "[SEngine] Engine not started.");
	SimpleDraw::AddAABB(minX, minY, minZ, maxX, maxY, maxZ, color);
}

void DrawOBB(const Geometry::OBB& obb, const Math::Vector4& color)
{
	ASSERT(GraphicsSystem::Get(), "[SEngine] Engine not started.");
	SimpleDraw::AddOBB(obb, color);
}

void DrawSphere(const Math::Vector3& center, float radius, const Math::Vector4& color, uint32_t slices, uint32_t rings)
{
	ASSERT(GraphicsSystem::Get(), "[SEngine] Engine not started.");
	SimpleDraw::AddSphere(center, radius, color, slices, rings);
}

void DrawSphere(float x, float y, float z, float radius, const Math::Vector4& color, uint32_t slices, uint32_t rings)
{
	ASSERT(GraphicsSystem::Get(), "[SEngine] Engine not started.");
	SimpleDraw::AddSphere(Math::Vector3(x, y, z), radius, color, slices, rings);
}

void DrawScreenLine(const Math::Vector2& v0, const Math::Vector2& v1, const Math::Vector4& color)
{
	ASSERT(GraphicsSystem::Get(), "[SEngine] Engine not started.");
	SimpleDraw::AddScreenLine(v0, v1, color);
}

void DrawScreenLine(float x0, float y0, float x1, float y1, const Math::Vector4& color)
{
	ASSERT(GraphicsSystem::Get(), "[SEngine] Engine not started.");
	SimpleDraw::AddScreenLine(x0, y0, x1, y1, color);
}

void DrawScreenLine(const Geometry::LineSegment2D & l, const Math::Vector4 & color)
{
	ASSERT(GraphicsSystem::Get(), "[SEngine] Engine not started.");
	SimpleDraw::AddScreenLine(l.from, l.to, color);
}

void DrawScreenRect(const Geometry::Rect& rect, const Math::Vector4& color)
{
	ASSERT(GraphicsSystem::Get(), "[SEngine] Engine not started.");
	SimpleDraw::AddScreenRect(rect.left, rect.top, rect.right, rect.bottom, color);
}

void DrawScreenRect(float l, float t, float r, float b, const Math::Vector4& color)
{
	ASSERT(GraphicsSystem::Get(), "[SEngine] Engine not started.");
	SimpleDraw::AddScreenRect(l, t, r, b, color);
}

void DrawScreenCircle(const Geometry::Circle& circle, const Math::Vector4& color)
{
	ASSERT(GraphicsSystem::Get(), "[SEngine] Engine not started.");
	SimpleDraw::AddScreenCircle(circle, color);
}

void DrawScreenCircle(const Math::Vector2& center, float r, const Math::Vector4& color)
{
	ASSERT(GraphicsSystem::Get(), "[SEngine] Engine not started.");
	SimpleDraw::AddScreenCircle(center, r, color);
}

void DrawScreenCircle(float x, float y, float r, const Math::Vector4& color)
{
	ASSERT(GraphicsSystem::Get(), "[SEngine] Engine not started.");
	SimpleDraw::AddScreenCircle(x, y, r, color);
}

void DrawScreenDiamond(const Math::Vector2& center, float size, const Math::Vector4& color)
{
	ASSERT(GraphicsSystem::Get(), "[SEngine] Engine not started.");
	SimpleDraw::AddScreenDiamond(center, size, color);
}

void DrawScreenDiamond(float x, float y, float size, const Math::Vector4& color)
{
	ASSERT(GraphicsSystem::Get(), "[SEngine] Engine not started.");
	SimpleDraw::AddScreenDiamond(x, y, size, color);
}

void DrawScreenText(const char* str, Math::Vector2 p, float size, const Math::Vector4& color)
{
	ASSERT(GraphicsSystem::Get(), "[SEngine] Engine not started.");
	static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	sTextCommands.emplace_back(converter.from_bytes(str), size, p.x, p.y, ToColor(color));
}

void DrawScreenText(const char* str, float x, float y, float size, const Math::Vector4& color)
{
	ASSERT(GraphicsSystem::Get(), "[SEngine] Engine not started.");
	static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	sTextCommands.emplace_back(converter.from_bytes(str), size, x, y, ToColor(color));
}

void DrawSprite(TextureId textureId, const Math::Vector2& position)
{
	ASSERT(GraphicsSystem::Get(), "[SEngine] Engine not started.");
	sSpriteCommands.emplace_back(textureId, position, 0.0f, 1.0f);
}

void DrawSprite(TextureId textureId, const Math::Vector2& position, float rotation, float scale)
{
	ASSERT(GraphicsSystem::Get(), "[SEngine] Engine not started.");
	sSpriteCommands.emplace_back(textureId, position, rotation, scale);
}

void DrawSprite(TextureId textureId, const Geometry::Rect& sourceRect, const Math::Vector2& position, float rotation, float scale)
{
	ASSERT(GraphicsSystem::Get(), "[SEngine] Engine not started.");
	sSpriteCommands.emplace_back(textureId, sourceRect, position, rotation, scale);
}

uint32_t GetSpriteWidth(TextureId textureId)
{
	Texture* texture = TextureManager::Get()->GetTexture(textureId);
	return texture ? texture->GetWidth() : 0u;
}

uint32_t GetSpriteHeight(TextureId textureId)
{
	Texture* texture = TextureManager::Get()->GetTexture(textureId);
	return texture ? texture->GetHeight() : 0u;
}

uint32_t GetScreenWidth()
{
	return GraphicsSystem::Get()->GetWidth();
}

uint32_t GetScreenHeight()
{
	return GraphicsSystem::Get()->GetHeight();
}

void Render()
{
	// Sprites
	SpriteRenderer::Get()->BeginRender();

	for (const auto& command : sSpriteCommands)
	{
		Texture* texture = TextureManager::Get()->GetTexture(command.textureId);
		if (texture)
		{
			if (Geometry::IsEmpty(command.sourceRect))
			{
				SpriteRenderer::Get()->Draw(*texture, command.position, command.rotation, command.scale);
			}
			else
			{
				SpriteRenderer::Get()->Draw(*texture, command.sourceRect, command.position, command.rotation, command.scale);
			}
		}
	}
	sSpriteCommands.clear();
	SpriteRenderer::Get()->EndRender();

	// Text
	for (const auto& command : sTextCommands)
	{
		Font::Get()->Draw(command.str.c_str(), command.size, command.x, command.y, command.color);
	}
	sTextCommands.clear();
}

} // namespace Graphics
} // namespace S