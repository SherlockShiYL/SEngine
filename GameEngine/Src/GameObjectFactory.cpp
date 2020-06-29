#include "Precompiled.h"
#include "GameObjectFactory.h"

#include "Common.h"
#include "Component.h"
#include "XMLReader.h"

using namespace S;

GameObjectFactory::GameObjectFactory(GameObjectAllocator& allocator)
	: mGameObjectAllocator{ allocator }
{}

GameObject* GameObjectFactory::CreateTank()
{
	GameObject* gameObject = mGameObjectAllocator.New();
	return gameObject;
}

GameObject* GameObjectFactory::Create(const char* templateFileName)
{
	GameObject* gameObject = mGameObjectAllocator.New();
	TiXmlDocument templateDoc;
	VERIFY(templateDoc.LoadFile(templateFileName), "[GameObjectFactory] Failed to open template file '%s.", templateDoc.Value());
	
	const TiXmlNode* gameObjectNode = templateDoc.FirstChild();
	if (strcmp(gameObjectNode->Value(), "GameObject") == 0)
	{
		const TiXmlNode* componentsNode = gameObjectNode->FirstChild();
		if (strcmp(componentsNode->Value(), "Components") == 0)
		{
			const TiXmlNode* nextComponentNode = componentsNode->FirstChild();

			while (nextComponentNode != nullptr)
			{
				const char* componentName = nextComponentNode->Value();

				const Core::MetaClass* metaClass = Core::MetaRegistry::FindMetaClass(componentName);
				if (metaClass == nullptr)
				{
					LOG("Class [%s] not registered with the meta system.", componentName);
				}
				else
				{
					auto newComponent = gameObject->AddComponent(metaClass);
					const TiXmlNode* data = nextComponentNode->FirstChild();
					VERIFY(Util::XMLReader::Read(newComponent, metaClass, data), "Failed to read %s.", componentName);
				}

				nextComponentNode = nextComponentNode->NextSibling();
			}
		}
	}
	return gameObject;
}

void GameObjectFactory::Destroy(GameObject* gameObject)
{
	mGameObjectAllocator.Delete(gameObject);
}
