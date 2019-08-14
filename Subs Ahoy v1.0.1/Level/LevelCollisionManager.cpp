#include "pch.hpp"
#include "LevelCollisionManager.hpp"
#include <GlobalVariables.hpp>

extern int gVariables::gScore = 0;
extern bool gVariables::gIsGameOver = false;

LevelCollisionManager::LevelCollisionManager() : WIDTH(1920), mRemoveEnemyProjectile(false), mRemovePlayerProjectile(false), mRemoveEnemyTag(false), mRemovePickup(false), mRemovePickupTag(false),
	mRemovePlayerTag(false), pCanRemoveEnemy(false), mRemoveMine(false), mIsExploded(false), mRemoveExplosion(false), mRemoveMineExplosion(false), mRemoveMinePickup(false), mRemoveMinePickupTag(false)
{
	pPlayer = new Player(Player::playerProjectile);

	pEnemy->loadProjectiles();

	loadPickUps();

	loadFonts();
}

LevelCollisionManager::~LevelCollisionManager()
{
	delete pEnemy;
}

void LevelCollisionManager::loadPickUps()
{
	sf::Texture temp;
	if (!temp.loadFromFile("Resources/Sprites/healthtank.png"))
		std::cerr << "HP pickup not found";

	sf::Texture temp1;
	if (!temp1.loadFromFile("Resources/Sprites/missile.png"))
		std::cerr << "Ammo not found";

	sf::Texture temp2;
	if (!temp2.loadFromFile("Resources/Sprites/explosion0.png"))
		std::cerr << "Explosion not found";

	mMinePickUpTextures.push_back(temp);
	mMinePickUpTextures.push_back(temp1);
	mPickUpTextures.push_back(temp);
	mPickUpTextures.push_back(temp1);
	mExplosionTextures.push_back(temp2);
}

void LevelCollisionManager::remove(std::vector<Enemies*>& enemy, Player* player)
{
	for (size_t i = 0; i < enemy.size(); i++)
	{
		for (int j = 0; j < enemy[i]->getEnemyProjectileSize(); j++)
		{
			if (mRemoveEnemyProjectile)
			{
				enemy[i]->removeProjectile(j);

				mRemoveEnemyProjectile = false;
			}
		}

		if (enemy[i]->getEnemyPosition().x < WIDTH)
			removeEnemy(enemy, i);
	}

	for (int i = 0; i < player->getPlayerProjectileSize(); i++)
	{
		if (mRemovePlayerProjectile)
		{
			player->removeProjectile(i);

			mRemovePlayerProjectile = false;
		}
	}
}

void LevelCollisionManager::removeProjectiles(std::vector<Enemies*>& enemy, Player* player, float & deltaTime)
{
	for (size_t i = 0; i < enemy.size(); i++)
	{
		for (int j = 0; j < enemy[i]->getEnemyProjectileSize(); j++)
		{
			if (enemy[i]->getEnemyProjectile(j).getProjectilePosition().x <= 0)
				mRemoveEnemyProjectile = true;
		}
	}

	for (int i = 0; i < player->getPlayerProjectileSize(); i++)
	{
		if (player->getPlayerProjectile(i).getProjectilePosition().x > WIDTH)
			mRemovePlayerProjectile = true;
	}
}

void LevelCollisionManager::removeEnemy(std::vector<Enemies*>& enemy, unsigned index)
{
	if (pCanRemoveEnemy)
	{
		enemy.erase(enemy.begin() + index);

		pCanRemoveEnemy = false;
	}
}

void LevelCollisionManager::removePickup()
{
	if (mRemovePickup)
	{
		mPickUps.clear();

		mRemovePickup = false;
	}

	if (mRemoveMinePickup)
	{
		mMinePickUps.clear();

		mRemoveMinePickup = false;
	}
}

void LevelCollisionManager::removeTags()
{

	if (mRemovePlayerTag)
	{
		mPlayerTags.clear();

		mRemovePlayerTag = false;
	}


	if (mRemoveEnemyTag)
	{
		mEnemyTags.clear();

		mRemoveEnemyTag = false;
	}

	if (mRemovePickupTag)
	{
		mPickupTags.clear();

		mRemovePickupTag = false;
	}

	if (mRemoveMineTag)
	{
		mMineTags.clear();

		mRemoveMineTag = false;
	}

	if (mRemoveMinePickupTag)
	{
		mMinePickupTags.clear();

		mRemoveMinePickupTag = false;
	}
}

