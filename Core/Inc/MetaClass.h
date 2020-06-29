#ifndef INCLUDED_CORE_METACLASS_H
#define INCLUDED_CORE_METACLASS_H

#include "MetaType.h"

namespace S {
namespace Core {

class MetaField;

class MetaClass : public MetaType
{
public:
	MetaClass(
		const char* const name,
		size_t size,
		MetaType::CreateFunc create,
		MetaType::DestroyFunc destroy,
		const MetaClass* const parent,
		const MetaField* const fields,
		size_t numFields);

	const MetaClass* GetParent() const;
	const MetaField* FindField(const char* name) const;

	void Dump() const;

private:
	const MetaClass* const mParent{ nullptr };
	const MetaField* const mFields{ nullptr };
	const size_t mNumFields{ 0 };
};

} // namespace Core
} // namespace S

#endif // !INCLUDED_CORE_METACLASS_H