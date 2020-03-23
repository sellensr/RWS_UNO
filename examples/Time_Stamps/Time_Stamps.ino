#include "RWS_UNO.h"

RWS_UNO uno = RWS_UNO();

void setup()
{
  uno.begin(115200);  // no time specified, so initializes to compile time
  uno.begin(115200, now(), -5, TIME_SOURCE_NTP);

  Serial.print("\n\nRWS UNO Time Stamp Tests\n\n");
  Serial.print(uno.bytesFree()); Serial.print(" bytes free in memory, approx.\n\n");
  PR(uno.setTimeZone(-5)); PR(" is being set as the time zone\n");
  PR(uno.getTimeZone()); PR(" is the current time zone setting\n\n");
}

 
void loop()
{
  String ts = "";       // test different time and date stamps
  double xs = micros()%5000000L/1000000.-2.5;   // a value for seconds adjustment, average zero
  PR("\nVarious Time and date stamp formats with an Extra decimal adjustment :\n");
  PR("                Time is now: "); ts = ""; PL(*uno.stampNow(&ts));
  PR("              Extra Seconds: "); PR(xs,6); PR("\n");
  PR("Just (time_t) now() Seconds: "); ts = ""; PL(*uno.stampTime(&ts,(time_t) (now()) ));
  PR("    Seconds + Extra Rounded: "); ts = ""; PL(*uno.stampTime(&ts,now()+xs,0));
  PR("    Seconds + Extra default: "); ts = ""; PL(*uno.stampTime(&ts,now()+xs));
  PR("   Seconds + Extra 5 places: "); ts = ""; PL(*uno.stampTime(&ts,now()+xs,5));
  PR("   Seconds + Extra 9 places: "); ts = ""; PL(*uno.stampTime(&ts,now()+xs,9));
  PR("\nSame Variations with Dates Attached\n");
  ts = ""; PL(*uno.stampDate(&ts,(time_t) (now()+xs) ));
  ts = ""; PL(*uno.stampDateTime(&ts,(time_t) (now()+xs) ));
  ts = ""; PL(*uno.stampDateTime(&ts,now()+xs,0));
  ts = ""; PL(*uno.stampDateTime(&ts,now()+xs));
  ts = ""; PL(*uno.stampDateTime(&ts,now()+xs,7));
  PR("Time Source is ");
  if (uno.timeSource == TIME_SOURCE_GPS) PR("GPS");
  else if (uno.timeSource == TIME_SOURCE_NTP) PR("NTP");
  else if (uno.timeSource == TIME_SOURCE_COMPILER) PR("compile time");
  else PR("unknown");
  PR(". Accuracy is "); PL(uno.timeAccuracy);
  
  delay(3234);           // wait for next scan
}
