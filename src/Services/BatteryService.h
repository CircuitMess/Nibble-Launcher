#ifndef BATTERYSERVICE_NIBBLE_H
#define BATTERYSERVICE_NIBBLE_H

#include <Loop/LoopListener.h>
#include <Support/Context.h>
#include <Display/Display.h>
#include <UI/Image.h>
extern Context* runningContext;

class BatteryService : public LoopListener
{
public:
	BatteryService(Display& display);
	void loop(uint _time) override;
	uint getVoltage();
	bool modalShown();
	static BatteryService* getInstance();
	void draw();
	void setModalCallback(void (* modalCallback)());
	void setModalDismissCallback(void (* modalDismissCallback)());

private:
	Display* display;
	Sprite *canvas;
	double averageVoltage;
	uint voltageSum;
	const uint measurementsSize = 100;
	uint measurementCounter;

	void bindInput();
	void drawWarning();
	bool showWarning = 0;
	const uint8_t warningWidth = 120;
	const uint8_t warningHeight = 90;
	bool warningShown = 0;

	void drawShutdown();
	bool showShutdown = 0;
	uint shutdownTime = 0;

	static BatteryService* instance;

	void(*modalCallback)() = nullptr;
	void(*modalDismissCallback)() = nullptr;

};

#endif