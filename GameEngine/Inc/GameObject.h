#ifndef INCLUDED_GAMEENGINE_GAMEOBJECT_H
#define INCLUDED_GAMEENGINE_GAMEOBJECT_H

namespace S {

class World;
class Component;
class GameObject;

using GameObjectHandle = Core::Handle<GameObject>;
using GameObjectHandlePool = Core::HandlePool<GameObject>;
using GameObjectAllocator = Core::TypedAllocator<GameObject>;

class GameObject
{
public:
	GameObject();
	~GameObject();

	void Initialize();
	void Terminate();

	void Update(float deltaTime);
	void Render();
	void Render2D();

	template <class T>
	typename std::enable_if_t < std::is_base_of_v<Component, T>, T*> AddComponent();

	Component* AddComponent(const Core::MetaClass* metaClass);

	Component* GetComponentAt(size_t index);
	Component* GetComponentByName(const Core::MetaClass* metaClass);
	size_t GetComponentCount() const		{ return mComponents.size(); }

	World& GetWorld() const					{ return *mWorld; }
	GameObjectHandle GetHandle() const		{ return mHandle; }
	const std::string& GetName() const		{ return mName; }

private:
	using Components = std::vector<std::unique_ptr<Component>>;

	friend class World;
	World* mWorld{ nullptr };
	std::string mName;
	GameObjectHandle mHandle;
	Components mComponents;
};

template <class T>
typename std::enable_if_t < std::is_base_of_v<Component, T>, T*>
	GameObject::AddComponent()
{
	auto& newComponent = mComponents.emplace_back(std::make_unique<T>());
	newComponent->mGameObject = this;
	return static_cast<T*>(newComponent.get());
}
}

#endif // !INCLUDED_GAMEENGINE_GAMEOBJECT_H