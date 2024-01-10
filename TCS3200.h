#ifndef _TCS3200_H
#define _TCS3200_H

#include "TCS3200Core.h"

#if SIZECOLORS == 8
#include "ColorTable/CT8.h"
#else
#warning "Color Table should be declared."
#endif

class TCS3200Test;
class TCS3200 : public TCS3200Core {
	friend class TCS3200Test;

  public:
	colorTable _ct[SIZECOLORS];
	TCS3200() : TCS3200Core() {}
	TCS3200(uint8_t S2, uint8_t S3, uint8_t OUT) : TCS3200Core(S2, S3, OUT) {}
	TCS3200(uint8_t S2, uint8_t S3, uint8_t OUT, uint8_t LED,
			uint8_t nEEPROM = 0)
		: TCS3200Core(S2, S3, OUT, LED, nEEPROM) {}
	TCS3200(uint8_t S2, uint8_t S3, uint8_t OUT, uint8_t S0, uint8_t S1,
			uint8_t LED, uint8_t nEEPROM = 0)
		: TCS3200Core(S2, S3, OUT, S0, S1, LED, nEEPROM) {}
	TCS3200(TCS3200Core &core) : TCS3200Core(core) {}

	uint8_t checkColor(colorData *rgb);
	String readColor();
	uint8_t readLastColorID();
	colorData setColorCal(uint8_t colorID);
	bool onChangeColor() override;

	// EEPROM Saving Values
	void saveCal(uint8_t nEEPROM = 0);
	void loadCal(uint8_t nEEPROM = 0);
	void saveCT(uint8_t nEEPROM = 0);
	void loadCT(uint8_t nEEPROM = 0);
	void loadExternalCT(colorTable CT[]);
	void voidCT();
	void setToFactoryCT(uint8_t nEEPROM = 0);
};

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

bool TCS3200::onChangeColor() {
	TCS3200Core::read();
	int cli = TCS3200::checkColor(&_rgb);

	if (cli != TCS3200Core::_lastColor) {
		TCS3200Core::_lastColor = cli;
		return true;
	} else {
		return false;
	}
}

String TCS3200::readColor() {
	TCS3200Core::read();
	int cli = TCS3200::checkColor(&_rgb);
	return _ct[cli].name;
}
uint8_t TCS3200::readLastColorID() { return _lastColor; }

colorData TCS3200::setColorCal(uint8_t colorID) {
	colorData colorcl;
	colorcl = TCS3200Core::readRGB();
	_ct[colorID].rgb = colorcl;
	return colorcl;
}

void TCS3200::saveCal(uint8_t nEEPROM) {
	TCS3200Core::saveBW(nEEPROM);
	TCS3200::saveCT(nEEPROM);
}

void TCS3200::loadCal(uint8_t nEEPROM) {
	TCS3200Core::loadBW(nEEPROM);
	TCS3200::loadCT(nEEPROM);
}

void TCS3200::saveCT(uint8_t nEEPROM) {
	// Save Color Table in EEPROM after Black and White sensorData Calibration
	// values
	int address = nEEPROM + 2 * sizeof(sensorData);
	for (int i = 0; i < SIZECOLORS; ++i) {
		EEPROM.put(address, _ct[i]);
		address += sizeof(colorTable);
	}
}

void TCS3200::loadCT(uint8_t nEEPROM) {
	// Load Color Table from EEPROM after Black and White sensorData Calibration
	int address = nEEPROM + 2 * sizeof(sensorData);
	for (int i = 0; i < SIZECOLORS; ++i) {
		EEPROM.get(address, _ct[i]);
		address += sizeof(colorTable);
	}
}

void TCS3200::loadExternalCT(colorTable externalCT[]) {
	// Load  External Color Table after Black and White sensorData Calibration
	memcpy(&_ct, &externalCT, SIZECOLORS * sizeof(colorTable));
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

;
#endif