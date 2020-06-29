#include "Precompiled.h"
#include "GameObject.h"

#include "Component.h"

using namespace S;

GameObject::GameObject()
	: mName{ "Unnamed" }
{}

GameObject::~GameObject()
{

}

void GameObject::Initialize()
{
	for (auto& component : mComponents)
	{
		component->Initialize();
	}
}

void GameObject::Terminate()
{
	for (auto& component : mComponents)
	{
		component->Terminate();
	}
}

void GameObject::Update(float deltaTime)
{
	for (auto& component : mComponents)
	{
		component->Update(deltaTime);
	}
}

void GameObject::Render()
{
	for (auto& component : mComponents)
	{
		component->Render();
	}
}

void GameObject::Render2D()
{
	for (auto& component : mComponents)
	{
		component->Render2D();
	}
}

Component* GameObject::AddComponent(const Core::MetaClass* metaClass)
{
	mComponents.emplace_back(static_cast<Component*>(metaClass->Create()));
	auto& newComponent = mComponents.back();
	newComponent->mGameObject = this;
	return newComponent.get();
}

Component* GameObject::GetComponentAt(size_t index)
{
	return mComponents[index].get();
}

Component* GameObject::GetComponentByName(const Core::MetaClass* metaClass)
{
	for (auto& component : mComponents)
	{
		if (strcmp(component->GetMetaClass()->GetName(), metaClass->GetName()) == 0)
		{
			return component.get();
		}
	}
	return nullptr;
}