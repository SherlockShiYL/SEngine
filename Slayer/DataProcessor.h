#ifndef INCLUDED_SLAYER_DATAPROCESSOR_H
#define INCLUDED_SLAYER_DATAPROCESSOR_H

//#include "Enemy.h"
#include "GameObjects/Player.h"
#include "GameObjects/Weapon.h"

class DataProcessor
{
public:
	static void LoadWeapon(const char* fileName, Weapon* weapon);
	//static void LoadEnemy(const char* fileName, Enemy*** enemy);
	static void LoadPlayer(const char* fileName, Player* player);

	//static void SaveLevel(char* fileName, Enemy* enemy);
};

#endif // !INCLUDED_SLAYER_DATAPROCESSOR_H