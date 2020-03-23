/**************************************************************************/
/*!
  @file RWS_UNO.h
*/
/**************************************************************************/
#ifndef RWS_UNO_H
#define RWS_UNO_H

// RWS_Arduino is just too big for an UNO or similar once you add SD -- Sorry!
// This stripped down UNO version was created 2019-12-17 and will be missing features

#define TIME_DRIFT_INFO // includes sysUnsyncedTime from TimeLib
#include <SPI.h>        // to support the SD Card
#include <Wire.h>       // to support I2C scanning
#include <TimeLib.h>    // to manage time conversions
#include <RTClib.h>     // to get the DateTime struct and supporting functions

// All this Serial.print() will get tedious -- define some shortcuts
// These will make it easy to generate CSV data
// P(thing) or PR(thing) expands to Serial.print(thing) 
#ifndef ARDUINO_ARCH_ESP32
  #define P Serial.print // (but it seems to make the ESP32 barf)
  #define PR Serial.print
#else
  #define PR Serial.print
#endif
// PL(thing) expands to Serial.println(thing)
#define PL Serial.println
// PCS(thing) expands to Serial.print(", ");Serial.print(thing)
#define PCS Serial.print(", ");Serial.print
// PCSL(thing) expands to Serial.print(", ");Serial.println(thing)
#define PCSL Serial.print(", ");Serial.println

// i2c details from various libraries
#define BMX_ADDRESS_A             0x76
#define BMX_ADDRESS_B             0x77
#define BMX_REGISTER_CHIPID       0xD0
#define BNO_ADDRESS_A             0x28
#define BNO_ADDRESS_B             0x29
#define BNO_REGISTER_CHIPID       0x00
#define BNO_CHIPID                0xA0
#define MCP9808_ADDRESS_A         0x18
#define MCP9808_ADDRESS_B         0x1F
#define MCP9808_REG_MANUF_ID      0x06     ///< manufacture ID
#define MCP9808_REG_DEVICE_ID     0x07     ///< device ID
#define TMP006_ADDRESS_A          0x40
#define TMP006_ADDRESS_B          0x47
#define TMP006_MANID              0xFE
#define TMP006_DEVID              0xFF
#define LIS3DH_ADDRESS_A          0x18
#define LIS3DH_ADDRESS_B          0x19
#define LIS3DH_REG_WHOAMI         0x0F

// RWS Standard Pin Assignments for consistency across projects
#define HALL_PIN 11 ///< Hall Effect Sensor goes low when magnet present
#define BUTTON_PIN                                                             \
  12 ///< Hard Wired, pulled low when pressed, good choice because 12 doesn't do
     ///< PWM on ItsyBitsy M0
#define LED_PIN 13 ///< On Feather Board, and just about all others
#define TMP36_PIN                                                              \
  A0 ///< Hard Wired if included, but TMP36 is being phased out of MECH 217
#define POT_LOW_PIN A0  ///< Plug pot directly to A0-A2 for calibration testing
#define POT_PIN A1      ///< Hard Wired to middle pin of pot
#define POT_HIGH_PIN A2 ///< The third pin of a direct plugged pot
#define AMP_PIN A2 ///< Hard Wired if included, connect amplifier output here
#define CDS_PIN                                                                \
  A3 ///< Hard Wired if included, good place for permanently mounted photocell
     ///< input
#define THERMISTOR_PIN                                                         \
  A4 ///< Connect Thermistor to A4/A5, resistor from A4 to Vcc
#define THERMISTOR_LOW_PIN A5 ///< Pull to ground for thermistor
#ifdef A7
#define BAT_PIN A7       ///< Battery monitoring for feathers
#endif
#define BAT_FACTOR 2.00  ///< voltage divider step down
#define KC_OFFSET 273.15 ///< K for 0 deg C conversion factor

#ifndef ADC_RESOLUTION
  #ifdef ARDUINO_ARCH_AVR
    #define ADC_RESOLUTION 10 ///< Use 10 because it must be an UNO or similar
  #else
    #ifdef ARDUINO_ARCH_ESP8266
      #define ADC_RESOLUTION 10
    #else
      #define ADC_RESOLUTION 16 ///< Will work even if actual is 12
    #endif
  #endif
