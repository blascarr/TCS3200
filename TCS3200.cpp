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


#include <TCS3200.h>
#include <EEPROM.h>

#define  DEBUG 1
  #if  DEBUG
      #define  DUMP(s, v)  { Serial.print(F(s)); Serial.print(v); }
      #define  DUMPS(s)    Serial.print(F(s))
    #else
      #define  DUMP(s, v)
      #define  DUMPS(s)
  #endif

TCS3200::TCS3200(uint8_t S2, uint8_t S3, uint8_t OUT,uint8_t LED){
  _S2 = S2;
  _S3 = S3;
  _OUT = OUT;
  _LED = LED;
  _freqSet = TCS3200_FREQ_HI;
}

TCS3200::TCS3200(uint8_t S2, uint8_t S3, uint8_t OUT , uint8_t S0, uint8_t S1, uint8_t LED){
  _S0 = S0;
  _S1 = S1;
  _S2 = S2;
  _S3 = S3;
  _OUT = OUT;
  _LED = LED;
  _freqSet = TCS3200_FREQ_HI;
}

void TCS3200::begin(){

  if (_S0 != NULL )pinMode(_S0,OUTPUT);
  if (_S1 != NULL )pinMode(_S1,OUTPUT);
  pinMode(_S2,OUTPUT);
  pinMode(_S3,OUTPUT);
  pinMode(_LED,OUTPUT);
  pinMode(_OUT,INPUT);

  LEDON(true);
  setFrequency(_freqSet);

}


void TCS3200::setFrequency(uint8_t f){
  _freqSet = f;
  switch (f){
    case TCS3200_FREQ_HI:  DUMPS("HI");  digitalWrite(_S0, HIGH);  digitalWrite(_S1, HIGH);  break;
    case TCS3200_FREQ_MID: DUMPS("MID"); digitalWrite(_S0, HIGH);  digitalWrite(_S1, LOW);   break;
    case TCS3200_FREQ_LO:  DUMPS("LO");  digitalWrite(_S0, LOW);   digitalWrite(_S1, HIGH);  break;
    case TCS3200_FREQ_OFF: DUMPS("OFF"); digitalWrite(_S0, LOW);   digitalWrite(_S1, LOW);   break;
    default:  DUMP("Unknown freq option", f); break;
  }
}

void TCS3200::setFilter(uint8_t f){
// set the sensor color filter

  switch (f){
    case TCS3200_RGB_R:   digitalWrite(_S2, LOW);   digitalWrite(_S3, LOW);   break;
    case TCS3200_RGB_G:   digitalWrite(_S2, HIGH);  digitalWrite(_S3, HIGH);  break;
    case TCS3200_RGB_B:   digitalWrite(_S2, LOW);   digitalWrite(_S3, HIGH);  break;
    case TCS3200_RGB_X:   digitalWrite(_S2, HIGH);  digitalWrite(_S3, LOW);   break;
    default:  DUMP("Unknown filter option", f); break;
  }
}

void  TCS3200::setRefreshTime(unsigned long refreshTime){
  TCS3200::refreshTime = refreshTime;
}

void  TCS3200::LEDON(bool ledON){
  digitalWrite(_LED, (ledON) ? HIGH : LOW );
}

void  TCS3200::setOutInterrupt(bool ON){
  if (ON){

  }else{

  }
};

void TCS3200::read(bool RGB) {
  TCS3200::currentMillis = millis();
  if(TCS3200::currentMillis-TCS3200::oldMillis >= TCS3200::refreshTime){
    
    if(RGB){
      TCS3200::readRGB();
    }else{
      TCS3200::readRAW();
    }
    
    DUMP(" Non Blocking system Time: ",TCS3200::currentMillis-TCS3200::oldMillis);
    TCS3200::oldMillis  = TCS3200::currentMillis;
  }
}

sensorData TCS3200::color(){
  sensorData sensorcolor;   

  for (int i = 0; i < RGB_SIZE; ++i){
    
    float value = 0;
    float fvalue = 0;
    int n = 0;

    for (int j = 0; j < _nSamples; ++j){
      setFilter(i); 
      float color = pulseIn(_OUT, digitalRead(_OUT) == HIGH ? LOW : HIGH) ;

      // 1.5 value of threshold
      if (color/_nSamples > fvalue / 1.5) {

        fvalue = (fvalue + color)/_nSamples;
        value = value + color;
        n++;
      }

      sensorcolor.value[i] = value/n;  

    }

  }

  /*DUMP(" Red: ", color.value[0]);
  DUMP(" Green: ", color.value[1]);
  DUMP(" Blue: ", color.value[2]);  */
  return sensorcolor;
}

void TCS3200::voidRAW(sensorData *d) {
  
  for (int i = 0; i < RGB_SIZE; ++i)
  {
    d->value[i] = 0;
  }
}

