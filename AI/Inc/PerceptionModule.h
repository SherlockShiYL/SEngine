#ifndef INCLUDED_AI_PERCEPTIONMODULE_H
#define INCLUDED_AI_PERCEPTIONMODULE_H

#include "MemoryRecord.h"
#include "Sensor.h"

namespace S {
namespace AI {

	class Agent;

	struct ImportanceCalculator
	{
		virtual void operator()(MemoryRecord* record) = 0;
	};

	class PerceptionModule
	{
	public:
		PerceptionModule(Agent& agent, ImportanceCalculator* calculator);
		~PerceptionModule();

		void AddSensor(Sensor* sensor);
		void Purge();

		void Update(float deltaTime);

		MemoryRecords& GetMemoryRecords() { return mMemory; }

	private:
		Agent& mAgent;
		ImportanceCalculator* mCalculator;
		Sensors mSensors;
		MemoryRecords mMemory;
	};

} // namespace AI
} // namespace S

#endif // #include INCLUDED_AI_PERCEPTIONMODULE_H