#endif

typedef enum {
  TIME_SOURCE_NONE,       ///< Time has not been set, or setter unknown
  TIME_SOURCE_COMPILER,   ///< Time supplied by compiler
  TIME_SOURCE_NTP,        ///< Time set from NTP
  TIME_SOURCE_GPS         ///< Time set from GPS
} rws_uno_time_source_t;

// A class with miscellaneous support functions created by Rick Sellens (RWS)
class RWS_UNO{
  public:
    RWS_UNO();                      // instantiate the class
    virtual ~RWS_UNO();             // destroy the class
    void begin(unsigned long spd = 115200, time_t tUTC = 0, double tzHours = 0.0, 
               rws_uno_time_source_t tSource = TIME_SOURCE_NONE, double tAccuracy = 0.0); 
    int run(bool reset = false);    // call each time through the loop for tracking 
  
    // report on loop timing
    double dt();                    // most recent time between calls [us]
    double dtAvg();                 // average time between calls [us]
    double dtMax();                 // maximum time between calls [us]
  
    // i2c and misc
    void i2cScan();                 // display details of everything connected to i2c
    unsigned long bytesFree();      // return an approximate number of bytes between the stack and the heap
    double normPulseTS(double t, double tMean, double tStd);
    double normpdf(double x);
    double randn();
  
    // manage voltages and ADC
    double setVReg(double v);    // set the Regulator Voltage (measure from board)
    double getVReg();
    double setVRef(double v); // set the appropriate Reference Voltage (for ADC)
    double getVRef();
    double getVBat();         // get the current battery voltage from a Feather
    int getBat();             // get a battery percentage charge estimate
    double getV(uint8_t pin); // get a voltage from an analog input
  
    // Format date and time stamps with variations on YYYY-MM-DD hh:mm:ss.sss
    String * stampTime(String *ts, time_t t);
    String * stampTime(String *ts, double t);
    String * stampTime(String *ts, double t, int dig);
    String * stampDate(String *ts, time_t t);
    String * stampDateTime(String *ts, time_t t);
    String * stampDateTime(String *ts, double t);
    String * stampDateTime(String *ts, double t, int dig);
    String * stampNow(String *ts);
    
    // Set and get time zones used in formatting stamps -- t remains in UTC
    double setTimeZone(double hours);
    double getTimeZone();
    void setTimeRWS(time_t tUTC, double tzHours, rws_uno_time_source_t tSource = TIME_SOURCE_NONE, double tAccuracy = 0.0); 

    rws_uno_time_source_t timeSource = TIME_SOURCE_NONE;
    double timeAccuracy = 0.0; ///< expected absolute accuracy in seconds, 0.0 for unknown

  private:
    unsigned long baud = 115200;
    
    // loop timing
    double _dt = 0.0;            // most recent time between calls [us]
    double _dtAvg = 0.0;         // average time between calls [us]
    double _dtMax = 0.0;         // largest time between calls [us]
    
    // i2c stuff
    uint16_t read16(uint8_t addr, uint8_t reg);
    uint8_t read8(uint8_t addr, uint8_t reg);
    void write8(uint8_t addr, uint8_t a, uint8_t data);
    unsigned checkBMX(uint8_t addr);
    unsigned checkBNO(uint8_t addr);
    unsigned checkMCP9808(uint8_t addr);
    unsigned checkTMP006(uint8_t addr);
    unsigned checkLIS3DH(uint8_t addr);
    void printAddr(unsigned address);
    void printSensor(const char *nom, unsigned id, uint8_t address);
    
    // voltages and ADC
    double _vRef = 3.30;      // analog reference voltage [V]
    double _vReg = 3.30;      // regulator output voltage to power the board [V]
    double _analogMax = 1023; // the maximum integer value from analogRead()
    
    // clock time
    double tzHours = 0.0;
    double tzSeconds = 0.0;
};

#endif //RWS_UNO_H
