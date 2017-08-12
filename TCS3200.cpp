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
  Compute buffer based on mean values and filtering errors.
  Mapping in different range values for colors.
  EEPROM memory for save some calibration data and color list.
  Remote Calibration with Bluetooth Devices adapted with SoftwareSerial.
  Remote Calibration with Bluetooth Devices adapted with HardwareSerial for MEGA.
  Discrete Event System support.
  Assign interruption pint to the INPUT signal to calculate samples. (WIP)
  
  This library is a modification of MD_TCS230 created by Marco Colli
    https://github.com/MajicDesigns/MD_TCS230
    
    Blascarr invests time and resources providing this open source code like some other libraries, please
    respect the job and support open-source software.
    
    Written by Adrian for Blascarr
*/

#include <TCS3200.h>
#include <EEPROM.h>

#define  DEBUG 1
#define  DEBUG_CAL 1

#define  DEBUG_MEGA 0
#define  DEBUG_BT 0

//Pattern for Debug detection
#define  DEBUG_STARTCMD "{"
#define  DEBUG_ENDCMD "}"
#define  DEBUG_SEPCMD ","

#define  DEBUG_MEGACAL 0 //MEGACAL uses Serial1 to connect Bluetooth module
#define  DEBUG_BTCAL 0
#define  DEBUG_RX 2
#define  DEBUG_TX 3
#define  BPS 9600
#define  DEBUG_ESP 0

//Pattern for Calibration detection
#define  DEBUGCAL_STARTCMD "<"
#define  DEBUGCAL_ENDCMD ">"
#define  DEBUGCAL_SEPCMD "|"

//Pattern for Color detection

