#ifndef INCLUDED_CORE_TIMER_H
#define INCLUDED_CORE_TIMER_H

namespace S {
namespace Core {

class Timer
{
public:
	Timer();

	void Initialize();
	void Update();

	float GetElapsedTime() const;
	float GetTotalTime() const;
	float GetFramesPerSecond() const;

private:
	// http://msdn2.microsoft.com/en-us/library/aa383713.aspx
	LARGE_INTEGER mTicksPerSecond;
	LARGE_INTEGER mLastTick;
	LARGE_INTEGER mCurrentTick;

	float mElapsedTime;
	float mTotalTime;

	float mLastUpdateTime;
	float mFrameSinceLastSecond;
	float mFramesPerSecond;
};

} // namespace Core
} // namespace S

#endif // #ifndef INCLUDED_CORE_TIMER_H