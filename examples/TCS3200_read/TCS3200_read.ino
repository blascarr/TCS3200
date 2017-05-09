#include <TCS3200.h>

/*
  S0              8
  S1              9
  OUT Pin         10
  S2 Pin          11   
  S3 Pin          12
  LED Pin         13 
*/

TCS3200 CS(11,12,5,13);
colorData  rgb;
sensorData  raw;

void setup() {
  Serial.begin (9600);

  CS.begin();
  
  //Number of samples to check color frequency  
  CS.nSamples(40);
  //Time for non-blocking reading event
  CS.setRefreshTime(200);

  CS.setFrequency(TCS3200_FREQ_MID);
}

void loop() {

  CS.read();
  Serial.println();
  int cli= CS.checkColor( &CS._rgb );
  Serial.print("Name Color: ");Serial.println(CS._ct[cli].name);

}
