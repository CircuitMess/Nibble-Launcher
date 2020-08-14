#include "Splash.h"
#include "Elements/Logo.h"
#include "Elements/GameTitle.h"
#include "GameScroller.h"
#include <Audio/Piezo.h>

struct Tone {
	uint freq;
	uint duration;
};

/*Vector<Tone> tones = {
		Tone { 392, 100 },
		Tone { 440, 100 },
		Tone { 493, 100 },
		Tone { 587, 200 }
};*/

Vector<Tone> tones = {
		Tone { 466, 120 },
		Tone { 523, 120 },
		Tone { 587, 120 },
		Tone { 659, 220 }
};

int tonePlaying = -1;
uint toneTotal;

Splash::Splash(Sprite* canvas, Logo* logo, GameTitle* title, GameScroller* scroller) : canvas(canvas), logo(logo), title(title), scroller(scroller){

}

void Splash::update(uint micros){
	if(f == 0){
		f += 0.001;
		return;
	}

	f = min(1.0, f + micros / 1000000.0);

	int i = 0;
	for(const Tone& tone : tones){
		if((f * 1000 > toneTotal) && tonePlaying < i){
			toneTotal += tone.duration;
			tonePlaying = i;
			Piezo.tone(tone.freq, tone.duration);
		}

		i++;
	}

	if(f >= hold){
		float fSecondHalf = f * 2.0 - 1.0;
		logo->splash(fSecondHalf);
		scroller->splash(fSecondHalf);
	}
}

bool Splash::done(){
	return f >= 1;
}
