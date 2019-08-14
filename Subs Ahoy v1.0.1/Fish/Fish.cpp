#include "pch.hpp"
#include "Fish.hpp"

extern int gVariables::gFishType = 0;

Fish::Fish(sf::Texture* texture, sf::Vector2f position) : mMovementComponent(nullptr), mAnimationComponent(nullptr), mFishTexture(nullptr), fishType(0)
{
	mFishTexture = texture;

	mPosition = position;

	mFishSprite.setTexture(*mFishTexture);

	mFishSprite.setPosition(mPosition);

	setAnimation();
}

Fish::~Fish()
{
	delete mMovementComponent;

	delete mAnimationComponent;
}

void Fish::render(sf::RenderTarget& target){ target.draw(mFishSprite); }

void Fish::update(float& deltaTime)
{
	mMovementComponent->update(deltaTime);

	move(20.F, 0.F, deltaTime);

	if (mMovementComponent->getState(MOVING))
		mAnimationComponent->play("MOVE", deltaTime);
}

void Fish::setAnimation()
{
	switch (gVariables::gFishType)
	{
	case FISH:
		createMovementComponent(350.F, 16.F, 6.F);

		createAnimationComponent(*mFishTexture);

		mAnimationComponent->addAnimation("MOVE", 16.F, 0, 0, 5, 0, 256, 256);

		break;

	case BLUE_FISH:
		createMovementComponent(350.F, 16.F, 6.F);

		createAnimationComponent(*mFishTexture);

		mAnimationComponent->addAnimation("MOVE", 16.F, 0, 0, 5, 0, 128, 128);

		break;

	case RED_FISH:
		createMovementComponent(350.F, 16.F, 6.F);

		createAnimationComponent(*mFishTexture);

		mAnimationComponent->addAnimation("MOVE", 16.F, 0, 0, 5, 0, 128, 128);

		break;

	case GREEN_FISH:

		createMovementComponent(350.F, 16.F, 6.F);

		createAnimationComponent(*mFishTexture);

		mAnimationComponent->addAnimation("MOVE", 16.F, 0, 0, 5, 0, 128, 128);

		break;

	default:
		break;
	}
}

void Fish::createMovementComponent(const float max_velocity, const float acceleration, const float deceleration)
{
	mMovementComponent = new MovementComponent(mFishSprite, max_velocity, acceleration, deceleration);
}

void Fish::createAnimationComponent(sf::Texture& texture_sheet)
{
	mAnimationComponent = new AnimationComponent(mFishSprite, texture_sheet);
}

void Fish::move(const float direction_x, const float direction_y, const float& deltaTime)
{
	if (mMovementComponent)
		mMovementComponent->move(direction_x, direction_y, deltaTime);
}
