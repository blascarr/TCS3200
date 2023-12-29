#ifndef _TCS3200NEURONA_H
#define _TCS3200NEURONA_H

#include "TCS3200Core.h"
#include <Neurona.h>

class NeuronCT {
  public:
	MLP &mlp;
	char **CT;
	NeuronCT(MLP &mlpInstance) : mlp(mlpInstance){};
	NeuronCT(MLP &mlpInstance, char *inputColors[]) : mlp(mlpInstance) {
		CT = inputColors;
	}
};

class TCS3200Neurona : public TCS3200Core {
  private:
	NeuronCT &neuron;

  public:
	TCS3200Neurona() : TCS3200Core() {}
	TCS3200Neurona(uint8_t S2, uint8_t S3, uint8_t OUT)
		: TCS3200Core(S2, S3, OUT) {}
	TCS3200Neurona(uint8_t S2, uint8_t S3, uint8_t OUT, uint8_t LED,
				   uint8_t nEEPROM = 0)
		: TCS3200Core(S2, S3, OUT, LED, nEEPROM) {}
	TCS3200Neurona(uint8_t S2, uint8_t S3, uint8_t OUT, uint8_t S0, uint8_t S1,
				   uint8_t LED, uint8_t nEEPROM = 0)
		: TCS3200Core(S2, S3, OUT, S0, S1, LED, nEEPROM) {}
	TCS3200Neurona(TCS3200Core &core, NeuronCT &neuronTable)
		: TCS3200Core(core), neuron(neuronTable) {}

	uint8_t checkColor(colorData *rgb);
	String readColor();
	uint8_t readLastColorID();
	bool onChangeColor() override;
};

uint8_t TCS3200Neurona::checkColor(colorData *rgb) {
	double netInput[] = {-1.0, 0.0, 0.0, 0.0};
	for (int i = 0; i <= RGB_SIZE; i++) {
		netInput[i] = (double)rgb->value[i] / 255.0;
	}
	uint8_t colorID = neuron.mlp.getActivation(netInput);
	Serial.println(colorID);
	return colorID;
}

bool TCS3200Neurona::onChangeColor() {
	TCS3200Core::read();
	int cli = TCS3200Neurona::checkColor(&_rgb);

	if (cli != TCS3200Core::_lastColor) {
		TCS3200Core::_lastColor = cli;
		return true;
	} else {
		return false;
	}
}
String TCS3200Neurona::readColor() {
	TCS3200Core::read();
	int cli = TCS3200Neurona::checkColor(&_rgb);
	return neuron.CT[cli];
}
uint8_t TCS3200Neurona::readLastColorID() { return _lastColor; }

#endif