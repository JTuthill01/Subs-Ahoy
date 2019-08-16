#include "pch.hpp"
#include "Air_and_Sea_Level.hpp"
#include <GlobalVariables.hpp>

extern bool gVariables::gIsGameOver;
extern bool gVariables::gLoadNext;
extern bool gVariables::gIsUnderwater;
extern int gVariables::gLevel;
extern int gVariables::gScore;

Air_and_Sea_Level::Air_and_Sea_Level() : mRemovePlaneProjectile(false), mRemoveSAM(false), height(1080), mIsExploded(false), mWIDTH(1920), mRemovePlaneTag(false)
{
	pEnemy->loadPlaneProjectiles();

	mPlaneSpawnTimer.restart(sf::seconds(0.2F));

	mPlayer = new Player(Player::playerProjectile);

	mPlayer->setPlayerPosition(sf::Vector2f(0.F, 900.F));

	mPlayer->loadProjectiles();

	loadExplosions();

	loadFont();
}

Air_and_Sea_Level::~Air_and_Sea_Level() = default;

void Air_and_Sea_Level::update(float & deltaTime)
{
	updatePlayerPosition(deltaTime);

	mPlayer->update(deltaTime);

	updateManager(deltaTime);

	pickupUpdate(mPlayer, deltaTime);

	checkCollision(deltaTime);

	collisonProjectiles(pEnemies, mPlayer, deltaTime);

	remove(pEnemies, mPlayer);

	removeProjectiles(pEnemies, mPlayer, deltaTime);

	removeSAMProjectiles(deltaTime);

	removePlaneProjectiles();

	for (size_t i = 0; i < mEnemyPlanes.size(); i++)
		mEnemyPlanes[i]->updatePlane(deltaTime);

	spawnPlanes(deltaTime);

	setBombExplosions(deltaTime);

	getExplosionPosition(deltaTime);

	removeTags();

	removePlaneProjectiles();

	mY = thor::random(800.F, 1000.F);
}

void Air_and_Sea_Level::render(sf::RenderTarget & target)
{
	for (size_t i = 0; i < mEnemyPlanes.size(); i++)
		mEnemyPlanes[i]->renderPlane(target);

	for (size_t i = 0; i < pEnemies.size(); i++)
		pEnemies[i]->render(target);

	mPlayer->render(target);

	for (size_t i = 0; i < mPlaneTextTags.size(); i++)
		mPlaneTextTags[i].render(target);

	for (size_t i = 0; i < mPlayerTextTags.size(); i++)
		mPlayerTextTags[i].render(target);

	if (mIsExploded)
		target.draw(mExplosionSprite);

	renderManager(target);
}

void Air_and_Sea_Level::updatePlayerPosition(float & deltaTime)
{
	mPlayerPosition.x = mPlayer->getPlayerPosition().x;

	if (mPlayerPosition.x > mWIDTH - 100.F)
	{
		gVariables::gLevel += 1;

		gVariables::gLoadNext = true;
	}
}

void Air_and_Sea_Level::levelStart()
{
	pEnemy->loadPlaneProjectiles();

	mPlaneSpawnTimer.start();

	mPlayer = new Player(Player::playerProjectile);

	mPlayer->setPlayerPosition(sf::Vector2f(0.F, 900.F));

	mPlayer->loadProjectiles();

	loadExplosions();
}

void Air_and_Sea_Level::setBombExplosions(float & deltaTime)
{
	for (size_t i = 0; i < mEnemyPlanes.size(); i++)
	{
		for (int j = 0; j < mEnemyPlanes[i]->getEnemyPlaneProjectileSize(); j++)
		{
			if (mEnemyPlanes[i]->getEnemyPlaneProjectile(j).getProjectilePosition().y > mY)
			{
				createExplosions();

				mIsExploded = true;

				mRemovePlaneProjectile = true;

				mExplosionTimer.restart(sf::seconds(0.5F));
			}
		}
	}

	if (mExplosionTimer.isExpired())
		mIsExploded = false;
}

void Air_and_Sea_Level::spawnPlanes(float & deltaTime)
{
	if (mPlaneSpawnTimer.isExpired())
	{
		mEnemyPlanes.push_back(new Enemies(Enemies::enemyPlaneProjectiles));

		pEnemies.push_back(new Enemies(Enemies::enemyProjectiles, sf::Vector2f(1600.F, thor::random(500.F, 1000.F))));

		mPlaneSpawnTimer.restart(sf::seconds(3.55F));
	}

	for (size_t i = 0; i < pEnemies.size(); i++)
		pEnemies[i]->update(deltaTime);
}

