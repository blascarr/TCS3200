// --- MENU VARIABLES ---
int freq = TCS3200_FREQ_HI;
bool ledState = true;
int refreshTime = 500;
int samples = 50;
TCS3200_LEDStatus ledToReadState = TCS3200_LEDON;
enum Colors {
	WHITE = 1,
	BLACK = 0,
	YELLOW = 2,
	ORANGE = 3,
	RED = 4,
	GREEN = 5,
	BLUE = 6,
	BROWN = 7
};
const char *colorNames[] = {"WHITE", "BLACK", "YELLOW", "ORANGE",
							"RED",	 "GREEN", "BLUE",	"BROWN"};

enum RGBMode { RGB, RAW };
int manualColorCal = 0;
RGBMode rgbState = RGB; // Establecer el modo por defecto a RGB
String freqStates[4] = {" - HIGH 100%", " - MID 20%", " - LOW 2%", " - OFF 0%"};

result doStop(eventMask e, navNode &nav, prompt &item) { return proceed; }

result readColor(eventMask e, navNode &nav, prompt &item) {
	CSDebug.readColor();
	CSDebug.read();
	return proceed;
}

result readRelativeColor(eventMask e, navNode &nav, prompt &item) {
	CSDebug.readColor();
	CSDebug.relativeColor();
	return proceed;
}

result setFrequency(eventMask e, navNode &nav, prompt &item) {
	CS.setFrequency(freq);
	return proceed;
}

result getFrequency(eventMask e, navNode &nav, prompt &item) {
	uint8_t freqLevel = CS.getFrequency();
	Serial.println();
	Serial.print("Frequency : ");
	Serial.print(freqStates[freqLevel - 1]);
	return proceed;
}

result ledEvent(eventMask e, navNode &nav, prompt &item) {
	CS.LEDON(ledState);
	return proceed;
}

result LEDToReadEvent(eventMask e, navNode &nav, prompt &item) {
	CS.setLEDtoRead(ledToReadState);
	return proceed;
}

result setRefreshTime(eventMask e, navNode &nav, prompt &item) {
	CS.setRefreshTime(refreshTime);
	return proceed;
}

result setSamples(eventMask e, navNode &nav, prompt &item) {
	CS.nSamples(samples);
	return proceed;
}

result rgbMode(eventMask e, navNode &nav, prompt &item) {
	CS.setRGBMode(rgbState);
	return proceed;
}

result readColorTable(eventMask e, navNode &nav, prompt &item) {
	CSDebug.readCT();
	return proceed;
}

result saveCT(eventMask e, navNode &nav, prompt &item) {
	CSDebug.saveCT();
	return proceed;
}

result factoryCT(eventMask e, navNode &nav, prompt &item) {
	CS.setToFactoryCT();
	return proceed;
}

result manualCalibrate(eventMask e, navNode &nav, prompt &item) {
	Serial.println(manualColorCal);
	Serial.print(" - Calibrate Color: ");
	Serial.println(colorNames[manualColorCal]);
	// Serial.println( CS._ct[manualColorCal].name );
	return proceed;
}

result SaveColor(eventMask e, navNode &nav, prompt &item) {
	colorData colorcl = CSDebug.readRGB();
	CS._ct[manualColorCal].rgb = colorcl;
	return proceed;
}

result darkCal(eventMask e, navNode &nav, prompt &item) {
	CSDebug.setDarkCal();
	CS.saveBW();
	return proceed;
}

result whiteCal(eventMask e, navNode &nav, prompt &item) {
	CSDebug.setWhiteCal();
	CS.saveBW();
	return proceed;
}

result currentDarkCal(eventMask e, navNode &nav, prompt &item) {
	CSDebug.getDarkCal();
	return proceed;
}

result currentWhiteCal(eventMask e, navNode &nav, prompt &item) {
	CSDebug.getWhiteCal();
	return proceed;
}

result loadBW(eventMask e, navNode &nav, prompt &item) {
	CSDebug.loadBW();
	return proceed;
}

