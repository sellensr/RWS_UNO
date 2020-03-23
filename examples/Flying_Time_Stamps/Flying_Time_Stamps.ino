#include "RWS_UNO.h"

RWS_UNO rws = RWS_UNO();

void setup()
{
  rws.begin(115200);  // no time specified, so initializes to compile time

  Serial.print("\n\nRWS UNO Time Stamp Tests\n\n");
  Serial.print(rws.bytesFree()); Serial.print(" bytes free in memory, approx.\n\n");
  P(rws.setTimeZone(-5)); P(" is being set as the time zone\n");
  P(rws.getTimeZone()); P(" is the current time zone setting\n\n");
}

 
void loop()
{
  String ts = "";       // test different time and date stamps
  double xs = micros()%5000000L/1000000.-2.5;   // a value for seconds adjustment, average zero
  P(" Seconds + Extra default: "); ts = ""; PL(*rws.stampTime(&ts,now()+xs));
  delay(2);           // wait for next scan
}
