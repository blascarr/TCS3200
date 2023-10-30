/*
		Design and created by Blascarr
		TCS3200
		Name    : Blascarr
		Description: TCS3200.h
		version : 1.0
	TCS3200 is a library for color detection with TCS3200 module.
	The main objective is improve the performance detection of differente range
	colors for simple robotic applications. Compute time event intervals to read
	input signal for non-blocking systems. Compute buffer based on mean values
	and filtering errors. Mapping in different range values for colors. EEPROM
	memory for save some calibration data and color list. Assign interruption
	pint to the INPUT signal to calculate samples. (WIP)

	This library is a modification of MD_TCS230 created by Marco Colli
	https://github.com/MajicDesigns/MD_TCS230

	Blascarr invests time and resources providing this open source code like
	some other libraries, please respect the job and support open-source
	software.

	Written by Adrian for Blascarr
*/

#ifndef _TCS3200_H
#define _TCS3200_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <EEPROM.h>
#include <Ticker.h>

// Frequency setting defines
#define TCS3200_FREQ_HI 0  // 100% prescaler
#define TCS3200_FREQ_MID 1 // 20%
#define TCS3200_FREQ_LO 2  // 2%
#define TCS3200_FREQ_OFF 3 // off

// Indices for any RGB data/ filter selection
#define TCS3200_RGB_R 0
#define TCS3200_RGB_G 1
#define TCS3200_RGB_B 2
#define TCS3200_RGB_X 3 // 'Clear' filter selection

#define RGB_SIZE 4 // array index counter limit
#define SIZENAME 10
#define SIZECOLORS 8

#define TCS3200_WHITE 0
#define TCS3200_BLACK 1
#define TCS3200_YELLOW 2
#define TCS3200_ORANGE 3
#define TCS3200_RED 4
#define TCS3200_GREEN 5
#define TCS3200_BLUE 6
#define TCS3200_BROWN 7
typedef struct {
	float value[RGB_SIZE]; // Raw data from the sensor
} sensorData;

typedef struct {
	uint8_t
		value[RGB_SIZE]; // the evaluated colour data (RGB value 0-255 or other)
} colorData;
typedef struct {
	char name[SIZENAME]; // color name
	colorData rgb;		 // RGB value
} colorTable;
typedef enum {
	TCS3200_LEDOFF = 0,
	TCS3200_LEDREAD = 1,
	TCS3200_LEDON = 2
} TCS3200_LEDStatus;

const bool FrequencySettings[][2] = {
	{HIGH, HIGH}, // TCS3200_FREQ_HI
	{HIGH, LOW},  // TCS3200_FREQ_MID
	{LOW, HIGH},  // TCS3200_FREQ_LO
	{LOW, LOW}	  // TCS3200_FREQ_OFF
};

const bool FilterSettings[][2] = {
	{LOW, LOW},	  // TCS3200_RGB_R
	{HIGH, HIGH}, // TCS3200_RGB_G
	{LOW, HIGH},  // TCS3200_RGB_B
	{HIGH, LOW}	  // TCS3200_RGB_X
};
const sensorData factory_BW[2] = {{254, 308, 275.5}, {97.5, 116.5, 99}};
const colorTable factory_ct[SIZECOLORS] = {
	colorTable{"WHITE", {254, 255, 253}},  colorTable{"BLACK", {0, 0, 0}},
	colorTable{"YELLOW", {233, 207, 120}}, colorTable{"ORANGE", {201, 98, 72}},
	colorTable{"RED", {191, 67, 73}},	   colorTable{"GREEN", {154, 143, 67}},
	colorTable{"BLUE", {139, 139, 166}},   colorTable{"BROWN", {145, 73, 67}}};

class TCS3200 {
  public:
	uint8_t _OUT;	  // output enable pin
	uint8_t _S0, _S1; // frequency scaler
	uint8_t _S2, _S3; // photodiode filter selection
	uint8_t _LED;

	uint8_t _freqSet = TCS3200_FREQ_MID;
	uint8_t _filterSet = 0;
	uint8_t _nEEPROM = 0;
	int _lastColor = 0;
	int _nSamples = 20;
	char _ID[SIZENAME];
	TCS3200_LEDStatus _LEDToRead = TCS3200_LEDON;

