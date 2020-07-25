#ifndef BATTERYSERVICE_NIBBLE_H
#define BATTERYSERVICE_NIBBLE_H

#include <Update/UpdateListener.h>

class BatteryService : public UpdateListener
{
public:
	BatteryService();
	void update(uint _time) override;
private:
	double averageVoltage;
	uint voltageSum;
	const uint measurementsSize = 100;
	uint measurementCounter;

};

#endif