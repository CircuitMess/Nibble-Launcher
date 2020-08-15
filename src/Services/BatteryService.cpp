#include "BatteryService.h"
#include <Input/Input.h>
#include <Input/I2cExpander.h>
#include <Nibble.h>

BatteryService* BatteryService::instance = nullptr;

BatteryService::BatteryService(Display& display) : display(&display), canvas(display.getBaseSprite())
{
	averageVoltage = (float)(analogRead(A0)) / 1024.0 * 1000.0;
	voltageSum = 0;
	measurementCounter = 0;
	showWarning = 0;
	instance = this;
	warningShown = 0;
	showShutdown = 0;
	if(averageVoltage < 650.0 && !showShutdown)
	{
		measurementCounter = measurementsSize + 1;
		voltageSum = (averageVoltage*1024.0/1000.0) * measurementCounter;
	}
}
void BatteryService::update(uint _time)
{
	if(showShutdown)
	{
		shutdownTime+=_time;
		if(shutdownTime >= 5000000)
		{
			// digitalWrite(BL_PIN, 0);
			display->getTft()->writecommand(16);
			I2cExpander::getInstance()->pinWrite(BL_PIN, 0);
			I2cExpander::getInstance()->portConfig(0xFFFF);
			ESP.deepSleep(0);
		}
		if(runningContext != nullptr){
			runningContext->draw();
		}
		drawShutdown();
		display->commit();
	}
	else if(showWarning){
		if(runningContext != nullptr){
			runningContext->draw();
		}
		drawWarning();
		display->commit();
	}
	if (measurementCounter > measurementsSize)
	{
		averageVoltage = (voltageSum/measurementsSize) / 1024.0 * 1000.0;
		voltageSum = 0;
		measurementCounter = 0;

		if(averageVoltage < 650.0 && !showShutdown)
		{
			if(runningContext != nullptr){
				runningContext->stop();
			}
			showShutdown = 1;
			shutdownTime = 0;
		}
		else if(averageVoltage < 680.0 && !warningShown)
		{
			if(runningContext != nullptr){
				runningContext->stop();
			}
			showWarning = 1;
			warningShown = 1;
			Input::getInstance()->setBtnPressCallback(BTN_A, [](){
				instance->showWarning = 0;
				Input::getInstance()->removeBtnPressCallback(BTN_A);
				Input::getInstance()->removeBtnPressCallback(BTN_B);

				if(runningContext != nullptr){
					runningContext->start();
				}
			});
			Input::getInstance()->setBtnPressCallback(BTN_B, [](){
				instance->showWarning = 0;
				Input::getInstance()->removeBtnPressCallback(BTN_A);
				Input::getInstance()->removeBtnPressCallback(BTN_B);

				if(runningContext != nullptr){
					runningContext->start();
				}
			});
		}
	}
	measurementCounter++;
	voltageSum+=analogRead(A0);
}
void BatteryService::drawWarning()
{
	uint8_t xOffset = (canvas->width() - warningWidth) / 2;
	uint8_t yOffset = (canvas->height() - warningHeight) / 2;
	canvas->fillRect(xOffset, yOffset, warningWidth, warningHeight, TFT_WHITE);
	canvas->drawRect(xOffset, yOffset, warningWidth, warningHeight, TFT_BLACK);
	canvas->drawRect(xOffset + 1, yOffset + 1, warningWidth - 2, warningHeight - 2, TFT_BLACK);
	canvas->setCursor(xOffset + 1, yOffset + 3);
	canvas->setTextFont(2);
	canvas->setTextSize(1);
	canvas->setTextColor(TFT_BLACK);
	canvas->printCenter("WARNING!");

	canvas->drawFastHLine(xOffset + 18, yOffset + 18, warningWidth - 2*18, TFT_RED);
	canvas->drawFastHLine(xOffset + 18, yOffset + 19, warningWidth - 2*18, TFT_RED);

	canvas->setCursor(xOffset + 20, yOffset + 19);
	canvas->printCenter("Low battery!");
	canvas->setCursor(xOffset + 20, yOffset + 36);
	canvas->printCenter("Please replace");
	canvas->setCursor(xOffset + 20, yOffset + 51);
	canvas->printCenter("batteries.");
}
void BatteryService::drawShutdown()
{
	uint8_t xOffset = (canvas->width() - warningWidth) / 2;
	uint8_t yOffset = (canvas->height() - warningHeight) / 2;
	canvas->fillRect(xOffset, yOffset, warningWidth, warningHeight, TFT_WHITE);
	canvas->drawRect(xOffset, yOffset, warningWidth, warningHeight, TFT_BLACK);
	canvas->drawRect(xOffset + 1, yOffset + 1, warningWidth - 2, warningHeight - 2, TFT_BLACK);
	canvas->setCursor(xOffset + 1, yOffset + 3);
	canvas->setTextFont(2);
	canvas->setTextSize(1);
	canvas->setTextColor(TFT_BLACK);
	canvas->printCenter("WARNING!");

	canvas->drawFastHLine(xOffset + 18, yOffset + 18, warningWidth - 2*18, TFT_RED);
	canvas->drawFastHLine(xOffset + 18, yOffset + 19, warningWidth - 2*18, TFT_RED);

	canvas->setCursor(xOffset + 20, yOffset + 22);
	canvas->printCenter("Battery too low!");
	canvas->setCursor(xOffset + 20, yOffset + 40);
	canvas->printCenter("Shutting down...");
}
uint BatteryService::getVoltage()
{
	return roundl(averageVoltage);
}