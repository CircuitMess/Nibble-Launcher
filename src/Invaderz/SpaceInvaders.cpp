#include "SpaceInvaders.h"
#include "sprites.hpp"
#include "icon.h"
SpaceInvaders* SpaceInvaders::instance = nullptr;
SpaceInvaders::SpaceInvaders(Display& display) :
		Game(display, "Invaderz", icon, "Classic Space Invaders remake."), baseSprite(display.getBaseSprite()),
		baseImage(&screen, screen.getWidth(), screen.getHeight()),
		buttons(Input::getInstance())
{
	instance = this;
	addSprite(&baseImage);
	highscoresPath = (char*)calloc(30, 1);
	strncpy(highscoresPath, "/", 30);
	if(getTitle()){
		strncat(highscoresPath, getTitle(), 30);
	}
	else
	{
		strncat(highscoresPath, "game", 30);
	}
	
	strncat(highscoresPath, ".sav", 30);
}
void SpaceInvaders::start()
{
	starsSetup();
	prevGamestatus = "";
	gamestatus = "title";
	File file = SPIFFS.open(highscoresPath, "r");
	deserializeJson(jb, file);
	JsonArray hiscores = jb.to<JsonArray>();
	file.close();
	if(!hiscores.isNull())
		savePresent = 1;
	else
	{
		Serial.println("No save present");
		JsonArray hiscores = jb.as<JsonArray>();
		File file = SPIFFS.open(highscoresPath, "w");
		serializeJson(hiscores, file);
		file.close();
	}
	serializeJsonPretty(hiscores, Serial);
	Serial.println("saves ok");
	UpdateManager::addListener(this);
}
void SpaceInvaders::stop()
{
	UpdateManager::removeListener(this);
	clearButtonCallbacks();
	jb.clear();
	delete[] highscoresPath;
}
void SpaceInvaders::draw(){
	if (gamestatus == "title") {
		showtitle();
	}
	if (gamestatus == "running") { // game running loop
		baseSprite->clear(TFT_BLACK);
		for(int i = 0; i < STAR_COUNT; i++)
		{
			// Remove the star from the screen by changing its pixel to the background color.
			baseSprite->drawPixel(stars[i].x, stars[i].y, BACKGROUND_COLOR);

			// Update the position of the star.
			stars[i].update();

			// If the star's Y position is off the screen after the update.
			if(stars[i].y >= baseSprite->height())
			{
				// Randomize its position.
				stars[i].randomize(0, baseSprite->width(), 0, baseSprite->height(), STAR_SPEED_MIN, STAR_SPEED_MAX);
				// Set its Y position back to the top.
				stars[i].y = 0;
			}

			// Draw the star at its new coordinate.
			baseSprite->fillRect(stars[i].x, stars[i].y, 2, 2, STAR_COLOR);
			yield();
		}
		showscore();
		drawplayership(); // draw player ship
		drawplayershot(); // move + draw player shoot
		drawinvaders(); // draw invaders
		drawInvaderShot(); // invaders shoot
		drawbunkers(); // draw bunkers & check collission with player shot
		drawsaucer(); // draw saucer & remove if hit
		showscore(); // show lives, score, level
	}
	if (gamestatus == "gameover") { // game over
		baseSprite->setTextColor(TFT_RED);
		baseSprite->setTextSize(2);
		baseSprite->setTextFont(1);
		baseSprite->drawRect(5, 45, 118, 38, TFT_WHITE);
		baseSprite->drawRect(4, 44, 120, 40, TFT_WHITE);
		baseSprite->fillRect(6, 46, 116, 36, TFT_BLACK);
		baseSprite->setCursor(47, 57);
		baseSprite->printCenter("GAME OVER");
	}
	if(gamestatus == "paused")
	{
		baseSprite->clear(TFT_BLACK);
		baseSprite->setTextColor(TFT_RED);
		baseSprite->setCursor(0, baseSprite->height()/2 - 18);
		baseSprite->setTextFont(2);
		baseSprite->setTextSize(2);
		baseSprite->printCenter("Paused");
		baseSprite->setCursor(4, 110);
		baseSprite->setFreeFont(TT1);
		baseSprite->printCenter("A:RESUME  B:QUIT");
	}
	if(gamestatus == "eraseData")
	{
		eraseData();
	}
	if(gamestatus == "dataDisplay")
	{
		dataDisplay();
	}
	if(gamestatus == "enterInitials")
	{
		enterInitials();
	}
	screen.commit();
}
void SpaceInvaders::update(uint)
{
	if(gamestatus != prevGamestatus)
	{
		screenChange = 1;
		prevGamestatus = gamestatus;
	}
	else
	{
		screenChange = 0;
	}
	
	if (gamestatus == "title") {
		if(screenChange)
		{
			titleSetup();
		}
		showtitle();
	}
	if (gamestatus == "newgame") { newgame(); } // new game

	if (gamestatus == "newlevel") { newlevel(); } // new level

	if (gamestatus == "running") { // game running loop
		invaderlogic(); // invader logic
		nextlevelcheck(); // next level?
		saucerappears(); // saucer appears?
		movesaucer(); // move saucer
	}
	if (gamestatus == "gameover") { // game over
		if(screenChange){
			clearButtonCallbacks();
			buttons->setBtnPressCallback(BTN_A, [](){
				File file = SPIFFS.open(instance->highscoresPath, "r");
				serializeJson(instance->jb, file);
				JsonArray hiscores = instance->jb.to<JsonArray>();
				file.close();
				for (JsonObject element : hiscores)
				{
					if(element["Rank"] == 1)
						instance->tempScore = element["Score"].as<int>();
				}
				hiscores.end();
				Serial.println("HERE");
				// delay(5);
				instance->gamestatus = "enterInitials";
			});
			buttons->setBtnPressCallback(BTN_B, [](){
				File file = SPIFFS.open(instance->highscoresPath, "r");
				serializeJson(instance->jb, file);
				JsonArray hiscores = instance->jb.to<JsonArray>();
				file.close();
				for (JsonObject element : hiscores)
				{
					if(element["Rank"] == 1)
						instance->tempScore = element["Score"].as<int>();
				}
				hiscores.end();
				Serial.println("HERE");
				// delay(5);
				instance->gamestatus = "enterInitials";
			});
		}
	}
	if(gamestatus == "paused")
	{
		if(screenChange){
			clearButtonCallbacks();
			buttons->setBtnPressCallback(BTN_A, [](){
				instance->gamestatus = "running";
				instance->setButtonsCallbacks();
			});
			buttons->setBtnPressCallback(BTN_B, [](){
				Serial.println("going to title");
				Serial.println(instance->highscoresPath);
				instance->gamestatus = "title";
			});
		}
	}
	if(gamestatus == "eraseData")
	{
		if(screenChange){
			eraseDataSetup();
		}
	}
	if(gamestatus == "dataDisplay")
	{
		if(screenChange){
			dataDisplaySetup();
		}
	}
	if(gamestatus == "enterInitials")
	{
		if(screenChange){
			enterInitialsSetup();
		}
	}
	draw();
}
void SpaceInvaders::starsSetup()
{
	// Loop through each star.
	for(int i = 0; i < STAR_COUNT; i++)
	{
		// Randomize its position && speed.
		stars[i].randomize(0, baseSprite->width(), 0, baseSprite->height(), STAR_SPEED_MIN, STAR_SPEED_MAX);
	}
} 
void SpaceInvaders::drawBitmap(int16_t x, int16_t y, const byte *bitmap, uint16_t color, uint8_t scale) {
	uint16_t w = *(bitmap++);
	uint16_t h = *(bitmap++);
	baseSprite->drawMonochromeIcon(bitmap, x, y, w, h, scale, color);
}