/*SENDCOLOR
  0 - No Serial for DUMPCOLOR
  1 - Serial
  2 - Serial1
  3 - BT
*/
#define  SENDCOLOR 1
#define  COLOR_STARTCMD "#"
#define  COLOR_ENDCMD "*"
#define  COLOR_SEPCMD "|"

  #if  DEBUG_BT || DEBUG_BTCAL
    #include <SoftwareSerial.h>
    SoftwareSerial BT(DEBUG_RX, DEBUG_TX);
  #endif

  #if  DEBUG
      #if DEBUG_MEGA && defined (__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
        #define  DUMP(s, v)  { Serial1.print(DEBUG_STARTCMD);Serial1.print(F(s)); Serial1.print(v);Serial1.print(DEBUG_ENDCMD); }
        #define  DUMPS(s)    { Serial1.print(DEBUG_STARTCMD);Serial1.print(F(s));Serial1.print(DEBUG_ENDCMD); }
        #define  DUMPPRINTLN() { Serial1.println();}
      #elif DEBUG_BT
        
        #define  DUMP(s, v)  { BT.print(DEBUG_STARTCMD);BT.print(F(s)); BT.print(v);BT.print(DEBUG_ENDCMD); }
        #define  DUMPS(s)    { BT.print(DEBUG_STARTCMD);BT.print(F(s)); BT.print(DEBUG_ENDCMD);}
        #define  DUMPPRINTLN() { BT.println();}
      #else
        #define  DUMP(s, v)  { Serial.print(DEBUG_STARTCMD); Serial.print(F(s)); Serial.print(v); Serial.print(DEBUG_ENDCMD); }
        #define  DUMPS(s)    { Serial.print(DEBUG_STARTCMD); Serial.print(F(s)); Serial.print(DEBUG_ENDCMD);}
        #define  DUMPPRINTLN() { Serial.println();}
      #endif
  #else
    #define  DUMP(s, v)
    #define  DUMPS(s)
    #define  DUMPPRINTLN() 
  #endif

  #if  DEBUG_CAL
    #if DEBUG_MEGACAL && defined (__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)

      #define  DUMPCAL(s, v)  { Serial1.print(DEBUGCAL_STARTCMD);Serial1.print(F(s)); Serial1.print(v);Serial1.print(DEBUGCAL_ENDCMD); }
      #define  DUMPSCAL(s)    { Serial1.print(DEBUGCAL_STARTCMD);Serial1.print(F(s)); Serial1.print(DEBUGCAL_ENDCMD);}
      #define  DUMPDEBUGCAL(s, v)  { Serial1.print(DEBUG_STARTCMD);Serial1.print(F(s)); Serial1.print(v);Serial1.print(DEBUG_ENDCMD); }
      #define  DUMPSDEBUGCAL(s)    { Serial1.print(DEBUG_STARTCMD);Serial1.print(F(s)); Serial1.print(DEBUG_ENDCMD);}
      #define  DUMPSAVAILABLE()    Serial1.available()
      #define  DUMPCOLOR(code) { Serial1.print(COLOR_STARTCMD); Serial1.print(code); Serial1.print(COLOR_ENDCMD);}
      #define  DUMPREADSTRING() { Serial1.readString();}
      #define  DUMPREAD(str) { str =Serial1.read();}
    #elif DEBUG_BTCAL
      #define  DUMPCAL(s, v)  { BT.print(DEBUGCAL_STARTCMD); BT.print(F(s)); BT.print(v);BT.print(DEBUGCAL_ENDCMD); }
      #define  DUMPSCAL(s)    { BT.print(DEBUGCAL_STARTCMD); BT.print(F(s)); BT.print(DEBUGCAL_ENDCMD);}
      #define  DUMPDEBUGCAL(s, v)  { BT.print(DEBUG_STARTCMD);BT.print(F(s)); BT.print(v);BT.print(DEBUG_ENDCMD); }
      #define  DUMPSDEBUGCAL(s)    { BT.print(DEBUG_STARTCMD);BT.print(F(s)); BT.print(DEBUG_ENDCMD);}
      #define  DUMPSAVAILABLE()    BT.available()
      #define  DUMPCOLOR(code) { BT.print(COLOR_STARTCMD);BT.print(code);BT.print(COLOR_ENDCMD);}
      #define  DUMPREADSTRING() {BT.readString();}
      #define  DUMPREAD(str) { str =BT.read();}
    #else
      #define  DUMPCAL(s, v)  { Serial.print(DEBUGCAL_STARTCMD); Serial.print(F(s)); Serial.print(v);Serial.print(DEBUGCAL_ENDCMD); }
      #define  DUMPSCAL(s)    { Serial.print(DEBUGCAL_STARTCMD); Serial.print(F(s));Serial.print(DEBUGCAL_ENDCMD);}
      #define  DUMPDEBUGCAL(s, v)  { Serial.print(DEBUG_STARTCMD);Serial.print(F(s)); Serial.print(v);Serial.print(DEBUG_ENDCMD); }
      #define  DUMPSDEBUGCAL(s)    { Serial.print(DEBUG_STARTCMD);Serial.print(F(s)); Serial.print(DEBUG_ENDCMD);}
      #define  DUMPSAVAILABLE()    Serial.available()
      #define  DUMPCOLOR(code) { Serial.print(COLOR_STARTCMD); Serial.print(code);Serial.print(COLOR_ENDCMD);}
      #define  DUMPREADSTRING() {Serial.readString();}    
      #define  DUMPREAD(str) {str =Serial.read();}
    #endif
  #else
    #define  DUMPCAL(s, v)
    #define  DUMPSCAL(s)
    #define  DUMPDEBUGCAL(s, v)
    #define  DUMPSDEBUGCAL(s)    
    #define  DUMPSAVAILABLE() false
    #define  DUMPCOLOR(code) 
    #define  DUMPREADSTRING()   
    #define  DUMPREAD(str) 
  #endif

  #if SENDCOLOR == 1
    #define  DUMPCOLOR(code) { Serial.print(COLOR_STARTCMD); Serial.print(code);Serial.print(COLOR_ENDCMD);}
  #elif SENDCOLOR == 2 && defined (__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    #define  DUMPCOLOR(code) { Serial1.print(COLOR_STARTCMD); Serial1.print(code);Serial1.print(COLOR_ENDCMD);}
  #endif

TCS3200::TCS3200(){
  
}

TCS3200::TCS3200(uint8_t S2, uint8_t S3, uint8_t OUT, uint8_t nEEPROM ){
  _S2 = S2;
  _S3 = S3;
  _OUT = OUT;
  _freqSet = TCS3200_FREQ_HI;
  _nEEPROM = nEEPROM;
}

TCS3200::TCS3200(uint8_t S2, uint8_t S3, uint8_t OUT,uint8_t LED, uint8_t nEEPROM ){
  _S2 = S2;
  _S3 = S3;
  _OUT = OUT;
  _LED = LED;
  _freqSet = TCS3200_FREQ_HI;
  _nEEPROM = nEEPROM;
}

TCS3200::TCS3200(uint8_t S2, uint8_t S3, uint8_t OUT , uint8_t S0, uint8_t S1, uint8_t LED, uint8_t nEEPROM ){
  _S0 = S0;
  _S1 = S1;
  _S2 = S2;
  _S3 = S3;
  _OUT = OUT;
  _LED = LED;
  _freqSet = TCS3200_FREQ_HI;
  _nEEPROM = nEEPROM;
}

