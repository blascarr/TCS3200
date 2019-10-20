
#define SERIAL_DEBUG
//#define SERIALDEBUG Serial
//#define DEBUG_CAL 
#define SERIALREMOTE Serial
#define BPS 9600

#include <TCS3200.h>

TCS3200 CS_F( 4, 5, 6, 2, 3, 7 );

void setup() {
  Serial.begin(9600);
  Serial.println("Ready: ");
  CS_F.begin();

  CS_F.nSamples(40);
  CS_F.setRefreshTime(200);
  CS_F.setFrequency(TCS3200_FREQ_HI);

  //CS_F.calibration(0);
  CS_F.loadCal(0);
}
 
void loop() {
  if (CS_F.onChangeColor()) {
    Serial.println(  CS_F.readColorID()  );
    Serial.println(  CS_F.readColor()  );
  }
 
}
