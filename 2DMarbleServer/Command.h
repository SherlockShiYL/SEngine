#include "NetworkMessage.h"

class Command
{
public:
	bool Empty() const				{ return !mCommand; }

	bool MoveUp() const				{ return mCommand&kUp; }
	bool MoveDown() const			{ return mCommand&kDown; }
	bool MoveRight() const			{ return mCommand&kRight; }
	bool MoveLeft() const			{ return mCommand&kLeft; }
	bool Attack() const				{ return mCommand&kAttack; }
	bool UseSkill() const			{ return mCommand&kSkill; }
	bool UseUlt() const				{ return mCommand&kUlt; }
	bool UseItem() const			{ return mCommand&kUseItem; }
	bool ItemLeftSlide() const		{ return mCommand&kItemLeftSlide; }
	bool ItemRightSlide() const		{ return mCommand&kItemRightSlide; }
	bool Jump() const				{ return mCommand&kJump; }

	void Clear()					{ mCommand = 0u; }
	void Serialize(S::Network::StreamWriter& write) const;
	void Deserialize(S::Network::StreamReader& read);

public:
	//friend class InputManager;
	enum Flag // old school
	{
		kUp				= 0x1 << 0,
		kDown			= 0x1 << 1,
		kRight			= 0x1 << 2,
		kLeft			= 0x1 << 3,
		kAttack			= 0x1 << 4,
		kSkill			= 0x1 << 5,
		kUlt			= 0x1 << 6,
		kItemLeftSlide	= 0x1 << 7,
		kItemRightSlide	= 0x1 << 8,
		kUseItem		= 0x1 << 9,
		kJump			= 0x1 << 10,
	};

	uint32_t mCommand{ 0u };
	//float mDuration{ 0.0f };
	//uint32_t mTimedCommand{ 0 };
	//uint32_t mOneShotCommand{ 0 };
};
