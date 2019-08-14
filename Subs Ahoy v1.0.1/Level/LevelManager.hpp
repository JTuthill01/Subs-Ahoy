#pragma once
#include "Underwater_Level.hpp"
#include "Air_and_Sea_Level.hpp"

class LevelManager : public Underwater_Level, public Air_and_Sea_Level
{
public:
	LevelManager(sf::RenderWindow* window);
	~LevelManager();

	void renderLevel(sf::RenderTarget& target);
	void updateLevel(float& deltaTime);

private:

	void setScore();
	void updateScore();
	void restart();
	void loadFonts();
	void gameOver();

	bool mIsRestart;

	sf::Texture mLevelTexture;
	sf::Sprite mLevelSprite;
	sf::Text mText;
	sf::Text mScoreText;
	sf::Text mEndScoreText;
	sf::Text mEndGameInputText;
	sf::Text mGameOverText;
	sf::Font mFont;
	sf::Font mFont1;
	sf::Font mEndGameFont;
	sf::RenderWindow* pWindow;

	Player* mPlayer;
	Underwater_Level underWater;
	Air_and_Sea_Level air;
	LevelSet set;
};

