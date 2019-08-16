#pragma once
#include <Projectiles/Mines.hpp>
#include <Entities/Enemies.hpp>
#include <Collision/Collision.hpp>
#include <Pickups/PickUp.hpp>

class LevelCollisionManager
{
public:
	LevelCollisionManager();
	~LevelCollisionManager();

private:
	void loadPickUps();
	void removePickup();
	void removeTags();
	void loadFonts();
	void removeExplosions();
	
	bool mRemoveMineExplosion;
	bool mRemoveExplosion;
	bool mCanBeRemoved;
	bool mRemovePickup;
	bool mRemoveMinePickup;
	bool mIsRestart;
	bool mLoadNextLevel;
	bool mRemovePlayerProjectile;
	bool mRemoveEnemyProjectile;
	bool mRemoveEnemyTag;
	bool mRemoveMineTag;
	bool mRemovePickupTag;
	bool mRemoveMinePickupTag;
	bool mRemovePlayerTag;
	bool mIsExploded;
	bool mRemoveMine;

	float WIDTH;

	sf::Font mFont;
	
	sf::Vector2f mExplosionPosition;
	sf::Sprite mExplosionSprite;

	thor::Timer mMineExplosionTimer;
	thor::Timer mMineTagTimer;
	thor::Timer mExplosionTimer;
	thor::Timer mSubExplosionTimer;
	thor::Timer mEnemyTagTimer;
	thor::Timer mPlayerTagTimer;
	thor::Timer mPickUpTimer;
	thor::Timer mMinePickupTimer;
	thor::Timer mTextTagPickupTimer;

	std::vector<Projectiles> mMineExplosions;
	std::vector<Projectiles> mEnemySubExplosions;
	std::vector<TextTags> mMineTags;
	std::vector<TextTags> mPlayerTags;
	std::vector<TextTags> mEnemyTags;
	std::vector<TextTags> mPickupTags;
	std::vector<TextTags> mMinePickupTags;
	std::vector<PickUp> mPickUps;
	std::vector<PickUp> mMinePickUps;

	std::vector<sf::Texture> mSubExplosionTextures;
	std::vector<sf::Texture> mExplosionTextures;
	std::vector<sf::Texture> mPickUpTextures;
	std::vector<sf::Texture> mMinePickUpTextures;

protected:
	virtual void renderManager(sf::RenderTarget& target);
	virtual void updateManager(float& deltaTime);
	virtual void collisonProjectiles(std::vector<Enemies*>& enemy, Player* player, float& deltaTime);
	virtual void remove(std::vector<Enemies*>& enemy, Player* player);
	virtual void removeProjectiles(std::vector<Enemies*>& enemy, Player* player, float& deltaTime);
	virtual void removeEnemy(std::vector<Enemies*>& enemy, unsigned index);
	virtual void pickupUpdate(Player* player, float& deltaTime);
	virtual void minePickupUpdate(Player* player, float& deltaTime);
	virtual void mineCollision(std::vector<Mines*>& mines, Player* player, float& deltaTime);
	virtual void removeMine(std::vector<Mines*>& mines, unsigned index);

	
	Player* pPlayer;
	Enemies* pEnemy;
	std::vector<Mines*> mMines;
	std::vector<Enemies*> pEnemies;
	std::vector<TextTags> pEnemyTags;
	std::vector<TextTags> pPlayerTags;

	bool pIsGameOver;
	bool pLoadNextLevel;
	bool pCanRemoveEnemy;

	int pScore;
};

