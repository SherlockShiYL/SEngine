#ifndef INCLUDED_NETWORK_MEMORYSTREAM_H
#define INCLUDED_NETWORK_MEMORYSTREAM_H

#include "SocketAddress.h"

namespace S {
namespace Network {

class MemoryStream
{
public:
	MemoryStream() = default;
	MemoryStream(uint32_t capacity);
	MemoryStream(const uint8_t* data, uint32_t size, bool takeOwnership = false);
	~MemoryStream();

	void Reset() { mHead = 0; }

	const uint8_t* GetData() const { return mBuffer; }
	uint32_t GetHead() const { return mHead; }
	uint32_t GetCapacity() const { return mCapacity; }

private:
	MemoryStream(const MemoryStream&) = delete;
	MemoryStream& operator=(const MemoryStream&) = delete;

	friend class StreamReader;
	friend class StreamWriter;

	void ReallocBuffer(uint32_t newCapacity);

	uint8_t* mBuffer{ nullptr };
	uint32_t mHead{ 0 };
	uint32_t mCapacity{ 0 };
};

} // namespace Network
} // namespace S

#endif // !INCLUDED_NETWORK_MEMORYSTREAM_H