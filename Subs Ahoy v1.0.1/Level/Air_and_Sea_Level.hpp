#pragma once
#include "LevelCollisionManager.hpp"
#include "LevelSet.hpp"

class Air_and_Sea_Level : public LevelCollisionManager, public LevelSet
{
public:
	Air_and_Sea_Level();
	~Air_and_Sea_Level();

	void update(float& deltaTime);
	void render(sf::RenderTarget& target);

protected:
	virtual void updatePlayerPosition(float& deltaTime);
	virtual void levelStart();

private:
	void setBombExplosions(float& deltaTime);
	void spawnPlanes(float& deltaTime);
	void checkCollision(float& deltaTime);
	void removeSAMProjectiles(float& deltaTime);
	void removePlane(unsigned index);
	void removePlaneProjectiles();
	void loadExplosions();
	void createExplosions();
	void getExplosionPosition(float& deltaTime);
	void removeTags();
	void loadFont();

	float height;
	float mY;

	bool mIsExploded;
	bool mRemovePlaneProjectile;
	bool mRemoveSAM;
	bool mRemovePlaneTag;
	bool mRemovePlayerTag;

	std::vector<TextTags> mPlaneTextTags;
	std::vector<TextTags> mPlayerTextTags;

	std::vector<sf::Texture> mExplosionTextures;
	sf::Sprite mExplosionSprite;
	sf::Vector2f mExplosionPosition;
	sf::Vector2f mPlayerPosition;

	thor::Timer mPlayerTextTagsTimer;
	thor::Timer mPlaneTextTagsTimer;

	sf::RenderWindow* pWindow;
	sf::Texture pLevelTexture;
	sf::Sprite mLevelSprite;
	sf::Font mFont;

	thor::Timer mSpawnTimer;
	thor::Timer mPlaneSpawnTimer;
	thor::Timer mEnemyTagTimer;
	thor::Timer mPlayerTagTimers;
	thor::Timer mExplosionTimer;

	Player* mPlayer;
	std::vector<Enemies*> mEnemyPlanes;
	std::vector<TextTags> mEnemyTags;
	std::vector<TextTags> mPlayerTags;

	int mLevel;
	int mScore;

	float mWIDTH;

	bool mIsUnderwater;
	bool mIsGameOver;
};

