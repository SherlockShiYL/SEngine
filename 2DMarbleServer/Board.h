#include <GameEngine/Inc/GameEngine.h>
#include "Command.h"

class Board
{
public:
	void Load(S::Math::Vector2 p, const char* fileName);

	void Update(float deltaTime);
	void Render();
	void MoveRight();
	void MoveLeft();

	void Serialize(S::Network::StreamWriter& writer) const;
	void Deserialize(S::Network::StreamReader& reader);

	const S::Math::Vector2& GetPosition() const;
	const S::Geometry::Rect& GetRect() const;

	void SetCommand(const Command& command);

private:
	S::Geometry::Rect mRect;
	S::Math::Vector2 mPosition;
	float mVelocity;
	S::Graphics::TextureId mId;
	Command mCommand;
};
