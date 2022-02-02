#include "Car.h"

using namespace S;
using namespace S::Graphics;

void Car::Load(float s, float gapDis, float max, Math::Vector2 d)
{
	mSpeed = 100.0f;
	mGapDistance = gapDis;
	mMaxSpeed = max;
	mDestination = d;
	mDestinationIndex = 0;

	mTexture = LoadTexture("AI_Generator_Images/RedArrow.png");

	mCollisionDirection.reserve(MAXCOLLISIONLINES);
	for (int i = 0; i < MAXCOLLISIONLINES; ++i)
	{
		mCollisionDirection.emplace_back(Math::Rotate(S::Math::Vector2{ 1.0f,0.0f }, i*(360.0f / MAXCOLLISIONLINES) * Math::kDegToRad));
	}

	Reset();

	mCollisionInput.reserve(MAXCOLLISIONINPUT);
	for (int i = 0; i < MAXCOLLISIONINPUT; ++i)
	{
		mCollisionInput.push_back(0);
	}
}

void Car::Spawn(Math::Vector2 p)
{
	mPosition = p;
}

void Car::SetDestination(Math::Vector2 d)
{
	mDestination = d;
}

void Car::Reset()
{
	mStopCountDown = 5.0f;
	mAngle = 0.0f;
	mSpeed = 50.0f;
	mDirection = { 1.0f,0.0f };
	mIsAlive = true;
	mDestinationIndex = 0;
	mCountDown = 5.0f;
	ResetAction();

	ResetCollisionLines();
}

void Car::ResetCollisionLines()
{
	for (int i = 0; i < MAXCOLLISIONLINES; ++i)
	{
		mCollisionDirection[i] = Math::Rotate(S::Math::Vector2{ 1.0f,0.0f }, i*(360.0f / MAXCOLLISIONLINES) * Math::kDegToRad);
	}
}

void Car::Update(float deltaTime)
{
	if (mIsAlive)
	{
		if (brain)
		{
			mCountDown -= deltaTime;
			mCollisionInput[MAXCOLLISIONLINES] = mSpeed;
			mCollisionInput[MAXCOLLISIONLINES + 1] = TurnLeftRate();
			mCollisionInput[MAXCOLLISIONLINES + 2] = TurnRightRate();
			mCollisionInput[MAXCOLLISIONLINES + 3] = Math::Angle(mDirection, mDestination - mPosition);
			auto result = brain->Evaluate(mCollisionInput);
			if (result[0] > 0.5)
				Accelerate();
			if (result[1] > 0.5)
				Decelerate();
			if (result[2] > 0.5)
				TurnLeft();
			if (result[3] > 0.5)
				TurnRight();
			
			fitness += mSpeed > 0 ? static_cast<size_t>(mSpeed * (mSpeed / 1000.0f)) : 0u;
			//float dis = Math::DistanceSqr(mDestination, mPosition) - Math::DistanceSqr(mDestination, mPosition + mDirection * deltaTime * mSpeed);
			//fitness += dis > 0 ? static_cast<size_t>(Math::Sqrt(dis)) : 0u;

			if (mSpeed == 0.0f)
			{
				mStopCountDown -= deltaTime;
			}
			if (mStopCountDown <= 0.0f)
			{
				mIsAlive = false;
			}
			if (mCountDown < 0.0f)
			{
				mCountDown = 5.0f;
				fitness = 10000 * mDestinationIndex;
			}
		}

		mSpeed += mAcceleration * deltaTime;
		mSpeed = mSpeed > mMaxSpeed ? mMaxSpeed : mSpeed;
		mSpeed = mSpeed < -50.0f ? -50.0f : mSpeed;

		mDirection = Math::Rotate(mDirection, mSpeed * mAngleRate * deltaTime * Math::kDegToRad);
		mAngle += mSpeed * mAngleRate * deltaTime;

		mPosition += mDirection * mSpeed * deltaTime;

		for (int i = 0; i < MAXCOLLISIONLINES; i++)
		{
			mCollisionDirection[i] = Math::Rotate(mCollisionDirection[i], mSpeed*mAngleRate*deltaTime*Math::kDegToRad);
		}

		ResetAction();
	}
}

