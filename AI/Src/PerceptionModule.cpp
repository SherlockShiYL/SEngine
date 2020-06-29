#include "Precompiled.h"
#include "PerceptionModule.h"

using namespace S;
using namespace AI;

PerceptionModule::PerceptionModule(Agent& agent, ImportanceCalculator* calculator)
	: mAgent(agent)
	, mCalculator(calculator)
{}

PerceptionModule::~PerceptionModule()
{
	ASSERT(mSensors.empty(), "[PerceptionModule] Purge() must be called before destruction.");
}

void PerceptionModule::AddSensor(Sensor* sensor)
{
	mSensors.push_back(sensor);
}

void PerceptionModule::Purge()
{
	for (auto s : mSensors)
	{
		SafeDelete(s);
	}
	mSensors.clear();
}

void PerceptionModule::Update(float deltaTime)
{
	for (auto s : mSensors)
	{
		s->Update(mAgent, mMemory, deltaTime);
	}

	for (auto m : mMemory)
	{
		(*mCalculator)(m);
	}

	mMemory.sort([](MemoryRecord* a, MemoryRecord* b)
	{
		return a->importance > b->importance;
	});
}