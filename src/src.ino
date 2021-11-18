/********************************************************************* 
 * Development and testing sketch for the library 
 *   Elements that are shown in examples may have been removed
 *   
 ********************************************************************/
#include "RWS_UNO.h"
#define VREF 3.28 // voltage measured from the board with a multimeter

RWS_UNO uno = RWS_UNO();
unsigned b[5000];

void setup()
{
  unsigned c[5000];
  uno.begin(0, 0, -5);
  Serial.print("\n\nRWS Arduino Extensions Testing\n\n");
  Serial.print(uno.bytesFree()); Serial.print(" bytes free in memory, approx.\n\n");
//  Serial.print(uno.setTimeZone(-5)); Serial.print(" is being set as the time zone\n");
//  Serial.print(uno.getTimeZone()); Serial.print(" is the current time zone setting\n\n");

  uno.setVReg(VREF);
  uno.setVRef(2.2);
  Serial.print("Voltages for Regulator: ");
  PL(uno.getVReg(), 3);
  Serial.print("         ADC Reference: ");
  PL(uno.getVRef(), 3);
  Serial.print("               Battery: ");
  PL(uno.getVBat(), 3);

}

void loop()
{
  String timeStamp = "";
  uno.stampNow(&timeStamp);
  Serial.print("\n"); Serial.println(timeStamp);
  Serial.print((unsigned long) now());

  Serial.print("          Analog on A0: ");
  PL(analogRead(A0));
  Serial.print("         Voltage on A0: ");
  PL(uno.getV(A0), 3);

  uno.run();
  uint32_t timeNow = micros();
  // take care of business

  uno.i2cScan();
  
  delay(6234);           // wait for next loop
}
