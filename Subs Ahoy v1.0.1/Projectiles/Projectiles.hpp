#pragma once

enum projectileType { TORPEDO = 0, SAM};

enum explosionType { EXPLOSION = 0, TARGET_DESTROYED };

class Projectiles
{
public:
	Projectiles(sf::Texture * texture, sf::Vector2f position, sf::Vector2f scale);
	Projectiles(sf::Texture* texture, sf::Vector2f position, sf::Vector2f scale, sf::Vector2f direction);
	~Projectiles();

	void render(sf::RenderTarget& target);
	void update(float& deltaTime);

	inline sf::Vector2f getProjectilePosition() { return mProjectileSprite.getPosition(); }
	inline sf::Sprite& getProjectile() { return mProjectileSprite; }

private:
	sf::Texture* mExplosionTexture;
	sf::Texture* mProjectileTexture;
	sf::Sprite mProjectileSprite;
	sf::Sprite mExplosionSprite;
	sf::Vector2f mPosition;
	sf::Vector2f mScale;
	sf::Vector2f mDirection;
	sf::Vector2f mExplosionPosition;
	sf::Vector2f mExplosionScale;
};

