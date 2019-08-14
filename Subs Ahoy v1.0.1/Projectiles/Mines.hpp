#pragma once
#include <GlobalVariables.hpp>

class Mines
{
public:
	Mines(sf::Vector2f position);
	Mines(sf::Texture* texture, sf::Vector2f position);
	~Mines();

	void render(sf::RenderTarget& target);
	void takeDamage(int damage);

	inline sf::Sprite& getMineSprite() { return mMineSprite; }
	inline sf::Vector2f getMinePosition() { return mMineSprite.getPosition(); }

private:
	sf::Texture mMineTexture;
	sf::Sprite mMineSprite;

	void load();

	bool mIsAlive;
};

