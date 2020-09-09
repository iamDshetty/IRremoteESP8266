
/***************************************************************************************************************
 *   File name      : ir_Transcold.h
 *   Description    : Driver for Transcold AC
 *   Author         : Chandrashekar Shetty (mailchandrashetty2017@gmail.com)
 *   GitHub			: @iamDshetty
*--------------------------------------------------------------------------------------------------------------
 *   Who     :  Chandrashekar Shetty
 *   What    :  Driver for Transcold AC
 *   When    :  4 Sep 2020
 *   -----------------------------------------------------------------------------------------------------------
 Raw Data Calculation: (UR 12)
//ON button
ON 	24	Auto	cool	close (right)	111011110001000001100001100111100101010010101011

//OFF button
OFF	24	Auto	cool	close (right)	111011110001000001110001100011100101010010101011
 
// MODE
Hot mode	24	auto	hot	close (right)	111010010001011010100001010111100101010010101011
Fan mode	0 (prev24)	low	fan	close (right)	"11101001	0001011000100001110111100101010010101011"
Dry	mode  24	low	dry	close (right)	"11101001	0001011011000001	00111110	0101010010101011"
Auto Mode	0(prev24)	low	auto	close (right)	"11101001	0001011011100001	00011110	0101010010101011"
Cool Mode	24	low	cool	close (right)	"11101001	0001011001100001	10011110	0101010010101011"

//FAN SPEED
fan Speed low	24	low	cool	close (right)	"11101001	0001011001100001	10011110	0101010010101011"
fan Speed  medium	24	medium	cool	close (right)	"11101101	000100100110000110011110	0101010010101011"
fan Speed high	24	high	cool	close (right)	"11101011	000101000110000110011110	0101010010101011"
fan Speed auto	24	auto	cool	close (right)	"11101111	000100000110000110011110	0101010010101011"
 
//SWING  
Swing open 	24	auto	cool	open (left)	"11110111	000010000110000110011110	0101010010101011"
Swing close	24	auto	cool	close (right)	"11101111	000100000110000110011110	0101010010101011"

//TEMPERATURE
temp 30degC	Auto	cool	close (right)	111011110001000001100100100100010101010010101011
temp 	29	Auto	cool	close (right)	111011110001000001101100100100010101010010101011
temp 	28	Auto	cool	close (right)	111011110001000001100010100100010101010010101011
temp 	27	Auto	cool	close (right)	111011110001000001101010100100010101010010101011
temp 	26	Auto	cool	close (right)	111011110001000001100110100100010101010010101011
temp 	25	Auto	cool	close (right)	111011110001000001101110100100010101010010101011
temp 	24	Auto	cool	close (right)	111011110001000001100001100111100101010010101011
temp 	23	Auto	cool	close (right)	111011110001000001101001100101100101010010101011
temp 	22	Auto	cool	close (right)	111011110001000001100101100101100101010010101011
temp 	21	Auto	cool	close (right)	111011110001000001101101100101100101010010101011
temp 	20	Auto	cool	close (right)	111011110001000001100011100101100101010010101011
temp 	19	Auto	cool	close (right)	111011110001000001101011100101100101010010101011
temp 	18	Auto	cool	close (right)	111011110001000001100111100110000101010010101011
temp 	17	Auto	cool	close (right)	111011110001000001100111100110000101010010101011
temp 	16	Auto	cool	close (right)	111011110001000001100111100110000101010010101011
 
 **************************************************************************************************************/
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1256

#ifndef IR_TRANSCOLD_H_
#define IR_TRANSCOLD_H_

#define __STDC_LIMIT_MACROS
#include <stdint.h>
#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include "IRremoteESP8266.h"
#include "IRsend.h"
#ifdef UNIT_TEST
#include "IRsend_test.h"
#endif

// Constants
// Modes
const uint8_t kTranscoldCool = 0b0110;
const uint8_t kTranscoldDry = 0b1100;
const uint8_t kTranscoldAuto = 0b1110;
const uint8_t kTranscoldHeat = 0b1010;
const uint8_t kTranscoldFan = 0b0010;                                 
const uint8_t kTranscoldModeOffset = 12;
const uint8_t kTranscoldModeSize = 4;          
const uint8_t kTranscoldZoneFollowMaskOffset = 12;

// Fan Control
const uint8_t kTranscoldFanOffset = 16;
const uint8_t kTranscoldFanSize = 4;
const uint8_t kTranscoldFanMin = 0b1001;
const uint8_t kTranscoldFanMed = 0b1101;
const uint8_t kTranscoldFanMax = 0b1011;
const uint8_t kTranscoldFanAuto = 0b1111;
const uint8_t kTranscoldFanAuto0 = 0b0110; //
const uint8_t kTranscoldFanZoneFollow = 0b0000; //
const uint8_t kTranscoldFanFixed = 0b1100; //

