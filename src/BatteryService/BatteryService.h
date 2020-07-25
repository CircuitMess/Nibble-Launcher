#ifndef BATTERYSERVICE_NIBBLE_H
#define BATTERYSERVICE_NIBBLE_H

#include <Update/UpdateListener.h>
#include <Support/Context.h>
#include <Display/Display.h>
#include <UI/Image.h>
extern Context* runningContext;

class BatteryService : public UpdateListener
{
public:
	BatteryService(Display& display);
	void update(uint _time) override;
	uint getVoltage();
private:
	Display* display;
	Sprite *canvas;
	double averageVoltage;
	uint voltageSum;
	const uint measurementsSize = 100;
	uint measurementCounter;

	void drawWarning();
	bool showWarning = 0;
	const uint8_t warningWidth = 120;
	const uint8_t warningHeight = 70;

	static BatteryService* instance;
};

#endif