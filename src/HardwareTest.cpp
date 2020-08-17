#include "HardwareTest.h"
#include <Audio/Piezo.h>
#include "SettingsMenu/SettingsStruct.hpp"

HardwareTest* HardwareTest::test = nullptr;

HardwareTest::HardwareTest(Display &_display) : canvas(_display.getBaseSprite()), display(&_display)
{
	test = this;

	tests.push_back({ HardwareTest::buttonsTest, "Buttons" });
	tests.push_back({ HardwareTest::voltageTest, "Voltage" });
}

void HardwareTest::start()
{
	Serial.println();
	Serial.printf("TEST:begin:%x\n", ESP.getChipId());

	canvas->clear(TFT_WHITE);
	canvas->setTextColor(TFT_BLACK);
	canvas->setTextFont(2);
	canvas->setTextSize(1);
	canvas->setCursor(0, 0);
	canvas->printCenter("TESTING MODE");
	canvas->setCursor(0, 10);
	canvas->println();
	display->commit();

	bool pass = true;
	for(const Test& test : tests){
		currentTest = test.name;

		canvas->setTextColor(TFT_BLACK);
		canvas->printf("%s: ", test.name);
		display->commit();

		bool result = test.test();

		canvas->setTextColor(result ? TFT_GREEN : TFT_RED);
		canvas->printf("%s\n", result ? "PASS" : "FAIL");
		display->commit();

		if(!(pass &= result)) break;
	}

	settings()->calibrated = pass;
	Settings::store();

	if(pass){
		Serial.println("TEST:pass");
		beeping();
	}else{
		Serial.printf("TEST:fail:%s\n", currentTest);

		while(true){
			delay(1000);
		}
	}
}


bool HardwareTest::buttonsTest(){
	return true;
}

bool HardwareTest::voltageTest()
{
	const uint measurementCount = 100;

	uint readingSum = 0;
	for(uint i = 0; i < measurementCount; i++)
	{
		readingSum += analogRead(A0);
	}

	float averageVoltage = (readingSum / measurementCount) / 1024.0 * 1000.0;
	//averageVoltage = map((uint) averageVoltage, 0, 1000, 0, 5444);
	averageVoltage = (averageVoltage / 1000.0) * 5444.0;

	// avgReading < 700.0 && avgReading > 600.0;
	bool measureOK = averageVoltage < 3810.8 && averageVoltage > 3266.4;

	uint diff = 3600 - averageVoltage;
	test->log("voltage", averageVoltage);

	return measureOK;
}

void HardwareTest::beeping()
{
	uint32_t blinkMillis = millis();
	bool blinkState = 0;
	while(1)
	{
		if(millis()-blinkMillis >= 400)
		{
			blinkMillis = millis();
			blinkState = !blinkState;
		}

		if(blinkState)
		{
			Piezo.tone(440, 50);
			canvas->setCursor(3, 90);
			canvas->setTextColor(TFT_GREEN);
			canvas->printCenter("TEST OK!");
			canvas->setTextColor(TFT_BLACK);
		}else{
			canvas->fillRect(0, 85, 128, 20, TFT_WHITE);
		}
		display->commit();
		yield();

	}
}

void HardwareTest::log(const char* property, char* value){
	Serial.printf("%s:%s:%s\n", currentTest, property, value);
}

void HardwareTest::log(const char* property, float value){
	Serial.printf("%s:%s:%f\n", currentTest, property, value);
}

void HardwareTest::log(const char* property, double value){
	Serial.printf("%s:%s:%lf\n", currentTest, property, value);
}

void HardwareTest::log(const char* property, bool value){
	Serial.printf("%s:%s:%d\n", currentTest, property, value ? 1 : 0);
}
