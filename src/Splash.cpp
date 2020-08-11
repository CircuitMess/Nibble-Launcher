#include "Splash.h"
#include "Elements/Logo.h"
#include "Elements/GameTitle.h"
#include "GameScroller.h"

Splash::Splash(Sprite* canvas, Logo* logo, GameTitle* title, GameScroller* scroller) : canvas(canvas), logo(logo), title(title), scroller(scroller){

}

void Splash::update(uint micros){
	if(f == 0){
		f += 0.001;
		return;
	}

	f = min(1.0, f + micros / 1000000.0);

	if(f >= hold){
		float fSecondHalf = f * 2.0 - 1.0;
		logo->splash(fSecondHalf);
		scroller->splash(fSecondHalf);
	}
}

bool Splash::done(){
	return f >= 1;
}
