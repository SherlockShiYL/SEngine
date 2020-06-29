#include "Precompiled.h"
#include "BlockAllocator.h"

using namespace S;
using namespace S::Core;

BlockAllocator::BlockAllocator(uint32_t blockSize, uint32_t capacity)
	: mBlockSize{ blockSize }
	, mCapacity{ capacity }
{
	mData = (uint8_t*)malloc(blockSize*capacity);
	// Reserve the capacity, do new just once. Keep the memory.
	mFreeSlots.reserve(capacity);
	for (uint32_t i = 0; i < capacity; ++i)
	{
		mFreeSlots.emplace_back(i);
	}
}

BlockAllocator::~BlockAllocator()
{
	free(mData);
}

void * BlockAllocator::Allocate()
{
	if (!mFreeSlots.empty())
	{
		void* block = mData + (mBlockSize * mFreeSlots.back());

		mFreeSlots.pop_back();

		return block;
	}
	return nullptr;
}

void BlockAllocator::Free(void * ptr)
{
	uint8_t* temp = reinterpret_cast<uint8_t*>(ptr);

	if (temp >= mData 
		&& temp < mData + mBlockSize * mCapacity
		&& (temp - mData) % mBlockSize == 0)
	{
		uint32_t pLocation = static_cast<uint32_t>(temp - mData) / mCapacity;
		mFreeSlots.emplace_back(pLocation);
	}
}
