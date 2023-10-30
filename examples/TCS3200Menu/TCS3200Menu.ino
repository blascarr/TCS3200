#define SERIAL_DEBUG
#include "ColorMenu.h"
#include "TCS3200Debug.h"
#include <TCS3200.h>

#define TCS3200_S2 2
#define TCS3200_S3 3
#define TCS3200_LED 4
#define TCS3200_OUT 5
#define TCS3200_S0 6
#define TCS3200_S1 7

TCS3200 CS(TCS3200_S2, TCS3200_S3, TCS3200_OUT, TCS3200_S0, TCS3200_S1,
		   TCS3200_LED, 0);
SerialDebugger SerialDebug = SerialDebugger();
TCS3200Debug CSDebug(CS, SerialDebug);

void globalTickerCallback() {
	if (CS.onChangeColor()) {
		Serial.println(CS.readColor());
		Serial.println(CS.readColorID());
	} else {
		CSDebug.read();
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
	// CS.loadBW();
	// CS.loadCT();
}

void loop() { nav.poll(); }
