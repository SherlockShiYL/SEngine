#ifndef INCLUDED_CORE_METAUTIL_H
#define INCLUDED_CORE_METAUTIL_H

namespace S {
namespace Core {

class MetaType;

template <class DataType>
inline const MetaType* GetMetaType()
{
	static_assert(sizeof(DataType) == -1, "Cannot deduce type");
}

template <class ClassType, class DataType>
inline const MetaType* GetMetaType(DataType ClassType::*)
{
	return GetMetaType<DataType>();
}

template <class ClassType, class DataType>
inline size_t GetFieldOffset(DataType ClassType::* field)
{
	return (size_t)(void*)&(((ClassType*)nullptr)->*field);
}

template <class BaseType, class DerivedType>
inline DerivedType* DynamicCast(BaseType* ptr)
{
	if (ptr->GetMetaClass() != DerivedType::StaticGetMetaClass())
		return nullptr;
	return static_cast<DerivedType*>(ptr);
}

template <class BaseType, class DerivedType>
inline bool IsBaseOf(BaseType* base, DerivedType* derived)
{
	auto parent = derived->StaticGetMetaClass()->GetParent();
	while (parent)
	{
		if (parent == base->StaticGetMetaClass())
			return true;
		parent = parent->GetParent();
	}
	return false;
}

} // namespace Core
} // namespace S

#endif // !INCLUDED_CORE_METAUTIL_H