//ported nonstandard
//----------------------------------------------------------------------------
void SpaceInvaders::newgame() {
	score = 0;
	lives = 3;
	gamelevel = 0;
	shipx = 60;
	shotx = -1;
	shoty = -1;
	deadcounter = -1;
	saucers = -1;
	starsSetup();
	for (int i = 0; i < 4; i++) {
		invadershotx[i] = -1;
		invadershoty[i] = -1;
	}
	gamestatus = "newlevel";
	setButtonsCallbacks();
}
//----------------------------------------------------------------------------
void SpaceInvaders::newlevel() {
	invaderanz = 30;
	invaderctr = 29;
	invaderxr = 1;
	invaderyr = 1;
	checkdir = 0;
	nextxdir = 4;
	nextydir = 0;
	yeahtimer = 0;
	delayBip = 0;
	invadershottimer = 120;
	saucertimer = 480;
	int down = gamelevel;
	if (gamelevel > 8) { down = 16*2; }
	for (int i = 0; i < 6; i++) {
		invaderx[i] = 10 + i * 8*2;
		invaderx[i + 6] = 10 + i * 8*2;
		invaderx[i + 12] = 10 + i * 8*2;
		invaderx[i + 18] = 10 + i * 8*2;
		invaderx[i + 24] = 10 + i * 8*2;
		invadery[i] = 14 + down;
		invadery[i + 6] = 13*2 + down;
		invadery[i + 12] = 19*2 + down;
		invadery[i + 18] = 25*2 + down;
		invadery[i + 24] = 31*2 + down;
		invaders[i] = 4;
		invaders[i + 6] = 2;
		invaders[i + 12] = 2;
		invaders[i + 18] = 0;
		invaders[i + 24] = 0;
		invaderframe[i] = 0;
		invaderframe[i + 6] = 0;
		invaderframe[i + 12] = 0;
		invaderframe[i + 18] = 0;
		invaderframe[i + 24] = 0;
		yield();
	}
	for (int i = 0; i < 4; i++) {
		bunkers[i] = 0;
		if (gamelevel > 5) { bunkers[i] = -1; }
		yield();
	}
	gamestatus = "running";
}
//----------------------------------------------------------------------------
void SpaceInvaders::showscore() {
	if (infoshow == 1 && saucers == -1) {
		if (lives > 1) { drawBitmap(0, 0, playership[0], TFT_WHITE, 2); }
		if (lives > 2) { drawBitmap(18, 0, playership[0], TFT_WHITE, 2); }
		baseSprite->cursor_x= 84 - 4 * (score > 9) - 4 * (score > 99) - 4 * (score > 999);
		baseSprite->cursor_y = 10;
		baseSprite->print(score);
		baseSprite->cursor_x = 112;
		baseSprite->cursor_y = 10;
		baseSprite->print(gamelevel + 1);
	}
}
//----------------------------------------------------------------------------
void SpaceInvaders::nextlevelcheck() {
	// increment timer after all invaders killed
	if (invaderanz == 0) {
		yeahtimer++;
		if (yeahtimer >= 90) {
			gamelevel++;
			gamestatus = "newlevel";
		}
	}
}
//----------------------------------------------------------------------------
void SpaceInvaders::handledeath() {
	deadcounter--;
	if (deadcounter == 0) {
		deadcounter = -1;
		lives--;
		shipx = 0;
		if (lives == 0) { gamestatus = "gameover"; }
	}
}


