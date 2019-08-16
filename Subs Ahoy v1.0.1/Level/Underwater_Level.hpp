#pragma once
#include <Entities/AnimatedEnemy.hpp>
#include "LevelCollisionManager.hpp"
#include "LevelSet.hpp"

class Underwater_Level : public LevelCollisionManager, public LevelSet
{
public:
	Underwater_Level();
	~Underwater_Level();

	void render(sf::RenderTarget& target);
	void update(float& deltaTime);

private:
	void spawnEnemies(float& deltaTime);
	void load();
	
	std::vector<sf::Texture> mAnimationTexture;
	std::vector<sf::Texture> mSubTexture;
	sf::Texture mLevelTexture;
	sf::Sprite mLevelSprite;

	thor::Timer mSpawnTimer;

	Player* mPlayer;
	std::vector<AnimatedEnemy*> mAnimated;
	std::vector<Enemies*> mEnemies;
	std::vector<Mines*> mMines;

	thor::Timer mMineSpawnTimer;

protected:
	virtual void uUpdatePlayerPosition(float& deltaTime);

	sf::Vector2f pPlayerPosition;
	
	thor::Timer pPlaneSpawnTimer;
	thor::Timer pEnemyTagTimer;
	
	float WIDTH;
};

