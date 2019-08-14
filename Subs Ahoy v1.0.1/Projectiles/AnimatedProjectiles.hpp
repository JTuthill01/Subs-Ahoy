
class AnimatedProjectiles
{
public:

	AnimatedProjectiles(sf::Texture& texture, sf::Vector2f position, sf::Vector2f direction);
	~AnimatedProjectiles();

	void render(sf::RenderTarget& target);
	void update(float& deltaTime);
	void setIsFiring(bool t_f) { mIsFiring = t_f; }
	void updateAnimations(float& deltaTime);

	inline sf::Vector2f getAProjectilePosition() { return mAProjectileSprite.getPosition(); }
	inline sf::Sprite& getAProjectile() { return mAProjectileSprite; }

private:
	void setAnimations();
	void createMovementComponent(const float max_velocity, const float acceleration, const float deceleration);
	void createAnimationComponent(sf::Texture& texture_sheet);
	void move(const float direction_x, const float direction_y, const float& deltaTime);

	bool mIsFiring;

	sf::Texture mAProjectileTexture;
	sf::Sprite mAProjectileSprite;
	sf::Vector2f mPosition;
	sf::Vector2f mScale;
	sf::Vector2f mDirection;

	MovementComponent* mMovementComponent;
	AnimationComponent* mAnimationComponent;
};