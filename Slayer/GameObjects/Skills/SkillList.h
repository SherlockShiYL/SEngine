#ifndef INCLUDED_SLAYER_SKILLLIST_H
#define INCLUDED_SLAYER_SKILLLIST_H

enum class ActiveSkillList
{
	None,
	AttackProjectile,
	ThunderFlash,
	BlackHole,
	ShivLighting,
};

using ActiveSkillLists = std::vector<ActiveSkillList>;

#endif // !INCLUDED_SLAYER_SKILLLIST_H