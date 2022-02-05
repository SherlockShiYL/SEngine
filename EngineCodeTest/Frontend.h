#ifndef INCLUND_ENGINECODETEST_FRONTEND_H
#define INCLUND_ENGINECODETEST_FRONTEND_H

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

#endif //#define INCLUND_ENGINECODETEST_FRONTEND_H

