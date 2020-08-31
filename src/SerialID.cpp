#include "SerialID.h"
#include "SettingsMenu/SettingsStruct.hpp"

void SerialID::loop(uint micros){
	if(Serial.available()){
		char now = 0;
		Serial.read(&now, 1);

		if(last == 'I' && now == 'D'){
			Serial.printf("Nibble:%d:%x\n", settings()->calibrated, ESP.getChipId());
		}

		last = now;
	}
}
