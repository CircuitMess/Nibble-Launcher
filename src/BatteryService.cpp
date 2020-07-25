#include "BatteryService.h"
BatteryService::BatteryService()
{
	averageVoltage = 4500.0;
	voltageSum = 0;
	measurementCounter = 0;
}
void BatteryService::update(uint _time)
{
	if (measurementCounter > measurementsSize)
	{
		averageVoltage = voltageSum/measurementsSize;
		voltageSum = 0;
		measurementCounter = 0;
	}
	measurementCounter++;
	voltageSum+=analogRead(A0);
}