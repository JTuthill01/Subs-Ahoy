#include "pch.hpp"
#include "LevelManager.hpp"

#include <GlobalVariables.hpp>

extern int gVariables::gScore;
extern bool gVariables::gIsUnderwater = false;

LevelManager::LevelManager(sf::RenderWindow* window) : pWindow(window), mIsRestart(false), mIsShown(false)
{
	loadFonts();

	setScore();

	gameOver();

	set.setLevel(mLevelTexture, mLevelSprite);

	showKeys();

	mHintTimer.restart(sf::seconds(2.F));
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

	if (mIsShown)
		target.draw(mKeys);

	if (mHintTimer.isRunning())
		target.draw(mHint);
}

void LevelManager::updateLevel(float& deltaTime)
{
	if (gVariables::gLoadNext)
		set.setLevel(mLevelTexture, mLevelSprite);

	if (gVariables::gIsUnderwater)
		underWater.update(deltaTime);

	else if (!gVariables::gIsUnderwater)
		air.update(deltaTime);

	updateScore();

	if (gVariables::gIsGameOver)
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			pWindow->close();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1))
	{
		mIsShown = true;

		mKeyTimer.restart(sf::seconds(1.F));
	}

	if (mKeyTimer.isExpired())
		mIsShown = false;
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
	mEndGameInputText.setPosition(sf::Vector2f(static_cast<float>(pWindow->getSize().x / 2.F) - 550.F, static_cast<float>(pWindow->getSize().y / 2.F) - 100.F));
	mEndGameInputText.setString("Press Esc to quit");
}

void LevelManager::showKeys()
{
	mKeys.setFont(mFont);
	mKeys.setCharacterSize(60U);
	mKeys.setFillColor(sf::Color(255, 140, 0, 255));
	mKeys.setPosition(sf::Vector2f(static_cast<float>(pWindow->getSize().x / 2.F), static_cast<float>(pWindow->getSize().y / 2.F) - 500));
	mKeys.setString("Movement: \n"  " W: up \n"  " D: right \n" "S: down \n" "A: left \n" "Left Mouse: shoot torpedo \n" "R Mouse: shoot SAM missile \n"
	"Next level: Position >  \n the positive x axis");

	mHint.setFont(mFont);
	mHint.setCharacterSize(60U);
	mHint.setFillColor(sf::Color(255, 140, 0, 255));
	mHint.setPosition(sf::Vector2f(static_cast<float>(pWindow->getSize().x / 2.F) - 600, static_cast<float>(pWindow->getSize().y / 2.F) - 300));
	mHint.setString("Press F1 for Keyboard controls");
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
