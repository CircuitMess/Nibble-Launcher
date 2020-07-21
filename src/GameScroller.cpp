#include "GameScroller.h"

#include <Display/Sprite.h>
#include <Update/UpdateManager.h>

#include "Elements/GameImage.h"

Color colors[] = { TFT_RED, TFT_BLUE, TFT_YELLOW, TFT_DARKGREEN };

GameScroller::GameScroller(Sprite* canvas, const GameInfo gameDefs[]) : canvas(canvas),
		origin((canvas->width() - width) / 2){

	for(int i = 0; i < sizeof(gameDefs); i++){
		games.push_back(new GameImage(canvas, gameDefs[i].icon));
		games.back()->setY(37);
	}

	repos();
}

uint8_t GameScroller::prev(){
	if(scrolling() && direction == PREV){
		multiplier = min(2, multiplier+1);
		queued = true;
		return (selectedGame < 2) ? games.size() - 2 + selectedGame : selectedGame - 2;
	}

	direction = PREV;

	if(scrolling()){ // direction == NEXT
		selectNext();
		delta = (float) width + (float) gutter - delta;
		queued = false;
	}else{
		delta = 1;
		multiplier = 1;
		getLLGame()->setX(origin - 2 * width - 2 * gutter);
		UpdateManager::addListener(this);
	}

	return (selectedGame < 1) ? games.size() - 1 : selectedGame - 1;
}

uint8_t GameScroller::next(){
	if(scrolling() && direction == NEXT){
		multiplier = min(2, multiplier+1);
		queued = true;
		return (selectedGame + 2) % games.size();
	}

	direction = NEXT;

	if(scrolling()){ // direction == PREV
		selectPrev();
		delta = (float) width + (float) gutter - delta;
		queued = false;
	}else{
		delta = 1;
		multiplier = 1;
		getRRGame()->setX(origin + 2 * width + 2 * gutter);
		UpdateManager::addListener(this);
	}

	return (selectedGame + 1) % games.size();
}

void GameScroller::draw(){
	for(const GameImage* game : games){
		game->draw();
	}
}

void GameScroller::update(uint micros){
	delta += speed * (micros / 1000000.0f) * (float) multiplier;

	if(direction == PREV){
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
		if(direction == NEXT){
			selectNext();
		}else{
			selectPrev();
		}

		if(queued){
			queued = false;
			delta = 1;
			return;
		}

		UpdateManager::removeListener(this);
		delta = 0;
		repos();
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

bool GameScroller::scrolling(){
	return delta != 0;
}

void GameScroller::selectNext(){
	selectedGame = (selectedGame + 1) % games.size();
}

void GameScroller::selectPrev(){
	if(selectedGame == 0){
		selectedGame = games.size() - 1;
	}else{
		selectedGame--;
	}
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