void LevelCollisionManager::loadFonts()
{
	if (!mFont.loadFromFile("Resources/Fonts/Anton-Regular.ttf"))
		std::cerr << "font failed to load";
}

void LevelCollisionManager::removeExplosions()
{
	if (mRemoveExplosion)
	{
		mEnemySubExplosions.clear();

		mRemoveExplosion = false;
	}

	if (mRemoveMineExplosion)
	{
		mMineExplosions.clear();

		mRemoveMineExplosion = false;
	}
}

void LevelCollisionManager::removeMine(std::vector<Mines*> & mines, unsigned index)
{
	if (mRemoveMine)
	{
		mines.erase(mines.begin() + index);

		mRemoveMine = false;
	}
}

void LevelCollisionManager::collisonProjectiles(std::vector<Enemies*>& enemy, Player* player, float & deltaTime)
{
	for (int i = 0; i < player->getPlayerProjectileSize(); i++)
	{
		player->getPlayerProjectile(i).update(deltaTime);

		for (size_t j = 0; j < enemy.size(); j++)
		{
			if (Collision::BoundingBoxTest(player->getPlayerProjectile(i).getProjectile(), enemy[j]->getEnemySprite()))
			{
				if (enemy[j]->getEnemyHp() > 0)
				{
					int damage = player->dealDamage();

					enemy[j]->takeDamage(damage);

					mRemovePlayerProjectile = true;

					mPlayerTagTimer.restart(sf::seconds(0.5F));

					mEnemySubExplosions.push_back(Projectiles(&mExplosionTextures[0], sf::Vector2f(enemy[j]->getEnemyPosition().x + 50.F, enemy[j]->getEnemyPosition().y + 20.F), sf::Vector2f(1.F, 1.F)));

					mPlayerTags.push_back(TextTags(&mFont, "BOOM", sf::Vector2f(enemy[j]->getEnemyPosition().x + 50.F, enemy[j]->getEnemyPosition().y + 20.F), sf::Vector2f(0.F, 1.5F), sf::Color(sf::Color::Red), 50U));

					mPlayerTags.push_back(TextTags(&mFont, "- " + std::to_string(damage), sf::Vector2f(enemy[j]->getEnemyPosition().x + 50.F, enemy[j]->getEnemyPosition().y + 70.F), sf::Vector2f(0.F, 0.5F), sf::Color(sf::Color::Red), 50U));

					mSubExplosionTimer.restart(sf::seconds(0.4F));
				}

				if (enemy[j]->getEnemyHp() == 0)
				{
					mEnemySubExplosions.push_back(Projectiles(&mExplosionTextures[0], sf::Vector2f(enemy[j]->getEnemyPosition().x + 50.F, enemy[j]->getEnemyPosition().y + 20.F), sf::Vector2f(1.F, 1.F)));

					mSubExplosionTimer.restart(sf::seconds(0.5F));

					//Add Pickup
					int dropChance = rand() % 100 + 1;

					if (dropChance > 10)
					{
						dropChance = rand() % 100 + 1;

						if (dropChance > 10)
						{
							mPickUps.push_back(PickUp(mPickUpTextures, sf::Vector2f(enemy[j]->getEnemyPosition()), thor::random(0, 1)));

							mPickUpTimer.restart(sf::seconds(3.F));
						}
					}

					pCanRemoveEnemy = true;

					removeEnemy(enemy, j);

					mRemovePlayerProjectile = true;

					gVariables::gScore += 1;
				}
			}
		}
	}

	for (size_t i = 0; i < enemy.size(); i++)
	{
		for (int j = 0; j < enemy[i]->getEnemyProjectileSize(); j++)
		{
			enemy[i]->getEnemyProjectile(j).update(deltaTime);

			if (Collision::BoundingBoxTest(enemy[i]->getEnemyProjectile(j).getProjectile(), player->getPlayerSprite()))
			{
				if (gVariables::gHp > 0)
				{
					mPlayerTagTimer.restart(sf::seconds(0.5F));

					mExplosionTimer.restart(sf::seconds(0.3F));

					player->setIsHit(true);

					int damage = enemy[i]->dealDamage();

					player->takeDamage(damage);

					mRemoveEnemyProjectile = true;

					mEnemyTagTimer.restart(sf::seconds(0.5F));

					mEnemyTags.push_back(TextTags(&mFont, "SHIT", sf::Vector2f(player->getPlayerPosition().x + 50.F, player->getPlayerPosition().y + 70.F), sf::Vector2f(0.F, 0.5F), sf::Color(sf::Color::Yellow), 50U));

					mEnemyTags.push_back(TextTags(&mFont, "- " + std::to_string(damage), sf::Vector2f(player->getPlayerPosition().x + 50.F, player->getPlayerPosition().y + 20.F), sf::Vector2f(0.5F, 0.5F), sf::Color(sf::Color::Yellow), 50U));
				}

				if (gVariables::gHp == 0)
				{
					player->setIsAlive(false);

					mRemoveEnemyProjectile = true;

					gVariables::gIsGameOver = true;
				}
			}
		}
	}

	if (mSubExplosionTimer.isExpired())
		mRemoveExplosion = true;

	if (mExplosionTimer.isExpired())
		player->setIsHit(false);

	if (mPickUpTimer.isExpired() && !mPickUps.empty())
		mPickUps.pop_back();

	if (mPlayerTagTimer.isExpired())
		mRemovePlayerTag = true;

	if (mEnemyTagTimer.isExpired())
		mRemoveEnemyTag = true;
}

