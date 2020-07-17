#include "GameScroller.h"

#include <Display/Sprite.h>
#include <Update/UpdateManager.h>

#include "Launcher.h"
#include "Elements/GameImage.h"

Color colors[] = { TFT_RED, TFT_BLUE, TFT_YELLOW, TFT_DARKGREEN };

GameScroller::GameScroller(Sprite* canvas, Launcher* launcher) : canvas(canvas), launcher(launcher),
		origin((canvas->width() - width) / 2){
	for(int i = 0; i < 4; i++){
		games.push_back(new GameImage(canvas));
		games.back()->setColor(colors[i]);
		games.back()->setY(37);
	}

	selectedGame = 1;
	repos();
}

void GameScroller::next(){
	if(delta != 0) return;

	getLLGame()->setX(origin - 2 * width - 2 * gutter);
	direction = RIGHT;
	UpdateManager::addListener(this);
}

void GameScroller::prev(){
	if(delta != 0) return;

	getRRGame()->setX(origin + 2 * width + 2 * gutter);
	direction = LEFT;
	UpdateManager::addListener(this);
}

void GameScroller::draw(){
	for(const GameImage* game : games){
		game->draw();
	}
}

void GameScroller::update(uint micros){
	delta += speed * (micros / 1000000.0f);

	if(direction == RIGHT){
		getLLGame()->setX(origin - 2 * width - 2 * gutter + delta);
		getLGame()->setX(origin - width - gutter + delta);
		getCGame()->setX(origin + delta);
		getRGame()->setX(origin + width + gutter + delta);
	}else{
		getRRGame()->setX(origin + 2 * width + 2 * gutter - delta);
		getRGame()->setX(origin + width + gutter - delta);
		getCGame()->setX(origin - delta);
		getLGame()->setX(origin - width - gutter - delta);
	}

	if(delta >= (width + gutter)){
		if(direction == LEFT){
			selectedGame = (selectedGame + 1) % games.size();
		}else{
			if(selectedGame == 0){
				selectedGame = games.size() - 1;
			}else{
				selectedGame--;
			}
		}

		UpdateManager::removeListener(this);
		delta = 0;
		repos();
		launcher->switched();
	}
}

void GameScroller::repos(){
	for(GameImage* game : games){
		game->setX(-128);
	}

	getLGame()->setX(origin - width - gutter);
	getCGame()->setX(origin);
	getRGame()->setX(origin + width + gutter);
}

GameImage* GameScroller::getCGame(){
	return games[selectedGame];
}

GameImage* GameScroller::getLGame(){
	if(selectedGame == 0) return games.back();
	return games[selectedGame - 1];
}

GameImage* GameScroller::getRGame(){
	return games[(selectedGame + 1) % games.size()];
}

GameImage* GameScroller::getLLGame(){
	if(selectedGame <= 1) return games[games.size()-2];
	return games[selectedGame - 2];
}

GameImage* GameScroller::getRRGame(){
	return games[(selectedGame + 2) % games.size()];
}