void Air_and_Sea_Level::checkCollision(float & deltaTime)
{
	for (int i = 0; i < mPlayer->getPlayerProjectileSize(); i++)
	{
		for (size_t j = 0; j < mEnemyPlanes.size(); j++)
		{
			if (Collision::BoundingBoxTest(mPlayer->getPlayerProjectile(i).getProjectile(), mEnemyPlanes[j]->getEnemyPlaneSprite()))
			{
				if (mEnemyPlanes[j]->getEnemyPlaneHp() > 0)
				{
					int damage = mPlayer->dealDamage();

					mEnemyPlanes[j]->planeTakeDamage(damage);

					mRemoveSAM = true;

					createExplosions();

					mPlayerTextTags.push_back(TextTags(&mFont, "- " + std::to_string(damage), sf::Vector2f(mEnemyPlanes[j]->getEnemyPlanePosition().x + 10, mEnemyPlanes[j]->getEnemyPlanePosition().y + 20), sf::Vector2f(0.5F, 1.F),
						sf::Color::Red, 50U));

					mPlayerTextTagsTimer.restart(sf::seconds(0.5F));
				}

				if (mEnemyPlanes[j]->getEnemyPlaneHp() == 0)
				{
					removePlane(j);

					mRemoveSAM = true;

					gVariables::gScore += 2;
				}
			}
		}
	}

	for (size_t i = 0; i < mEnemyPlanes.size(); i++)
	{
		for (int j = 0; j < mEnemyPlanes[i]->getEnemyPlaneProjectileSize(); j++)
		{
			mEnemyPlanes[i]->getEnemyPlaneProjectile(j).update(deltaTime);

			if (Collision::BoundingBoxTest(mEnemyPlanes[i]->getEnemyPlaneProjectile(j).getProjectile(), mPlayer->getPlayerSprite()))
			{
				if (gVariables::gHp > 0)
				{
					mExplosionTimer.restart(sf::seconds(0.3F));

					mPlayer->setIsHit(true);

					int damage = mEnemyPlanes[i]->planeDealDamage();

					mPlayer->takeDamage(damage);

					mRemovePlaneProjectile = true;

					createExplosions();

					mPlaneTextTags.push_back(TextTags(&mFont, "- " + std::to_string(damage), sf::Vector2f(mPlayer->getPlayerPosition().x + 50.F, mPlayer->getPlayerPosition().y + 20.F), sf::Vector2f(0.F, 1.F), 
						sf::Color(sf::Color::Yellow), 50U));
				
					mPlaneTextTagsTimer.restart(sf::seconds(0.5F));
				}

				if (gVariables::gHp == 0)
				{
					mPlayer->setIsAlive(false);

					mRemovePlaneProjectile = true;

					gVariables::gIsGameOver = true;
				}
			}
		}
	}

	if (mExplosionTimer.isExpired())
		pPlayer->setIsHit(false);

	if (mPlaneTextTagsTimer.isExpired() && !mPlaneTextTags.empty())
		mRemovePlaneTag = true;

	if (mPlayerTextTagsTimer.isExpired() && !mPlaneTextTags.empty())
		mRemovePlayerTag = true;
}

void Air_and_Sea_Level::removeSAMProjectiles(float & deltaTime)
{
	for (int i = 0; i < pPlayer->getPlayerProjectileSize(); i++)
	{
		if (mRemoveSAM)
		{
			mPlayer->removeProjectile(i);

			mRemoveSAM = false;
		}
	}
}

void Air_and_Sea_Level::removePlane(unsigned index) { mEnemyPlanes.erase(mEnemyPlanes.begin() + index); }

void Air_and_Sea_Level::removePlaneProjectiles()
{
	for (size_t i = 0; i < mEnemyPlanes.size(); i++)
	{
		for (int j = 0; j < mEnemyPlanes[i]->getEnemyPlaneProjectileSize(); j++)
		{
			if (mRemovePlaneProjectile)
			{
				mEnemyPlanes[i]->removePlaneProjectile(j);

				mRemovePlaneProjectile = false;
			}
		}

		if (mEnemyPlanes[i]->getEnemyPlanePosition().x < 10.F)
			removePlane(i);
	}

	for (int i = 0; i < pPlayer->getPlayerProjectileSize(); i++)
	{
		if (mRemoveSAM)
		{
			mPlayer->removeProjectile(i);

			mRemoveSAM = false;
		}
	}
}

void Air_and_Sea_Level::loadExplosions()
{
	sf::Texture temp;
	if (!temp.loadFromFile("Resources/Sprites/explosion1.png"))
		std::cerr << "explosion not found";

	mExplosionTextures.push_back(temp);
}

void Air_and_Sea_Level::createExplosions()
{
	mExplosionSprite.setTexture(mExplosionTextures[0]);
	mExplosionSprite.setScale(1.F, 1.F);
	mExplosionSprite.setPosition(mExplosionPosition);
}

void Air_and_Sea_Level::getExplosionPosition(float & deltaTime)
{
	for (size_t i = 0; i < mEnemyPlanes.size(); i++)
	{
		for (int j = 0; j < mEnemyPlanes[i]->getEnemyPlaneProjectileSize(); j++)
			mExplosionPosition = mEnemyPlanes[i]->getEnemyPlaneProjectile(j).getProjectilePosition();
	}
}

void Air_and_Sea_Level::removeTags()
{
	if (mRemovePlaneTag)
	{
		mPlaneTextTags.clear();

		mRemovePlaneTag = false;
	}

	if (mRemovePlayerTag)
	{
		mPlayerTextTags.clear();

		mRemovePlayerTag = false;
	}
}

void Air_and_Sea_Level::loadFont()
{
	if (!mFont.loadFromFile("Resources/Fonts/Anton-Regular.ttf"))
		std::cerr << "font failed to load";
}
