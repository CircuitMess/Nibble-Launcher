#include "Logo.h"

#include <Display/Sprite.h>

#include "../Bitmaps/logo.hpp"

Logo::Logo(Sprite* canvas) : canvas(canvas), x((canvas->width() - width) / 2), currentY(startY){

}

void Logo::loop(uint micros){
	f += (float) micros / 1000000.0f;
	if(f > 2 * M_PI) f -= 2 * M_PI;

	currentY = y + pow(cos(f * 1000.0f / speed), 2) * -amplitude;
}

void Logo::splash(float f){
	currentY = f * (float) (y - startY) + startY;
}

void Logo::draw(){
	canvas->drawIcon(logo, x, currentY, width, height);
}
