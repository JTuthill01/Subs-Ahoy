#include "pch.hpp"
#include "LevelSet.hpp"
#include <GlobalVariables.hpp>

extern int gVariables::gLevel = 0;
extern bool gVariables::gIsUnderwater;
extern bool gVariables::gLoadNext = false;

LevelSet::LevelSet() = default;

LevelSet::~LevelSet() = default;

void LevelSet::setLevel(sf::Texture & texture, sf::Sprite & sprite)
{
	switch (gVariables::gLevel)
	{
	case 0:
		if (gVariables::gLevel == 0)
		{
			if (!texture.loadFromFile("Resources/Backgrounds/water1.png"))
				std::cerr << "Background not found";

			sprite.setTexture(texture);

			gVariables::gIsUnderwater = true;

			gVariables::gLoadNext = false;
		}

		break;

	case 1:
		if (gVariables::gLevel == 1 && gVariables::gLoadNext)
		{
			if (!texture.loadFromFile("Resources/Backgrounds/Custom.png"))
				std::cerr << "Background not found";

			sprite.setTexture(texture);

			gVariables::gIsUnderwater = false;

			gVariables::gLoadNext = false;
		}

		break;

	case 2:
		if (gVariables::gLevel == 2)
		{
			if (!texture.loadFromFile("Resources/Backgrounds/MainMenu.png"))
				std::cerr << "Background not found";

			sprite.setTexture(texture);

			gVariables::gIsUnderwater = true;

			gVariables::gLoadNext = false;
		}

		break;

	case 3:
		if (gVariables::gLevel == 3)
		{
			if (!texture.loadFromFile("Resources/Backgrounds/0.png"))
				std::cerr << "Background not found";

			sprite.setTexture(texture);

			gVariables::gIsUnderwater = true;

			gVariables::gLoadNext = false;
		}

		break;

	case 4:
		if (gVariables::gLevel == 4)
		{
			if (!texture.loadFromFile("Resources/Backgrounds/1.png"))
				std::cerr << "Background not found";

			sprite.setTexture(texture);

			gVariables::gIsUnderwater = true;

			gVariables::gLoadNext = false;
		}

		break;

	case 5:
		if (gVariables::gLevel == 5)
		{
			if (!texture.loadFromFile("Resources/Backgrounds/2.png"))
				std::cerr << "Background not found";

			sprite.setTexture(texture);

			gVariables::gIsUnderwater = true;

			gVariables::gLoadNext = false;
		}

		break;

	default:
		break;
	}
}