// Temperature
const uint8_t kTranscoldTempMin = 17;  // Celsius
const uint8_t kTranscoldTempMax = 30;  // Celsius
const uint8_t kTranscoldTempRange = kTranscoldTempMax - kTranscoldTempMin + 1;
const uint8_t kTranscoldFanTempCode = 0b1111;  // Part of Fan Mode.
const uint8_t kTranscoldTempOffset = 8;
const uint8_t kTranscoldTempSize = 4;
const uint8_t kTranscoldTempMap[kTranscoldTempRange] = {
    0b0111,  // 17C transcold starts with 18 
    0b0111,  // 18c 
    0b1011,  // 19C
    0b0011,  // 20C
    0b1101,  // 21C
    0b0101,  // 22C
    0b1001,  // 23C 
    0b0001,  // 24C 
    0b1110,  // 25C 
    0b0110,  // 26C 
    0b1010,  // 27C
    0b0010,  // 28C
    0b1100,  // 29C
    0b0100   // 30C
};
const uint8_t kTranscoldSensorTempMin = 16;  // Celsius
const uint8_t kTranscoldSensorTempMax = 30;  // Celsius
const uint8_t kTranscoldSensorTempIgnoreCode = 0b1101;
const uint8_t kTranscoldSensorTempOffset = 8;
const uint8_t kTranscoldSensorTempSize = 4;

const uint8_t kTranscoldPrefix = 0b0000;//0b1011;  // 0xB
const uint8_t kTranscoldUnknown = 0xFF;
const uint32_t kTranscoldOff    = 0b111011110111100101010100;   
const uint32_t kTranscoldSwing  = 0b111001110110000101010100;  
const uint32_t kTranscoldSwingH = 0b111101110110000101010100;  //NA
const uint32_t kTranscoldSwingV = 0b111001110110000101010100;  //NA
const uint32_t kTranscoldSleep  = 0b101100101110000000000011;  //NA
const uint32_t kTranscoldTurbo  = 0b101101011111010110100010; //NA
const uint32_t kTranscoldLed    = 0b101101011111010110100101;  //NA
const uint32_t kTranscoldClean  = 0b101101011111010110101010; //NA
const uint32_t kTranscoldCmdFan = 0b111011110110000101010100;  //NA

const uint32_t kTranscoldDefaultState = 0b111011110111100101010100;  

// Classes
class IRTranscoldAC {
 public:
  explicit IRTranscoldAC(const uint16_t pin, const bool inverted = false,
                      const bool use_modulation = true);
  void stateReset();
#if SEND_TRANSCOLD
  void send(const uint16_t repeat = kTranscoldDefaultRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_TRANSCOLD
  void begin();
  void on();
  void off();
  void setPower(const bool state);
  bool getPower();
  void setTemp(const uint8_t temp);
  uint8_t getTemp();
  void setSensorTemp(const uint8_t desired);
  uint8_t getSensorTemp();
  void clearSensorTemp();
  void setFan(const uint8_t speed, const bool modecheck = true);
  uint8_t getFan();
  void setMode(const uint8_t mode);
  uint8_t getMode();
  void setSwing();
  bool getSwing();
  void setSleep();
  bool getSleep();
  void setTurbo();
  bool getTurbo();
  void setLed();
  bool getLed();
  void setClean();
  bool getClean();
  bool getZoneFollow();
  uint32_t getRaw();
  void setRaw(const uint32_t new_code);
  uint8_t convertMode(const stdAc::opmode_t mode);
  uint8_t convertFan(const stdAc::fanspeed_t speed);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  stdAc::state_t toCommon(const stdAc::state_t *prev = NULL);
  String toString();
#ifndef UNIT_TEST

 private:
  IRsend _irsend;  ///< Instance of the IR send class
#else
  /// @cond IGNORE
  IRsendTest _irsend;  ///< Instance of the testing IR send class
  /// @endcond
#endif
  // internal state
  bool    powerFlag;
  bool    turboFlag;
  bool    ledFlag;
  bool    cleanFlag;
  bool    sleepFlag;
  bool    zoneFollowFlag;
  bool    swingFlag;
  bool    swingHFlag;
  bool    swingVFlag;

  uint32_t remote_state ;  ///< The state of the IR remote in IR code form.
  uint32_t saved_state;   ///< Copy of the state if we required a special mode.
  void setTempRaw(const uint8_t code);
  uint8_t getTempRaw();
  void setSensorTempRaw(const uint8_t code);
  void setZoneFollow(const bool on);
  bool isSpecialState(void);
  bool handleSpecialState(const uint32_t data);
  void updateSavedState(void);
  void recoverSavedState(void);
  uint32_t getNormalState(void);
};

#endif  // IR_TRANSCOLD_H_
