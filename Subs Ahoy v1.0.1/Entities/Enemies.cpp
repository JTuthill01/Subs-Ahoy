#include "pch.hpp"
#include "Enemies.hpp"

std::vector<sf::Texture> Enemies::enemyProjectiles;
std::vector<sf::Texture> Enemies::enemyPlaneProjectiles;

Enemies::Enemies(std::vector<sf::Texture>& projectileTexture, sf::Vector2f position) : mIsAttacking(false), mHp(10), mHpMax(10), mDamage(1), mDamageMax(4), mIsAlive(true)
{
	mPosition = position;

	mPlayer = new Player();

	mProjectileTextures = &projectileTexture;

	load();

	mShootTimer.restart(sf::seconds(0.2F));
}

Enemies::Enemies(std::vector<sf::Texture>& projectileTexture) : mPlaneHp(10), mPlaneHpMax(20), mPlaneDamage(1), mPlaneDamageMax(3), mIsPlaneAlive(true)
{
	mProjectileTextures = &projectileTexture;

	loadPlane();

	mPlaneShootTimer.restart(sf::seconds(0.2F));
}

Enemies::~Enemies() { delete mPlayer; }

const int Enemies::dealDamage() const
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

void Enemies::takeDamage(int damage)
{
	mHp -= damage;

	if (mHp <= 0)
	{
		mHp = 0;

		mIsAlive = false;
	}
}

const int Enemies::planeDealDamage() const
{
	int damage = 0;

	damage = rand() % mPlaneDamage + mPlaneDamageMax;

	return damage;
}

void Enemies::planeTakeDamage(int damage)
{
	mPlaneHp -= damage;

	if (mPlaneHp <= 0)
	{
		mPlaneHp = 0;

		mIsPlaneAlive = false;
	}
}

void Enemies::renderPlane(sf::RenderTarget & target)
{
	target.draw(mEnemyPlaneSprite);

	target.draw(mPlaneHpBar);

	for (size_t i = 0; i < mPlaneProjectiles.size(); i++)
		mPlaneProjectiles[i].render(target);
}

void Enemies::render(sf::RenderTarget & target)
{
	target.draw(mEnemySprite);

	target.draw(mHpBar);

	for (size_t i = 0; i < mProjectiles.size(); i++)
		mProjectiles[i].render(target);
}

void Enemies::update(float & deltaTime)
{
	mEnemySprite.move(-0.4F, 0.F);

	setHpBar();

	for (size_t i = 0; i < mProjectiles.size(); i++)
		mProjectiles[i].update(deltaTime);

	attack();
}

void Enemies::updatePlane(float & deltaTime)
{
	mEnemyPlaneSprite.move(-0.3F, 0.F);

	setPlaneHpBar();

	for (size_t i = 0; i < mPlaneProjectiles.size(); i++)
		mPlaneProjectiles[i].update(deltaTime);

	planeAttack();
}

void Enemies::loadPlaneProjectiles()
{
	sf::Texture temp;
	if (!temp.loadFromFile("Resources/Sprites/Bomb_2.png"))
		std::cerr << "Bomb not found";

	Enemies::enemyPlaneProjectiles.push_back(temp);
}

void Enemies::planeAttack()
{
	if (mPlaneShootTimer.isExpired())
	{
		mPlaneProjectiles.push_back(Projectiles(&Enemies::enemyPlaneProjectiles[0], sf::Vector2f(mEnemyPlaneSprite.getPosition().x + 20, mEnemyPlaneSprite.getPosition().y + 30), 
			sf::Vector2f(0.2F, 0.2F), sf::Vector2f(0.F, 0.5F)));

		mPlaneShootTimer.restart(sf::seconds(2.5F));
	}
}

void Enemies::loadPlane()
{
	if (!mEnemyPlaneTexture.loadFromFile("Resources/Sprites/Fokker.png"))
		std::cerr << "Plane not found";

	mEnemyPlaneSprite.setTexture(mEnemyPlaneTexture);
	mEnemyPlaneSprite.setPosition(sf::Vector2f(1700.F, thor::random(10.F, 100.F)));
}

void Enemies::load()
{
	if (!mEnemyTexture.loadFromFile("Resources/Sprites/sub4.png"))
		std::cerr << "Enemy Sub not found";

	mEnemySprite.setTexture(mEnemyTexture);
	mEnemySprite.setPosition(mPosition);
}

void Enemies::attack()
{
	if (mEnemySprite.getPosition().x >= mPlayer->getPlayerPosition().x)
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

void Enemies::loadWeapon()
{
	switch (mWeaponType)
	{
	case TORPEDO:
		mProjectiles.push_back(Projectiles(&Enemies::enemyProjectiles[TORPEDO], sf::Vector2f(mEnemySprite.getPosition().x + 100.F, mEnemySprite.getPosition().y + 60.F), sf::Vector2f(0.09F, 0.09F), sf::Vector2f(-0.8F, 0.F)));
		break;

	default:
		break;
	}
}

void Enemies::setHpBar()
{
	mHpBar.setSize(sf::Vector2f(160.F, 10.F));
	mHpBar.setFillColor(sf::Color(0, 255, 0, 255));
	mHpBar.setPosition(sf::Vector2f(mEnemySprite.getPosition().x + 50.F, mEnemySprite.getPosition().y + 100));

	if (mHp <= 10)
		mHpBar.setFillColor(sf::Color::Yellow);

	if (mHp <= 5)
		mHpBar.setFillColor(sf::Color::Red);
}

void Enemies::setPlaneHpBar()
{
	mPlaneHpBar.setSize(sf::Vector2f(160.F, 10.F));
	mPlaneHpBar.setFillColor(sf::Color(0, 255, 0, 255));
	mPlaneHpBar.setPosition(sf::Vector2f(mEnemyPlaneSprite.getPosition().x + 50.F, mEnemyPlaneSprite.getPosition().y + 200));

	if (mHp <= 10)
		mHpBar.setFillColor(sf::Color::Yellow);

	if (mHp <= 5)
		mHpBar.setFillColor(sf::Color::Red);
}

void Enemies::loadProjectiles()
{
	sf::Texture temp;
	if (!temp.loadFromFile("Resources/Sprites/EnemyTorpedo.png"))
		std::cerr << "Torpedo not found";

	sf::Texture temp1;
	if (!temp1.loadFromFile("Resources/Sprites/SAM.png"))
		std::cerr << "SAM not found";

	Enemies::enemyProjectiles.push_back(temp);
	Enemies::enemyProjectiles.push_back(temp1);
}

void Enemies::removeProjectile(unsigned index) { mProjectiles.erase(mProjectiles.begin() + index); }

void Enemies::removePlaneProjectile(unsigned index) { mPlaneProjectiles.erase(mPlaneProjectiles.begin() + index); }

Projectiles & Enemies::getEnemyProjectile(unsigned index) { return mProjectiles[index]; }

Projectiles & Enemies::getEnemyPlaneProjectile(unsigned index) { return mPlaneProjectiles[index]; }