void TCS3200::setPins(uint8_t S2, uint8_t S3, uint8_t OUT,uint8_t LED, uint8_t nEEPROM ){
  _S2 = S2;
  _S3 = S3;
  _OUT = OUT;
  _LED = LED;
  _freqSet = TCS3200_FREQ_HI;
  _nEEPROM = nEEPROM;
}

void TCS3200::setPins(uint8_t S2, uint8_t S3, uint8_t OUT , uint8_t S0, uint8_t S1, uint8_t LED, uint8_t nEEPROM ){
  _S0 = S0;
  _S1 = S1;
  TCS3200::setPins( S2,  S3,  OUT, LED,  nEEPROM );
  /*_S2 = S2;
  _S3 = S3;
  _OUT = OUT;
  _LED = LED;
  _freqSet = TCS3200_FREQ_HI;
  _nEEPROM = nEEPROM;*/
}

void TCS3200::begin(){

  #if  DEBUG || DEBUG_CAL || SENDCOLOR > 0
    #if ( DEBUG_MEGA || DEBUG_MEGACAL || SENDCOLOR == 2 ) && (defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__))
      Serial1.begin(BPS);
      DUMPS(" MEGA DEBUG");
    #elif DEBUG_BT || DEBUG_BTCAL
      BT.begin(BPS);
      DUMPS(" Bluetooth DEBUG");
    #else
      Serial.begin(BPS);
      DUMPS(" NORMAL DEBUG");
    #endif

    #if DEBUG_CAL || DEBUG_MEGACAL || DEBUG_BTCAL
      DUMPS(" CALIBRATION DEBUG MODE ");
    #endif

  #endif
    
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

void  TCS3200::setEEPROMaddress( uint8_t nEEPROM ){
  TCS3200::_nEEPROM = nEEPROM;
}

void  TCS3200::LEDON(bool ledON){
  digitalWrite(_LED, (ledON) ? HIGH : LOW );
}

void  TCS3200::setID(String ID){
  ID.toCharArray(TCS3200::_ID, SIZENAME);
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
    
    //DUMP("\nNon Blocking system Time: ",TCS3200::currentMillis-TCS3200::oldMillis);
    //DUMPS("\n");

    TCS3200::oldMillis  = TCS3200::currentMillis;
    
  }
}

void TCS3200::sendColor(){
  String code ;
  String str (TCS3200::_ID);
  if(str != ""){
    code += TCS3200::_ID;
    code += COLOR_SEPCMD;
  }
  code += TCS3200::_ct[TCS3200::_lastColor].name;
  DUMPCOLOR(code); 
} 

bool TCS3200::onChangeColor(){
  TCS3200::read();
  int cli= TCS3200::checkColor( &_rgb );

  if(cli != TCS3200::_lastColor){
      
      TCS3200::_lastColor = cli;
      #if SENDCOLOR
        TCS3200::sendColor();
      #endif
      return true;
  }else{
    return false;
  }
}
/*sensorData TCS3200::color(){
  sensorData color;   

  //RED  Photodiode
  setFilter(0); 
  color.value[0] = pulseIn(_OUT, digitalRead(_OUT) == HIGH ? LOW : HIGH);  

  //BLUE  Photodiode  
  setFilter(1);
  color.value[1] = pulseIn(_OUT, digitalRead(_OUT) == HIGH ? LOW : HIGH);  

  //GREEN  Photodiode 
  setFilter(2); 
  color.value[2] = pulseIn(_OUT, digitalRead(_OUT) == HIGH ? LOW : HIGH);

  //Clear Photodiode
  setFilter(3); 
  color.value[3] = pulseIn(_OUT, digitalRead(_OUT) == HIGH ? LOW : HIGH);

  DUMP(" Red: ", color.value[0]);
  DUMP(" Green: ", color.value[1]);
  DUMP(" Blue: ", color.value[2]);  
  return color;
}*/

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
  _raw.value[TCS3200_RGB_X] = rawcl.value[3];

  DUMP(" Red : ",_raw.value[TCS3200_RGB_R]);
  DUMP(" Green : ",_raw.value[TCS3200_RGB_G]);
  DUMP(" Blue : ",_raw.value[TCS3200_RGB_B]);
  DUMP(" Clear : ",_raw.value[TCS3200_RGB_X]);
  DUMPPRINTLN();

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
  colorData color = TCS3200::raw2RGB();

  DUMP(" Red : ",_rgb.value[TCS3200_RGB_R]);
  DUMP(" Green : ",_rgb.value[TCS3200_RGB_G]);
  DUMP(" Blue : ",_rgb.value[TCS3200_RGB_B]);
  return color;
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
  DUMPSDEBUGCAL(" BLACK Calibration ");
  DUMPPRINTLN();
  TCS3200::voidRAW(&darkcl);
  bool sure= false;
  while (sure == false){

    while(!DUMPSAVAILABLE()){
    
    }
    DUMPREADSTRING();

    darkcl = TCS3200::readRAW();
    DUMPSDEBUGCAL("RGB BLACK Values"); 

    String dataRGB = "";
    for (int i = 0; i < RGB_SIZE; ++i){
      dataRGB += darkcl.value[i];
      dataRGB += DEBUGCAL_SEPCMD;
    }
    DUMPCAL("",dataRGB ); 
    DUMPPRINTLN();

    DUMPDEBUGCAL("",dataRGB);
    DUMPPRINTLN();
    DUMPSDEBUGCAL(" Are you sure this values are correct for BLACK Calibration? (Y/N)");
    while(!DUMPSAVAILABLE()){
    
    }
    DUMPPRINTLN();
    char chr;
    DUMPREAD(chr);
    DUMPDEBUGCAL("Char Read : ",chr);
    if (chr == 'Y'){
      _darkraw = darkcl;
      sure = true;
    }
  }
  DUMPPRINTLN();
  DUMPSDEBUGCAL(" End BLACK Calibration");
}


