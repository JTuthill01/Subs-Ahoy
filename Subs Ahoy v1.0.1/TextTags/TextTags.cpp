#include "pch.hpp"
#include "TextTags.hpp"

TextTags::TextTags(sf::Font * font, std::string text, sf::Vector2f position, sf::Vector2f direction, sf::Color color, unsigned size)
{
	mText.setCharacterSize(size);
	mText.setFont(*font);
	mText.setPosition(position);
	mText.setFillColor(color);
	mText.setString(text);
	mText.move(direction);
}

TextTags::~TextTags() = default;

void TextTags::render(sf::RenderTarget & target)
{
	target.draw(mText);
}