//ported specific
//----------------------------------------------------------------------------
void SpaceInvaders::clearButtonCallbacks()
{
	for(uint8_t i = 0; i < 7; i++)
	{
		buttons->setBtnReleaseCallback(i, nullptr);
		buttons->setBtnPressCallback(i, nullptr);
		buttons->setButtonHeldRepeatCallback(i, 0, nullptr);
		buttons->setButtonHeldCallback(i, 0, nullptr);
	}
}
void SpaceInvaders::setButtonsCallbacks() {
	clearButtonCallbacks();
	buttons->setButtonHeldRepeatCallback(BTN_LEFT, 10, [](uint){
		if (instance->shipx > 0 && instance->deadcounter == -1) {
			instance->shipx-=1;
		}
	});
	buttons->setButtonHeldRepeatCallback(BTN_RIGHT, 10, [](uint){
		if (instance->shipx < 111 && instance->deadcounter == -1) {
			instance->shipx+=1;
		}
	});
	buttons->setBtnPressCallback(BTN_A, [](){
		if (instance->shotx == -1 && instance->deadcounter == -1) {
			instance->shotx = instance->shipx + 6;
			instance->shoty = 106;
			tone(BUZZ_PIN, 400, 50);
		}
	});
	buttons->setBtnPressCallback(BTN_B, [](){
		Serial.println("paused");
		instance->gamestatus = "paused";
	});
}
//----------------------------------------------------------------------------
void SpaceInvaders::drawplayership() {
	if (deadcounter == -1) {
		drawBitmap(shipx, 110, playership[0], TFT_WHITE, 2);
	}
	else {
		drawBitmap(shipx, 110, playership[1 + invadershotframe], TFT_YELLOW, 2);
		handledeath();
	}
}
//----------------------------------------------------------------------------
void SpaceInvaders::drawplayershot() {
	if (shotx != -1) {
		shoty = shoty - 2;
		baseSprite->drawLine(shotx, shoty, shotx, shoty + 6, TFT_YELLOW);
		baseSprite->drawLine(shotx+1, shoty, shotx+1, shoty + 6, TFT_YELLOW);
		if (shoty < 0) {
			shotx = -1;
			shoty = -1;
		}
	}
}
//----------------------------------------------------------------------------
void SpaceInvaders::invaderlogic() {
	// increment invader counter
	if (invaderanz > 0) {
		checkdir = 0;
		do {
			invaderctr++;
			if (invaderctr > 30) {
				invaderctr = 0;
				checkdir = 1;
				invadersound = ++invadersound % 4;
				if (delayBip <= 0) {
					if (invaderanz < 6) {
						delayBip = 5;
					}
					else if (invaderanz < 11) {
						delayBip = 3;
					} if (invaderanz < 21) {
						delayBip = 2;
					}
				}
				else { delayBip--; }
			}
			yield();
		} while (invaders[invaderctr] == -1);

		// change direction?
		if (checkdir == 1) {
			if (nextydir != 0) {
				invaderxr = 0;
				invaderyr = 2;
			}
			else {
				invaderxr = nextxdir;
				invaderyr = 0;
			}
			checkdir = 0;
		}

		// change invader position
		invaderx[invaderctr] = invaderx[invaderctr] + invaderxr;
		invadery[invaderctr] = invadery[invaderctr] + invaderyr;

		// determine bunker removal if invaders are too low
		if (invadery[invaderctr] > 80) {
			for (int i = 0; i < 4; i++) {
				bunkers[i] = -1;
			}
		}

		// determine game over if invaders reach bottom
		if (invadery[invaderctr] > 100) {
			gamestatus = "gameover";
		}

		// determine screen border hit -> go down, then change direction
		if (invaderx[invaderctr] > 108 && invaderxr > 0) {
			nextxdir = -4;
			nextydir = 4;
		}
		if (invaderx[invaderctr] < 4 && invaderxr < 0) {
			nextxdir = 4;
			nextydir = 4;
		}
		if (invaderyr != 0) { nextydir = 0; }

		//change invader shape
		invaderframe[invaderctr] = ++invaderframe[invaderctr] % 2;

		// remove killed invader
		if (invaders[invaderctr] == 6) {
			invaders[invaderctr] = -1;
			invaderanz--;
		}

		// release invadershoot
		if (invadershottimer <= 0 && invadershots < gamelevel + 1 && invadershots < 4 && invadery[invaderctr] < 80) {
			invadershottimer = 120 - gamelevel * 10;
			invadershots++;
			int flag = 0;
			for (int u = 0; u < 4; u++) {
				if (flag == 0 && invadershotx[u] == -1) {
					invadershotx[u] = invaderx[invaderctr] + 2;
					invadershoty[u] = invadery[invaderctr];
					flag = 1;
				}
			}
		}
	}
}
//----------------------------------------------------------------------------
void SpaceInvaders::drawinvaders() {
	infoshow = 1;
	for (int i = 0; i < 30; i++) {
		if (invaders[i] != -1) {
			if (invaders[i] == 0) drawBitmap(invaderx[i], invadery[i], invader[invaders[i] + invaderframe[i]], TFT_ORANGE, 2);
			if (invaders[i] == 2) drawBitmap(invaderx[i], invadery[i], invader[invaders[i] + invaderframe[i]], TFT_PINK, 2);
			if (invaders[i] == 4) drawBitmap(invaderx[i], invadery[i], invader[invaders[i] + invaderframe[i]], TFT_BLUE, 2);
			if (invaders[i] == 6) drawBitmap(invaderx[i], invadery[i], invader[invaders[i] + invaderframe[i]], TFT_YELLOW, 2);
			
			if (invadery[i] < 5) {
				infoshow = 0;
			}
		}

		// determine collission: invader & player shoot
		checkl = invaderx[i];
		checkr = invaderx[i] + 12;
		checkt = invadery[i];
		checkb = invadery[i] + 8;
		if (invaders[i] == 4) {
			checkl++;
			checkr--;
		}
		if (invaders[i] != -1 && invaders[i] != 6 && shotx >= checkl && shotx <= checkr && shoty + 2 >= checkt && shoty <= checkb) {
			score = score + invaders[i] * 10 + 10;
			invaders[i] = 6;
			shotx = -1;
			shoty = -1;
			// destroyed->note(10, 0.05);
			tone(BUZZ_PIN, 50, 50);

			// invaderDestroyed->play();
		}
		yield();
	}
}
//----------------------------------------------------------------------------
void SpaceInvaders::drawInvaderShot() {
	// handle invadershoot timer & framecounter
	invadershottimer--;
	if(invadershotframe == pastInvaderShotFrame)
		invadershotframe = !invadershotframe;
	else
		pastInvaderShotFrame = !pastInvaderShotFrame;
	// move invadershots, draw & check collission
	for (int i = 0; i < 4; i++) {
		if (invadershotx[i] != -1) {
			invadershoty[i] = invadershoty[i] + 1;
			drawBitmap(invadershotx[i], invadershoty[i], bomb[invadershotframe], TFT_RED, 2);

			// check collission: invadershot & bunker
			for (int u = 0; u < 4; u++) {
				checkl = 22 + u * 36;
				checkr = 22 + u * 36 + 14;
				checkt = 90;
				checkb = 100;
				if (bunkers[u] != -1 && invadershotx[i] + 1 >= checkl && invadershotx[i] <= checkr && invadershoty[i] + 3 >= checkt && invadershoty[i] <= checkb) {
					bunkers[u]++;
					if (bunkers[u] > 4) { bunkers[u] = -1; }
					invadershotx[i] = -1;
					invadershoty[i] = -1;
					invadershots--;
				}
				yield();
			}

			// check collission: invadershot & player
			checkl = shipx;
			checkr = shipx + 12;
			checkt = 107;
			checkb = 110;
			if (deadcounter == -1 && invadershotx[i] + 1 >= checkl && invadershotx[i] <= checkr && invadershoty[i] + 3 >= checkt && invadershoty[i] <= checkb) {
				deadcounter = 60;
				// destroyed->note(10, 0.05);
				tone(BUZZ_PIN, 50, 50);
				// playerDestroyed->play();
			}

			//check collission: invadershot & playershoot
			checkl = invadershotx[i];
			checkr = invadershotx[i] + 1;
			checkt = invadershoty[i];
			checkb = invadershoty[i] + 3;
			if (shotx >= checkl && shotx <= checkr && shoty + 2 >= checkt && shoty <= checkb) {
				shotx = -1;
				shoty = -1;
				invadershotx[i] = -1;
				invadershoty[i] = -1;
				invadershots--;
			}

			// invadershoot on bottom off screen?
			if (invadershoty[i] > 128) {
				invadershotx[i] = -1;
				invadershoty[i] = -1;
				invadershots--;
			}
		}
		yield();
	}
}
//----------------------------------------------------------------------------
void SpaceInvaders::drawbunkers() {
	for (int i = 0; i < 4; i++) {
		checkl = 12 + i * 30;
		checkr = 12 + i * 30 + 14;
		checkt = 90;
		checkb = 100;
		if (bunkers[i] != -1 && shotx >= checkl && shotx <= checkr && shoty + 2 >= checkt && shoty <= checkb) {
			bunkers[i]++;
			shotx = -1;
			shoty = -1;
			if (bunkers[i] > 4) { bunkers[i] = -1; }
		}

		if (bunkers[i] != -1) {
			drawBitmap(12 + i * 30, 90, bunker[bunkers[i]], TFT_GREEN, 2);
		}
		yield();
	}
}
//----------------------------------------------------------------------------
void SpaceInvaders::saucerappears() {
	saucertimer--;
	if (saucertimer <= 0) {
		saucertimer = 480;
		if (infoshow == 1 && saucers == -1) {
			// mainMusic->pause();
			// removeTrack(mainMusic);
			// addTrack(ufoSound);
			// ufoSound->setSpeed(2);
			// ufoSound->setRepeat(1);
			// ufoSound->play();
			saucers = 0;
			int i = random(2);
			if (i == 0) {
				saucerx = 0;
				saucerdir = 1;
			}
			else {
				saucerx = 146;
				saucerdir = -1;
			}
		}
	}
}
//----------------------------------------------------------------------------
void SpaceInvaders::movesaucer() {
	if (saucers == 0) {
		saucerx = saucerx + saucerdir;
		if (saucerx <= 0 || saucerx >= 146) {
	// removeTrack(ufoSound);
	// mainMusic->resume();
	// addTrack(mainMusic);
			saucers = -1;
		}

		// check collission: player shot & saucer  
		checkl = saucerx;
		checkr = saucerx + 20;
		checkt = 0;
		checkb = 6;
		if (shotx >= checkl && shotx <= checkr && shoty + 2 >= checkt && shoty <= checkb) {
			score += 100;
			saucers = 1;
			shotx = -1;
			shoty = -1;
			saucerwait = 30;
		}
	}
}
//----------------------------------------------------------------------------
void SpaceInvaders::drawsaucer() {
	if (saucers != -1) {
		drawBitmap(saucerx, 0, saucer[saucers], TFT_RED, 2);
		if (saucers == 1) {
			saucerwait--;
			if (saucerwait <= 0) {
		// removeTrack(ufoSound);
		// addTrack(mainMusic);
		// mainMusic->play();
		// mainMusic->setRepeat(1);
				saucers = -1;
			}
		}
	}
}


