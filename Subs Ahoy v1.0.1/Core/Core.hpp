#pragma once
#include <Entities/Player.hpp>
#include <Entities/Enemies.hpp>
#include <Level/LevelManager.hpp>
#include "MainMenu.hpp"


class Core
{
public:
	Core();
	~Core();

	void run();

private:
	void update(float& deltaTime);
	void render(sf::RenderTarget& target);
	void events();
	void startDeltaTime();

	sf::RenderWindow* mWindow;
	sf::Event e;
	sf::Clock mClock;

	float mDeltaTime;

	int WIDTH, HEIGHT;

	bool mIsMainMenu;

	LevelManager* mManager;
	MainMenu* mMenu;
};

