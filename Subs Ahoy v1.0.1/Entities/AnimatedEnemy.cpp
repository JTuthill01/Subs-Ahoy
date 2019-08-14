#include "pch.hpp"
#include "AnimatedEnemy.hpp"

std::vector<sf::Texture> AnimatedEnemy::enemySubProjectiles;

AnimatedEnemy::AnimatedEnemy(std::vector<sf::Texture>& projectileTexture, sf::Texture* texture, sf::Vector2f position) : mIsAttacking(false), mHp(10), mHpMax(10), mDamage(1), mDamageMax(4), mIsAlive(true),
	mMovementComponent(nullptr), mAnimationComponent(nullptr), mPlayer(nullptr), mProjectileType(0), mWeaponType(0)
{
	mSubTexture = texture;

	mPosition = position;

	mProjectileTextures = &projectileTexture;

	mSubSprite.setTexture(*texture);

	mSubSprite.setPosition(position);

	mPlayer = new Player();

	setAnimations();
}

AnimatedEnemy::~AnimatedEnemy()
{
	delete mPlayer;

	delete mMovementComponent;

	delete mAnimationComponent;
}

void AnimatedEnemy::render(sf::RenderTarget& target)
{
	target.draw(mSubSprite);

	target.draw(mHpBar);

	for (size_t i = 0; i < mAProjectiles.size(); i++)
		mAProjectiles[i].render(target);
}

void AnimatedEnemy::update(float& deltaTime)
{
	mSubSprite.move(-0.4F, 0.F);

	setHpBar();

	for (size_t i = 0; i < mAProjectiles.size(); i++)
		mAProjectiles[i].update(deltaTime);

	attack(deltaTime);

	updateAnimations(deltaTime);
}

void AnimatedEnemy::setAnimations()
{
	createMovementComponent(350.F, 16.F, 6.F);

	createAnimationComponent(*mSubTexture);

	mAnimationComponent->addAnimation("MOVE", 16.F, 0, 0, 3, 0, 264, 89);

	mAnimationComponent->addAnimation("DEAD", 16.F, 0, 1, 2, 1, 264, 89);
}

void AnimatedEnemy::updateAnimations(float& deltaTime)
{
	mAnimationComponent->play("MOVE", deltaTime);

	move(-0.4F, 0.F, deltaTime);

	mMovementComponent->update(deltaTime);

	if (mHp <= 0)
		mAnimationComponent->play("DEAD", deltaTime);
}

void AnimatedEnemy::createMovementComponent(const float max_velocity, const float acceleration, const float deceleration)
{
	mMovementComponent = new MovementComponent(mSubSprite, max_velocity, acceleration, deceleration);
}

void AnimatedEnemy::createAnimationComponent(sf::Texture& texture_sheet)
{
	mAnimationComponent = new AnimationComponent(mSubSprite, texture_sheet);
}

void AnimatedEnemy::move(const float direction_x, const float direction_y, const float& deltaTime)
{
	if (mMovementComponent)
		mMovementComponent->move(direction_x, direction_y, deltaTime);
}

const int AnimatedEnemy::dealDamage() const
{
	int damage = 0;

	switch (mProjectileType)
	{
	case TORPEDO:
		damage = rand() % mDamage + mDamageMax;
		break;

	default:
		break;
	}

	return damage;
}

void AnimatedEnemy::takeDamage(int damage)
{
	mHp -= damage;

	if (mHp <= 0)
	{
		mHp = 0;

		mIsAlive = false;
	}
}

void AnimatedEnemy::loadProjectiles()
{
	sf::Texture temp;
	if (!temp.loadFromFile("Resources/Sprites/T.png"))
		std::cerr << "Torpedo not found";

	AnimatedEnemy::enemySubProjectiles.push_back(temp);
}

void AnimatedEnemy::removeProjectile(unsigned index)
{
	mProjectiles.erase(mProjectiles.begin() + index);
}

Projectiles& AnimatedEnemy::getEnemyProjectile(unsigned index) { return mProjectiles[index]; }

void AnimatedEnemy::attack(float & deltaTime)
{
	if (mSubSprite.getPosition().x >= mPlayer->getPlayerPosition().x)
	{
		mIsAttacking = true;

		if (mIsAttacking && mShootTimer.isExpired())
		{
			loadWeapon();

			mShootTimer.restart(sf::seconds(3.F));

			mIsAttacking = false;
		}
	}
}

void AnimatedEnemy::loadWeapon()
{
	switch (mWeaponType)
	{
	case TORPEDO:
		mAProjectiles.push_back(AnimatedProjectiles(AnimatedEnemy::enemySubProjectiles[TORPEDO], sf::Vector2f(mSubSprite.getPosition().x + 100.F, mSubSprite.getPosition().y + 60.F), sf::Vector2f(-0.8F, 0.F)));
		break;

	default:
		break;
	}
}

void AnimatedEnemy::setHpBar()
{
	mHpBar.setSize(sf::Vector2f(160.F, 10.F));
	mHpBar.setFillColor(sf::Color(0, 255, 0, 255));
	mHpBar.setPosition(sf::Vector2f(mSubSprite.getPosition().x + 50.F, mSubSprite.getPosition().y + 100));

	if (mHp <= 10)
		mHpBar.setFillColor(sf::Color::Yellow);

	if (mHp <= 5)
		mHpBar.setFillColor(sf::Color::Red);
}
