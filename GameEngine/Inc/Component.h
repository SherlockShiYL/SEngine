#ifndef INCLUDED_GAMEENGINE_COMPONENT_H
#define INCLUDED_GAMEENGINE_COMPONENT_H

namespace S {
class GameObject;

class Component
{
public:
	META_CLASS_DECLARE;

	Component(std::string typeId);
	virtual ~Component();

	virtual void Initialize() {}
	virtual void Terminate() {}

	virtual void Update(float deltaTime) {}
	virtual void Render() {}
	virtual void Render2D() {}

	GameObject& GetOwner() { return *mGameObject; }
	const GameObject& GetOwner() const { return *mGameObject; }

private:
	friend class GameObject;

	GameObject* mGameObject;
	std::string mTypeId;
};

} // namespace S
#endif // !INCLUDED_GAMEENGINE_COMPONENT_H