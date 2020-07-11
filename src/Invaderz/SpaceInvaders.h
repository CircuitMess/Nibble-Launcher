#ifndef SPACEINVADERS_H
#define SPACEINVADERS_H
#include <Arduino.h>
#include <CircuitOS.h>
#include <Input/I2cExpander.h>
#include <Input/InputI2C.h>
#include <Update/UpdateManager.h>
#include "Star.h"
#include <ArduinoJson.h>
#include <spiffs_api.h>
#include <gpio.h>
#include <avr/pgmspace.h>
#include <Support/Context.h>
#include "../ByteBoy.hpp"
#include <UI/Image.h>

constexpr int STAR_COUNT = 30;            // Number of stars on the screen. Arduino UNO maxes out around 250.
constexpr int BACKGROUND_COLOR = 0x0000;   // Background color in hex. 0x0000 is black.
constexpr int STAR_SPEED_MIN = 1;          // Minimum movement in pixels per update. (value is inclusive)
constexpr int STAR_SPEED_MAX = 2;         // Maximum movement in pixels per update. (value is inclusive)
constexpr int STAR_COLOR = 0xffff;  

class SpaceInvaders: public Game, public UpdateListener{
public:
	SpaceInvaders(Display& display);
	void draw() override;
	void start() override;
	void update(uint) override;
	void stop() override;
private:
	Sprite *baseSprite;
	Image baseImage;
	Input *buttons;
	StaticJsonDocument<8000> jb;
	String gamestatus;
	int score;
	int lives;
	int gamelevel;
	int shipx;
	int invaderx[55];
	int invadery[55];
	int invaders[55];
	int invaderframe[55];
	int invaderanz;
	int invaderctr;
	int invadersound;
	int checkdir;
	int nextxdir;
	int nextydir;
	int invaderxr;
	int invaderyr;
	int invadershotx[4];
	int invadershoty[4];
	bool invadershotframe = 0;
	bool pastInvaderShotFrame = 0;
	int invadershots;
	int invadershottimer;
	int bunkers[4];
	int shotx;
	int shoty;
	int checkl;
	int checkr;
	int checkt;
	int checkb;
	int yeahtimer;
	int infoshow;
	int deadcounter;
	int saucerx;
	int saucerdir;
	int saucers;
	int saucertimer;
	int saucerwait;
	int delayBip;
	uint32_t blinkMillis = millis();
	bool blinkState = 0;
	unsigned long elapsedMillis = millis();
	uint16_t hiscoresSize = 0;
	char nameArray[6][4];
	uint16_t scoreArray[6];
	uint8_t charCursor = 0;
	String previous = "";
	uint32_t hiscoreMillis = millis();
	bool hiscoreBlink = 0;
	char *highscoresPath;
	bool savePresent = 0;
	uint16_t tempScore = 0;
	String name = "";
	uint8_t cursor = 0;
	char key = NO_KEY;
	Star stars[STAR_COUNT];
	String prevGamestatus = gamestatus;
	bool screenChange = 0;
	static SpaceInvaders* instance;

	void starsSetup();
	void drawBitmap(int16_t x, int16_t y, const byte *bitmap, uint16_t color, uint8_t scale);
	void newgame();
	void newlevel();
	void showscore();
	void nextlevelcheck();
	void handledeath();
	void clearButtonCallbacks();
	void setButtonsCallbacks();
	void drawplayership();
	void drawplayershot();
	void invaderlogic();
	void drawinvaders();
	void drawInvaderShot();
	void drawbunkers();
	void saucerappears();
	void movesaucer();
	void drawsaucer();
	void eraseDataSetup();
	void eraseData();
	void dataDisplaySetup();
	void dataDisplay();
	void showtitle();
	void titleSetup();
	void enterInitialsSetup();
	void enterInitials();

};

#endif