#pragma once
#include <GlobalVariables.hpp>

enum { FISH = 0, BLUE_FISH, RED_FISH, GREEN_FISH };

class Fish
{
public:
	Fish(sf::Texture* texture, sf::Vector2f position);
	~Fish();

	void render(sf::RenderTarget& target);
	void update(float& deltaTime);

	int fishType;

private:
	void setAnimation();
	void createMovementComponent(const float max_velocity, const float acceleration, const float deceleration);
	void createAnimationComponent(sf::Texture& texture_sheet);
	void move(const float direction_x, const float direction_y, const float& deltaTime);

	

	sf::Texture* mFishTexture;
	sf::Sprite mFishSprite;
	sf::Vector2f mPosition;

	MovementComponent* mMovementComponent;
	AnimationComponent* mAnimationComponent;
};

