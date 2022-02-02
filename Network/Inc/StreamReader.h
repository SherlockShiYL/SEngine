#ifndef INCLUDED_NETWORK_STREAMREADER_H
#define INCLUDED_NETWORK_STREAMREADER_H

#include "MemoryStream.h"

namespace S {
namespace Network {

class StreamReader
{
public:
	StreamReader(MemoryStream& memoryStream);

	void Read(void* data, uint32_t size);
	void Read(std::string& str);

	template <class T>
	void Read(T& data)
	{
		static_assert(std::is_arithmetic_v<T> || std::is_enum_v<T>, "Generic Read only supports primitive data types");
		Read(&data, static_cast<uint32_t>(sizeof(T)));
	}

	template <class T>
	void Read(const std::vector<T>& data)
	{
		Read(data.size());
		for (T& item : data)
		{
			Read(item);
		}
	}

	uint32_t GetRemainingDataSize() const
	{
		return mMemoryStream.mCapacity - mMemoryStream.mHead;
	}

private:
	MemoryStream& mMemoryStream;
};

} // namespace Network
} // namespace S

#endif // !INCLUDED_NETWORK_STREAMREADER_H