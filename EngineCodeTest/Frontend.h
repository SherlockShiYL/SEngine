#ifndef INCLUDED_ENGINECODETEST_FRONTEND_H
#define INCLUDED_ENGINECODETEST_FRONTEND_H

#include "GameState.h"

class Frontend : public GameState
{
public:
	Frontend();
	~Frontend() override;

	void Load() override;
	void Unload() override;
	Transition Update(float deltaTime) override;
	void Render() override;

private:

};

#endif //#define INCLUDED_ENGINECODETEST_FRONTEND_H

