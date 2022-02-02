#ifndef INCLUDED_SLAYER_GAMEOBJECTMANAGER_H
#define INCLUDED_SLAYER_GAMEOBJECTMANAGER_H

#define SHIVLIGHTINGNUM 20

#include <AI/Inc/AI.h>
#include "../DamageDisplay.h"
#include "../DataProcessor.h"
#include "Source.h"
//#include "GameObject.h"

// Buildings
#include "Buildings/Building.h"
#include "Buildings/LoggingFarm.h"

// Enemies
#include "AI/TinyBot.h"
#include "Enemies/Slime.h"

// Skills
#include "Skills/SkillList.h"
#include "Skills/BlackHole.h"
#include "Skills/ShivLighting.h"

class GameObjectManager
{
public:
	static void StaticInitialize();
	static void StaticTerminate();
	static GameObjectManager* Get();

public:
	GameObjectManager() {};
	~GameObjectManager();

	void CreateEnemies(FILE* file);
	void CreateEnemies(const char* fileName);
	void CreateDefaultGameObjects(); // From file loader
	
	// Active attackprojectile
	void ActiveGameObjectsFrom(Player* p, ActiveSkillList sl);
	//void ActiveGameObjectsFrom(Enemy* e, ActiveSkillList sl);

	// GameObjectManager should contain all the characters' class
	Player* GetPlayer();
	AttackProjectile* FindAttackProjectile(uint32_t id);
	void CheckProjectileCollision();

	/*bool Intersect(const AttackProjectile* a, const Enemy* e)
	{
		if (!S::Math::Intersect(a->GetRectInWorld(), e->GetRectInWorld()))
			return false;
		return true;
	}*/

	void Update(float deltaTime);
	//void Update(Network::StreamReader& reader);
	void Render(S::Math::Vector2 p, float scale);

	S::AI::AIWorld aiWorld;
	S::AI::Graph graph;

private:

	//Enemy ***mEnemies;
	uint32_t numSlime{ 30 };
	Slime **mSlime;
	Player *mPlayer;
	//TinyBot *mTinyBot;

	ShivLighting *mShivLighting[SHIVLIGHTINGNUM]; // No Initialize yet, 20 is for testing
	AttackProjectiles mAttackProjectiles;

	ServerDamageDisplayList mServerDamageDisplayList;

	Weapon *mWeapons[5]; // No Initialize yet, 5 is for testing

	// Buildings
	//LoggingFarm* mLoggingFarm{ nullptr };

	struct TypeNum
	{
		int type;
		uint32_t amount;
	};
	std::list<TypeNum> mTypeNum;

	S::TileMap mTileMap;
};

#endif // !INCLUDED_SLAYER_GAMEOBJECTMANAGER_H