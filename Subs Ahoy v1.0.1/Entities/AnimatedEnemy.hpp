#include "Entities/Player.hpp"
#include <Projectiles/Projectiles.hpp>
#include <Projectiles/AnimatedProjectiles.hpp>

class AnimatedEnemy
{
public:

	AnimatedEnemy(std::vector<sf::Texture>& projectileTexture, sf::Texture* texture, sf::Vector2f position);
	~AnimatedEnemy();

	static std::vector<sf::Texture> enemySubProjectiles;

	void setAnimations();
	void render(sf::RenderTarget& target);
	void update(float& deltaTime);
	void updateAnimations(float& deltaTime);
	void createMovementComponent(const float max_velocity, const float acceleration, const float deceleration);
	void createAnimationComponent(sf::Texture& texture_sheet);
	void move(const float direction_x, const float direction_y, const float& deltaTime);
	const int dealDamage() const;
	void takeDamage(int damage);
	void loadProjectiles();
	void removeProjectile(unsigned index);
	Projectiles& getEnemyProjectile(unsigned index);

private:
	void attack(float& deltaTime);
	void loadWeapon();
	void setHpBar();

	int mProjectileType;
	int mWeaponType;
	int mHp;
	int mHpMax;
	int mDamage;
	int mDamageMax;

	bool mIsAttacking;
	bool mIsAlive;

	sf::RectangleShape mHpBar;
	sf::Vector2f mPosition;

	sf::Texture* mSubTexture;
	sf::Sprite mSubSprite;

	thor::Timer mShootTimer;

	Player* mPlayer;
	MovementComponent* mMovementComponent;
	AnimationComponent* mAnimationComponent;

	std::vector<AnimatedProjectiles> mAProjectiles;
	std::vector<Projectiles> mProjectiles;
	std::vector<sf::Texture>* mProjectileTextures;
};

