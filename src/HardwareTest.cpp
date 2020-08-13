#include "HardwareTest.h"
#include <Audio/Piezo.h>
#include "SettingsMenu/SettingsStruct.hpp"
HardwareTest::HardwareTest(Display &display) : canvas(display.getBaseSprite()), display(&display)
{
}
void HardwareTest::start()
{
	Serial.println("START");
	canvas->clear(TFT_WHITE);
	canvas->setTextColor(TFT_BLACK);
	canvas->setTextFont(2);
	canvas->setTextSize(1);
	canvas->setCursor(0,0);
	canvas->printCenter("TESTING MODE");
	canvas->setCursor(3, 20);
	canvas->print("Buttons: ");
	canvas->setTextColor(TFT_GREEN);
	canvas->print("OK");
	canvas->setTextColor(TFT_BLACK);
	canvas->setCursor(3, 38);
	canvas->print("Voltage: testing");
	display->commit();

	voltageTest();
	settings()->calibrated = 1;
	Settings::store();
	Serial.println("END");
	Serial.println("OK");
	beeping();

}
void HardwareTest::voltageTest()
{
	bool measureOK = 1;
	for(measurementCounter = 0; measurementCounter <= measurementsSize; measurementCounter++)
	{
		voltageSum+=analogRead(A0);
	}
	averageVoltage = (voltageSum/measurementsSize) / 1024.0 * 1000.0;
	if(averageVoltage > 700.0 || averageVoltage < 600.0)
	{
		measureOK = 0;
	}
	else
	{
		measureOK = 1;
	}
	canvas->clear(TFT_WHITE);
	canvas->setTextColor(TFT_BLACK);
	canvas->setTextFont(2);
	canvas->setTextSize(1);
	canvas->setCursor(0,0);
	canvas->printCenter("TESTING MODE");
	canvas->setCursor(3, 20);
	canvas->print("Buttons: ");
	canvas->setTextColor(TFT_GREEN);
	canvas->print("OK");
	canvas->setTextColor(TFT_BLACK);
	canvas->setCursor(3, 38);
	canvas->print("Voltage: ");
	if(measureOK)
	{
		canvas->setTextColor(TFT_GREEN);
		canvas->print("OK");
	}
	else
	{
		canvas->setTextColor(TFT_RED);
		canvas->print("ERROR");
		canvas->setCursor(3, 90);
		canvas->setTextColor(TFT_RED);
		canvas->printCenter("ADC ERROR!");
		canvas->setCursor(3, 108);
		canvas->printCenter((float)averageVoltage);
		canvas->setTextColor(TFT_BLACK);
	}
	canvas->setTextColor(TFT_BLACK);
	display->commit();
	if(!measureOK)
	{
		Serial.println("END");
		Serial.println("ERR");
		while(1)
		{
			yield();
		}
	}
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

		canvas->clear(TFT_WHITE);
		canvas->setTextColor(TFT_BLACK);
		canvas->setTextFont(2);
		canvas->setTextSize(1);
		canvas->setCursor(0,0);
		canvas->printCenter("TESTING MODE");
		canvas->setCursor(3, 20);
		canvas->print("Buttons: ");
		canvas->setTextColor(TFT_GREEN);
		canvas->print("OK");
		canvas->setTextColor(TFT_BLACK);
		canvas->setCursor(3, 38);
		canvas->print("Voltage: ");
		canvas->setTextColor(TFT_GREEN);
		canvas->print("OK");
		canvas->setTextColor(TFT_BLACK);
		canvas->setCursor(3, 56);
		canvas->print("Buzzer: ");
		canvas->setTextColor(TFT_GREEN);
		canvas->print("beeping!");
		canvas->setTextColor(TFT_BLACK);

		if(blinkState)
		{
			Piezo.tone(440, 50);
			canvas->setCursor(3, 90);
			canvas->setTextColor(TFT_GREEN);
			canvas->printCenter("TEST OK!");
			canvas->setTextColor(TFT_BLACK);
		}
		display->commit();
		yield();
		
	}
}