void TCS3200::setWhiteCal(){
  sensorData whitecl;
  DUMPPRINTLN();
  DUMPSDEBUGCAL(" WHITE Calibration ");
  TCS3200::voidRAW(&whitecl);
  bool sure= false;
  while (sure == false){

    while(!DUMPSAVAILABLE()){
    
    }
    DUMPREADSTRING();

    whitecl = TCS3200::readRAW();
    DUMPSDEBUGCAL("RGB WHITE Values"); 

    String dataRGB = "";
    for (int i = 0; i < RGB_SIZE; ++i){
      dataRGB += whitecl.value[i];
      dataRGB += DEBUGCAL_SEPCMD;
    }
    DUMPCAL("",dataRGB ); 
    DUMPDEBUGCAL("",dataRGB);
    DUMPPRINTLN();
    DUMPSDEBUGCAL(" Are you sure this values are correct for WHITE Calibration? (Y/N)");
    while(!DUMPSAVAILABLE()){
    
    }
    DUMPPRINTLN();
    char chr;
    DUMPREAD(chr);
    DUMPDEBUGCAL("Char Read : ",chr);
    if (chr == 'Y'){
      _whiteraw = whitecl;
      sure = true;
    }
  }
  
  DUMPSDEBUGCAL(" End WHITE Calibration");
}

void TCS3200::setColorCal(){
  
  DUMPSDEBUGCAL(" Color Calibration ");
  
  while(!DUMPSAVAILABLE()){
    
  }
  DUMPREADSTRING();
  for (int i = 0; i < SIZECOLORS; ++i){
    bool sure= false;
    while (sure == false){
      DUMPPRINTLN();
      DUMPDEBUGCAL("Calibration for color - ", _ct[i].name);
      DUMPPRINTLN();
      colorData colorcl = TCS3200::readRGB();
      
      String dataRGB = "";
      for (int i = 0; i < RGB_SIZE; ++i){
        dataRGB += colorcl.value[i];
        dataRGB += DEBUGCAL_SEPCMD;
      }
      DUMPCAL("",dataRGB );
      DUMPPRINTLN();
      DUMPSDEBUGCAL("Is Correct? ");
      while(!DUMPSAVAILABLE()){
      }
      
      char readY;
      DUMPREAD(readY);
      DUMPDEBUGCAL("Char Read : ",readY);
      DUMPPRINTLN();

      if (readY == 'Y'){
        _ct[i].rgb = colorcl;
        sure = true;
      }else if(readY == 'N'){
        //sure = true;
      }
      DUMPREADSTRING();
    }
  }
}

