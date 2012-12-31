/**
 * @file SimpleTuner.ino
 * @brief NS73M Stereo FM transmitter - Arduino library example
 * @author SkyWodd
 * @version 1.0
 * @see http://skyduino.wordpress.com/
 * @note Take a look at the html documentation for wiring and API informations ;)
 *
 * This program is an example of the Arduino library for the NS73M low power FM transmitter.
 *
 * Licence :
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
#include <NS73M.h> // For library API

/* Instanciate NS73M object */
NS73M tuner;

/* Defines */
#define BUFFER_SIZE 16

/* Setup() */
void setup() {
  
  /* Init serial and show welcome message */
  Serial.begin(9600);
  Serial.println("-   NS73M Library Example   -");
  Serial.println("- Serial FM Broadcast Tuner -");

  /* Setup tuner using user-friendly begin() function */
  delay(100); // Power on delay
  tuner.begin(97300000);
  
  /* Show usage message */
  Serial.println();
  Serial.println("(To tune NS73M send frequency like 97300000 for 97.3Mhz or 105000000 for 105Mhz)");
  Serial.println("> Loaded by default at 97.3 Mhz");
}

/* Loop() */
void loop() {
  
  /* Incoming data buffer and counter */
  char c, buffer[BUFFER_SIZE];
  byte nb_char = 0;
  
  /* Show prompt */
  Serial.print("> ");
  
  /* Do until a newline (\n) is received */
  do {
    
    /* Check for buffer overflow */
    if(nb_char == BUFFER_SIZE) return;
    
    /* Wait for a char */
    while(Serial.available() < 1);
    
    /* Read char */
    c = Serial.read();
    
    /* Store char in buffer */
    buffer[nb_char++] = c;
  
  /* Check for newline */
  } while(c != '\n');
  
  /* Computed frequency */
  unsigned long frequency;
  
  /* Read frequency from string (and check for errors) */
  if(sscanf(buffer, "%lu", &frequency) != 1) return;
  
  /* Show frequency (for confirmation / debug) */
  Serial.print("Loading at ");
  Serial.print(frequency / 1000000.0); // In MHz for easier reading
  Serial.println(" MHz");
  
  /* Check frequency range */
  if(frequency < 87500000 || frequency > 108000000) {
    Serial.println("Frequency out of FM range 87.5MHz ~ 108MHz");
    return;
  }
  
  /* Tune NS73M to target frequency */
  tuner.tuneTo(frequency, true);
  
  /* This part of code is optionnal */
  delay(10);     // For safety 
  tuner.reset(); // Just to be sure that the transmitter PLL clock will be ok
}


