#include "Logo.h"

#include <Display/Sprite.h>

Logo::Logo(Sprite* canvas) : canvas(canvas), x((canvas->width() - width) / 2){

}

void Logo::draw(){
	double m = micros() / 1000.0;
	canvas->fillRect(x, y + cos(m / speed) * amplitude, width, height, TFT_GREEN);
}