result loadCT(eventMask e, navNode &nav, prompt &item) {
	CSDebug.loadCT();
	return proceed;
}

TOGGLE(ledState, LEDToggle, "LED: ", doNothing, noEvent, noStyle,
	   VALUE("ON", HIGH, ledEvent, enterEvent),
	   VALUE("OFF", LOW, ledEvent, enterEvent));

TOGGLE(ledToReadState, LEDToReadToggle, "LED TO READ: ", doNothing, noEvent,
	   noStyle, VALUE("OFF", 0, LEDToReadEvent, enterEvent),
	   VALUE("ONREAD", 1, LEDToReadEvent, enterEvent),
	   VALUE("ON", 2, LEDToReadEvent, enterEvent));

TOGGLE(freq, frequencyToggle, "SET FREQ: ", doNothing, noEvent, noStyle,
	   VALUE("HIGH", TCS3200_FREQ_HI, setFrequency, enterEvent),
	   VALUE("MID", TCS3200_FREQ_MID, setFrequency, enterEvent),
	   VALUE("LOW", TCS3200_FREQ_LO, setFrequency, enterEvent),
	   VALUE("LOW", TCS3200_FREQ_OFF, setFrequency, enterEvent));

TOGGLE(rgbState, RGBToggle, "RGB MODE: ", doNothing, noEvent, noStyle,
	   VALUE("RGB", HIGH, rgbMode, enterEvent),
	   VALUE("RAW", LOW, rgbMode, enterEvent));

MENU(configurationMenu, "SENSOR CONFIGURATION", doNothing, noEvent, noStyle,
	 SUBMENU(LEDToggle),
	 FIELD(refreshTime, "Refresh Time", "", 0, 1000, 100, 10, setRefreshTime,
		   enterEvent, wrapStyle),
	 FIELD(samples, "Samples", "", 0, 500, 50, 10, setSamples, enterEvent,
		   wrapStyle),
	 SUBMENU(RGBToggle), OP("RELATIVE COLOR", readRelativeColor, enterEvent),
	 SUBMENU(LEDToReadToggle), SUBMENU(frequencyToggle),
	 OP("GET FREQUENCY", getFrequency, enterEvent), EXIT("<Back"));

MENU(eepromMenu, "EEPROM CONFIGURATION", doNothing, noEvent, noStyle,
	 OP("CURRENT COLOR TABLE", readColorTable, enterEvent),
	 OP("LOAD BW EEPROM", loadBW, enterEvent),
	 OP("LOAD COLOR TABLE", loadCT, enterEvent),
	 OP("FACTORY COLOR TABLE", factoryCT, enterEvent), EXIT("<Back"));

MENU(manualCalibrationMenu, "MANUAL CALIBRATION", doNothing, noEvent, noStyle,
	 FIELD(manualColorCal, "COLOR", "", 0, 7, 1, 0, manualCalibrate, enterEvent,
		   noStyle),
	 OP("SAVE COLOR", SaveColor, enterEvent),
	 OP("SAVE EEPROM COLOR TABLE", saveCT, enterEvent), EXIT("<Back"));

MENU(calibrationMenu, "CALIBRATE SENSOR", doNothing, noEvent, noStyle,
	 OP("DARK CALIBRATION", darkCal, enterEvent),
	 OP("WHITE CALIBRATION", whiteCal, enterEvent),
	 OP("CURRENT DARK VALUE CALIBRATED ", currentDarkCal, enterEvent),
	 OP("CURRENT WHITE VALUE CALIBRATED ", currentWhiteCal, enterEvent),
	 SUBMENU(manualCalibrationMenu),
	 OP("NEURONA CALIBRATION", doNothing, enterEvent) // Training in Neuron
	 ,
	 EXIT("<Back"));

MENU(mainMenu, "Main Menu", doNothing, noEvent, wrapStyle,
	 OP("READ COLOR", readColor, enterEvent), SUBMENU(calibrationMenu),
	 SUBMENU(eepromMenu), SUBMENU(configurationMenu));

NAVROOT(nav, mainMenu, MAX_DEPTH, serial, out); // the navigation root object