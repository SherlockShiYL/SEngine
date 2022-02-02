#include "Precompiled.h"
#include "StreamWriter.h"

using namespace S;
using namespace S::Network;

StreamWriter::StreamWriter(MemoryStream& memoryStream)
	: mMemoryStream{ memoryStream }
{
}

void StreamWriter::Write(const void* data, uint32_t size)
{
	const uint32_t newHead = mMemoryStream.mHead + size;
	if (newHead > mMemoryStream.mCapacity)
	{
		mMemoryStream.ReallocBuffer(std::max(mMemoryStream.mCapacity * 2, newHead));
	}
	memcpy(mMemoryStream.mBuffer + mMemoryStream.mHead, data, size);
	mMemoryStream.mHead = newHead;
}

void StreamWriter::Write(const std::string& str)
{
	const size_t length = str.length();
	Write(length);
	Write(str.c_str(), length);
}