#include "Character.h"

using namespace S;

namespace
{
	const float diagonal = Math::Normalize({ 1.0f,1.0f }).x;
}

Character::~Character()
{}

Character::Character(AI::AIWorld& world, int type, uint32_t id)
	:Agent{ world }
{
	Id() = id;
	Type() = type;
}

void Character::AddCommand(const Command& command)
{
	if (mCommand.GetCommand() != command.GetCommand())
	{
		mCommand = command;
		mCommandList.push_back(command);
	}
}

void Character::CheckDirection(const Command &command)
{
	if (command.isMoveDown())
	{
		if (command.isMoveRight())
		{
			Heading() = { diagonal,diagonal };
		}
		else if (command.isMoveLeft())
		{
			Heading() = { -diagonal,diagonal };
		}
		else
		{
			Heading() = { 0.0f,1.0f };
		}
	}
	else if (command.isMoveUp())
	{
		if (command.isMoveRight())
		{
			Heading() = { diagonal,-diagonal };
		}
		else if (command.isMoveLeft())
		{
			Heading() = { -diagonal,-diagonal };
		}
		else
		{
			Heading() = { 0.0f,-1.0f };
		}
	}
	else if (command.isMoveRight())
	{
		Heading() = { 1.0f,0.0f };
	}
	else if (command.isMoveLeft())
	{
		Heading() = { -1.0f,0.0f };
	}
}

void Character::ReduceHP(int incomedamage)
{
	mHP -= incomedamage;
}