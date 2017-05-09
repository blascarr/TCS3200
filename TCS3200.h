/*
  Design and created by Blascarr

  TCS3200
  Name    : Blascarr
  Description: TCS3200.h
  version : 1.0

	TCS3200 is a library for color detection with TCS3200 module.

	The main objective is improve the performance detection of differente range colors
	for simple robotic applications.

	Compute time event intervals to read input signal for non-blocking systems.
	Assign interruption pint to the INPUT signal to calculate samples. 
	Compute buffer based on mean values and filtering errors.
	Mapping in different range values for colors.

	EEPROM memory for save some calibration data and color list.
  
  This library is a modification of MD_TCS230 created by Marco Colli

    https://github.com/MajicDesigns/MD_TCS230


  	
  	Blascarr invests time and resources providing this open source code like some other libraries, please
  	respect the job and support open-source software.
    
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

    // Frequency setting defines
	#define TCS3200_FREQ_HI  0 // 100% prescaler
	#define TCS3200_FREQ_MID 1 // 20%
	#define TCS3200_FREQ_LO  2 // 2%
	#define TCS3200_FREQ_OFF 3 // off

	// Indices for any RGB data/ filter selection
	#define TCS3200_RGB_R  0
	#define TCS3200_RGB_G  1
	#define TCS3200_RGB_B  2
	#define TCS3200_RGB_X  3 // 'Clear' filter selection

	#define RGB_SIZE  4 // array index counter limit
    #define SIZECOLORS 7
    typedef struct{
		float value[RGB_SIZE];  // Raw data from the sensor
	} sensorData;

	typedef struct{
		uint8_t value[RGB_SIZE]; // the evaluated colour data (RGB value 0-255 or other)
	} colorData;
	typedef struct	{
		char    name[SIZECOLORS];  // color name 8+nul
		colorData rgb;    // RGB value
	} colorTable;

	class TCS3200 {
		public:

			uint8_t   _OUT;    // output enable pin
			uint8_t   _S0, _S1; // frequency scaler
			uint8_t   _S2, _S3; // photodiode filter selection
			uint8_t   _LED;
			uint8_t   _freqSet; 
			int	  _nSamples = 10;
			
			colorTable _ct[SIZECOLORS]={
				colorTable {"WHITE", {255, 255, 255} },
				colorTable {"BLACK", {0, 0, 0} },
				colorTable {"YELLOW", {250, 250, 225} },
				//colorTable {"ORANGE", {250, 220, 190} },
				colorTable {"RED", {240, 175, 180} },
				colorTable {"GREEN", {105, 200, 175} },
				colorTable {"BLUE", {110, 170, 220} },
				colorTable {"BROWN", {170, 80, 20} }
			};
  
			bool _interruption = false;

			unsigned long currentMillis,oldMillis;
			unsigned long refreshTime= 50; 

			sensorData  _raw;    // current raw sensor reading
			
			//Calibration white and Black
			sensorData  _relraw;    // current relative raw sensor reading
			sensorData  _darkraw;    // Dark Calibration values
			sensorData  _whiteraw;    // White Calibration values

			colorData _rgb;   // colour based data for current reading
			sensorData  _relrgb;    // current relative raw sensor reading
				

			TCS3200(uint8_t S2, uint8_t S3, uint8_t OUT,uint8_t LED );
			TCS3200(uint8_t S2, uint8_t S3, uint8_t OUT , uint8_t S0, uint8_t S1,uint8_t LED );

			//  Data handling
			
			void  begin();        // used to initialise hardware
			void  LEDON(bool ledON);
			void  nSamples(int nSamples){_nSamples = nSamples;}
			void  voidRAW(sensorData *d);

			void  setRefreshTime(unsigned long refreshTime);
			void  setOutInterrupt(bool ON);

			void  setFilter(uint8_t f); // set the photodiode filter
			void  setFrequency(uint8_t f);// set frequency prescaler - default 100%	

			void read(bool RGB = true);      // sydnchronously non-blocking reading value
			sensorData  color();	//Single Reading
			sensorData  relativeColor(bool RGB = true);
			void  getRGB(colorData *rgb); // return RGB color data for the last reading
			void  getRaw(sensorData *d);  // return the raw data from the last reading
			sensorData readRAW();      // Read RAW Values
			colorData readRGB();      // Read RGB Values
			
			uint32_t readColor();      

			//Events for Calibration
			void  setDarkCal();
			void  setWhiteCal();
			void  calibration();
			void  setColorCal();

			colorData  raw2RGB(void);  // convert raw data to RGB
			uint8_t checkColor(colorData *rgb);
			//EEPROM Saving Values

			void  saveCal(sensorData *whiteRGB, sensorData *darkRGB);
			void  readCal();
			void  saveCT(sensorData *rgb[]);
			void  readCT();
	};

#endif