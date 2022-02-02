#ifndef INCLUDED_CORE_DELETEUTIL_H
#define INCLUDED_CORE_DELETEUTIL_H

namespace S {

namespace Graphics {
	using TextureId = std::size_t;
}
template <class T>
inline void SafeDelete(T*& ptr)
{
	delete ptr;
	ptr = nullptr;
}

template <class T>
inline void SafeDeleteArray(T*& ptr)
{
	delete[] ptr;
	ptr = nullptr;
}

template <class T>
inline void SafeRelease(T*& ptr)
{
	if (ptr)
	{
		ptr->Release();
		ptr = nullptr;
	}
}

} // namespace S

#endif // !INCLUDED_CORE_DELETEUTIL_H