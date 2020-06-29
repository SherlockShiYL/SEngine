#ifndef INCLUDED_GAMEENGINE_WORLD_H
#define INCLUDED_GAMEENGINE_WORLD_H

#include "GameObjectFactory.h"

namespace S {

class Service;

class World
{
public:
	void Initialize(uint32_t capacity);
	void Terminate();

	template<class T>
	T* AddService();
	template<class T>
	T* GetService();

	void LoadLevel(const char* levelFileName);

	GameObjectHandle Create(const char* templateFileName, const char* name);
	GameObjectHandle Find(const char* name);
	void Destroy(GameObjectHandle handle);

	void Update(float deltaTime);
	void Render();
	void Render2D();

private:
	void DestroyInternal(GameObject* gameObject);
	void ProcessDestroyList();

	using Services = std::vector<std::unique_ptr<Service>>;
	using GameObjectList = std::vector<GameObject*>;

	Services mServices;

	std::unique_ptr<GameObjectAllocator> mGameObjectAllocator;
	std::unique_ptr<GameObjectHandlePool> mGameObjectHandlePool;
	std::unique_ptr<GameObjectFactory> mGameObjectFactory;

	GameObjectList mUpdateList;
	GameObjectList mDestroyList;
	bool mInitialized{ false };
	bool mUpdating{ false };
};

template<class T>
T* World::AddService()
{
	static_assert(std::is_base_of_v<Service, T>, "[World] only child of Service is allowed");
	ASSERT(!mInitialized, "[World] Cannot add Service after world has already initialized.");
	mServices.emplace_back(std::make_unique<T>());
	auto& service = mServices.back();
	service->mWorld = this;
	return static_cast<T*>(service.get());
}

template<class T>
T* World::GetService()
{
	for (auto& s : mServices)
	{
		if (T::StaticGetTypeId() == s->GetTypeId())
			return static_cast<T*>(s.get());
	}
	return nullptr;
}

} // namespace S

#endif // !INCLUDED_GAMEENGINE_WORLD_H