sensorData TCS3200::readRAW() {
  sensorData cl;
  sensorData rawcl;
  TCS3200::voidRAW(&rawcl);

  rawcl = TCS3200::color();
  _raw.value[TCS3200_RGB_R] = rawcl.value[0];
  _raw.value[TCS3200_RGB_G] = rawcl.value[1];
  _raw.value[TCS3200_RGB_B] = rawcl.value[2];

  return rawcl;
}

sensorData  TCS3200::relativeColor(bool RGB){

  if (RGB){
    uint32_t sumcolor = _rgb.value[0]+_rgb.value[1]+_rgb.value[2];
    _relrgb.value[TCS3200_RGB_R] = _rgb.value[TCS3200_RGB_R]/sumcolor;
    _relrgb.value[TCS3200_RGB_G] = _rgb.value[TCS3200_RGB_G]/sumcolor;
    _relrgb.value[TCS3200_RGB_B] = _rgb.value[TCS3200_RGB_B]/sumcolor;
    //_relraw.value[TCS3200_RGB_X] = _raw.value[TCS3200_RGB_X]/sumcolor;

    DUMP(" RelRed : ",_relrgb.value[TCS3200_RGB_R]);
    DUMP(" RelGreen : ",_relrgb.value[TCS3200_RGB_G]);
    DUMP(" RelBlue : ",_relrgb.value[TCS3200_RGB_B]);
    //DUMP(" RelClear : ",_relraw.value[TCS3200_RGB_X]);
    return _relrgb;
  }else{
    uint32_t sumcolor = _raw.value[0]+_raw.value[1]+_raw.value[2];
    _relraw.value[TCS3200_RGB_R] = _raw.value[TCS3200_RGB_R]/sumcolor;
    _relraw.value[TCS3200_RGB_G] = _raw.value[TCS3200_RGB_G]/sumcolor;
    _relraw.value[TCS3200_RGB_B] = _raw.value[TCS3200_RGB_B]/sumcolor;
    //_relraw.value[TCS3200_RGB_X] = _raw.value[TCS3200_RGB_X]/sumcolor;

    DUMP(" RelRed : ",_relraw.value[TCS3200_RGB_R]);
    DUMP(" RelGreen : ",_relraw.value[TCS3200_RGB_G]);
    DUMP(" RelBlue : ",_relraw.value[TCS3200_RGB_B]);
    //DUMP(" RelClear : ",_relraw.value[TCS3200_RGB_X]);
    return _relraw;
  }

}

colorData TCS3200::readRGB() {
  TCS3200::readRAW();
  TCS3200::raw2RGB();

  DUMP(" RGB Red : ",_rgb.value[TCS3200_RGB_R]);
  DUMP(" RGB Green : ",_rgb.value[TCS3200_RGB_G]);
  DUMP(" RGB Blue : ",_rgb.value[TCS3200_RGB_B]);
}

void TCS3200::getRGB(colorData *rgb){
    if (rgb == NULL)
    return;

  DUMPS("\ngetRGB");
  for (uint8_t i=0; i<RGB_SIZE; i++)
  {
    rgb->value[i] = _rgb.value[i];
    DUMP(" ", rgb->value[i]);
  }
}

void TCS3200::getRaw(sensorData *d){
// get the raw data of the current reading
// useful to set dark and white calibration data

  if (d == NULL)
    return;

  DUMPS("\ngetRAW");
  for (uint8_t i=0; i<RGB_SIZE; i++)
  {
    d->value[i] = _raw.value[i];
    DUMP(" ", d->value[i]);
  }
}

void TCS3200::setDarkCal(){
  sensorData darkcl;
  DUMPS(" Dark Calibration ");
  TCS3200::voidRAW(&darkcl);
  bool sure= false;
  while (sure == false){

    while(!Serial.available()){
    
    }
    Serial.readString();

    darkcl = TCS3200::readRAW();
    Serial.print(F("RGB Dark Values")); 

    for (int i = 0; i < RGB_SIZE; ++i){
      Serial.print("  ");Serial.print(darkcl.value[i]); 
    }
    
    Serial.println();
    DUMPS(" Are you sure this values are correct for Dark Calibration? (Y/N)");
    while(!Serial.available()){
    
    }
    Serial.println();
    if (Serial.read() == 'Y'){
      _darkraw = darkcl;
      sure = true;
    }
  }
  
  DUMPS(" End Dark Calibration");
}


