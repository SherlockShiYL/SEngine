#ifndef INCLUDED_CORE_BLOCKALLOCATOR_H
#define INCLUDED_CORE_BLOCKALLOCATOR_H

#include <vector>
namespace S {
namespace Core {

class BlockAllocator
{
public:
	BlockAllocator(uint32_t blockSize, uint32_t capacity);
	~BlockAllocator();

	BlockAllocator(const BlockAllocator&) = delete;
	BlockAllocator& operator=(const BlockAllocator&) = delete;

	void* Allocate();
	void Free(void* ptr);

protected:
	uint8_t* mData;
	uint32_t mBlockSize;
	uint32_t mCapacity;
	std::vector<uint32_t> mFreeSlots;
	//int mFreeSlot; // more optimize
};

} // namespace Core
} // namespace S

#endif // #ifndef INCLUDED_CORE_BLOCKALLOCATOR_H