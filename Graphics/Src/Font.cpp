#include "Precompiled.h"
#include "Font.h"

#include "GraphicsSystem.h"
#include <FW1FontWrapper_1_1/FW1FontWrapper.h>

#pragma comment(lib, "FW1FontWrapper.lib")

using namespace S;
using namespace S::Graphics;

namespace
{
	Font* sFont = nullptr;
}

void Font::StaticInitialize()
{
	ASSERT(sFont == nullptr, "[Font] System already initialized!");
	sFont = new Font();
	sFont->Initialize();
}

void Font::StaticTerminate()
{
	if (sFont != nullptr)
	{
		sFont->Terminate();
		SafeDelete(sFont);
	}
}

Font* Font::Get()
{
	ASSERT(sFont != nullptr, "[SpriteRenderer] No instance registered.");
	return sFont;
}

Font::Font()
	: mFW1Factory{ nullptr }
	, mFontWrapper{ nullptr }
{
}

Font::~Font()
{
}

void Font::Initialize()
{
	ID3D11Device* device = GraphicsSystem::Get()->GetDevice();
	FW1CreateFactory(FW1_VERSION, &mFW1Factory);
	mFW1Factory->CreateFontWrapper(device, L"Consolas", &mFontWrapper);
}

void Font::Terminate()
{
	SafeRelease(mFontWrapper);
	SafeRelease(mFW1Factory);
}

void Font::Draw(const wchar_t* str, float x, float y, float size, uint32_t color)
{
	ID3D11DeviceContext* context = GraphicsSystem::Get()->GetContext();
	mFontWrapper->DrawString(context, str, x, y, size, color, FW1_RESTORESTATE);
}