#include "pch.hpp"
#include "Player.hpp"
#include <GlobalVariables.hpp>

std::vector<sf::Texture> Player::playerProjectile;

extern int gVariables::gHp = 20;
extern int gVariables::gHpMax = 20;
extern int gVariables::gTorpedos = 30;
extern int gVariables::gTorpedosMax = 30;

Player::Player() = default;

Player::Player(std::vector<sf::Texture>& projectileTexture) : mIsAlive(true), mDamage(1), mDamageMax(4), mIsFiring(false), mIsHit(false), mIsMoving(false)
{
	mProjectileTextures = &projectileTexture;

	load();

	playerHit();

	mShootTimer.restart(sf::seconds(0.1F));

	mKeyTimer.restart(sf::seconds(0.1F));

	mProjectileType = TORPEDO;

	HUD();

	gVariables::gHp = gVariables::gHpMax;

	gVariables::gTorpedos = gVariables::gTorpedosMax;
}

Player::~Player() = default;

const int Player::dealDamage() const
{
	int damage = 0;

	switch (mProjectileType)
	{
	case TORPEDO:
		damage = rand() % mDamage + mDamageMax;
		break;

	case SAM:
		damage = rand() % mDamage + mDamageMax;
		break;

	default:
		break;
	}

	return damage;
}

void Player::HUD()
{
	mText.setFont(mFont);
	mText.setCharacterSize(50U);
	mText.setFillColor(sf::Color(255, 255, 224, 255));
	mText.setPosition(sf::Vector2f(50.F, 800.F));

	mAmmoLeftText.setFont(mFont);
	mAmmoLeftText.setCharacterSize(50U);
	mAmmoLeftText.setFillColor(sf::Color(255, 255, 224, 255));
	mAmmoLeftText.setPosition(sf::Vector2f(1750, 900.F));

	mOutofAmmo.setFont(mFont);
	mOutofAmmo.setCharacterSize(50U);
	mOutofAmmo.setFillColor(sf::Color::Red);
	mOutofAmmo.setPosition(sf::Vector2f(1650, 900.F));
	mOutofAmmo.setString("Out of Ammo");

	mHpText.setFont(mFont);
	mHpText.setCharacterSize(50U);
	mHpText.setFillColor(sf::Color(255, 255, 224, 255));
	mHpText.setPosition(sf::Vector2f(875.F, 900.F));

	mTextTag.push_back(TextTags(&mFont, "Location X axis", sf::Vector2f(50.F, 750.F), sf::Vector2f(0.F, 0.F), sf::Color(sf::Color(255, 255, 224, 255)), 50U));
	mTextTag.push_back(TextTags(&mFont, "Location Y axis", sf::Vector2f(50.F, 900.F), sf::Vector2f(0.F, 0.F), sf::Color(sf::Color(255, 255, 224, 255)), 50U));

	mTextTag.push_back(TextTags(&mFont, "Torpedos", sf::Vector2f(1700.F, 850.F), sf::Vector2f(0.F, 0.F), sf::Color(sf::Color(255, 255, 224, 255)), 50U));

	mTextTag.push_back(TextTags(&mFont, "Health", sf::Vector2f(875.F, 850.F), sf::Vector2f(0.F, 0.F), sf::Color(sf::Color(255, 255, 224, 255)), 50U));
}

void Player::takeDamage(int damage)
{
	gVariables::gHp -= damage;

	if (gVariables::gHp <= 0)
	{
		gVariables::gHp = 0;

		mIsAlive = false;
	}
}

void Player::render(sf::RenderTarget & target)
{
	if (mIsMoving)
		target.draw(mBubbleSprite);

	if(mIsAlive)
		target.draw(mPlayerSprite);

	if (mIsHit)
		target.draw(mPlayerHitSprite);

	for (size_t i = 0; i < mPlayerProjectiles.size(); i++)
		if(gVariables::gTorpedos >= 1)
			mPlayerProjectiles[i].render(target);
	
	for (size_t i = 0; i < mExplosion.size(); i++)
		if (mIsFiring)
			mExplosion[i].render(target);

	target.draw(mText);

	if (gVariables::gTorpedos >= 1)
		target.draw(mAmmoLeftText);

	else
		target.draw(mOutofAmmo);

	target.draw(mHpText);

	for (size_t i = 0; i < mTextTag.size(); i++)
		mTextTag[i].render(target);
}

void Player::update(float & deltaTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		mPlayerSprite.move(0.315F, 0.F);

		mIsMoving = true;
	}

	else
		mIsMoving = false;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		mPlayerSprite.move(-0.25F, 0.F);

		mIsMoving = true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		mPlayerSprite.move(0.15F, -0.15F);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		mPlayerSprite.move(0.F, 0.15F);

	for (size_t i = 0; i < mPlayerProjectiles.size(); i++)
		mPlayerProjectiles[i].update(deltaTime);

	attack();

	mPlayerHitSprite.setPosition(mPlayerSprite.getPosition().x + 280.F, mPlayerSprite.getPosition().y + 100.F);

	bubbleUpdate(deltaTime);

	HUDUpdate(deltaTime);

	if (!mIsFiring && !mExplosion.empty())
		mExplosion.pop_back();
}

void Player::removeProjectile(unsigned index) { mPlayerProjectiles.erase(mPlayerProjectiles.begin() + index); }

Projectiles & Player::getPlayerProjectile(unsigned index) { return mPlayerProjectiles[index]; }

