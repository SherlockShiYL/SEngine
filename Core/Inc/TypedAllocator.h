#ifndef INCLUDED_CORE_TYPEDALLOCATOR_H
#define INCLUDED_CORE_TYPEDALLOCATOR_H

#include "BlockAllocator.h"

namespace S{
namespace Core {

template<class T>
class TypedAllocator : private BlockAllocator
{
public:
	TypedAllocator(uint32_t capacity)
		: BlockAllocator(sizeof(T), capacity)
	{
	}

	// Variadic template arguments
	// all the template class types will be replace to here: class...
	template<class... Args>
	T* New(Args&&... args)
	{
		T* instance = static_cast<T*>(Allocate());
		if (instance)
		{
			new (instance)T(std::forward<Args>(args)...);
		}
		return instance;
	}

	void Delete(T* ptr)
	{
		if (ptr == nullptr)
		{
			return;
		}
		ptr->~T();
		Free(ptr);
	}
};

} // namespace Core
} // namespace S

#endif // !INCLUDED_CORE_TYPEDALLOCATOR_H