#ifndef INCLUDED_AI_MEMORYRECORD_H
#define INCLUDED_AI_MEMORYRECORD_H

//#include <Math/Inc/SMath.h>

namespace S {
namespace AI {

	class MemoryRecord
	{
	public:
		MemoryRecord()
			: lastRecordedTime(0.0f)
			, importance(0.0)
		{}

		virtual ~MemoryRecord() {}

		float lastRecordedTime;
		Math::Vector2 lastLocation;
		float importance;
	};

	typedef std::list<MemoryRecord*> MemoryRecords;

} // namespace AI
} // namespace S

#endif // #include INCLUDED_AI_MEMORYRECORD_H