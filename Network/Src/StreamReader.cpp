#include "Precompiled.h"
#include "StreamReader.h"

using namespace S;
using namespace S::Network;

StreamReader::StreamReader(MemoryStream& memoryStream)
	: mMemoryStream{ memoryStream }
{
}

void StreamReader::Read(void* data, uint32_t size)
{
	const uint32_t newHead = mMemoryStream.mHead + size;
	if (newHead <= mMemoryStream.mCapacity)
	{
		std::memcpy(data, mMemoryStream.mBuffer + mMemoryStream.mHead, size);
		mMemoryStream.mHead = newHead;
	}
}

void StreamReader::Read(std::string& str)
{
	size_t length{ 0 };
	Read(length);
	str.resize(length);
	Read(&str[0], static_cast<uint32_t>(length));
}