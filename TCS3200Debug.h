/*
  Design and created by Blascarr
  DebugInterface
  Name    : Blascarr
  Description: TCS3200Debug.h
  version : 1.0
		TCS3200Debug is an implementation to debug funcionality for TCS3200
  color sensor.

		Blascarr invests time and resources providing this open source code like
  some other libraries, please respect the job and support open-source software.

		Written by Adrian for Blascarr
*/

#ifndef _TCS3200DEBUG_H
#define _TCS3200DEBUG_H

#include "DebugInterface/DebugInterface.h"
#include "TCS3200.h"

const char COLORMESSAGE[] PROGMEM = "\nColor";
const char REDCOLORMESSAGE[] PROGMEM = " Red ";
const char GREENCOLORMESSAGE[] PROGMEM = " Green ";
const char BLUECOLORMESSAGE[] PROGMEM = " Blue ";
const char BLACKCOLORMESSAGE[] PROGMEM = " Black ";
const char WHITECOLORMESSAGE[] PROGMEM = " WHITE ";
const char SEPARATOR[] PROGMEM = " - ";
const char RGBMODEMESSAGE[] PROGMEM = "RGB MODE ";
const char RAWMODEMESSAGE[] PROGMEM = "RAW MODE ";
const char SAVERAWMESSAGE[] PROGMEM = "SAVE RAW ";

class TCS3200Debug {
  private:
	TCS3200 &cs;
	SerialDebugger &debugger;

  public:
	TCS3200Debug(TCS3200 &tcs3200, SerialDebugger &dbg)
		: cs(tcs3200), debugger(dbg) {}

	String readColor() {
		String colorValue = cs.readColor();
		debugger.dumpln(COLORMESSAGE, colorValue);
		return colorValue;
	};

	void read() {
		if (cs.getRGBMode()) {
			debugger.prints(RGBMODEMESSAGE);
			readRGB();
		} else {
			debugger.prints(RAWMODEMESSAGE);
			readRAW();
		}
	}

	sensorData color() {
		sensorData data = cs.color();
		debug(data);
		return data;
	}

	colorData readRGB() {
		colorData data = cs.readRGB();
		debug(cs._rgb);
		return data;
	}

	sensorData readRAW() {
		sensorData data = cs.readRAW();
		debug(cs._raw);
		return data;
	}

	sensorData relativeColor() {
		if (cs.getRGBMode()) {
			debugger.prints(RGBMODEMESSAGE);
		} else {
			debugger.prints(RAWMODEMESSAGE);
		}
		sensorData data = cs.relativeColor();
		debug(cs._relraw);
		return data;
	}

	void readCT() {
		debugger.printsln(SEPARATOR);
		for (int i = 0; i < SIZECOLORS; ++i) {
			debugger.dump(COLORMESSAGE, cs._ct[i].name);
			for (uint8_t j = 0; j < 3; j++) {
				debugger.dump(SEPARATOR, cs._ct[i].rgb.value[j]);
			}
			debugger.printsln(SEPARATOR);
		}
	}

	void saveCT() {
		cs.saveCT();
		readCT();
	}

	void loadCT() {
		cs.loadCT();
		readCT();
	}

	void setDarkCal() {
		sensorData data = cs.setDarkCal(true);
		debugger.prints(SAVERAWMESSAGE);
		debug(data);
	}
	void setWhiteCal() {
		sensorData data = cs.setWhiteCal(true);
		debugger.prints(SAVERAWMESSAGE);
		debug(data);
	}

	void getDarkCal() {
		sensorData data = cs.getDarkCal();
		debug(data);
	}
	void getWhiteCal() {
		sensorData data = cs.getWhiteCal();
		debug(data);
	}

	void loadBW() {
		cs.loadBW();
		debugger.prints(BLACKCOLORMESSAGE);
		getDarkCal();
		debugger.prints(WHITECOLORMESSAGE);
		getWhiteCal();
	}

	void setColorCal(uint8_t colorID) {
		colorData data = cs.setColorCal(colorID);
		debugger.prints(RGBMODEMESSAGE);
		debugger.dump(REDCOLORMESSAGE, data.value[0]);
		debugger.dump(GREENCOLORMESSAGE, data.value[1]);
		debugger.dumpln(BLUECOLORMESSAGE, data.value[2]);
	}

	void debug(const sensorData &data) { debugData(data); }
	void debug(const colorData &data) { debugData(data); }

	template <typename T> void debugData(const T &data) {
		debugger.dump(REDCOLORMESSAGE, data.value[0]);
		debugger.dump(GREENCOLORMESSAGE, data.value[1]);
		debugger.dumpln(BLUECOLORMESSAGE, data.value[2]);
	}
};

#endif
