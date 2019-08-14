#pragma once
#include <Projectiles/Projectiles.hpp>
#include <Fish/Fish.hpp>
#include <GlobalVariables.hpp>

class MainMenu
{
public:
	MainMenu(sf::RenderWindow* window);
	~MainMenu();

	void render(sf::RenderTarget& target);
	void handleButtonEvents(sf::Event& e);
	void update(float& deltaTime);
	void setIsClosed(bool t_f) { mIsClosed = t_f; }
	bool getIsClosed() { return mIsClosed; }

private:
	void loadFish();
	void initTextures();
	void gui();
	void setText();

	bool mIsClosed;

	std::vector<Projectiles*> mProjectiles;
	std::vector<Fish*> mFishes;
	Fish* mFish;

	//SFML Stuff
	sf::RenderWindow* mWindow;
	sf::Font mFont1;
	sf::Font mFont;
	sf::Text mText;
	std::vector<sf::Texture> mBubble;
	std::vector<sf::Texture> mFishTexture;
	sf::Texture mBackgroundTexture;
	sf::Sprite mBackgroundSprite;

	thor::Timer mTimer;
	thor::Timer mFishTimer;

	tgui::Gui* mGui;

	int mType;

	float width, height;
};

