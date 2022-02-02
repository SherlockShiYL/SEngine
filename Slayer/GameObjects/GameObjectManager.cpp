#include "GameObjectManager.h"

using namespace S;

namespace
{
	GameObjectManager* sInstance = nullptr;
}

void GameObjectManager::StaticInitialize()
{
	ASSERT(sInstance == nullptr, "[GameObjectManager] Manager already initialized.");
	sInstance = new GameObjectManager();
}

void GameObjectManager::StaticTerminate()
{
	ASSERT(sInstance != nullptr, "[GameObjectManager] Manager already terminated.");
	SafeDelete(sInstance);
}

GameObjectManager* GameObjectManager::Get()
{
	ASSERT(sInstance != nullptr, "[GameObjectManager] Manager not initialized.");
	return sInstance;
}

GameObjectManager::~GameObjectManager()
{
	/*for (uint32_t i = 0; i < mTypeofEnemies; ++i)
	{
		for (uint32_t j = 0; j < mNumofEnemies[i]; ++j)
		{
			S::SafeDelete(mEnemies[i][j]);
		}
	}*/
}

//Network::ObjectFactory& GameObjectManager::GetFactory()
//{
//	return mObjectFactory;
//}

void GameObjectManager::CreateDefaultGameObjects()
{
	aiWorld.GetQuadrant().Initialize({ 1000.0f,1000.0f }, 1u, 1u);

	mAttackProjectiles.Initialize();
	mPlayer = new Player(aiWorld, &mAttackProjectiles, static_cast<int>(CharacterType::Player), 0);

	for (uint32_t i = 0; i < 20; ++i)
	{
		mShivLighting[i] = new ShivLighting();
	}

	mSlime = new Slime*[numSlime];
	for (uint32_t i = 0; i < numSlime; ++i)
	{
		mSlime[i] = new Slime(aiWorld, static_cast<int>(CharacterType::Slime), i, &mAttackProjectiles); // aiWorld would remember every enemy even the enemy is not active
	}

	/*mLoggingFarm = new LoggingFarm(800, { 300.0f,300.0f }, 1, 20, 1.0f, 1.0f, { 0.0f,0.0f,100.0f,100.0f });
	mLoggingFarm->LoadTexture();*/

	/*mTinyBot = new TinyBot(aiWorld);
	mTinyBot->Load({ 400.0f,400.0f }, 0);*/
}

void GameObjectManager::ActiveGameObjectsFrom(Player* p, ActiveSkillList sl)
{
	// Check what skills player has
	// Doing with for loop, linear check every skills if is available
	switch (sl)
	{
	case ActiveSkillList::None:
		break;
	case ActiveSkillList::AttackProjectile:
		for (uint32_t i = 0; i < 20; ++i)
		{
			/*if (!mAttackProjectile[i]->IsAlive())
			{
				float ProjectileFlyingSpeed{ 0.0f };
				mAttackProjectile[i]->Wake(p->GetDamage(), 0u, p->GetId()
								, 0.0f, ProjectileFlyingSpeed, p->Heading()
								, p->GetPosition(), p->GetSkillList());
				break;
			}*/
		}
		break;
	case ActiveSkillList::ThunderFlash:
		break;
	case ActiveSkillList::BlackHole:
		break;
	case ActiveSkillList::ShivLighting:
		for (uint32_t i = 0; i < 20; ++i)
		{
			if (!mShivLighting[i]->IsAlive())
			{
				mShivLighting[i]->Wake(p->GetDamage(), 0u, p->GetId(), p->GetPosition());
				break;
			}
		}
		break;
	default:
		break;
	}

}

//void GameObjectManager::ActiveGameObjectsFrom(Enemy* e, ActiveSkillList sl)
//{
//	// Check what skills enemy has
//	for (uint32_t i = 0; i < 20; ++i)
//	{
//		if (!mShivLighting[i]->IsAlive())
//		{
//			//mShivLighting[i]->Wake(e->GetDamage(), 1u, e->GetId(), e->GetPosition());
//			break;
//		}
//	}
//}

Player* GameObjectManager::GetPlayer()
{
	return mPlayer;
}

AttackProjectile * GameObjectManager::FindAttackProjectile(uint32_t id)
{
	return nullptr; //mAttackProjectile[id];
}

void GameObjectManager::CheckProjectileCollision()
{
	for (uint32_t i = 0; i < MaxProjectile; ++i)
	{
		if(mAttackProjectiles.GetAttackProjectiles()[i].IsAlive())
		{
			if (mAttackProjectiles.GetAttackProjectiles()[i].GetType() == static_cast<uint32_t>(CharacterType::Player))
			{
				for (auto& t : mTypeNum)
				{
					switch (t.type)
					{
						case static_cast<int>(CharacterType::Slime) :
						{
							for (uint32_t j = 0; j < t.amount; ++j)
							{
								if (mSlime[j]->IsActive())
								{
									if (S::Geometry::Intersect({ mSlime[j]->GetPosition(),mSlime[j]->Radius() },
										mAttackProjectiles.GetAttackProjectiles()[i].GetRectInWorld()))
									{
										ServerDamageDisplay temp;
										temp.mDamage = mAttackProjectiles.GetAttackProjectiles()[i].GetDamage();
										temp.mPosition = mSlime[j]->GetPosition();
										mServerDamageDisplayList.push_back(temp);
										mSlime[j]->ReduceHP(mAttackProjectiles.GetAttackProjectiles()[i].GetDamage());
										mAttackProjectiles.GetAttackProjectiles()[i].Kill();
										break;
									}
								}
							}
							break;
						}

						default:
							break;
					}
				}

			}
			else // Check Enemies' Projectile with Players
			{

			}
		}
	}
}

