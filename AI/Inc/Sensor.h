#ifndef INCLUDED_AI_SENSOR_H
#define INCLUDED_AI_SENSOR_H

#include "MemoryRecord.h"

namespace S {
namespace AI {

	class Agent;

	class Sensor
	{
	public:
		Sensor() {}
		virtual ~Sensor() {}

		virtual void Update(Agent& agent, MemoryRecords& memory, float deltaTime) = 0;
		virtual const char* GetName() const = 0;
	};

	typedef std::vector<Sensor*> Sensors;

} // namespace AI
} // namespace S

#endif // #include INCLUDED_AI_SENSOR_H