#ifndef NIBBLE_SERIALID_H
#define NIBBLE_SERIALID_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Loop/LoopListener.h>

class SerialID : public LoopListener {
public:
	void loop(uint micros) override;

private:
	char last = 0;
};


#endif //NIBBLE_SERIALID_H
