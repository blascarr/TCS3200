#define SERIAL_DEBUG true
#include "ColorMenu.h"
#include <TCS3200Neurona.h>

#include "Neurons/mlp_3x2x10.h"
#define TCS3200_S2 2
#define TCS3200_S3 3
#define TCS3200_LED 4
#define TCS3200_OUT 5
#define TCS3200_S0 6
#define TCS3200_S1 7

char *colors[SIZECOLORS] = {"WHITE", "BLACK", "YELLOW", "ORANGE",
							"RED",	 "GREEN", "BLUE",	"BROWN"};

NeuronCT CT(mlp, colors);
TCS3200Core CSBase(TCS3200_S2, TCS3200_S3, TCS3200_OUT, TCS3200_S0, TCS3200_S1,
				   TCS3200_LED, 0);
TCS3200Neurona CS(CSBase, CT);

void globalTickerCallback() {
	if (CS.onChangeColor()) {
		Serial.println(CS.readColor());
		Serial.println(CS.readLastColorID());
	}
}

Ticker timer(globalTickerCallback, 1000, 0, MILLIS);

#include "MenuEvents.h"

void setup() {
	Serial.begin(9600);
	CS.setTicker(timer);
	CS.begin();
	CS.setFrequency(0);
	CS.nSamples(40);
	CS.setRefreshTime(500);
	CS.voidBW(); // Load default Factory Black & White calibration
}

void loop() { nav.poll(); }