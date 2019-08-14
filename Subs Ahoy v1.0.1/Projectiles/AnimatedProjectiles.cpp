#include "pch.hpp"
#include "AnimatedProjectiles.hpp"

AnimatedProjectiles::AnimatedProjectiles(sf::Texture & texture, sf::Vector2f position, sf::Vector2f direction) : mMovementComponent(NULL), 
	mAnimationComponent(NULL), mIsFiring(false)
{
	mAProjectileTexture = texture;

	mPosition = position;

	mDirection = direction;

	mAProjectileSprite.setTexture(texture);

	mAProjectileSprite.setPosition(mPosition);

	createMovementComponent(350.F, 16.F, 6.F);

	createAnimationComponent(texture);

	setAnimations();
}

AnimatedProjectiles::~AnimatedProjectiles() = default;

void AnimatedProjectiles::render(sf::RenderTarget& target) { target.draw(mAProjectileSprite); }

void AnimatedProjectiles::update(float& deltaTime) { mAProjectileSprite.move(mDirection); }

void AnimatedProjectiles::setAnimations() { mAnimationComponent->addAnimation("MOVE", 16.F, 0, 0, 4, 0, 256, 120); }

void AnimatedProjectiles::updateAnimations(float & deltaTime)
{
	mAnimationComponent->play("MOVE", deltaTime);

	move(mDirection.x, mDirection.y, deltaTime);

	mMovementComponent->update(deltaTime);
}

void AnimatedProjectiles::createMovementComponent(const float max_velocity, const float acceleration, const float deceleration)
{
	mMovementComponent = new MovementComponent(mAProjectileSprite, max_velocity, acceleration, deceleration);
}

void AnimatedProjectiles::createAnimationComponent(sf::Texture& texture_sheet)
{
	mAnimationComponent = new AnimationComponent(mAProjectileSprite, texture_sheet);
}

void AnimatedProjectiles::move(const float direction_x, const float direction_y, const float& deltaTime)
{
	if (mMovementComponent)
		mMovementComponent->move(direction_x, direction_y, deltaTime);
}
