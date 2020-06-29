#ifndef INCLUDED_GAMEENGINE_UTILS_H
#define INCLUDED_GAMEENGINE_UTILS_H

#include <string.h>

namespace S {
namespace Utils
{
	inline bool AreEqual(const char* str1, const char* str2)
	{
		return strcmp(str1, str2) == 0;
	}
} // namespace Utils
} // namespace S

#endif // #ifndef INCLUDED_GAMEENGINE_UTILS_H