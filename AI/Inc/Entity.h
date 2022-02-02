#ifndef INCLUDED_ENTITY_H
#define INCLUDED_ENTITY_H

namespace S {
namespace AI {

class AIWorld;

class Entity
{
public:
	Entity(AIWorld& aiWorld);
	virtual ~Entity();

	void Load(Math::Vector2 p, uint32_t type, uint32_t id);
	void Load(float tileSize, int columns, int rows, uint32_t type, uint32_t id);

	virtual void Render(Math::Vector2 p) {}

	AIWorld& World() const					{ return mWorld; }

	void SetBlocked(bool b = true);

	Math::Vector2& Position()				{ return mPosition; }
	const Math::Vector2& Position() const	{ return mPosition; }

	uint32_t& Type()						{ return mType; }
	const uint32_t& Type() const			{ return mType; }

	uint32_t& Id()							{ return mId; }
	const uint32_t& Id() const				{ return mId; }

protected:
	AIWorld& mWorld;
	Math::Vector2 mPosition{ 0.0f,0.0f };
	uint32_t mType{ 0 };
	uint32_t mId{ 0 };
};

using EntityList = std::vector<Entity*>;

} // namespace AI
} // namespace S

#endif // !INCLUDED_ENTITY_H