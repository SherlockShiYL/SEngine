#include "Precompiled.h"
#include "XMLReader.h"

#include "Common.h"

using namespace S;
using namespace S::Core;

namespace
{
	bool Read(int* instance, const TiXmlNode* node)
	{
		if (node == nullptr)
			return false;
		*instance = atoi(node->Value());
		return true;
	}
	bool Read(float* instance, const TiXmlNode* node)
	{
		if (node == nullptr)
			return false;
		*instance = static_cast<float>(atof(node->Value()));
		return true;
	}
	bool Read(bool* instance, const TiXmlNode* node) // 1 or 0
	{
		if (node == nullptr)
			return false;
		*instance = static_cast<bool>(atoi(node->Value()));
		return true;
	}
	bool Read(std::string* instance, const TiXmlNode* node)
	{
		if (node == nullptr)
			return false; 
		*instance = node->Value();
		return true;
	}
	bool Read(Math::Vector2* instance, const TiXmlNode* node)
	{
		if (node->FirstChild() == nullptr ||
			node->NextSibling() == nullptr ||
			node->NextSibling()->FirstChild() == nullptr)
			return false;

		Read(&(instance->x), node->FirstChild());
		Read(&(instance->y), node->NextSibling()->FirstChild());
		return true;
	}
	bool Read(Math::Vector3* instance, const TiXmlNode* node)
	{
		auto pEle = node->ToElement();
		if (node->FirstChild() == nullptr ||
			node->NextSibling() == nullptr ||
			node->NextSibling()->FirstChild() == nullptr ||
			node->NextSibling()->NextSibling() == nullptr ||
			node->NextSibling()->NextSibling()->FirstChild() == nullptr)
		{
			return false;
		}
		Read(&(instance->x), node->FirstChild());
		Read(&(instance->y), node->NextSibling()->FirstChild());
		Read(&(instance->z), node->NextSibling()->NextSibling()->FirstChild());
		return true;
	}
	bool Read(Math::Vector4* instance, const TiXmlNode* node)
	{
		if (node->FirstChild() == nullptr ||
			node->NextSibling() == nullptr ||
			node->NextSibling()->FirstChild() == nullptr ||
			node->NextSibling()->NextSibling() == nullptr ||
			node->NextSibling()->NextSibling()->FirstChild() == nullptr ||
			node->NextSibling()->NextSibling()->NextSibling() == nullptr ||
			node->NextSibling()->NextSibling()->NextSibling()->FirstChild() == nullptr
			)
			return false;

		Read(&(instance->x), node->FirstChild());
		Read(&(instance->y), node->NextSibling()->FirstChild());
		Read(&(instance->z), node->NextSibling()->NextSibling()->FirstChild());
		Read(&(instance->w), node->NextSibling()->NextSibling()->NextSibling()->FirstChild());
		return true;
	}
}

bool ReadPrimitive(void* instance, const MetaType* metaType, const TiXmlNode* node)
{
	if (metaType->GetId() == GetMetaType<int>()->GetId())
		return Read(static_cast<int*>(instance), node);
	if (metaType->GetId() == GetMetaType<float>()->GetId())
		return Read(static_cast<float*>(instance), node);
	if (metaType->GetId() == GetMetaType<bool>()->GetId())
		return Read(static_cast<bool*>(instance), node);
	if (metaType->GetId() == GetMetaType<std::string>()->GetId())
		return Read(static_cast<std::string*>(instance), node);
	if (metaType->GetId() == GetMetaType<Math::Vector2>()->GetId())
		return Read(static_cast<Math::Vector2*>(instance), node);
	if (metaType->GetId() == GetMetaType<Math::Vector3>()->GetId())
		return Read(static_cast<Math::Vector3*>(instance), node);
	if (metaType->GetId() == GetMetaType<Math::Vector4>()->GetId())
		return Read(static_cast<Math::Vector4*>(instance), node);
	return false;
}

bool Read(void* instance, const MetaType* metaType, const TiXmlNode* node)
{
	if (node == nullptr)
		return false;

	switch (metaType->GetCategory())
	{
	case MetaType::Category::Primitive:
		return ReadPrimitive(instance, metaType, node);
	case MetaType::Category::Class:
		return Util::XMLReader::Read(instance, static_cast<const S::Core::MetaClass*>(metaType), node);
	case MetaType::Category::Pointer:
		return true;
	case MetaType::Category::Array:
		return true;
	}
	return false;
}

bool Util::XMLReader::Read(void* instance, const S::Core::MetaClass* metaClass, const TiXmlNode * node)
{
	while(node != nullptr)
	{
		const TiXmlElement* element = node->ToElement();
		if (element != nullptr)
		{
			const char* fieldName = element->Attribute("name");
			const MetaField* metaField = metaClass->FindField(fieldName);
			if (metaField != nullptr)
			{
				void* member = static_cast<uint8_t*>(instance) + metaField->GetOffset();
				bool success = ::Read(member, metaField->GetMetaType(), element->FirstChild());
				if (!success)
				{
					return false;
				}
			}
		}
		node = node->NextSibling();
	}
	return true;
}