void LevelCollisionManager::mineCollision(std::vector<Mines*> & mines, Player * player, float & deltaTime)
{
	for (size_t i = 0; i < mines.size(); i++)
	{
		for (int j = 0; j < player->getPlayerProjectileSize(); j++)
		{
			if (Collision::BoundingBoxTest(player->getPlayerProjectile(j).getProjectile(), mines[i]->getMineSprite()))
			{
				if (gVariables::gMineHp > 0)
				{
					int damage = player->dealDamage();

					mines[i]->takeDamage(damage);

					mRemovePlayerProjectile = true;

					mIsExploded = true;

					mMineExplosions.push_back(Projectiles(&mExplosionTextures[0], sf::Vector2f(mines[i]->getMinePosition().x + 50.F, mines[i]->getMinePosition().y + 20.F), sf::Vector2f(1.F, 1.F)));

					mMineTags.push_back(TextTags(&mFont, "BOOM", sf::Vector2f(mines[i]->getMinePosition().x + 100.F, mines[i]->getMinePosition().y + 100.F), sf::Vector2f(0.F, 0.5F), sf::Color::Red, 50U));

					mMineTagTimer.restart(sf::seconds(0.4F));

					mMineExplosionTimer.restart(sf::seconds(0.4F));

					gVariables::gScore += 1;
				}

				if (gVariables::gMineHp == 0)
				{
					mMineExplosions.push_back(Projectiles(&mExplosionTextures[0], sf::Vector2f(mines[i]->getMinePosition().x + 50.F, mines[i]->getMinePosition().y + 20.F), sf::Vector2f(1.F, 1.F)));

					//Add Pickup
					int dropChance = rand() % 100 + 1;

					if (dropChance > 10)
					{
						dropChance = rand() % 100 + 1;

						if (dropChance > 10)
						{
							mMinePickUps.push_back(PickUp(mMinePickUpTextures, sf::Vector2f(mines[i]->getMinePosition()), thor::random(0, 1)));

							mMinePickupTimer.restart(sf::seconds(3.F));
						}
					}

					mRemoveMine = true;

					mMineExplosionTimer.restart(sf::seconds(0.4F));

					mRemovePlayerProjectile = true;

					removeMine(mines, i);

					gVariables::gScore += 1;
				}
			}
		}
	}

	for (size_t i = 0; i < mines.size(); i++)
	{
		if (Collision::BoundingBoxTest(player->getPlayerSprite(), mines[i]->getMineSprite()))
		{
			if (gVariables::gHp > 0)
			{
				removeMine(mines, i);

				mRemoveMine;

				int damage = 1;

				player->takeDamage(damage);

				mMineTags.push_back(TextTags(&mFont, "BOOM", sf::Vector2f(mines[i]->getMinePosition().x + 100.F, mines[i]->getMinePosition().y + 100.F), sf::Vector2f(0.F, 0.5F), sf::Color::Red, 50U));

				mMineTagTimer.restart(sf::seconds(0.2F));
			}

			if (gVariables::gHp == 0)
			{
				player->setIsAlive(false);

				mRemoveMine = true;

				gVariables::gIsGameOver = true;
			}

			if (gVariables::gMineHp > 0)
			{
				int damage = player->dealDamage();

				mines[i]->takeDamage(damage);

				mRemovePlayerProjectile = true;

				mIsExploded = true;

				mMineExplosions.push_back(Projectiles(&mExplosionTextures[0], sf::Vector2f(mines[i]->getMinePosition().x + 50.F, mines[i]->getMinePosition().y + 20.F), sf::Vector2f(1.F, 1.F)));

				mMineTags.push_back(TextTags(&mFont, "BOOM", sf::Vector2f(mines[i]->getMinePosition().x + 100.F, mines[i]->getMinePosition().y + 100.F), sf::Vector2f(0.F, 0.5F), sf::Color::Red, 50U));

				mMineTagTimer.restart(sf::seconds(0.4F));

				mMineExplosionTimer.restart(sf::seconds(0.4F));

				gVariables::gScore += 1;
			}

			if (gVariables::gMineHp == 0)
			{
				mMineExplosions.push_back(Projectiles(&mExplosionTextures[0], sf::Vector2f(mines[i]->getMinePosition().x + 50.F, mines[i]->getMinePosition().y + 20.F), sf::Vector2f(1.F, 1.F)));

				mRemoveMine = true;

				mMineExplosionTimer.restart(sf::seconds(0.4F));

				mRemovePlayerProjectile = true;

				removeMine(mines, i);

				gVariables::gScore += 1;
			}
		}
	}

	if (mMineTagTimer.isExpired())
		mRemoveMineTag = true;

	if (mMineExplosionTimer.isExpired())
		mRemoveMineExplosion = true;

	if (mMinePickupTimer.isExpired())
		mRemoveMinePickup = true;
}

