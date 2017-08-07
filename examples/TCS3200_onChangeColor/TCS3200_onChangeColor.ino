#include <TCS3200.h>
#include <EEPROM.h>

//S2,S3, OUT, 
TCS3200 CS(4,5,6);

void setup() {
  Serial.begin(9600);
  CS.begin();
  CS.nSamples(40);
  CS.setRefreshTime(200);
  CS.setID("CS_FORWARD");
  CS.loadCal();
  CS.loadCT();
  
  CS.setFrequency(TCS3200_FREQ_HI);
}

void loop() {
  if (CS.onChangeColor()){
    //Do something you want when event onChangeColor is Detected
  }

  delay(300);
}