//----------------------------------------------------------------------------
void SpaceInvaders::eraseDataSetup()
{
	elapsedMillis = millis();
	blinkState = 1;
	clearButtonCallbacks();
	buttons->setBtnPressCallback(BTN_B, [](){
		instance->gamestatus = "dataDisplay";

	});
	buttons->setBtnPressCallback(BTN_A, [](){
		JsonArray empty = instance->jb.to<JsonArray>();
		File file = SPIFFS.open(instance->highscoresPath, "w");
		serializeJson(empty, file);
		file.close();
		instance->gamestatus = "title";

	});
}
void SpaceInvaders::eraseData()
{
	baseSprite->clear(TFT_BLACK);
	baseSprite->setTextFont(2);

	if (millis() - elapsedMillis >= 350) {
		elapsedMillis = millis();
		blinkState = !blinkState;
	}

	baseSprite->setTextColor(TFT_WHITE);
	baseSprite->setCursor(4, 5);
	baseSprite->printCenter("ARE YOU SURE?");
	baseSprite->setCursor(4, 25);
	baseSprite->printCenter("This cannot");
	baseSprite->setCursor(4, 41);
	baseSprite->printCenter("be reverted!");

	if (blinkState){
		baseSprite->drawRect((baseSprite->width() - 60)/2, 102, 30*2, 9*2, TFT_RED);
		baseSprite->setTextColor(TFT_RED);
		baseSprite->setCursor(28*2, 103);
		baseSprite->printCenter("DELETE");
	}
	else {
		baseSprite->fillRect((baseSprite->width() - 60)/2, 102, 30*2, 9*2, TFT_RED);
		baseSprite->setTextColor(TFT_WHITE);
		baseSprite->setCursor(28*2, 103);
		baseSprite->printCenter("DELETE");
	}
}

