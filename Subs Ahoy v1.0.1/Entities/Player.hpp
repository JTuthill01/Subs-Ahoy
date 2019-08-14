#pragma once
#include <Projectiles/Projectiles.hpp>
#include <TextTags/TextTags.hpp>

class Player
{
public:
	Player();
	Player(std::vector<sf::Texture>& projectileTexture);
	~Player();

	const int dealDamage() const;
	void HUD();
	void setIsHit(bool t_f) { mIsHit = t_f; }
	void setIsAlive(bool t_f) { mIsAlive = t_f; }
	void takeDamage(int damage);
	void render(sf::RenderTarget& target);
	void update(float& deltaTime);
	void removeProjectile(unsigned index);
	void loadProjectiles();
	void hpUpdate(int hp);
	void torpedoUpdate(int torpedo);
	void setHpBarColor(sf::Color color) { mHpText.setFillColor(color); }
	Projectiles& getPlayerProjectile(unsigned index);

	const inline sf::Vector2f& getPlayerPosition() { return mPlayerSprite.getPosition(); }
	inline void setPlayerPosition(sf::Vector2f playerPosition) { mPlayerSprite.setPosition(playerPosition); }
	inline const int getPlayerProjectileSize() { return mPlayerProjectiles.size(); }
	inline sf::Sprite& getPlayerSprite() { return mPlayerSprite; }
	inline sf::FloatRect getPlayerGlobalBounds() const { return mPlayerSprite.getGlobalBounds(); }
	inline bool isPlayerAlive() { return mIsAlive; }

	static std::vector<sf::Texture> playerProjectile;

private:
	void load();
	void attack();
	void weaponType();
	void playerHit();
	void HUDUpdate(float& deltaTime);
	void bubbleUpdate(float& deltaTime);

	bool mIsHit;
	bool mIsFiring;
	bool mIsAlive;
	bool mIsMoving;

	int mProjectileType;
	std::vector<Projectiles> mExplosion;
	std::vector<Projectiles> mPlayerProjectiles;
	std::vector<sf::Texture>* mProjectileTextures;
	std::vector<sf::Texture> mExplosionTexture;

	sf::Texture mBubbleTexture;
	sf::Texture mPlayerTexture;
	sf::Texture mPlayerHitTexture;

	sf::Sprite mBubbleSprite;
	sf::Sprite mPlayerHitSprite;
	sf::Sprite  mPlayerSprite;

	sf::Vector2f mPosition;

	sf::Text mText;
	sf::Text mAmmoLeftText;
	sf::Text mHpText;
	sf::Text mOutofAmmo;

	thor::Timer mKeyTimer;
	thor::Timer mShootTimer;
	thor::Timer mExplosionTimer;

	std::vector<TextTags> mTextTag;
	sf::Font mFont;

	int mDamage;
	int mDamageMax;
};