void Player::load()
{
	if (!mPlayerTexture.loadFromFile("Resources/Sprites/sub.png"))
		std::cerr << "Player failed to load";

	if (!mBubbleTexture.loadFromFile("Resources/Sprites/Test.png"))
		std::cerr << "No PNG found";

	mBubbleSprite.setTexture(mBubbleTexture);
	mPlayerSprite.setTexture(mPlayerTexture);

	if (!mFont.loadFromFile("Resources/Fonts/Anton-Regular.ttf"))
		std::cerr << "Font not found";
}

void Player::attack()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && mKeyTimer.isExpired() && mShootTimer.isExpired() && mIsAlive)
	{
		mProjectileType = TORPEDO;

		weaponType();

		mShootTimer.restart(sf::seconds(0.5F));

		mKeyTimer.restart(sf::seconds(0.5F));

		mExplosion.push_back(Projectiles(&mExplosionTexture[EXPLOSION], sf::Vector2f(mPlayerSprite.getPosition().x + 280.F, mPlayerSprite.getPosition().y + 70.F), sf::Vector2f(0.15F, 0.15F)));

		mExplosionTimer.restart(sf::seconds(0.3F));

		mIsFiring = true;

		gVariables::gTorpedos -= 1;

		if (gVariables::gTorpedos <= 0)
			gVariables::gTorpedos = 0;
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && mKeyTimer.isExpired() && mShootTimer.isExpired() && mIsAlive)
	{
		mProjectileType = SAM;

		weaponType();

		mShootTimer.restart(sf::seconds(0.5F));

		mKeyTimer.restart(sf::seconds(0.5F));

		mExplosion.push_back(Projectiles(&mExplosionTexture[EXPLOSION], sf::Vector2f(mPlayerSprite.getPosition().x + 180.F, mPlayerSprite.getPosition().y + 30.F), sf::Vector2f(0.15F, 0.15F)));

		mExplosionTimer.restart(sf::seconds(0.1F));

		mIsFiring = true;
	}

	if (mExplosionTimer.isExpired())
		mIsFiring = false;
}

void Player::weaponType()
{
	switch (mProjectileType)
	{

	case TORPEDO:
		if(gVariables::gTorpedos >= 1)
			mPlayerProjectiles.push_back(Projectiles(&Player::playerProjectile[TORPEDO], sf::Vector2f(mPlayerSprite.getPosition().x + 280.F, mPlayerSprite.getPosition().y + 100.F), sf::Vector2f(0.09F, 0.09F), 
				sf::Vector2f(0.8F, 0.F)));
		break;

	case SAM:
		mPlayerProjectiles.push_back(Projectiles(&Player::playerProjectile[SAM], sf::Vector2f(mPlayerSprite.getPosition().x + 200.F, mPlayerSprite.getPosition().y + 60.F), sf::Vector2f(0.2F, 0.2F), 
			sf::Vector2f(0.F, -0.7F)));
		break;

	default:
		break;
	}
}

void Player::playerHit()
{
	if (!mPlayerHitTexture.loadFromFile("Resources/Sprites/explosion1.png"))
		std::cerr << "Explosion failed to load";

	mPlayerHitSprite.setTexture(mPlayerHitTexture);
}

void Player::HUDUpdate(float & deltaTime)
{
	std::stringstream ss;

	ss << mPlayerSprite.getPosition().x << "\n" << "\n" << mPlayerSprite.getPosition().y;

	mText.setString(ss.str());

	std::stringstream remainingAmmo;

	remainingAmmo << gVariables::gTorpedos << "/" << gVariables::gTorpedosMax;

	mAmmoLeftText.setString(remainingAmmo.str());

	std::stringstream remainingHp;

	remainingHp << gVariables::gHp << "/" << gVariables::gHpMax;

	mHpText.setString(remainingHp.str());

	if (gVariables::gTorpedos < 10)
		mAmmoLeftText.setFillColor(sf::Color::Red);

	if (gVariables::gHp <= 10)
		mHpText.setFillColor(sf::Color::Yellow);

	if (gVariables::gHp <= 5)
		mHpText.setFillColor(sf::Color::Red);
}

void Player::bubbleUpdate(float& deltaTime)
{
	mBubbleSprite.setPosition((sf::Vector2f(mPlayerSprite.getPosition().x - 250, mPlayerSprite.getPosition().y + 50)));
}

void Player::loadProjectiles()
{
	sf::Texture temp;
	if (!temp.loadFromFile("Resources/Sprites/Torpedo_Bubbles.png"))
		std::cerr << "Torpedo not found";

	sf::Texture temp1;
	if (!temp1.loadFromFile("Resources/Sprites/SAM1.png"))
		std::cerr << "Torpedo not found";

	sf::Texture temp2;
	if (!temp2.loadFromFile("Resources/Sprites/explosion.png"))
		std::cerr << "Explosion not found";

	Player::playerProjectile.push_back(temp);
	Player::playerProjectile.push_back(temp1);

	mExplosionTexture.push_back(temp2);
	mExplosionTexture.push_back(temp2);
}

void Player::hpUpdate(int hp)
{
	gVariables::gHp += hp;

	if (gVariables::gHp > gVariables::gHpMax)
		gVariables::gHpMax = gVariables::gHp;
}

void Player::torpedoUpdate(int torpedo)
{
	gVariables::gTorpedos += torpedo;

	if (gVariables::gTorpedos > gVariables::gTorpedosMax)
		gVariables::gTorpedosMax = gVariables::gTorpedos;
}
