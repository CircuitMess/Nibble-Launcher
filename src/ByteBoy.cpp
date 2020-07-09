#include "ByteBoy.hpp"
Game* Game::instance = nullptr;
Game::Game(Display &display, char* title, const unsigned short* gameIcon, char* description) : 
		Context(display), gameIcon(gameIcon), description(description), title(title)
{
	instance = this;
}

const unsigned short* Game::getIcon()
{
	return instance->gameIcon;
}
const char* Game::getTitle()
{
	return instance->title;
}
const char* Game::getDescription()
{
	return instance->description;
}