void TCS3200::setWhiteCal(){
  sensorData whitecl;
  DUMPS(" White Calibration ");
  TCS3200::voidRAW(&whitecl);
  bool sure= false;
  while (sure == false){

    while(!Serial.available()){
    
    }
    Serial.readString();

    whitecl = TCS3200::readRAW();
    Serial.print(F("RGB White Values")); 

    for (int i = 0; i < RGB_SIZE; ++i)
    {
      Serial.print("  ");Serial.print(whitecl.value[i]); 
    }
    
    Serial.println();
    DUMPS(" Are you sure this values are correct for White Calibration? (Y/N)");
    while(!Serial.available()){
    
    }
    Serial.println();
    if (Serial.read() == 'Y'){
      _whiteraw = whitecl;
      sure = true;
    }
  }
  
  DUMPS(" End Dark Calibration");
}

void TCS3200::setColorCal(){
  colorData colorcl;
  DUMPS(" Color Calibration ");
  
  while(!Serial.available()){
  
  }
  Serial.readString();
  for (int i = 0; i < SIZECOLORS; ++i){
    bool sure= false;
    while (sure == false){

      Serial.print(F("Calibration for color - "));
      Serial.println(_ct[i].name);
      colorcl = TCS3200::readRGB();

      Serial.println(F("Is Correct? "));
      while(!Serial.available()){
    
      }
      
      char readY = Serial.read();
      if (readY == 'Y'){
        _ct[i].rgb = colorcl;
        sure = true;
      }else if(readY == 'N'){
        sure = true;
      }
      Serial.readString();
    }
  }
}

void TCS3200::calibration(void){
  TCS3200::setDarkCal();
  TCS3200::setWhiteCal();
  Serial.readString();
  Serial.flush();
  Serial.println("Do you want to save Calibration in EEPROM?");
  while(!Serial.available()){
    
  }
  Serial.println();
  if (Serial.read() == 'Y'){
    TCS3200::saveCal(&_whiteraw, &_darkraw);
  }

  Serial.readString();Serial.println("Do you want to set Color values or Default RGB Values?");
  while(!Serial.available()){
    
  }
  Serial.println();
  if (Serial.read() == 'Y'){
    Serial.println("Setting RGB Values");
    TCS3200::setColorCal();

  }else{
    Serial.println("Default RGB Values");
  }

  Serial.readString();Serial.println("Do you want to save Calibration Colors in EEPROM?");
  while(!Serial.available()){
    
  }
  Serial.println();
  if (Serial.read() == 'Y'){
    Serial.println("Saved in EEPROM");
    //TCS3200::saveCal(&_whiteraw, &_darkraw);
  }else{
    Serial.println("Not saved in EEPROM");
  }

}

colorData TCS3200::raw2RGB(void){
// Exploiting linear relationship to remap the range 

  int32_t x;
  colorData color;
  for (uint8_t i=0; i<RGB_SIZE; i++){
    x = (_raw.value[i] - _darkraw.value[i]) * 255;
    x /= (_whiteraw.value[i] - _darkraw.value[i]);

    // copy results back into the global structures
    if (x < 0) color.value[i] = 0; 
    else if (x > 255) color.value[i] = 255;
    else color.value[i] = x;
  }
  _rgb = color;
  return color;
}

uint8_t TCS3200::checkColor(colorData *rgb){
  int32_t   d;
  uint32_t  v, minV = 999999L;
  uint8_t   minI;

  for (uint8_t i=0; i< SIZECOLORS; i++)
  {
    v = 0;
    for (uint8_t j=0; j<RGB_SIZE; j++)
    {
      d = _ct[i].rgb.value[j] - rgb->value[j];
      v += (d * d);
    }
    if (v < minV) // new best
    {
      minV = v;
      minI = i;
    }
    if (v == 0)   // perfect match, no need to search more
      break;
  }
  DUMP("Color Checked : ", minI);
  return(minI);
}

void  TCS3200::saveCal(sensorData *whiteRGB, sensorData *darkRGB){
  int calWhiteAddress = 0;
  int calDarkAddress = 0;
  calDarkAddress += sizeof(float);
  for (uint8_t i=0; i<4; i++){
    DUMP("Color: ", whiteRGB->value[i]);
  }
  EEPROM.put(calWhiteAddress, whiteRGB);
  EEPROM.put(calDarkAddress, darkRGB);
  DUMPS("Saved Calibration");
}

void  TCS3200::readCal(){
  int calWhiteAddress = 0;
  sensorData whiteRGB;
  EEPROM.get( calWhiteAddress, whiteRGB );
  for (uint8_t i=0; i<4; i++){
    DUMP("R", whiteRGB.value[i]);
  }

  int calDarkAddress = 0;
  calDarkAddress += sizeof(float);
  
  /*sensorData darkRGB;
  
  EEPROM.get( calDarkAddress, darkRGB );
  _darkraw = darkRGB;
  _whiteraw = whiteRGB;*/
}

void  TCS3200::saveCT(sensorData *rgb[]){

}

void  TCS3200::readCT(){

}
