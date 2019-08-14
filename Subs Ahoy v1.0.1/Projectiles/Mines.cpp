#include "pch.hpp"
#include "Mines.hpp"

extern int gVariables::gMineHp = 1;

Mines::Mines(sf::Vector2f position) : mIsAlive(true)
{
	load();

	mMineSprite.setTexture(mMineTexture);
	mMineSprite.setPosition(position);
	mMineSprite.setScale(0.5F, 0.5F);
}

Mines::Mines(sf::Texture* texture, sf::Vector2f position) : mIsAlive(true)
{
}

Mines::~Mines() = default;

void Mines::render(sf::RenderTarget & target) 
{ 
	if(mIsAlive)
		target.draw(mMineSprite); 
}

void Mines::takeDamage(int damage)
{
	gVariables::gMineHp -= damage;

	if (gVariables::gMineHp <= 0)
	{
		gVariables::gMineHp = 0;

		mIsAlive = false;
	}
}

void Mines::load()
{
	if (!mMineTexture.loadFromFile("Resources/Sprites/mine.png"))
		std::cerr << "Mine texture not found";
}