void SpaceInvaders::dataDisplaySetup()
{
	jb.clear();
	File file = SPIFFS.open(highscoresPath, "r");
	serializeJson(instance->jb, file);
	JsonArray hiscores = jb.to<JsonArray>();
	file.close();
	memset(scoreArray, 0, 6);
	hiscoresSize = hiscores.size();
	for(uint8_t i = 0; i < 6; i++)
	{
		for(JsonObject element:hiscores)
		{
			if(element["Rank"] == i)
			{
				strncpy(nameArray[i], element["Name"], 4);
				scoreArray[i] = element["Score"];
			}
			yield();
		}
	}
	clearButtonCallbacks();
	buttons->setBtnPressCallback(BTN_A, [](){
		instance->gamestatus = "title";
	});
	buttons->setBtnPressCallback(BTN_B, [](){
		instance->gamestatus = "title";
	});
	buttons->setBtnPressCallback(BTN_C, [](){
		instance->gamestatus = "eraseData";
	});
}
void SpaceInvaders::dataDisplay()
{
	baseSprite->clear(TFT_BLACK);
	baseSprite->setCursor(32, -2);
	baseSprite->setTextSize(1);
	baseSprite->setTextFont(2);
	baseSprite->setTextColor(TFT_RED);
	baseSprite->printCenter("HIGHSCORES");
	baseSprite->setCursor(3, 110);
	for (int i = 1; i < 6;i++)
	{
		baseSprite->setCursor(6, i * 20);
		if(i <= hiscoresSize)
			baseSprite->printf("%d.   %.3s    %04d", i, nameArray[i], scoreArray[i]);
		else
			baseSprite->printf("%d.    ---   ----", i);
	}
	baseSprite->setCursor(2, 115);
	baseSprite->print("Erase");
}
void SpaceInvaders::showtitle() {
	baseSprite->clear(TFT_BLACK);
	for(int i = 0; i < STAR_COUNT; i++)
	{
		// Remove the star from the screen by changing its pixel to the background color.
		baseSprite->drawPixel(stars[i].x, stars[i].y, BACKGROUND_COLOR);

		// Update the position of the star.
		stars[i].update();

		// If the star's Y position is off the screen after the update.
		if(stars[i].y >= baseSprite->height())
		{
			// Randomize its position.
			stars[i].randomize(0, baseSprite->width(), 0, baseSprite->height(), STAR_SPEED_MIN, STAR_SPEED_MAX);
			// Set its Y position back to the top.
			stars[i].y = 0;
		}

		// Draw the star at its new coordinate.
		baseSprite->fillRect(stars[i].x, stars[i].y, 2, 2, STAR_COLOR);
	}
	if(millis() - blinkMillis >= 250)
	{
		blinkMillis = millis();
		blinkState = !blinkState;
	}
	baseSprite->setTextColor(TFT_WHITE);
	baseSprite->drawIcon(titleLogo, 4, 10, 60, 18, 2, TFT_BLACK);
	baseSprite->setTextColor(TFT_RED);
	baseSprite->setFreeFont(TT1);
	baseSprite->setTextSize(2);
	baseSprite->setCursor(10, 80);
	baseSprite->printCenter("START");
	baseSprite->setCursor(10, 100);
	baseSprite->printCenter("HIGHSCORES");
	baseSprite->setCursor(10, 120);
	baseSprite->printCenter("QUIT");
	if(blinkState)
	{
		baseSprite->drawRect(15, 67 + cursor * 20, 98, 16, TFT_RED);
		baseSprite->drawRect(14, 66 + cursor * 20, 100, 18, TFT_RED);
	}

	
	
}
void SpaceInvaders::titleSetup()
{
	cursor = 0;
	blinkMillis = millis();
	blinkState = 0;
	clearButtonCallbacks();

	buttons->setBtnPressCallback(BTN_UP, [](){
		if(instance->cursor > 0)
		{
			instance->cursor--;
			instance->blinkMillis = millis();
			instance->blinkState = 1;
		}
	});
	buttons->setBtnPressCallback(BTN_DOWN, [](){
		if(instance->cursor < 2)
		{
			instance->cursor++;
			instance->blinkMillis = millis();
			instance->blinkState = 1;
		}
	});
	
	buttons->setBtnPressCallback(BTN_A, [](){
		
		switch (instance->cursor)
		{
			case 0:
				instance->gamestatus = "newgame";
				break;
			case 1:
				instance->gamestatus = "dataDisplay";
				break;
			case 2:
				instance->pop();
		}
	});
}

