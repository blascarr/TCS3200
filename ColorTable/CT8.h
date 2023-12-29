#define SIZECOLORS 8

#define TCS3200_WHITE 0
#define TCS3200_BLACK 1
#define TCS3200_YELLOW 2
#define TCS3200_ORANGE 3
#define TCS3200_RED 4
#define TCS3200_GREEN 5
#define TCS3200_BLUE 6
#define TCS3200_BROWN 7

const colorTable factory_ct[SIZECOLORS] = {
	colorTable{"WHITE", {254, 255, 253}},  colorTable{"BLACK", {0, 0, 0}},
	colorTable{"YELLOW", {233, 207, 120}}, colorTable{"ORANGE", {201, 98, 72}},
	colorTable{"RED", {191, 67, 73}},	   colorTable{"GREEN", {154, 143, 67}},
	colorTable{"BLUE", {139, 139, 166}},   colorTable{"BROWN", {145, 73, 67}}};
