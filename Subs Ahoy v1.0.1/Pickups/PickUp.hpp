#pragma once

enum pickUps { HEALTH = 0, AMMO, SPEED };

class PickUp
{
public:
	PickUp(std::vector<sf::Texture> &textures, sf::Vector2f position, int type);
	~PickUp();

	void render(sf::RenderTarget& target);

	inline sf::Sprite& getPickupSprite() { return mSprite; }
	inline const int& getType() const { return mType; }
	inline sf::Vector2f getPosition() { return mSprite.getPosition(); }
private:
	std::vector<sf::Texture>* mTextures;

	sf::Vector2f mPosition;
	sf::Sprite mSprite;

	int mType;
};

