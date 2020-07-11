#include "ByteBoy.hpp"
Game::Game(Display &display) : Context(display)
{
}
GameInfo Game::getGameInfo()
{
	return info;
}