	colorTable _ct[SIZECOLORS];

	typedef void (TCS3200::*_f_RGBMODE)();
	_f_RGBMODE f_RGB_MODE = &TCS3200::read_RGB;

	unsigned long refreshTime = 50;
	Ticker *timer = nullptr;

	sensorData _raw; // current raw sensor reading

	// Calibration white and Black
	sensorData _relraw;	  // current relative raw sensor reading
	sensorData _darkraw;  // Dark Calibration values
	sensorData _whiteraw; // White Calibration values

	colorData _rgb;		// colour based data for current reading
	sensorData _relrgb; // current relative raw sensor reading

	TCS3200();
	TCS3200(uint8_t S2, uint8_t S3, uint8_t OUT, uint8_t nEEPROM = 0);
	TCS3200(uint8_t S2, uint8_t S3, uint8_t OUT, uint8_t LED,
			uint8_t nEEPROM = 0);
	TCS3200(uint8_t S2, uint8_t S3, uint8_t OUT, uint8_t S0, uint8_t S1,
			uint8_t LED, uint8_t nEEPROM = 0);

	//  Data handling
	void setPins(uint8_t S2, uint8_t S3, uint8_t OUT, uint8_t nEEPROM = 0);
	void setPins(uint8_t S2, uint8_t S3, uint8_t OUT, uint8_t LED,
				 uint8_t nEEPROM = 0);
	void setPins(uint8_t S2, uint8_t S3, uint8_t OUT, uint8_t S0, uint8_t S1,
				 uint8_t LED, uint8_t nEEPROM = 0);
	void setID(String ID);

	void begin(); // used to initialise hardware
	void LEDON(bool ledON);
	void nSamples(int nSamples) { _nSamples = nSamples; }
	void setEEPROMaddress(uint8_t nEEPROM);
	void setLEDtoRead(TCS3200_LEDStatus ledRead) { _LEDToRead = ledRead; };
	bool getLEDtoRead() { return _LEDToRead; }
	void voidRAW(sensorData *d);

	void setRefreshTime(unsigned long refreshTime);
	void setInterrupt(bool ON); // TO DO - Define OUT as Interrupt pin

	void setFilter(uint8_t f);	  // set the photodiode filter
	void setFrequency(uint8_t f); // set frequency prescaler - default 100%
	uint8_t getFrequency() { return _freqSet; }

	// set RGB Mode (true) or RAW Mode (false) in readings
	void setRGBMode(bool _RGBMode);
	bool getRGBMode();
	void read(); // synchronously non-blocking reading value
	void update() {
		if (timer) {
			timer->update();
		}
	};

	void setTicker(Ticker &tick) {
		timer = &tick;
		timer->start();
	}

	bool onChangeColor();
	sensorData color(); // Single Reading
	sensorData relativeColor();
	void getRGB(colorData *rgb); // return RGB color data for the last reading
	void getRaw(sensorData *d);	 // return the raw data from the last reading

	sensorData readRAW();	 // Read RAW Values
	colorData raw2RGB(void); // Convert raw data to RGB
	colorData readRGB();	 // Read RGB Values
	uint8_t checkColor(colorData *rgb);
	void read_RAW() { readRAW(); };
	void read_RGB() { readRGB(); };

	String readColor();
	uint8_t readLastColorID();

	// Events for Calibration
	sensorData setDarkCal(bool saveDarkRaw = false);
	sensorData setWhiteCal(bool saveWhiteRaw = false);
	sensorData getDarkCal() { return _darkraw; };
	sensorData getWhiteCal() { return _whiteraw; };
	colorData setColorCal(uint8_t colorID);

	// EEPROM Saving Values
	void saveCal(uint8_t nEEPROM = 0);
	void loadCal(uint8_t nEEPROM = 0);
	void saveBW(uint8_t nEEPROM = 0);
	void loadBW(uint8_t nEEPROM = 0);
	void voidBW();
	void saveCT(uint8_t nEEPROM = 0);
	void loadCT(uint8_t nEEPROM = 0);
	void voidCT();
	void setToFactoryCT(uint8_t nEEPROM = 0);