void GameObjectManager::Update(float deltaTime)
{
	aiWorld.GetQuadrant().Update();
	//Physics::ColliderManager::Get()->Update();
	if (mPlayer->IsActive())
	{
		mPlayer->Update(deltaTime);
	}

	mAttackProjectiles.Update(deltaTime);

	for (auto& t : mTypeNum)
	{
		switch (t.type)
		{
		case static_cast<int>(CharacterType::Slime) :
		{
			for (uint32_t i = 0; i < t.amount; ++i)
			{
				if (mSlime[i]->IsActive())
				{
					mSlime[i]->Update(deltaTime);
				}
			}
			break;
		}
		default:
			break;
		}
	}

	CheckProjectileCollision();
	//mTinyBot->Update(deltaTime);
	//Physics::ColliderManager::Get()->CheckCollision();
}

void GameObjectManager::Render(Math::Vector2 p, float scale)
{
	aiWorld.GetQuadrant().DebugRender(p);
	Physics::ColliderManager::Get()->Render(p, scale);
	for (auto& t : mTypeNum)
	{
		switch (t.type)
		{
		case static_cast<int>(CharacterType::Slime):
		{
			for (uint32_t i = 0; i < t.amount; ++i)
			{
				if (mSlime[i]->IsActive())
					mSlime[i]->Render(p, scale);
			}
			break;
		}
		default:
			break;
		}
	}

	mAttackProjectiles.Render(p, scale);

	//mLoggingFarm->Render(p, scale);

	//mTinyBot->Render(p);
}

void GameObjectManager::CreateEnemies(FILE* file)
{
	mTypeNum.clear();
	while (true)
	{
		char buffer[128];
		int res = fscanf_s(file, "%s", buffer, static_cast<unsigned int>(std::size(buffer)));
		if (res == EOF)
			break;

		if (strcmp(buffer, "Type:") == 0)
		{
			TypeNum tempTypeNum;
			fscanf_s(file, "%d\n", &tempTypeNum.type);
			switch (tempTypeNum.type)
			{
			case static_cast<int>(CharacterType::Slime):
				fscanf_s(file, "%s", buffer, static_cast<unsigned int>(std::size(buffer)));
				if (strcmp(buffer, "Amount:") == 0)
				{
					fscanf_s(file, "%d\n", &tempTypeNum.amount);
					mTypeNum.push_back(tempTypeNum);
					Slime slime(aiWorld, 0u, 99, nullptr);
					do
					{
						res = fscanf_s(file, "%s", buffer, static_cast<unsigned int>(std::size(buffer)));
						if (res == EOF)
							break;

						if (strcmp(buffer, "HP:") == 0)
						{
							uint32_t temp;
							fscanf_s(file, "%d\n", &temp);
							slime.SetBasicHP(temp);
						}
						else if (strcmp(buffer, "Damage:") == 0)
						{
							uint32_t temp;
							fscanf_s(file, "%d\n", &temp);
							slime.SetBasicDamage(temp);
						}
						else if (strcmp(buffer, "AttackSpeed:") == 0)
						{
							float temp;
							fscanf_s(file, "%f\n", &temp);
							slime.SetBasicAttackSpeed(temp);
						}
						else if (strcmp(buffer, "Speed:") == 0)
						{
							float temp;
							fscanf_s(file, "%f\n", &temp);
							slime.SetBasicSpeed(temp);
						}
						else if (strcmp(buffer, "MoveCD:") == 0)
						{
							float temp;
							fscanf_s(file, "%f\n", &temp);
							slime.SetBasicMoveCD(temp);
						}
						else if (strcmp(buffer, "Radius:") == 0)
						{
							float temp;
							fscanf_s(file, "%f\n", &temp);
							slime.SetRadius(temp);
						}
						else if (strcmp(buffer, "MoveTime:") == 0)
						{
							float temp;
							fscanf_s(file, "%f\n", &temp);
							slime.SetBasicMoveTime(temp);
						}
					} while (strcmp(buffer, "Position:") != 0);

					for (uint32_t i = 0; i < tempTypeNum.amount; ++i)
					{
						mSlime[i]->CopyData(slime);
						mSlime[i]->Id() = i;
						fscanf_s(file, "%f %f\n", &mSlime[i]->Position().x, &mSlime[i]->Position().y);
						mSlime[i]->Wake();
					}
				}
				break;
			default:
				break;
			}
		}
	}
	fclose(file);
}

void GameObjectManager::CreateEnemies(const char* fileName)
{
	FILE* file = nullptr;
	fopen_s(&file, fileName, "r");
	ASSERT(file, "[DataLoader] Failed to open file %s.", fileName);

	CreateEnemies(file);
}
