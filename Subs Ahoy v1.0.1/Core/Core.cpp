#include "pch.hpp"
#include "Core.hpp"

Core::Core() : WIDTH(1920), HEIGHT(1098), mDeltaTime(0.F), mIsMainMenu(false)
{
	mWindow = new sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT), "Subs Ahoy", sf::Style::Close);

	mMenu = new MainMenu(mWindow);

	mManager = new LevelManager(mWindow);
}

Core::~Core()
{
	delete mWindow;

	delete mMenu;

	delete mManager;
}

void Core::run()
{
	while (mWindow->isOpen())
	{
		update(mDeltaTime);

		render(*mWindow);

		startDeltaTime();
	}
	
}

void Core::update(float & deltaTime)
{
	events();

	if (!mMenu->getIsClosed())
		mMenu->update(deltaTime);

	if (mMenu->getIsClosed())
		mManager->updateLevel(deltaTime);

	if (mIsMainMenu)
		mMenu->update(deltaTime);
}

void Core::render(sf::RenderTarget & target)
{
	mWindow->clear();

	if (!mMenu->getIsClosed())
		mMenu->render(target);

	if (mMenu->getIsClosed())
		mManager->renderLevel(target);

	mWindow->display();
}

void Core::events()
{
	while (mWindow->pollEvent(e))
	{
		if (e.type == sf::Event::Closed)
			mWindow->close();

		if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)
			mMenu->setIsClosed(false);

		if (!mMenu->getIsClosed())
			mMenu->handleButtonEvents(e);
	}
}

void Core::startDeltaTime() { mDeltaTime = mClock.restart().asSeconds(); }