void TCS3200::calibration(uint8_t nEEPROM){
  TCS3200::setDarkCal();
  TCS3200::setWhiteCal();
  DUMPREADSTRING();

  DUMPSDEBUGCAL("\nDo you want to save Calibration in EEPROM?");
  while(!DUMPSAVAILABLE()){
  }

  char readY;
  DUMPREAD(readY);
  DUMPDEBUGCAL("Char Read : ",readY);
  if (readY == 'Y'){
    DUMPDEBUGCAL("\nBlack and White Calibration saved in EEPROM Address:  ",TCS3200::_nEEPROM);
    DUMPPRINTLN();
    TCS3200::saveCal(nEEPROM);
  }

  DUMPREADSTRING();DUMPSDEBUGCAL("\nDo you want to set Color values (Y) or Load EEPROM Values (N)?");
  while(!DUMPSAVAILABLE()){
  }

  DUMPREAD(readY);
  DUMPDEBUGCAL("Char Read : ",readY);
  if (readY == 'Y'){
    DUMPSDEBUGCAL("\nSetting RGB Values");
    TCS3200::setColorCal();
    DUMPREADSTRING();DUMPSDEBUGCAL("\nDo you want to save Calibration Colors in EEPROM?\n");
    while(!DUMPSAVAILABLE()){
    }

    DUMPREAD(readY);
    DUMPDEBUGCAL("Char Read : ",readY);
    if (readY == 'Y'){
      DUMPDEBUGCAL("\nColour table saved in EEPROM in EEPROM Address:  ", TCS3200::_nEEPROM);
      TCS3200::saveCT(TCS3200::_nEEPROM);
    }else{
      DUMPSDEBUGCAL("\nNot saved in EEPROM");
    }
  }else if (readY == 'N'){
    DUMPSDEBUGCAL("\nEEPROM RGB Values");
    TCS3200::loadCT( nEEPROM );
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

void  TCS3200::saveCal(uint8_t nEEPROM){
  _nEEPROM = nEEPROM;
  int calWhiteAddress = _nEEPROM;
  int calDarkAddress = calWhiteAddress + sizeof(sensorData);
  EEPROM.put(calWhiteAddress, _whiteraw);
  EEPROM.put(calDarkAddress, _darkraw);

  DUMPS("\t White Calibration: ");
  for (uint8_t i=0; i<4; i++){
    DUMP(" Color: ", _whiteraw.value[i]);
  }
  DUMPS("\n \t Black Calibration: ");
  for (uint8_t i=0; i<4; i++){
    DUMP(" Color: ", _darkraw.value[i]);
  }

  DUMPS("\nSaved Calibration");
}

void  TCS3200::loadCal(uint8_t nEEPROM){
  TCS3200::loadBW(nEEPROM);
  TCS3200::loadCT(nEEPROM);
}

void  TCS3200::loadBW(uint8_t nEEPROM){
  _nEEPROM = nEEPROM;
  int calWhiteAddress = _nEEPROM;
  int calDarkAddress = calWhiteAddress + sizeof(sensorData);

  EEPROM.get(calWhiteAddress, _whiteraw);
  EEPROM.get(calDarkAddress, _darkraw);

  DUMPS("\n \t White Calibration: ");
  for (uint8_t i=0; i<4; i++){
    DUMP("Color: ", _whiteraw.value[i]);
  }
  DUMPS("\n \tBlack Calibration: ");
  for (uint8_t i=0; i<4; i++){
    DUMP("Color: ", _darkraw.value[i]);
  }
  DUMPS("\n Calibration Loaded ");
}

void  TCS3200::saveCT(uint8_t nEEPROM ){
  int address = nEEPROM + 2*sizeof(sensorData);
  
  for (int i = 0; i < SIZECOLORS; ++i){

    EEPROM.put(address, _ct[i]);
    address += sizeof(colorTable);
  }
  if (DEBUG) TCS3200::readCT();

  DUMPS("\n Color Table Calibration Saved ");
  DUMPPRINTLN();
}

void  TCS3200::loadCT(uint8_t nEEPROM ){
  int address = nEEPROM + 2*sizeof(sensorData);
  
  for (int i = 0; i < SIZECOLORS; ++i){
      EEPROM.get(address, _ct[i]);
      address += sizeof(colorTable);
  }
  if (DEBUG) TCS3200::readCT();

  DUMPS("\n Color Table Calibration Loaded ");
}

void  TCS3200::readCT(){
  for (int i = 0; i < SIZECOLORS; ++i){
      DUMP("\nColor: ",_ct[i].name);
      for (uint8_t j=0; j<3; j++){
        
        DUMP("  -  ", _ct[i].rgb.value[j]);
        
      }
      DUMPS("\n");
  }
}
