#include "pch.hpp"
#include "LevelManager.hpp"

#include <GlobalVariables.hpp>

extern int gVariables::gScore;
extern bool gVariables::gIsUnderwater = false;

LevelManager::LevelManager(sf::RenderWindow* window) : pWindow(window), mIsRestart(false)
{
	loadFonts();

	setScore();

	gameOver();

	set.setLevel(mLevelTexture, mLevelSprite);
}

LevelManager::~LevelManager() = default;

void LevelManager::renderLevel(sf::RenderTarget & target)
{
	target.draw(mLevelSprite);

	if (gVariables::gIsUnderwater)
		underWater.render(target);

	else 
		air.render(target);

	if (gVariables::gIsGameOver)
	{
		target.draw(mGameOverText);

		target.draw(mEndGameInputText);
	}

	target.draw(mScoreText);

	target.draw(mText);
}

void LevelManager::updateLevel(float & deltaTime)
{
	if (gVariables::gLoadNext)
		set.setLevel(mLevelTexture, mLevelSprite);

	if (gVariables::gIsUnderwater)
		underWater.update(deltaTime);

	else if (!gVariables::gIsUnderwater)
		air.update(deltaTime);

	updateScore();

	if (gVariables::gIsGameOver)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			pWindow->close();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1))
			restart();
	}
}

void LevelManager::setScore()
{
	mText.setFont(mFont);
	mText.setString("SCORE");
	mText.setPosition(sf::Vector2f(10.F, 10.F));
	mText.setFillColor(sf::Color(255, 255, 224, 255));
	mText.setCharacterSize(50U);

	mScoreText.setFont(mFont);
	mScoreText.setPosition(sf::Vector2f(50.F, 60.F));
	mScoreText.setFillColor(sf::Color(255, 255, 224, 255));
	mScoreText.setCharacterSize(40U);
	mScoreText.setStyle(sf::Text::Bold);
}

void LevelManager::updateScore() { mScoreText.setString(std::to_string(gVariables::gScore)); }

void LevelManager::restart()
{
	gVariables::gIsUnderwater = true;

	

	gVariables::gIsGameOver = false;
}

void LevelManager::gameOver()
{
	mGameOverText.setFont(mEndGameFont);
	mGameOverText.setCharacterSize(150U);
	mGameOverText.setFillColor(sf::Color::Red);
	mGameOverText.setPosition(sf::Vector2f(static_cast<float>(pWindow->getSize().x / 2) - 700.F, static_cast<float>(pWindow->getSize().y / 2) - 300.F));
	mGameOverText.setString("GAME OVER!!!!!");
	
	mEndScoreText.setFont(mFont);
	mEndScoreText.setCharacterSize(70U);

	mEndGameInputText.setFont(mFont);
	mEndGameInputText.setCharacterSize(100U);
	mEndGameInputText.setFillColor(sf::Color::Red);
	mEndGameInputText.setPosition(sf::Vector2f(static_cast<float>(pWindow->getSize().x / 2.F) - 750.F, static_cast<float>(pWindow->getSize().y / 2.F) - 100.F));
	mEndGameInputText.setString("Press Esc to quit");
}

void LevelManager::loadFonts()
{
	if (!mFont.loadFromFile("Resources/Fonts/Anton-Regular.ttf"))
		std::cerr << "font failed to load";

	if (!mFont1.loadFromFile("Resources/Fonts/Anton-Regular.ttf"))
		std::cerr << "font failed to load";

	if (!mEndGameFont.loadFromFile("Resources/Fonts/MetalMacabre.ttf"))
		std::cerr << "No font found";
}
