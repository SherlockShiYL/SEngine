#ifndef INCLUDED_CORE_METATYPE_H
#define INCLUDED_CORE_METATYPE_H

namespace S {
namespace Core {

class MetaType
{
public:
	using CreateFunc = std::function<void*()>;
	using DestroyFunc = std::function<void(void*)>;

	enum class Category
	{
		Primitive,
		Class,
		Array,
		Pointer
	};

	MetaType(const char* const name, Category category, size_t size, CreateFunc create, DestroyFunc destroy);

	const char* GetName() const		{ return mName; }
	Category GetCategory() const	{ return mCategory; }
	size_t GetId() const			{ return mId; }
	size_t GetSize() const			{ return mSize; }

	void* Create() const			{ return mCreate(); }
	void Destroy(void* data) const	{ mDestroy(data); }

private:
	const char* mName;
	Category mCategory;
	size_t mId;
	size_t mSize;

	CreateFunc mCreate;
	DestroyFunc mDestroy;
};

} // namespace Core
} // namespace S

#endif // !INCLUDED_CORE_METATYPE_H