#include "Board.h"

using namespace S;
using namespace S::Graphics;

void Board::Load(Math::Vector2 p, const char* fileName)
{
	mCommand.Clear();
	mPosition = p;
	mRect = { mPosition.x - 64.0f,mPosition.y - 16.0f,mPosition.x + 64.0f,mPosition.y + 16.0f };
	mId = LoadTexture(fileName);
}

void Board::Update(float deltaTime)
{
	if (mCommand.MoveLeft())
	{
		MoveLeft();
	}
	if (mCommand.MoveRight())
	{
		MoveRight();
	}
	mPosition.x += mVelocity * deltaTime;
	mVelocity = 0.0f;
	mRect = { mPosition.x - 64.0f,mPosition.y - 16.0f,mPosition.x + 64.0f,mPosition.y + 16.0f };
}

void Board::Render()
{
	SimpleDraw::AddScreenRect(GetRect(), Math::Vector4::Magenta());
	DrawSprite(mId, { mPosition.x - 16.0f,mPosition.y });
	DrawSprite(mId, { mPosition.x - 48.0f,mPosition.y });
	DrawSprite(mId, { mPosition.x + 16.0f,mPosition.y });
	DrawSprite(mId, { mPosition.x + 48.0f,mPosition.y });
}

void Board::MoveRight()
{
	mVelocity += 350.0f;
}

void Board::MoveLeft()
{
	mVelocity -= 350.0f;
}

void Board::Serialize(Network::StreamWriter & writer) const
{
	
}

void Board::Deserialize(Network::StreamReader & reader)
{
	//reader.Read(Id());
}

const Math::Vector2& Board::GetPosition() const
{
	return mPosition;
}

const Geometry::Rect& Board::GetRect() const
{
	return mRect;
}

void Board::SetCommand(const Command& command)
{
	if (mCommand.mCommand != command.mCommand)
	{
		mCommand = command;
	}
}