void LevelCollisionManager::pickupUpdate(Player* player, float & deltaTime)
{
	for (size_t i = 0; i < mPickUps.size(); i++)
	{
		if (Collision::BoundingBoxTest(player->getPlayerSprite(), mPickUps[i].getPickupSprite()))
		{
			mRemovePickup = true;

			int health = 5;

			int torpedos = 5;

			switch (mPickUps[i].getType())
			{
			case HEALTH:
				if (gVariables::gHp < gVariables::gHpMax)
				{
					mPickUpTimer.restart(sf::seconds(0.5F));

					player->hpUpdate(health);

					mPickupTags.push_back(TextTags(&mFont, "+ " + std::to_string(5) + " " + "HEALTH", sf::Vector2f(mPickUps[i].getPosition()), sf::Vector2f(0.F, 1.5F), sf::Color(220, 20, 60, 255), 40U));
				}

				else
				{
					gVariables::gScore += 5;

					mPickUpTimer.restart(sf::seconds(0.5F));

					mPickupTags.push_back(TextTags(&mFont, "+ " + std::to_string(5) + "SCORE", sf::Vector2f(mPickUps[i].getPosition()), sf::Vector2f(0.F, 1.5F), sf::Color(220, 20, 60, 255), 40U));
				}

				break;

			case AMMO:
				if (gVariables::gTorpedos < gVariables::gTorpedosMax)
				{
					mPickUpTimer.restart(sf::seconds(0.5F));

					player->torpedoUpdate(torpedos);

					mPickupTags.push_back(TextTags(&mFont, "+ " + std::to_string(5) + " " + "TORPEDOS", sf::Vector2f(mPickUps[i].getPosition()), sf::Vector2f(0.F, 1.5F), sf::Color(220, 20, 60, 255), 40U));
				}

				else
				{
					gVariables::gScore += 5;

					mPickUpTimer.restart(sf::seconds(0.5F));

					mPickupTags.push_back(TextTags(&mFont, "+ " + std::to_string(5) + "SCORE", sf::Vector2f(mPickUps[i].getPosition()), sf::Vector2f(0.F, 1.5F), sf::Color(220, 20, 60, 255), 40U));
				}

				break;

			default:
				break;
			}
		}

	}

	if (mPickUpTimer.isExpired())
		mRemovePickupTag = true;
}
void LevelCollisionManager::minePickupUpdate(Player* player, float& deltaTime)
{
	for (size_t i = 0; i < mMinePickUps.size(); i++)
	{
		if (Collision::BoundingBoxTest(player->getPlayerSprite(), mMinePickUps[i].getPickupSprite()))
		{
			mRemovePickup = true;

			int health = 5;

			int torpedos = 5;

			switch (mMinePickUps[i].getType())
			{
			case HEALTH:
				if (gVariables::gHp < gVariables::gHpMax)
				{
					mMinePickupTimer.restart(sf::seconds(0.5F));

					player->hpUpdate(health);

					mMinePickupTags.push_back(TextTags(&mFont, "+ " + std::to_string(5) + " " + "HEALTH", sf::Vector2f(mMinePickUps[i].getPosition()), sf::Vector2f(0.F, 1.5F), sf::Color(220, 20, 60, 255), 40U));
				}

				else
				{
					gVariables::gScore += 5;

					mMinePickupTimer.restart(sf::seconds(0.5F));

					mMinePickupTags.push_back(TextTags(&mFont, "+ " + std::to_string(5) + "SCORE", sf::Vector2f(mMinePickUps[i].getPosition()), sf::Vector2f(0.F, 1.5F), sf::Color(220, 20, 60, 255), 40U));
				}

				break;

			case AMMO:
				if (gVariables::gTorpedos < gVariables::gTorpedosMax)
				{
					mMinePickupTimer.restart(sf::seconds(0.5F));

					player->torpedoUpdate(torpedos);

					mMinePickupTags.push_back(TextTags(&mFont, "+ " + std::to_string(5) + " " + "TORPEDOS", sf::Vector2f(mMinePickUps[i].getPosition()), sf::Vector2f(0.F, 1.5F), sf::Color(220, 20, 60, 255), 40U));
				}

				else
				{
					gVariables::gScore += 5;

					mMinePickupTimer.restart(sf::seconds(0.5F));

					mMinePickupTags.push_back(TextTags(&mFont, "+ " + std::to_string(5) + "SCORE", sf::Vector2f(mMinePickUps[i].getPosition()), sf::Vector2f(0.F, 1.5F), sf::Color(220, 20, 60, 255), 40U));
				}

				break;

			default:
				break;
			}
		}

	}

	if (mMinePickupTimer.isExpired())
		mRemoveMinePickupTag = true;
}

void LevelCollisionManager::renderManager(sf::RenderTarget & target)
{
	for (size_t i = 0; i < mMineExplosions.size(); i++)
		mMineExplosions[i].render(target);

	for (size_t i = 0; i < mEnemySubExplosions.size(); i++)
		mEnemySubExplosions[i].render(target);

	for (size_t i = 0; i < mPickUps.size(); i++)
		mPickUps[i].render(target);

	for (size_t i = 0; i < mPickupTags.size(); i++)
		mPickupTags[i].render(target);

	for (size_t i = 0; i < mEnemyTags.size(); i++)
		mEnemyTags[i].render(target);

	for (size_t i = 0; i < mPlayerTags.size(); i++)
		mPlayerTags[i].render(target);

	for (size_t i = 0; i < mMineTags.size(); i++)
		mMineTags[i].render(target);

	for (size_t i = 0; i < mMinePickUps.size(); i++)
		mMinePickUps[i].render(target);

	for (size_t i = 0; i < mMinePickupTags.size(); i++)
		mMinePickupTags[i].render(target);
}

void LevelCollisionManager::updateManager(float & deltaTime)
{
	removeTags();

	removePickup();

	removeExplosions();
}

