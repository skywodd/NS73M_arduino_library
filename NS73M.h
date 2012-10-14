/**
 * NNNNNNNN        NNNNNNNN   SSSSSSSSSSSSSSS 77777777777777777777 333333333333333   MMMMMMMM               MMMMMMMM
 * N:::::::N       N::::::N SS:::::::::::::::S7::::::::::::::::::73:::::::::::::::33 M:::::::M             M:::::::M
 * N::::::::N      N::::::NS:::::SSSSSS::::::S7::::::::::::::::::73::::::33333::::::3M::::::::M           M::::::::M
 * N:::::::::N     N::::::NS:::::S     SSSSSSS777777777777:::::::73333333     3:::::3M:::::::::M         M:::::::::M
 * N::::::::::N    N::::::NS:::::S                       7::::::7             3:::::3M::::::::::M       M::::::::::M
 * N:::::::::::N   N::::::NS:::::S                      7::::::7              3:::::3M:::::::::::M     M:::::::::::M
 * N:::::::N::::N  N::::::N S::::SSSS                  7::::::7       33333333:::::3 M:::::::M::::M   M::::M:::::::M
 * N::::::N N::::N N::::::N  SS::::::SSSSS            7::::::7        3:::::::::::3  M::::::M M::::M M::::M M::::::M
 * N::::::N  N::::N:::::::N    SSS::::::::SS         7::::::7         33333333:::::3 M::::::M  M::::M::::M  M::::::M
 * N::::::N   N:::::::::::N       SSSSSS::::S       7::::::7                  3:::::3M::::::M   M:::::::M   M::::::M
 * N::::::N    N::::::::::N            S:::::S     7::::::7                   3:::::3M::::::M    M:::::M    M::::::M
 * N::::::N     N:::::::::N            S:::::S    7::::::7                    3:::::3M::::::M     MMMMM     M::::::M
 * N::::::N      N::::::::NSSSSSSS     S:::::S   7::::::7         3333333     3:::::3M::::::M               M::::::M
 * N::::::N       N:::::::NS::::::SSSSSS:::::S  7::::::7          3::::::33333::::::3M::::::M               M::::::M
 * N::::::N        N::::::NS:::::::::::::::SS  7::::::7           3:::::::::::::::33 M::::::M               M::::::M
 * NNNNNNNN         NNNNNNN SSSSSSSSSSSSSSS   77777777             333333333333333   MMMMMMMM               MMMMMMMM
 *
 * @file NS73M.h
 * @brief NS73M Stereo FM transmitter - Arduino library
 * @mainpage NS73M Arduino Library
 * @author SkyWodd
 * @version 3.0
 * @see http://skyduino.wordpress.com/
 *
 * @section intro_sec Introduction
 * This program is an Arduino library for NS73M low power FM transmitter.\n
 * NS73M is a low power FM transmitter controlable by I2C or SPI bus (only software SPI is used in this library).\n
 * It can be used like any chinese FM car transmitter but with full control of transmission specifications.\n
 * This include power management, input volume level, emphasis, target frequency, and much other !\n
 * \n
 * Please report bug to <skywodd at gmail.com>
 *
 * @note Based on software SPI function and frequency formula from Cai Maver
 * @see http://dduino.blogspot.fr/2012/03/arduino-and-ns73m-fm-transmitter-rttl.html
 *
 * @section licence_sec Licence
 *  This program is free software: you can redistribute it and/or modify\n
 *  it under the terms of the GNU General Public License as published by\n
 *  the Free Software Foundation, either version 3 of the License, or\n
 *  (at your option) any later version.\n
 * \n
 *  This program is distributed in the hope that it will be useful,\n
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of\n
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n
 *  GNU General Public License for more details.\n
 * \n
 *  You should have received a copy of the GNU General Public License\n
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.\n
 *
 * @section changelog_sec Changelog history
 * - 07/06/2011 : First version
 * - 11/02/2012 : Second version (memory footprint improved)
 * - 18/09/2012 : Third version (total remake, lot of things improved, documentation added)
 *
 * @section wiring_sec How to wire the breakout board
 * Take a look at the image below to know how to wire your breakout board with your arduino.
 * @image html wiring.png
 * \n
 * Breakout NS73M (3v3) -> Arduino UNO (5v)\n
 * CK -> 10K resistor -> Digital 12 (by default)\n
 * DA -> 10K resistor -> Digital 11 (by default)\n
 * LA -> 10K resistor -> Digital 10 (by default)\n
 * IIC -> GND Power Rail\n
 * VCC -> 3V3 Power Rail\n
 * GND -> GND Power Rail\n
 *
 * @section purchase_sec Where to buy this breakout board ?
 * This library is tested with the following breakout boards :
 * - FM Radio Module Breakout Board (Sparkfun) : http://www.sparkfun.com/products/8482
 *
 * @section arduino_sec Arduino versions compatibility
 * This library use only basic arduino functions like pinMode() and digitalWrite().\n
 * By this way this library is virtualy compatible with any version of Arduino IDE.\n
 * @remark This library SHOULD work on boards derivated from arduino project like chipkit or maple.
 */
 
#ifndef _NS73M_H_
#define _NS73M_H_

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

/**
 * Pre-emphasis values
 *
 * @enum Pre_emphasis_t
 */
typedef enum {
  NO_PRE_EMPHASIS,    /*!< No pre-emphasis at all */
  PRE_EMPHASIS_50_US, /*!< European country */
  PRE_EMPHASIS_75_US, /*!< Other country like USA */
} Pre_emphasis_t;

/**
 * Audio input level values
 *
 * @enum Input_level_t
 */
