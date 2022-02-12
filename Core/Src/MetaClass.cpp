#include "Precompiled.h"
#include "MetaClass.h"

#include "Debug.h"
#include "MetaField.h"
#include "MetaRegistry.h"

using namespace S;
using namespace S::Core;

MetaClass::MetaClass(
	const char* const name,
	size_t size,
	MetaType::CreateFunc create,
	MetaType::DestroyFunc destroy,
	const MetaClass* const parent,
	const MetaField* const fields,
	size_t numFields)
	: MetaType(name, MetaType::Category::Class, size, std::move(create), std::move(destroy))
	, mParent{ parent }
	, mFields{ fields }
	, mNumFields{ numFields }
{
	MetaRegistry::Register(this);
}

const MetaClass* MetaClass::GetParent() const
{
	return mParent;
}

const MetaField* MetaClass::FindField(const char* name) const
{
	const MetaField* field = nullptr;
	for (size_t i = 0; i < mNumFields; ++i)
	{
		const MetaField& f = mFields[i];
		if (strcmp(f.GetName(), name) == 0)
		{
			field = &f;
			break;
		}
	}

	// If the field is not found, try the parent
	if (field == nullptr&& mParent != nullptr)
	{
		field = mParent->FindField(name);
	}
	return field;
}

void MetaClass::Dump() const
{
	LOG("Class [%s]", GetName());
	if (mParent)
	{
		LOG("is derived from [%s]", mParent->GetName());
	}
	for (size_t i = 0; i < mNumFields; ++i)
	{
		auto& field = mFields[i];
		LOG("has field [%s] of type [%s] at offset [%zd]", field.GetName(), field.GetMetaType()->GetName(), field.GetOffset());
	}
}