	~TCS3200() {
		if (timer) {
			delete timer;
			timer = nullptr;
		}
	}
};

TCS3200::TCS3200() : timer(nullptr) {
	voidBW();
	voidCT();
}

TCS3200::TCS3200(uint8_t S2, uint8_t S3, uint8_t OUT, uint8_t nEEPROM)
	: TCS3200() {
	_S2 = S2;
	_S3 = S3;
	_OUT = OUT;
	_freqSet = TCS3200_FREQ_HI;
	_nEEPROM = nEEPROM;
	voidBW();
	voidCT();
}

TCS3200::TCS3200(uint8_t S2, uint8_t S3, uint8_t OUT, uint8_t LED,
				 uint8_t nEEPROM)
	: TCS3200() {
	_S2 = S2;
	_S3 = S3;
	_OUT = OUT;
	_LED = LED;
	_freqSet = TCS3200_FREQ_HI;
	_nEEPROM = nEEPROM;
	voidBW();
	voidCT();
}

TCS3200::TCS3200(uint8_t S2, uint8_t S3, uint8_t OUT, uint8_t S0, uint8_t S1,
				 uint8_t LED, uint8_t nEEPROM)
	: TCS3200() {
	_S0 = S0;
	_S1 = S1;
	_S2 = S2;
	_S3 = S3;
	_OUT = OUT;
	_LED = LED;
	_freqSet = TCS3200_FREQ_HI;
	_nEEPROM = nEEPROM;
	voidBW();
	voidCT();
}

void TCS3200::begin() {
	if (_S0 != NULL)
		pinMode(_S0, OUTPUT);
	if (_S1 != NULL)
		pinMode(_S1, OUTPUT);
	pinMode(_S2, OUTPUT);
	pinMode(_S3, OUTPUT);
	pinMode(_LED, OUTPUT);
	pinMode(_OUT, INPUT);

	LEDON(true);
	setFrequency(_freqSet);
}

void TCS3200::setRefreshTime(unsigned long refreshTime) {
	TCS3200::refreshTime = refreshTime;
	int value;
	if (timer) {
		timer->interval(refreshTime);
	}
}

void TCS3200::setEEPROMaddress(uint8_t nEEPROM) { TCS3200::_nEEPROM = nEEPROM; }

void TCS3200::LEDON(bool ledON) { digitalWrite(_LED, (ledON) ? HIGH : LOW); }

void TCS3200::setID(String ID) { ID.toCharArray(TCS3200::_ID, SIZENAME); }

void TCS3200::setFrequency(uint8_t f) {
	_freqSet = f;
	/* set the sensor Frequiency
	|	Freq    |	S0   	|   S1      |
	|	OFF     |   LOW 	|	LOW 	|
	|	LOW     |	LOW     |   HIGH    |
	|	MEDIUM 	|	HIGH    |  LOW      |
	|   HIGH    |   HIGH    |	HIGH 	|
	*/
	if (f < sizeof(FrequencySettings) / sizeof(FrequencySettings[0])) {
		digitalWrite(_S0, FrequencySettings[f][0]);
		digitalWrite(_S1, FrequencySettings[f][1]);
	}
}

void TCS3200::setFilter(uint8_t f) {
	/* set the sensor color filter
	|RGB | S2  |    S3      |
	| R | LOW  |	LOW     |
	| G | HIGH |    HIGH 	|
	| B | LOW  |    HIGH    |
	| X | HIGH |	LOW     |
	*/
	_filterSet = f;

	if (f < sizeof(FilterSettings) / sizeof(FilterSettings[0])) {
		digitalWrite(_S2, FilterSettings[f][0]);
		digitalWrite(_S3, FilterSettings[f][1]);
	}
}

/*
-------------------------------------------------------
---------------------READINGS--------------------------
-------------------------------------------------------
*/
void TCS3200::setRGBMode(bool _RGBMode) {
	if (_RGBMode) {
		f_RGB_MODE = &TCS3200::read_RGB;
	} else {
		f_RGB_MODE = &TCS3200::read_RAW;
	}
}

bool TCS3200::getRGBMode() {
	return (f_RGB_MODE == &TCS3200::read_RGB) ? true : false;
}

