#include "pch.hpp"
#include "PickUp.hpp"

PickUp::PickUp(std::vector<sf::Texture> &textures, sf::Vector2f position, int type)
{
	mPosition = position;
	mType = type;
	mTextures = &textures;

	mSprite.setTexture((*mTextures)[mType]);
	mSprite.setPosition(position);
	mSprite.setScale(sf::Vector2f(1.F, 1.F));
}

PickUp::~PickUp() = default;

void PickUp::render(sf::RenderTarget & target)
{
	target.draw(mSprite);
}