void Car::Render(S::Math::Vector2 p)
{
	DrawSprite(mTexture, mPosition + p, (mAngle + 90.0f) * Math::kDegToRad);
	DrawScreenLine(mPosition + p, mDestination + p, { 1.0f,0.0f,0.0f,0.1f });
	if (IsAlive())
	{
		DrawScreenCircle({ mPosition + p ,mRadius }, Math::Vector4::Cyan());
		/*for (int i = 0; i < MAXCOLLISIONLINES; i++)
		{
			DrawScreenLine(Geometry::LineSegment2D{ mPosition,mPosition + p + mCollisionDirection[i] * mCollisionLength }, Math::Vector4::Magenta());
		}*/
	}
	else
	{
		DrawScreenCircle({ mPosition + p ,mRadius }, Math::Vector4::Red());
	}

	std::string txt = std::to_string(fitness);
	//std::string txt = std::to_string(mCollisionInput[MAXCOLLISIONLINES + 3] + Math::kPi);
	DrawScreenText(txt.c_str(), mPosition.x + p.x, mPosition.y + p.y - 40.0f, 20.0f, S::Math::Vector4::White());
}

const Geometry::Circle Car::GetCircle() const
{
	return Geometry::Circle(mPosition, mMaxSpeed);
}

void Car::TurnLeft()
{
	if (mSpeed <= 0.0f)
	{
		mAngleRate = mSpeed / 50.0f;
	}
	else if (mSpeed >= 50.0f)
	{
		mAngleRate = -0.4f - 0.6f * ((mMaxSpeed - mSpeed) / (mMaxSpeed - 50.0f));
	}
	else
	{
		mAngleRate = -mSpeed / 50.0f;
	}
}

void Car::TurnRight()
{
	if (mSpeed <= 0.0f)
	{
		mAngleRate = -mSpeed / 50.0f;
	}
	else if (mSpeed >= 50.0f)
	{
		mAngleRate = 0.4f + 0.6f * ((mMaxSpeed - mSpeed) / (mMaxSpeed - 50.0f));
	}
	else
	{
		mAngleRate = mSpeed / 50.0f;
	}
}

void Car::Accelerate()
{
	mAcceleration = mMaxSpeed;
}

void Car::Decelerate()
{
	mAcceleration = -mMaxSpeed;
}

void Car::ResetAction()
{
	mAcceleration = 0.0f;
	mAngleRate = 0.0f;
}

float Car::TurnLeftRate()
{
	if (mSpeed <= 0.0f)
	{
		return mSpeed / 50.0f;
	}
	else if (mSpeed >= 50.0f)
	{
		return -0.4f - 0.6f * ((mMaxSpeed - mSpeed) / (mMaxSpeed - 50.0f));
	}
	else
	{
		return -mSpeed / 50.0f;
	}
}

float Car::TurnRightRate()
{
	if (mSpeed <= 0.0f)
	{
		return -mSpeed / 50.0f;
	}
	else if (mSpeed >= 50.0f)
	{
		return 0.4f + 0.6f * ((mMaxSpeed - mSpeed) / (mMaxSpeed - 50.0f));
	}
	else
	{
		return mSpeed / 50.0f;
	}
}

bool Car::IsArrived()
{
	if (Math::DistanceSqr(mDestination, mPosition) <= (40.0f + mRadius)*(40.0f + mRadius))
	{
		//mIsAlive = false;
		fitness += 10000u;
		mCountDown = 5.0f;
		return true;
	}
	return false;
}

