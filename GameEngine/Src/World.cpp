#include "Precompiled.h"
#include "World.h"

#include "Common.h"
#include "Service.h"
#include "XMLReader.h"

using namespace S;

void World::Initialize(uint32_t capacity)
{
	ASSERT(!mInitialized, "[World] World already initialized.");

	for (auto& s : mServices)
	{
		s->Initialize();
	}

	mGameObjectAllocator = std::make_unique<GameObjectAllocator>(capacity);
	mGameObjectHandlePool = std::make_unique <GameObjectHandlePool>(capacity);
	mGameObjectFactory = std::make_unique<GameObjectFactory>(*mGameObjectAllocator);

	mInitialized = true;
}

void World::Terminate()
{
	if (!mInitialized)
		return;

	ASSERT(!mUpdating, "[World] Cannot terminate world during update.");

	// Destroy all active objects
	mUpdating = true;
	std::for_each(mUpdateList.begin(),mUpdateList.end(),
		[this](GameObject* gameObject)
	{
		Destroy(gameObject->GetHandle());
	});
	mUpdating = false;
	mUpdateList.clear();

	ProcessDestroyList();

	mGameObjectAllocator.reset();
	mGameObjectHandlePool.reset();
	mGameObjectFactory.reset();

	for (auto& s : mServices)
	{
		s->Terminate();
	}

	mInitialized = false;
}

void World::LoadLevel(const char* levelFileName)
{
	TiXmlDocument doc(levelFileName);
	VERIFY(doc.LoadFile(), "Cannot load file.");
	
	TiXmlHandle hDoc(&doc);
	TiXmlNode* pRoot = doc.FirstChild();
	ASSERT(pRoot != nullptr, "Root cannot be nullptr.");
	ASSERT(strcmp(pRoot->Value(), "GameObjectList") == 0,"The xml is not the Level file.");

	TiXmlElement* pEle = pRoot->FirstChildElement();
	while (pEle != nullptr)
	{
		ASSERT(strcmp(pEle->Value(), "GameObject") == 0, "It's not a valid xml file.");

		auto handle = Create(pEle->Attribute("template"), pEle->Attribute("name"));

		TiXmlNode* tempNode = pEle->FirstChild();
		while (tempNode != nullptr)
		{
			const char* componentName = tempNode->Value();
			const S::Core::MetaClass* metaClass = Core::MetaRegistry::FindMetaClass(componentName);
			if (metaClass == nullptr)
			{
				LOG("Class [%s] not registered with the meta system.", componentName);
			}
			else
			{
				auto newComponent = handle->GetComponentByName(metaClass);
				const TiXmlNode* data = tempNode->FirstChild();
				VERIFY(Util::XMLReader::Read(newComponent, metaClass, data), "Failed to read %s.", componentName);
			}
			tempNode = tempNode->NextSibling();
		}
		pEle = pEle->NextSiblingElement();
	}
}

GameObjectHandle World::Create(const char * templateFileName, const char * name)
{
	auto gameObject = mGameObjectFactory->Create(templateFileName);
	ASSERT(gameObject != nullptr, "[World] Failed to create game object from template %s", templateFileName);

	// Register with the handle pool
	auto handle = mGameObjectHandlePool->Register(gameObject);

	// Initialize the game object
	gameObject->mWorld = this;
	gameObject->mHandle = handle;
	gameObject->mName = name;
	gameObject->Initialize();

	// Add game object to the update list
	mUpdateList.push_back(gameObject);

	return handle;
}

GameObjectHandle World::Find(const char * name)
{
	for (auto& object : mUpdateList)
	{
		if (strcmp(object->GetName().c_str(), name) == 0)
		{
			return object->GetHandle();
		}
	}
	return GameObjectHandle();
}

void World::Destroy(GameObjectHandle handle)
{
	if (!handle.IsValid())
		return;

	// Cache the pointer and unregister with the handle pool
	GameObject* gameObject = handle.Get();
	mGameObjectHandlePool->Unregister(handle);

	if (!mUpdating)
		DestroyInternal(gameObject);
	else
		mDestroyList.push_back(gameObject);
}

void World::Update(float deltaTime)
{
	ASSERT(!mUpdating, "[World] Already updating the world.");
	for (auto& s : mServices)
	{
		s->Update(deltaTime);
	}
	// Lock the update list
	mUpdating = true;

	// Re-compute size in case new objects are added to the update list during literation
	for (size_t i = 0; i < mUpdateList.size(); ++i)
	{
		GameObject* gameObject = mUpdateList[i];
		if (gameObject->GetHandle().IsValid())
		{
			gameObject->Update(deltaTime);
		}
	}

	// Unlock the update list
	mUpdating = false;

	ProcessDestroyList();
}

void World::Render()
{
	for (auto& s : mServices)
	{
		s->Render();
	}
	for (auto gameObject : mUpdateList)
	{
		gameObject->Render();
	}
}

void World::Render2D()
{
	for (auto& s : mServices)
	{
		s->Render2D();
	}
	for (auto gameObject : mUpdateList)
	{
		gameObject->Render2D();
	}
}

void World::DestroyInternal(GameObject * gameObject)
{
	ASSERT(!mUpdating, "[World] Cannot destroy game objects during update.");

	// If pointer is invalid, nothing to do so just exit
	if (gameObject == nullptr)
		return;

	// First remove it from the update list
	auto iter = std::find(mUpdateList.begin(), mUpdateList.end(), gameObject);
	if (iter != mUpdateList.end())
		mUpdateList.erase(iter);

	// Terminate the game object
	gameObject->Terminate();

	// Next destroy the game object
	mGameObjectFactory->Destroy(gameObject);
}

void World::ProcessDestroyList()
{
	for (auto gameObject : mDestroyList)
	{
		DestroyInternal(gameObject);
	}
	mDestroyList.clear();
}