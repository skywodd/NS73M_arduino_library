/**
 * See header file for details on this library
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 * 
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* Includes */
#include "NS73M.h" // For prototypes

/* Macro */
#define setRegister(reg, mask, value) ((reg) = ((reg) & ~(mask)) | (value))
// Set register bits according bitmask and data

/* Registers address */
#define REG0 0x00
#define REG1 0x01
#define REG2 0x02
#define REG3 0x03
#define REG4 0x04
// No register 5
#define REG6 0x06
// No register 7
#define REG8 0x08
#define REG14 0x0E

// REGISTER 0 ----------------------------------
// Power SW
#define PW_MASK 1
#define PW_ON 1
#define PW_OFF 0

// Crystal SW
#define PDX_MASK 2
#define PDX_ON 0
#define PDX_OFF 2

// Mute SW
#define MUTE_MASK 4
#define MUTE 4
#define UNMUTE 0 

// Pre-emphasis SW
#define PE_MASK 16
#define PE_ON 0
#define PE_OFF 16

// Pre-emphasis selection
#define EMS_MASK 32
#define EMS_50 0  // Europe
#define EMS_75 32 // USA

// Audio input level
#define INPUT_MASK 192
#define INPUT_LOW 0
#define INPUT_MEDIUM 64
#define INPUT_HIGH 128

// REGISTER 1 ----------------------------------
// Pilot tune
#define PLT_MASK 8
#define PLT_ON 0
#define PLT_OFF 8

// Force subcarrier
#define SUBC_MASK 64
#define SUBC_ON 0
#define SUBC_OFF 64

// REGISTER 2 ----------------------------------
// Tx power level
#define POWER_MASK 3
#define POWER_LOW 1
#define POWER_MEDIUM 2
#define POWER_HIGH 3

// Locked Detect
#define ULD_MASK 4
#define ULD_ON 4
#define ULD_OFF 0

// REGISTER 6 ----------------------------------
// Charge pumps
#define CIA_MASK 30
#define CIA_1_25 2
#define CIA_80 6
#define CIB_320 24

// REGISTER 8 ----------------------------------
#define FREQUENCY_BAND_MASK 3

NS73M::NS73M(byte pin_ck, byte pin_da, byte pin_la) :
  pin_ck(pin_ck), pin_da(pin_da), pin_la(pin_la) {

  pinMode(pin_ck, OUTPUT);
  pinMode(pin_da, OUTPUT);
  pinMode(pin_la, OUTPUT);
  digitalWrite(pin_la, LOW);
  
  allReset();
}

void NS73M::powerOn(void) {
  setRegister(registers[0], PW_MASK, PW_ON);
  setRegister(registers[0], PDX_MASK, PDX_ON);
  spiSend(REG0, registers[0]);
}

void NS73M::powerOff(void) {
  setRegister(registers[0], PW_MASK, PW_OFF);
  setRegister(registers[0], PDX_MASK, PDX_OFF);
  spiSend(REG0, registers[0]);
}

void NS73M::mute(void) {
  setRegister(registers[0], MUTE_MASK, MUTE);
  spiSend(REG0, registers[0]);
}

void NS73M::unmute(void) {
  setRegister(registers[0], MUTE_MASK, UNMUTE);
  spiSend(REG0, registers[0]);
}

void NS73M::soundInputLevel(byte input_level) {
  switch(input_level) {
    case INPUT_LEVEL_LOW:
	  setRegister(registers[0], INPUT_MASK, INPUT_LOW);
	  break;
    case INPUT_LEVEL_MEDIUM:
	  setRegister(registers[0], INPUT_MASK, INPUT_MEDIUM);
	  break;
	case INPUT_LEVEL_HIGH:
	  setRegister(registers[0], INPUT_MASK, INPUT_HIGH);
	  break;
  }
  spiSend(REG0, registers[0]);
}

void NS73M::powerOutputLevel(byte power_level) {
  switch(power_level) {
    case OUTPUT_LEVEL_LOW:
	  setRegister(registers[2], POWER_MASK, POWER_LOW);
	  break;
    case OUTPUT_LEVEL_MEDIUM:
	  setRegister(registers[2], POWER_MASK, POWER_MEDIUM);
	  break;
	case OUTPUT_LEVEL_HIGH:
	  setRegister(registers[2], POWER_MASK, POWER_HIGH);
	  break;
  }
  spiSend(REG2, registers[2]);
}

