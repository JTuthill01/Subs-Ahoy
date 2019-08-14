#include "pch.hpp"
#include "MainMenu.hpp"

extern int gVariables::gFishType;

MainMenu::MainMenu(sf::RenderWindow* window) : mWindow(window), mIsClosed(false), width(1920), height(1098)
{
	mGui = new tgui::Gui();

	mGui->setTarget(*window);

	setText();

	gui();

	mTimer.restart(sf::seconds(0.1F));

	mFishTimer.restart(sf::seconds(0.1F));

	initTextures();

	loadFish();
}

MainMenu::~MainMenu()
{
	delete mGui;
}

void MainMenu::render(sf::RenderTarget & target)
{
	if (!mIsClosed)
	{
		target.draw(mBackgroundSprite);

		for (size_t i = 0; i < mFishes.size(); i++)
			mFishes[i]->render(target);

		target.draw(mText);

		mGui->draw();

		for (size_t i = 0; i < mProjectiles.size(); i++)
			mProjectiles[i]->render(target);
	}
}

void MainMenu::handleButtonEvents(sf::Event & e)
{
	mGui->handleEvent(e);
}

void MainMenu::update(float & deltaTime)
{
	if (mTimer.isExpired())
	{
		mProjectiles.push_back(new Projectiles(&mBubble[0], sf::Vector2f(thor::random(0.F, width), thor::random(0.F, height)), sf::Vector2f(0.05F, 0.05F), sf::Vector2f(0.F, -0.1F)));
		mTimer.restart(sf::seconds(0.5F));
	}

	gVariables::gFishType = thor::random(0, 4);

	if (mFishTimer.isExpired())
	{
		if(gVariables::gFishType == 0)
			mFishes.push_back(new Fish(&mFishTexture[FISH], sf::Vector2f(thor::random(0.F, 950.F), thor::random(0.F, 1300.F))));

		if(gVariables::gFishType == 1)
			mFishes.push_back(new Fish(&mFishTexture[BLUE_FISH], sf::Vector2f(thor::random(0.F, 950.F), thor::random(0.F, 1300.F))));

		if(gVariables::gFishType == 2)
			mFishes.push_back(new Fish(&mFishTexture[RED_FISH], sf::Vector2f(thor::random(0.F, 950.F), thor::random(0.F, 1300.F))));

		if(gVariables::gFishType == 3)
			mFishes.push_back(new Fish(&mFishTexture[GREEN_FISH], sf::Vector2f(thor::random(0.F, 950.F), thor::random(0.F, 1400.F))));

		mFishTimer.restart(sf::seconds(1.5F));
	}

	for (size_t i = 0; i < mFishes.size(); i++)
		mFishes[i]->update(deltaTime);

	for (size_t i = 0; i < mProjectiles.size(); i++)
		mProjectiles[i]->update(deltaTime);

	for (size_t i = 0; i < mProjectiles.size(); i++)
		if (mProjectiles[i]->getProjectilePosition().y <= 1)
			mProjectiles.erase(mProjectiles.begin() + i);
}

void MainMenu::loadFish()
{
	sf::Texture temp;
	if (!temp.loadFromFile("Resources/Sprites/Fish/fish.png"))
		std::cerr << "Fish not found";
	mFishTexture.push_back(temp);

	if (!temp.loadFromFile("Resources/Sprites/Fish/fishColor3.png"))
		std::cerr << "Fish not found";
	mFishTexture.push_back(temp);

	if (!temp.loadFromFile("Resources/Sprites/Fish/fishColor1.png"))
		std::cerr << "Fish not found";
	mFishTexture.push_back(temp);


	if (!temp.loadFromFile("Resources/Sprites/Fish/fishColor2.png"))
		std::cerr << "Fish not found";
	mFishTexture.push_back(temp);
}

void MainMenu::initTextures()
{
	if (!mBackgroundTexture.loadFromFile("Resources/Backgrounds/MainMenu.png"))
		std::cerr << "Background failed to load" << "\n";

	sf::Texture temp;
	if (!temp.loadFromFile("Resources/Sprites/Bubble.png"))
		std::cerr << "No bubble texture found";

	mBubble.push_back(temp);

	mBackgroundSprite.setTexture(mBackgroundTexture);
}

void MainMenu::gui()
{
	tgui::Theme theme{ "Resources/themes/black.txt" };

	auto mButtons = tgui::Button::create();
	mButtons->setPosition(400, 600);
	mButtons->setRenderer(theme.getRenderer("Button"));
	mButtons->setText("Play");
	mButtons->setTextSize(25U);
	mButtons->setSize(150, 50);
	mButtons->connect("pressed", [&]() { mIsClosed = true;});
	mGui->add(mButtons);

	auto mButton = tgui::Button::create();
	mButton->setPosition(400, 700);
	mButton->setRenderer(theme.getRenderer("Button"));
	mButton->setText("Quit");
	mButton->setTextSize(25U);
	mButton->setSize(150, 50);
	mButton->connect("pressed", [&]() { mWindow->close(); });
	mGui->add(mButton);
}

void MainMenu::setText()
{
	if (!mFont.loadFromFile("Resources/Fonts/Anton-Regular.ttf"))
		std::cerr << "Font failed to load" << "\n";

	mText.setFillColor(sf::Color(255, 255, 255, 190));
	mText.setCharacterSize(100U);
	mText.setFont(mFont);
	mText.setString("Subs Ahoy v1.0.1");
	mText.setPosition(sf::Vector2f(static_cast<float>(mWindow->getSize().x) / 2 - 450, static_cast<float>(mWindow->getSize().y) - 800.F));
}
