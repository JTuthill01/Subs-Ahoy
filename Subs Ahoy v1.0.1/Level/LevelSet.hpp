#pragma once
#include <Entities/Player.hpp>

class LevelSet
{
public:
	LevelSet();
	~LevelSet();

	virtual void setLevel(sf::Texture& texture, sf::Sprite& sprite);

private:
	sf::Texture mLevelTexture;

protected:
	sf::Sprite pLevelSprite;
};

