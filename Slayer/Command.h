#ifndef INCLUDED_SLAYER_COMMAND_H
#define INCLUDED_SLAYER_COMMAND_H

class Command
{
public:
	bool isEmpty() const				{ return !mCommand; }

	bool isMoveUp() const				{ return mCommand&kUp; }
	bool isMoveDown() const				{ return mCommand&kDown; }
	bool isMoveRight() const			{ return mCommand&kRight; }
	bool isMoveLeft() const				{ return mCommand&kLeft; }
	bool isAttack() const				{ return mCommand&kAttack; }
	bool isUseSkill() const				{ return mCommand&kSkill; }
	bool isUseUlt() const				{ return mCommand&kUlt; }
	bool isUseItem() const				{ return mCommand&kUseItem; }
	bool isItemLeftSlide() const		{ return mCommand&kItemLeftSlide; }
	bool isItemRightSlide() const		{ return mCommand&kItemRightSlide; }
	bool isJump() const					{ return mCommand&kJump; }

	void MoveUp()						{ mCommand |= kUp; }
	void MoveDown()						{ mCommand |= kDown; }
	void MoveRight()					{ mCommand |= kRight; }
	void MoveLeft()						{ mCommand |= kLeft; }
	void Attack()						{ mCommand |= kAttack; }
	void UseSkill()						{ mCommand |= kSkill; }
	void UseUlt()						{ mCommand |= kUlt; }
	void UseItem()						{ mCommand |= kUseItem; }
	void ItemLeftSlide()				{ mCommand |= kItemLeftSlide; }
	void ItemRightSlide()				{ mCommand |= kItemRightSlide; }
	void Jump()							{ mCommand |= kJump; }

	void Clear()						{ mCommand = 0u; }
	const uint32_t GetCommand() const	{ return mCommand; }

private:
	//friend class InputManager;
	enum Flag // old school
	{
		kUp =				0x1 << 0,
		kDown =				0x1 << 1,
		kRight =			0x1 << 2,
		kLeft =				0x1 << 3,
		kAttack =			0x1 << 4,
		kSkill =			0x1 << 5,
		kUlt =				0x1 << 6,
		kItemLeftSlide =	0x1 << 7,
		kItemRightSlide =	0x1 << 8,
		kUseItem =			0x1 << 9,
		kJump =				0x1 << 10,
	};
	uint32_t mCommand{ 0 };
	//float mDuration{ 0.0f };
	//uint32_t mTimedCommand{ 0 };
	//uint32_t mOneShotCommand{ 0 };
};

#endif // !INCLUDED_SLAYER_COMMAND_H