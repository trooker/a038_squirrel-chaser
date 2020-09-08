#include "Arduino.h"
/* f038 serial_test program supporting a038_critterChaser
 * 200819 Matching Android TX and Arduino responding action + TX
 * 200817 Abandon F310 for Orion Arduino with JS0
 *        need to mesh this spike with the s016 orion_otg
 * 200712 Initial rewrite of work executed by Hariharan Mathavan
 *        see: https://www.allaboutcircuits.com/projects/communicate-with-your-arduino-through-android/
 *        Need: Establish a "good reliable" pathway to use the
 *        F310 gamepad via the otg to android to support UDP wifi
 *        to the arduino (critterChaser).
 *        Tested/asserted return values from http://www.asciitable.com/
 *        TX "c" as ascii value of 99 RC return value of 100
 */


void setup()
{
 Serial.begin(9600);
}
void loop()
{
 if(Serial.available())
 {
	 char c = Serial.read();  //TX "c" ascii value of 99
	 if ((c != 10) and (c != 13))
	 {
		 Serial.println("Arduino TX: character read value + 1.");
		 Serial.print(c+1); //increments ASCII character value by 1
	 } //Check for 0D0A
	 Serial.println("");
  } //Serial.available()
}   // loop