void TCS3200::read() { (this->*f_RGB_MODE)(); }

/*----------------------------------------------------
*------------ Non - BlockingFunctions
-----------------
------------------------------------------------------
*/
bool TCS3200::onChangeColor() {
	TCS3200::read();
	int cli = TCS3200::checkColor(&_rgb);

	if (cli != TCS3200::_lastColor) {
		TCS3200::_lastColor = cli;
		return true;
	} else {
		return false;
	}
}

String TCS3200::readColor() {
	TCS3200::read();
	int cli = TCS3200::checkColor(&_rgb);
	return _ct[cli].name;
}
uint8_t TCS3200::readLastColorID() { return _lastColor; }

sensorData TCS3200::color() {
	TCS3200::LEDON(_LEDToRead);
	sensorData sensorcolor;

	for (int i = 0; i < RGB_SIZE; ++i) {
		float value = 0;
		float fvalue = 0;
		int n = 0;

		for (int j = 0; j < _nSamples; ++j) {
			setFilter(i);
			float color = pulseIn(_OUT, digitalRead(_OUT) == HIGH ? LOW : HIGH);
			// 1.5 value of threshold. Exclude values which are 50% higher or
			// lower than color reading.
			float threshold = 1.5;
			if (color > (fvalue / threshold)) {
				n++;
				value += color;
				fvalue += color;
			}
		}
		sensorcolor.value[i] = value / n;
	}
	TCS3200::LEDON(_LEDToRead > 1);
	return sensorcolor;
}

void TCS3200::voidRAW(sensorData *d) {
	for (int i = 0; i < RGB_SIZE; ++i) {
		d->value[i] = 0;
	}
}

colorData TCS3200::readRGB() {
	TCS3200::readRAW();
	colorData color = TCS3200::raw2RGB();
	return color;
}

sensorData TCS3200::readRAW() {
	sensorData rawcl;
	rawcl = TCS3200::color();
	_raw.value[TCS3200_RGB_R] = rawcl.value[0];
	_raw.value[TCS3200_RGB_G] = rawcl.value[1];
	_raw.value[TCS3200_RGB_B] = rawcl.value[2];
	_raw.value[TCS3200_RGB_X] = rawcl.value[3];
	return rawcl;
}

sensorData TCS3200::relativeColor() {
	if (f_RGB_MODE == &TCS3200::read_RGB) {
		uint32_t sumcolor = _rgb.value[0] + _rgb.value[1] + _rgb.value[2];
		_relrgb.value[TCS3200_RGB_R] = _rgb.value[TCS3200_RGB_R] / sumcolor;
		_relrgb.value[TCS3200_RGB_G] = _rgb.value[TCS3200_RGB_G] / sumcolor;
		_relrgb.value[TCS3200_RGB_B] = _rgb.value[TCS3200_RGB_B] / sumcolor;
		_relraw.value[TCS3200_RGB_X] = _rgb.value[TCS3200_RGB_X] / sumcolor;
		return _relrgb;
	} else {
		uint32_t sumcolor = _raw.value[0] + _raw.value[1] + _raw.value[2];
		_relraw.value[TCS3200_RGB_R] = _raw.value[TCS3200_RGB_R] / sumcolor;
		_relraw.value[TCS3200_RGB_G] = _raw.value[TCS3200_RGB_G] / sumcolor;
		_relraw.value[TCS3200_RGB_B] = _raw.value[TCS3200_RGB_B] / sumcolor;
		_relraw.value[TCS3200_RGB_X] = _raw.value[TCS3200_RGB_X] / sumcolor;
		return _relraw;
	}
}

void TCS3200::getRGB(colorData *rgb) {
	if (rgb == NULL)
		return;
	for (uint8_t i = 0; i < RGB_SIZE; i++) {
		rgb->value[i] = _rgb.value[i];
	}
}

void TCS3200::getRaw(sensorData *d) {
	// get the raw data of the current reading useful to set dark and white
	// calibration data
	if (d == NULL)
		return;

	for (uint8_t i = 0; i < RGB_SIZE; i++) {
		d->value[i] = _raw.value[i];
	}
}

