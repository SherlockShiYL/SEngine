#include <GameEngine/Inc/GameEngine.h>

class Marble
{
public:
	void Load();
	void InitializeVelocity();

	void Update(float deltaTime);
	void Render();

	void SetVelocity(const S::Math::Vector2& v)		{ mVelocity = v; }
	void SetPosition(const S::Math::Vector2& p)		{ mPosition = p; }
	void SetSpeed(const float s)					{ mSpeed = s; }
	S::Geometry::Circle GetCircle() const			{ return S::Geometry::Circle(mPosition, mRadius); }
	const S::Math::Vector2& GetVelocity() const		{ return mVelocity; }
	const S::Math::Vector2& GetPosition() const		{ return mPosition; }
	const float GetSpeed() const					{ return mSpeed; }

private:
	float mRadius{ 11.0f };
	float mSpeed{ 100.0f };
	S::Graphics::TextureId mId;
	S::Math::Vector2 mPosition;
	S::Math::Vector2 mVelocity{ 0.0f,0.0f };
};