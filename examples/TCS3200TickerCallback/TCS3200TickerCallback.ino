#include <TCS3200.h>

#define TCS3200_S2 2
#define TCS3200_S3 3
#define TCS3200_LED 4
#define TCS3200_OUT 5
#define TCS3200_S0 6
#define TCS3200_S1 7

TCS3200 CS(TCS3200_S2, TCS3200_S3, TCS3200_OUT, TCS3200_S0, TCS3200_S1,
		   TCS3200_LED, 0);

void colorChanged(int color) {
	if (CS.onChangeColor()) {
		if (CS.onChangeCallback)
			CS.onChangeCallback(CS.readLastColorID());
	}
}

void colorCallback(int color) {
	Serial.print("Color cambiado a: ");
	Serial.println(CS._ct[color].name);
}

Ticker CSTicker(colorChanged, CS.refreshTime, 0, MILLIS);

void setup() {
	Serial.begin(9600);
	CS.setTicker(CSTicker);
	CS.begin();

	CS.setFrequency(0);
	CS.nSamples(40);
	CS.setRefreshTime(2000);
	CS.voidBW(); // Load default Factory Black & White calibration
	CS.voidCT(); // Load default Factory Color Table calibration

	// CS.loadBW(); // Load EEPROM Black & White calibration
	// CS.loadCT(); // Load EEPROM Color Table calibration
	CS.setOnChangeCallback(colorCallback);
}

void loop() { CS.update(); }