void Car::CheckMapCollision(TileMap& tileMap)
{
	for (uint32_t i = 0; i < mCollisionDirection.size(); i++)
	{
		int tempx1 = static_cast<int>(mPosition.x / tileMap.GetTileSize());
		int tempy1 = static_cast<int>(mPosition.y / tileMap.GetTileSize());
		int tempx2 = static_cast<int>((mPosition.x + mCollisionDirection[i].x*mCollisionLength) / tileMap.GetTileSize());
		int tempy2 = static_cast<int>((mPosition.y + mCollisionDirection[i].y*mCollisionLength) / tileMap.GetTileSize());

		if (tempx1 > tempx2)
		{
			tempx1 = tempx1 + tempx2;
			tempx2 = tempx1 - tempx2;
			tempx1 = tempx1 - tempx2;
		}
		if (tempy1 > tempy2)
		{
			tempy1 = tempy1 + tempy2;
			tempy2 = tempy1 - tempy2;
			tempy1 = tempy1 - tempy2;
		}
		Geometry::LineSegment2D line{ mPosition,mPosition + mCollisionDirection[i] * mCollisionLength };
		Math::Vector2 contactPoint;
		float dis{ FLT_MAX };
		for (int i = tempx1; i <= tempx2; i++)
		{
			for (int j = tempy1; j <= tempy2; j++)
			{
				if (tileMap.GetTile(i,j) > 5 && Geometry::Intersect(line, tileMap.GetTileRect(i, j)))
				{
					Math::Vector2 tempPoint = Geometry::GetClosestPoint(line, tileMap.GetTileRect(i, j));
					float temp = Math::DistanceSqr(mPosition, tempPoint);
					if (temp < dis)
					{
						dis = temp;
						contactPoint = tempPoint;
					}
				}
			}
		}
		if (dis > mCollisionLength*mCollisionLength)
		{
			Graphics::DrawScreenLine({ mPosition,mPosition + mCollisionDirection[i] * mCollisionLength }, Math::Vector4::Magenta());
		}
		else
		{
			Graphics::DrawScreenLine({ mPosition,mPosition + mCollisionDirection[i] * std::sqrtf(dis) }, Math::Vector4::Magenta());
		}
		mCollisionInput[i] = dis;
	}









	/*for (int i = 0; i < MAXCOLLISIONLINES; i++)
	{
		int tile = tileMap.GetTile(static_cast<int>((mCollisionDirection[i].x + mPosition.x) / tileMap.GetTileSize()), static_cast<int>((mCollisionDirection[i].y + mPosition.y) / tileMap.GetTileSize()));
		if (tile > 5)
			mCollisionInput[i] = 1;
		else
			mCollisionInput[i] = 0;
	}*/

	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			int tile = tileMap.GetTile(static_cast<int>((mPosition.x + i * tileMap.GetTileSize()) / tileMap.GetTileSize()),
				static_cast<int>((mPosition.y + j * tileMap.GetTileSize()) / tileMap.GetTileSize()));
			float left = static_cast<int>(mPosition.x / tileMap.GetTileSize() + i) * tileMap.GetTileSize() + 2.0f;
			float right = static_cast<int>(mPosition.x / tileMap.GetTileSize() + 1.0f + i) * tileMap.GetTileSize() - 2.0f;
			float top = static_cast<int>(mPosition.y / tileMap.GetTileSize() + j) * tileMap.GetTileSize() + 2.0f;
			float bottom = static_cast<int>(mPosition.y / tileMap.GetTileSize() + 1.0f + j) * tileMap.GetTileSize() - 2.0f;
			/*if (tile > 5)
			{
				DrawScreenRect({ left,top,right,bottom }, Math::Vector4::Red());
				std::string txt = std::to_string(tile);
				S::DrawScreenText(txt.c_str(), left, top, 20.0f, S::Math::Vector4::Red());
			}
			else
			{
				DrawScreenRect({ left,top,right,bottom }, Math::Vector4::Green());
				std::string txt = std::to_string(tile);
				S::DrawScreenText(txt.c_str(), left, top, 20.0f, S::Math::Vector4::Green());
			}*/
			if (Geometry::Intersect(Geometry::Rect{ left,top,right,bottom }, Geometry::Circle{ mPosition,mRadius }) && tile > 5)
			{
				Kill();
			}
		}
	}
	
}
