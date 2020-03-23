/**************************************************************************/
/*!
  @file RWS_stamp.cpp

  @section intro Introduction

  This is code provides Date and Time Stamping Functions
  ISO Standard Date Format, with leading zeros https://xkcd.com/1179/

  A version without TZ support has been added to RW_LOG so they can be used 
  independently.

  @subsection author Author

  Written by Rick Sellens.

  @subsection license License

  CCBY license
*/
/**************************************************************************/
#include "RWS_UNO.h"

/**************************************************************************/
/*!
 *    @brief  Add time stamp to a String
 *    @param  ts    pointer to a String
 *    @param  t     unix time in seconds either integer or double
 *    @param  dig   number of digits after decimal
 *    @return pointer to the String
 */
String *RWS_UNO::stampTime(String *ts, time_t t) {
  return stampTime(ts, t, 0);
}
String *RWS_UNO::stampTime(String *ts, double t) {
  return stampTime(ts, t, 3);
}
String *
RWS_UNO::stampTime(String *ts, double t,
                       int dig) { // adds an hh:mm:ss.sss time stamp onto the
                                  // string, with dig places of decimal seconds
  time_t tUTC = t;
  double xSec = t - tUTC;
  t += tzSeconds; // adjust for time zone in final formatting only
  if (dig == 0)
    t += 0.499999; // round up if needed
  unsigned h = hour(t);
  unsigned m = minute(t);
  unsigned s = second(t);
  if (h < 10)
    *ts += "0";
  *ts += (String)h;
  *ts += ":";
  if (m < 10)
    *ts += "0";
  *ts += (String)m;
  *ts += ":";
  if (s < 10)
    *ts += "0";
  *ts += (String)s;
  double ds = 10 * (xSec - (int)xSec); // 10ths of a second
  unsigned j = 1;
  if (dig > 0) { // need a decimal place and digits
    *ts += ".";
    while (dig > 1) { // count down and scale up
      j *= 10;
      ds *= 10;
      dig--;
    }
    ds = (int)(ds + 0.5); // round as appropriate
    while (j > 1) {       // add leading zeros as necessary
      if (ds < j)
        *ts += "0";
      j /= 10;
    }
    *ts += (String)((int)(ds));
  }
  return ts;
}

/**************************************************************************/
/*!
 *    @brief  Add date stamp to a String
 *    @param  ts    pointer to a String
 *    @param  t     unix time in seconds either integer or double
 *    @return pointer to the String
 */
String *RWS_UNO::stampDate(
    String *ts, time_t t) { // adds an ISO YYYY-MM-DD date stamp onto the string
  unsigned y, mn, d;
  t += tzSeconds; // adjust for time zone in final formatting only
  y = year(t);
  mn = month(t);
  d = day(t);
  *ts += (String)y;
  *ts += "-";
  if (mn < 10)
    *ts += "0";
  *ts += (String)mn;
  *ts += "-";
  if (d < 10)
    *ts += "0";
  *ts += (String)d;
  return ts;
}

/**************************************************************************/
/*!
 *    @brief  Add date and time stamp to a String
 *    @param  ts    pointer to a String
 *    @param  t     unix time in seconds either integer or double
 *    @param  dig   number of digits after decimal
 *    @return pointer to the String
 */
String *RWS_UNO::stampDateTime(String *ts, time_t t) {
  return stampDateTime(ts, t, 0);
}
String *RWS_UNO::stampDateTime(String *ts, double t) {
  return stampDateTime(ts, t, 3);
}
String *RWS_UNO::stampDateTime(String *ts, double t, int dig) {
  stampDate(ts, t);
  *ts += " ";
  stampTime(ts, t, dig);
  return ts;
}

/**************************************************************************/
/*!
 *    @brief  Add current date and time stamp to a String with details
 *    @param  ts    pointer to a String
 *    @param  t     unix time in seconds either integer or double
 *    @param  dig   number of digits after decimal
 *    @return pointer to the String
 */
String *RWS_UNO::stampNow(String *ts) {
  stampDate(ts, now());
  *ts += " ";
  stampTime(ts, now(), 0);
  if (timeSource == TIME_SOURCE_GPS) *ts += " (GPS) ";
  else if (timeSource == TIME_SOURCE_NTP)  *ts += " (NTP) ";
  else if (timeSource == TIME_SOURCE_COMPILER)  *ts += " (compile time) ";
  else  *ts += " (unknown) ";
  *ts += "+/-";
  if (timeAccuracy == 0.0) *ts += "(unknown)";
  else *ts += String(timeAccuracy,2);
  *ts += " s";
  return ts;
}

/**************************************************************************/
/*!
 *    @brief  Set the time zone
 *    @param  hours Difference from UTC in hours
 *    @return the time zone set
 */
double RWS_UNO::setTimeZone(double hours) {
  while (hours > 12.0)
    hours -= 12.0;
  while (hours < -12.0)
    hours += 12.0;
  tzHours = hours;
  tzSeconds = hours * 3600;
  return tzHours;
}

/**************************************************************************/
/*!
 *    @brief  Get the time zone that is currently set.
 *    @return the time zone set in hours
 */
double RWS_UNO::getTimeZone() { return tzHours; }