void SpaceInvaders::enterInitialsSetup()
{
	name = "AAA";
	charCursor = 0;
	previous = "";
	elapsedMillis = millis();
	hiscoreMillis = millis();
	blinkState = 1;
	hiscoreBlink = 0;
	clearButtonCallbacks();
	buttons->setBtnPressCallback(BTN_UP,[](){
		instance->blinkState = 1;
		instance->elapsedMillis = millis();
		instance->name[instance->charCursor]++;
		// A-Z 0-9 :-? !-/ ' '
		if (instance->name[instance->charCursor] == '0') instance->name[instance->charCursor] = ' ';
		if (instance->name[instance->charCursor] == '!') instance->name[instance->charCursor] = 'A';
		if (instance->name[instance->charCursor] == '[') instance->name[instance->charCursor] = '0';
		if (instance->name[instance->charCursor] == '@') instance->name[instance->charCursor] = '!';
	});
	buttons->setButtonHeldRepeatCallback(BTN_UP, 200, [](uint){
		instance->blinkState = 1;
		instance->elapsedMillis = millis();
		instance->name[instance->charCursor]++;
		// A-Z 0-9 :-? !-/ ' '
		if (instance->name[instance->charCursor] == '0') instance->name[instance->charCursor] = ' ';
		if (instance->name[instance->charCursor] == '!') instance->name[instance->charCursor] = 'A';
		if (instance->name[instance->charCursor] == '[') instance->name[instance->charCursor] = '0';
		if (instance->name[instance->charCursor] == '@') instance->name[instance->charCursor] = '!';
	});
	buttons->setButtonHeldRepeatCallback(BTN_DOWN, 200, [](uint){
		instance->blinkState = 1;
		instance->elapsedMillis = millis();
		instance->name[instance->charCursor]--;
		if (instance->name[instance->charCursor] == ' ') instance->name[instance->charCursor] = '?';
		if (instance->name[instance->charCursor] == '/') instance->name[instance->charCursor] = 'Z';
		if (instance->name[instance->charCursor] == 31)  instance->name[instance->charCursor] = '/';
		if (instance->name[instance->charCursor] == '@') instance->name[instance->charCursor] = ' ';
	});
	buttons->setBtnPressCallback(BTN_DOWN, [](){
		instance->blinkState = 1;
		instance->elapsedMillis = millis();
		instance->name[instance->charCursor]--;
		if (instance->name[instance->charCursor] == ' ') instance->name[instance->charCursor] = '?';
		if (instance->name[instance->charCursor] == '/') instance->name[instance->charCursor] = 'Z';
		if (instance->name[instance->charCursor] == 31)  instance->name[instance->charCursor] = '/';
		if (instance->name[instance->charCursor] == '@') instance->name[instance->charCursor] = ' ';
	});
	buttons->setBtnPressCallback(BTN_LEFT, [](){
		if(instance->charCursor > 0){
			instance->charCursor--;
			instance->blinkState = 1;
			instance->elapsedMillis = millis();
		}
	});
	buttons->setBtnPressCallback(BTN_RIGHT, [](){
		if(instance->charCursor < 2){
			instance->charCursor++;
			instance->blinkState = 1;
			instance->elapsedMillis = millis();
		}
	});
	buttons->setBtnPressCallback(BTN_A, [](){
		instance->charCursor++;
		instance->blinkState = 1;
		instance->elapsedMillis = millis();
	});
}
void SpaceInvaders::enterInitials() {
  
    if (millis() - elapsedMillis >= 350) //cursor blinking routine
	{
		elapsedMillis = millis();
		blinkState = !blinkState;
	}
    if(millis()-hiscoreMillis >= 1000)
    {
      hiscoreMillis = millis();
      hiscoreBlink = !hiscoreBlink;
    }
    previous = name;

    if (previous != name)
    {
      blinkState = 1;
      elapsedMillis = millis();
    }
	
	baseSprite->clear(TFT_BLACK);
    baseSprite->setCursor(16, 8);
    baseSprite->setTextFont(2);
    baseSprite->setTextColor(TFT_WHITE);
    baseSprite->setTextSize(1);
    baseSprite->printCenter("ENTER NAME");
    baseSprite->setCursor(20, 80);
	
    if(hiscoreBlink && score > tempScore)
      baseSprite->printCenter("NEW HIGH!");
    else
      baseSprite->printf("SCORE: %04d", score);

    baseSprite->setCursor(40, 40);
    baseSprite->print(name[0]);
	baseSprite->setCursor(55, 40);
    baseSprite->print(name[1]);
	baseSprite->setCursor(70, 40);
    baseSprite->print(name[2]);
    // baseSprite->drawRect(30, 38, 100, 20, TFT_WHITE);
	if(blinkState){
		baseSprite->drawFastHLine(38 + 15*charCursor, 56, 12, TFT_WHITE);
	}


	if(charCursor >= 3)
	{
		File file = SPIFFS.open(highscoresPath, "r");
		jb.clear();
		deserializeJson(jb, file);
		JsonArray hiscores2 = jb.as<JsonArray>();
		file.close();
		DynamicJsonDocument doc = DynamicJsonDocument(1024);
		JsonObject newHiscore = doc.to<JsonObject>();
		newHiscore["Name"] = name;
		newHiscore["Score"] = score;
		newHiscore["Rank"] = 1;

		if(savePresent && hiscores2.size() > 0)
		{
			newHiscore["Rank"] = 999;
			// Serial.println(hiscores2.size());
			uint16_t tempSize = hiscores2.size();
			for (int16_t i = 0; i < tempSize;i++)//searching for a place in the leaderboard for our new score
			{
				Serial.printf("i: %d\n", i);
				Serial.println((uint16_t)(hiscores2[i]["Rank"]));
				Serial.println((uint16_t)(hiscores2[i]["Score"]));
				delay(5);
				if(score >= (uint16_t)(hiscores2[i]["Score"]))
				{
					Serial.println("ENTERED");
					delay(5);
					if((uint16_t)(newHiscore["Rank"]) >  (uint16_t)(hiscores2[i]["Rank"]))
					{
						newHiscore["Rank"] = (uint16_t)(hiscores2[i]["Rank"]);
					}
					DynamicJsonDocument docTemp = DynamicJsonDocument(1024);
					JsonObject tempObject = docTemp.as<JsonObject>();
					tempObject["Name"] = (const char *)(hiscores2[i]["Name"]);
					tempObject["Score"] = (uint16_t)(hiscores2[i]["Score"]);
					tempObject["Rank"] = (uint16_t)(hiscores2[i]["Rank"]) + 1;
					serializeJsonPretty(tempObject, Serial);
					// delay(5);
					hiscores2.remove(i);
					hiscores2.add(tempObject);
					tempSize--;
					i--;
				}
				else
				{
					if(newHiscore["Rank"] <= (uint16_t)(hiscores2[i]["Rank"]) || newHiscore["Rank"] == 999)
						newHiscore["Rank"] = (uint16_t)(hiscores2[i]["Rank"]) + 1;
				}
			}
		}
		hiscores2.add(newHiscore);
		doc.clear();
		file = SPIFFS.open(highscoresPath, "w");
		serializeJson(hiscores2, file);
		file.close();
		gamestatus = "dataDisplay";
	}
}
