#include "pch.hpp"
#include "Underwater_Level.hpp"
#include <GlobalVariables.hpp>

extern bool gVariables::gIsUnderwater;
extern bool gVariables::gLoadNext;
extern int gVariables::gLevel;

Underwater_Level::Underwater_Level() : WIDTH(1920)
{
	mPlayer = new Player(Player::playerProjectile);

	mPlayer->loadProjectiles();

	mPlayer->setPlayerPosition(sf::Vector2f(10.F, 10.F));

	mSpawnTimer.restart(sf::seconds(0.2F));

	mMineSpawnTimer.restart(sf::seconds(60.F));

	setLevel(mLevelTexture, mLevelSprite);

	load();
}

Underwater_Level::~Underwater_Level() { delete mPlayer; }

void Underwater_Level::render(sf::RenderTarget & target)
{
	for (size_t i = 0; i < mAnimated.size(); i++)
		mAnimated[i]->render(target);

	for (size_t i = 0; i < mEnemies.size(); i++)
		mEnemies[i]->render(target);

	for (size_t i = 0; i < mMines.size(); i++)
		mMines[i]->render(target);

	mPlayer->render(target);

	renderManager(target);
}

void Underwater_Level::update(float & deltaTime)
{
	for (size_t i = 0; i < mEnemies.size(); i++)
		mEnemies[i]->update(deltaTime);

	for (size_t i = 0; i < mAnimated.size(); i++)
		mAnimated[i]->update(deltaTime);

	for (size_t i = 0; i < mAnimated.size(); i++)
		mAnimated[i]->updateAnimations(deltaTime);

	uUpdatePlayerPosition(deltaTime);

	mPlayer->update(deltaTime);

	updateManager(deltaTime);

	mineCollision(mMines, mPlayer, deltaTime);

	for(size_t i = 0; i < mMines.size(); i++)
		removeMine(mMines, i);

	pickupUpdate(mPlayer, deltaTime);

	minePickupUpdate(mPlayer, deltaTime);

	collisonProjectiles(mEnemies, mPlayer, deltaTime);

	spawnEnemies(deltaTime);

	removeProjectiles(mEnemies, mPlayer, deltaTime);

	remove(mEnemies, mPlayer);

	for (size_t i = 0; i < mEnemies.size(); i++)
		removeEnemy(mEnemies, i);
}

void Underwater_Level::spawnEnemies(float & deltaTime)
{
	if (mSpawnTimer.isExpired())
	{
		mEnemies.push_back(new Enemies(Enemies::enemyProjectiles, sf::Vector2f(1800.F, thor::random(0.F, 1800.F))));

		mSpawnTimer.restart(sf::seconds(2.55F));
	}

	if (mMineSpawnTimer.isRunning())
		mMines.push_back(new Mines(sf::Vector2f(thor::random(100.F, 1060.F), thor::random(0.F, 1800.F))));

	if (mMines.size() > 10)
		mMineSpawnTimer.stop();
}

void Underwater_Level::load()
{
	sf::Texture temp;
	if (!temp.loadFromFile("Resources/Sprites/Sub/Sub_Combined.png"))
		std::cerr << "PNG not found";

	sf::Texture temp1;
	if (!temp1.loadFromFile("Resources/Sprites/T.png"))
		std::cerr << "Torpedo not found";

	mAnimationTexture.push_back(temp);
	AnimatedEnemy::enemySubProjectiles.push_back(temp1);
}

void Underwater_Level::uUpdatePlayerPosition(float & deltaTime)
{
	pPlayerPosition.x = mPlayer->getPlayerPosition().x;

	if (pPlayerPosition.x > WIDTH - 100.F)
	{
		gVariables::gLevel += 1;

		gVariables::gLoadNext = true;

		if (gVariables::gIsUnderwater)
			mPlayer->setPlayerPosition(sf::Vector2f(10.F, 900.F));

		else
			mPlayer->setPlayerPosition(sf::Vector2f(0.F, 900.F));
	}
}