void NS73M::tuneTo(unsigned long frequency, byte doReset) {
  if (frequency < 88500000UL) {
    selectBand(3);
  } else if (frequency < 97900000UL) {
    selectBand(2);
  } else if (frequency < 103000000UL) {
    selectBand(1);
  } else{
    selectBand(0);
  }

  frequency = (frequency + 304000) / 8192;
  registers[3] = frequency & 255;
  registers[4] = frequency >> 8;

  chargePumps(CHARGE_PUMPS_80);
  spiSend(REG3, registers[3]);
  spiSend(REG4, registers[4]);
  chargePumps(CHARGE_PUMPS_1_25);
  
  if(doReset)
    reset();
}

void NS73M::begin(unsigned long frequency) {
  allReset();
  reset();
  piloteTone(ENABLE);
  powerOutputLevel(OUTPUT_LEVEL_MEDIUM);
  unlockDetect(ENABLE);
  forcedSubCarrier(DISABLE);
  tuneTo(frequency, false);
  powerOn();
  unmute();
  preEmphasis(PRE_EMPHASIS_50_US);
  soundInputLevel(INPUT_LEVEL_MEDIUM);
  reset();
}

void NS73M::allReset(void) {
  registers[0] = 0x00;
  registers[1] = 0xB4;
  registers[2] = 0x05;
  registers[3] = 0x8A; // Default frequency : 97.3 Mhz
  registers[4] = 0x2E; //
  registers[6] = 0x1A;  
  registers[8] = 0x1B;
}

void NS73M::reset(void) {
  spiSend(REG14, 0x05);
}

void NS73M::preEmphasis(byte pre_emphasis) {
  switch(pre_emphasis) {
    case NO_PRE_EMPHASIS:
	  setRegister(registers[0], PE_MASK, PE_OFF);
	  break;
    case PRE_EMPHASIS_50_US:
	  setRegister(registers[0], PE_MASK, PE_ON);
	  setRegister(registers[0], EMS_MASK, EMS_50);
	  break;
	case PRE_EMPHASIS_75_US:
	  setRegister(registers[0], PE_MASK, PE_ON);
	  setRegister(registers[0], EMS_MASK, EMS_75);
	  break;
  }
  spiSend(REG0, registers[0]);
}

void NS73M::piloteTone(byte mode) {
  setRegister(registers[1], PLT_MASK, (mode) ? PLT_ON : PLT_OFF);
  spiSend(REG1, registers[1]);
}

void NS73M::forcedSubCarrier(byte mode) {
  setRegister(registers[1], SUBC_MASK, (mode) ? SUBC_ON : SUBC_OFF);
  spiSend(REG1, registers[1]);
}

void NS73M::chargePumps(byte charge_pumps) {
  switch(charge_pumps) {
    case CHARGE_PUMPS_1_25:
	  setRegister(registers[6], CIA_MASK, CIA_1_25 | CIB_320);
	  break;
	case CHARGE_PUMPS_80:
	  setRegister(registers[6], CIA_MASK, CIA_80 | CIB_320);
	  break;
  }
  spiSend(REG6, registers[6]);
}

void NS73M::unlockDetect(byte mode) {
  setRegister(registers[2], ULD_MASK, (mode) ? ULD_ON : ULD_OFF);
  spiSend(REG2, registers[2]);
}

void NS73M::selectBand(byte frequency_band) {
  setRegister(registers[8], FREQUENCY_BAND_MASK, frequency_band);
  spiSend(REG8, registers[8]);
}

void NS73M::spiSend(uint8_t reg, uint8_t data)
{
  uint8_t x;
  digitalWrite(pin_la, LOW);

  for(x = 0 ; x < 4 ; x++) {
    digitalWrite(pin_ck, LOW);
    digitalWrite(pin_da, (reg >> x) & 1);  
    digitalWrite(pin_ck, HIGH);	
  }

  for(x = 0 ; x < 8 ; x++) {
    digitalWrite(pin_ck, LOW);	 
    digitalWrite(pin_da, (data >> x) & 1);  
    digitalWrite(pin_ck, HIGH);
  }
  delayMicroseconds(1);		

  digitalWrite(pin_la, HIGH);
  delayMicroseconds(4);
  digitalWrite(pin_la, LOW);

  digitalWrite(pin_ck, LOW);
}

void NS73M::serialDump(void) {
  for(byte i = 0; i < 9; ++i) {
    Serial.print("Reg ");
	Serial.print(i);
	Serial.print(" = ");
	Serial.println(registers[i]);
  }
}