colorData TCS3200::raw2RGB(void) {
	// Exploiting linear relationship to remap the range
	int32_t x;
	colorData color;
	for (uint8_t i = 0; i < RGB_SIZE; i++) {
		x = (_raw.value[i] - _darkraw.value[i]) * 255;
		x /= (_whiteraw.value[i] - _darkraw.value[i]);

		// copy results back into the global structures
		if (x < 0)
			color.value[i] = 0;
		else if (x > 255)
			color.value[i] = 255;
		else
			color.value[i] = x;
	}
	_rgb = color;
	return color;
}

uint8_t TCS3200::checkColor(colorData *rgb) {
	int32_t d;
	uint32_t v, minV = 999999L;
	uint8_t minI;

	for (uint8_t i = 0; i < SIZECOLORS; i++) {
		v = 0;
		for (uint8_t j = 0; j < RGB_SIZE; j++) {
			d = _ct[i].rgb.value[j] - rgb->value[j];
			v += (d * d);
		}
		if (v < minV) {
			minV = v;
			minI = i;
		}
		if (v == 0)
			break; // perfect match, no need to search more
	}
	return (minI);
}

/* --------------------------------------------------
 * --------------- CALIBRATION METHODS --------------
 * --------------------------------------------------
 */

sensorData TCS3200::setDarkCal(bool saveDarkRaw) {
	sensorData darkcl;
	darkcl = TCS3200::readRAW();
	if (saveDarkRaw) {
		_darkraw = darkcl;
	}
	return darkcl;
}

sensorData TCS3200::setWhiteCal(bool saveWhiteRaw) {
	sensorData whitecl;
	whitecl = TCS3200::readRAW();
	if (saveWhiteRaw) {
		_whiteraw = whitecl;
	}
	return whitecl;
}

colorData TCS3200::setColorCal(uint8_t colorID) {
	colorData colorcl;
	colorcl = TCS3200::readRGB();
	_ct[colorID].rgb = colorcl;
	return colorcl;
}

void TCS3200::saveBW(uint8_t nEEPROM) {
	_nEEPROM = nEEPROM;
	int calWhiteAddress = _nEEPROM;
	int calDarkAddress = calWhiteAddress + sizeof(sensorData);
	EEPROM.put(calWhiteAddress, _whiteraw);
	EEPROM.put(calDarkAddress, _darkraw);
}

void TCS3200::saveCal(uint8_t nEEPROM) {
	TCS3200::saveBW(nEEPROM);
	TCS3200::saveCT(nEEPROM);
}

void TCS3200::loadCal(uint8_t nEEPROM) {
	TCS3200::loadBW(nEEPROM);
	TCS3200::loadCT(nEEPROM);
}

void TCS3200::loadBW(uint8_t nEEPROM) {
	_nEEPROM = nEEPROM;
	int calWhiteAddress = _nEEPROM;
	int calDarkAddress = calWhiteAddress + sizeof(sensorData);

	EEPROM.get(calWhiteAddress, _whiteraw);
	EEPROM.get(calDarkAddress, _darkraw);
}

void TCS3200::voidBW() {
	_darkraw = factory_BW[0];
	_whiteraw = factory_BW[1];
}

void TCS3200::saveCT(uint8_t nEEPROM) {
	// Save Color Table after Black and White sensorData Calibration values
	int address = nEEPROM + 2 * sizeof(sensorData);
	for (int i = 0; i < SIZECOLORS; ++i) {
		EEPROM.put(address, _ct[i]);
		address += sizeof(colorTable);
	}
}

void TCS3200::loadCT(uint8_t nEEPROM) {
	// Load Color Table after Black and White sensorData Calibration values
	int address = nEEPROM + 2 * sizeof(sensorData);
	for (int i = 0; i < SIZECOLORS; ++i) {
		EEPROM.get(address, _ct[i]);
		address += sizeof(colorTable);
	}
}

void TCS3200::voidCT() {
	for (uint8_t i = 0; i < SIZECOLORS; i++) {
		_ct[i] = factory_ct[i];
	}
}
void TCS3200::setToFactoryCT(uint8_t nEEPROM) {
	TCS3200::voidCT();
	TCS3200::saveCT(nEEPROM);
}

#endif