typedef enum {
  INPUT_LEVEL_LOW,    /*!< Low volume input */
  INPUT_LEVEL_MEDIUM, /*!< Medium volume input */
  INPUT_LEVEL_HIGH,   /*!< High volume input */
} Input_level_t;

/**
 * Mode values
 *
 * @enum Mode_t
 */
typedef enum {
  DISABLE, /*!< Option is disable */
  ENABLE,  /*!< Option is enable */
} Mode_t;

/**
 * Output power level values
 *
 * @enum Output_level_t
 */
typedef enum {
  OUTPUT_LEVEL_LOW,    /*!< Low power output */
  OUTPUT_LEVEL_MEDIUM, /*!< Medium power output */
  OUTPUT_LEVEL_HIGH,   /*!< High power output */
} Output_level_t;

/**
 * Charge pumps values
 *
 * @enum Charge_pumps_t
 */
typedef enum {
  CHARGE_PUMPS_1_25, /*!< Charge pumps at 1.25uA */
  CHARGE_PUMPS_80,   /*!< Charge pumps at 80uA */
} Charge_pumps_t;

/**
 * @todo Understand why this two define make compilation fail
 */
#undef abs
#undef round

/**
 * NS73M class
 */
class NS73M {
public:
  /**
   * Instanciate a new NS73M controler object
   *
   * @param pin_ck SPI clock pin (default 12)
   * @param pin_da SPI data pin (default 11)
   * @param pin_la SPI latch pin (default 10)
   */
  NS73M(byte pin_ck = 12, byte pin_da = 11, byte pin_la = 10);
  
  /**
   * Power on NS73M
   */
  void powerOn(void);
  
  /**
   * Power off NS73M
   */
  void powerOff(void);
  
  /**
   * Mute sound
   */
  void mute(void);
  
  /**
   * Unmute sound
   */
  void unmute(void);
  
  /**
   * Set sound level of audio input  
   *
   * @param input_level Sound level of audio input, can be INPUT_LEVEL_LOW, INPUT_LEVEL_MEDIUM, INPUT_LEVEL_HIGH
   */
  void soundInputLevel(byte input_level);
  
  /**
   * Set output power level of transmitter
   *
   * @param power_level Output power level of transmitter, can be OUTPUT_LEVEL_LOW, OUTPUT_LEVEL_MEDIUM, OUTPUT_LEVEL_HIGH
   */
  void powerOutputLevel(byte power_level);
  
  /**
   * Tune transmitter to a specific frequency (in hertz)
   *
   * @param frequency Target frequency in hertz
   * @param doReset Reset flag, true to reset the NS73M chipset and apply immediatly the frequency change (true by default)
   */
  void tuneTo(unsigned long frequency, byte doReset = true);
  
  /**
   * User friendly, one step setup and quickstart function
   *
   * This function will set any NS73M registers to default values :
   * - Power On
   * - Unmuted
   * - INPUT_LEVEL_MEDIUM
   * - OUTPUT_LEVEL_MEDIUM
   * - frequency tunning w/ reset
   * - PRE_EMPHASIS_50_US
   * - Pilote tone enable
   * - Force sub carrier disable
   * - CHARGE_PUMPS_80
   * - PLL unlock detection enable
   *
   * @param frequency Target frequency in hertz
   */
  void begin(unsigned long frequency);
  
  /**
   * Reset to default values the (library side) NS73M registers.
   *
   * @remarks This function should (normally) not be used, only usefull for debug purpose
   */
  void allReset(void);
  
  /**
   * Reset by software the NS73M chipset
   */
  void reset(void);
  
  /**
   * Set pre-emphasis level of transmitter
   *
   * @warning Advanced users only
   * @see http://en.wikipedia.org/wiki/Pre-emphasis
   *
   * Common values :
   * - 50us for european country
   * - 75us for USA
   * 
   * @param pre_emphasis Pre-emphasis level, can be NO_PRE_EMPHASIS, PRE_EMPHASIS_50_US or PRE_EMPHASIS_75_US
   */
  void preEmphasis(byte pre_emphasis);
  
  /**
   * Set pilote tone mode
   *
   * @warning Advanced users only
   * @param mode Pilote tone mode, can be DISABLE or ENABLE
   */
  void piloteTone(byte mode);
  
  /**
   * Set force sub carrier mode
   *
   * @warning Advanced users only
   * @param mode Sub carrier mode, can be DISABLE or ENABLE
   */
  void forcedSubCarrier(byte mode);
  
  /**
   * Set charge pumps level
   *
   * @warning Advanced users only
   * @param charge_pumps Charge pumps level, can be CHARGE_PUMPS_1_25 or CHARGE_PUMPS_80
   */
  void chargePumps(byte charge_pumps);
  
  /**
   * Set frequency PLL unlock detection mode
   *
   * @warning Advanced users only
   * @param mode PLL unlock detection mode, can be DISABLE or ENABLE
   */
  void unlockDetect(byte mode);
  
  /**
   * Dump (library side) NS73M registers values to Serial output
   */
  void serialDump(void);
  
private:	
  byte pin_ck, pin_da, pin_la; // Clock pin, Data pin, Latch pin
  byte registers[9];           // Internal NS73M registers (double buffered by library)
  
  /**
   * Select transmitter frequency band
   *
   * @param frequency_band Frequency band to target, can be BAND_0, BAND_1, BAND_2 or BAND_3
   */
  void selectBand(byte frequency_band);
  
  /**
   * Send data to specified register using a software SPI communication layer
   *
   * @note All credits for this function goes to Cai Maver, please see library description for details
   * @param dest_register Destination register of commmited data
   * @param data Data to set into the destination register 
   */
  void spiSend(byte dest_register, byte data);
};

#endif // _NS73M_H_
