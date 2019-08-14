#include "pch.hpp"
#include "Projectiles.hpp"

Projectiles::Projectiles(sf::Texture * texture, sf::Vector2f position, sf::Vector2f scale) : mExplosionTexture(nullptr)
{
	mExplosionTexture = texture;

	mExplosionPosition = position;

	mExplosionScale = scale;

	mExplosionSprite.setTexture(*mExplosionTexture);
	mExplosionSprite.setPosition(mExplosionPosition);
	mExplosionSprite.setScale(mExplosionScale);
}

Projectiles::Projectiles(sf::Texture* texture, sf::Vector2f position, sf::Vector2f scale, sf::Vector2f direction) : mProjectileTexture(nullptr)
{
	mProjectileTexture = texture;

	mPosition = position;

	mScale = scale;

	mDirection = direction;

	mProjectileSprite.setTexture(*mProjectileTexture);
	mProjectileSprite.setScale(mScale);
	mProjectileSprite.setPosition(mPosition);
}

Projectiles::~Projectiles() = default;

void Projectiles::render(sf::RenderTarget & target)
{
	target.draw(mProjectileSprite);

	target.draw(mExplosionSprite);
}

void Projectiles::update(float & deltaTime)
{
	mProjectileSprite.move(mDirection);
}

