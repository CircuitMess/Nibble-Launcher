#include "Logo.h"

#include <Display/Sprite.h>

#include "../Bitmaps/logo.hpp"

Logo::Logo(Sprite* canvas) : canvas(canvas), x((canvas->width() - width) / 2){

}

void Logo::draw(){
	double m = micros() / 1000.0;
	canvas->drawIcon(logo, x, y + pow(cos(m / speed), 2) * amplitude, width, height);
}
