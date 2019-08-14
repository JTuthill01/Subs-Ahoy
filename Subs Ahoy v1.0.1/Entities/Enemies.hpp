#pragma once
#include "Player.hpp"

class Enemies
{
public:
	Enemies(std::vector<sf::Texture>& projectileTexture, sf::Vector2f position);
	Enemies(std::vector<sf::Texture>& projectileTexture);
	~Enemies();

	const int dealDamage() const;
	void takeDamage(int damage);
	const int planeDealDamage() const;
	void planeTakeDamage(int damage);
	void renderPlane(sf::RenderTarget& target);
	void render(sf::RenderTarget& target);
	void update(float& deltaTime);
	void updatePlane(float& deltaTime);
	void loadPlane();
	void loadPlaneProjectiles();
	void loadProjectiles();
	void removeProjectile(unsigned index);
	void removePlaneProjectile(unsigned index);
	Projectiles& getEnemyProjectile(unsigned index);
	Projectiles& getEnemyPlaneProjectile(unsigned index);
	void setIsAlive(bool t_f) { mIsAlive = t_f; }

	static std::vector<sf::Texture> enemyProjectiles;
	static std::vector<sf::Texture> enemyPlaneProjectiles;

	inline sf::Vector2f getEnemyPosition() { return mEnemySprite.getPosition(); }
	inline const int getEnemyProjectileSize() { return mProjectiles.size(); }
	inline sf::Sprite& getEnemySprite() { return mEnemySprite; }
	inline int getEnemyHp() { return mHp; }
	inline bool getIsAlive() { return mIsAlive; }
	inline sf::Vector2f getEnemyPlanePosition() { return mEnemyPlaneSprite.getPosition(); }
	inline const int getEnemyPlaneProjectileSize() { return mPlaneProjectiles.size(); }
	inline sf::Sprite& getEnemyPlaneSprite() { return mEnemyPlaneSprite; }
	inline int getEnemyPlaneHp() { return mPlaneHp; }

private:
	void planeAttack();
	void load();
	void attack();
	void loadWeapon();
	void setHpBar();
	void setPlaneHpBar();
	
	bool mIsAttacking;
	bool mIsAlive;
	bool mIsPlaneAlive;

	int mProjectileType;
	int mWeaponType;
	int mHp;
	int mHpMax;
	int mDamage;
	int mDamageMax;

	int mPlaneHp;
	int mPlaneHpMax;
	int mPlaneDamage;
	int mPlaneDamageMax;

	sf::RectangleShape mPlaneHpBar;
	sf::RectangleShape mHpBar;
	sf::Vector2f mPosition;
	sf::Vector2f mPlanePosition;

	sf::Texture mSubTexture;
	sf::Texture* mSubsTexture;
	sf::Texture mEnemyPlaneTexture;
	sf::Texture mEnemyTexture;
	sf::Sprite mEnemySprite;
	sf::Sprite mEnemyPlaneSprite;
	sf::Sprite mSubsSprite;
	sf::Sprite mSubSprite;

	thor::Timer mExplosionTimer;
	thor::Timer mPlaneShootTimer;
	thor::Timer mShootTimer;

	Player* mPlayer;

	std::vector<Projectiles> mProjectiles;
	std::vector<Projectiles> mPlaneProjectiles;
	std::vector<sf::Texture>* mProjectileTextures;
	std::vector<sf::Texture>* mPlaneProjectileTextures;
};

