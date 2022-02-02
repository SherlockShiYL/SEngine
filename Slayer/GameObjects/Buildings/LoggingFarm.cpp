#include "LoggingFarm.h"

using namespace S;

void LoggingFarm::Update(float deltaTime)
{
}

void LoggingFarm::Render(S::Math::Vector2 p, float scale)
{
	Graphics::DrawSprite(mId, (mPosition + Math::Vector2{ 52.0f, 46.0f })*scale + p, 0.0f, 1.5f*scale);
	Graphics::DrawScreenRect({ GetRect().left*scale + p.x,GetRect().top*scale + p.y,GetRect().right*scale + p.x,GetRect().bottom*scale + p.y }
		, Math::Vector4::Orange());
}

void LoggingFarm::LoadTexture()
{
	mId = S::Graphics::LoadTexture("House.png");
}

void LoggingFarm::Load(Math::Vector2 position)
{
	mPosition = position;
}
