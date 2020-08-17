#ifndef NIBBLE_SERIALID_H
#define NIBBLE_SERIALID_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Update/UpdateListener.h>

class SerialID : public UpdateListener {
public:
	void update(uint micros) override;

private:
	char last = 0;
};


#endif //NIBBLE_SERIALID_H
