#include "Arduino.h"
/* Copyright © 1988-2020 by Abbott Analytical Products. All Rights Reserved.
 *
 * Use Orion, joystick, potentimeter to control direction and speed of
 *     critterChaser.
 * 201210_tr Added AxisJoystick-2.2.3 library
 * 201209_tr Try https://www.best-microcontroller-projects.com/arduino-joystick.html
 *           Broke code into sections 1, 2, 3
 * 200919_tr Disabled speed () to allow js readings.
 * 200816_tr X/Y axis values -500 to +500
 * 200719_tr initial
 *           adapted MeJoystickTest.ino,  PotentiometerTest.ino
 */

//Begin Section 1
/*  */
#include "MeOrion.h"

MeJoystick joystick(PORT_7);      //was 8
MePotentiometer zPotentiometer(PORT_8);    //was 7


int16_t x = 0;    //a variable for the Joystick's x value
int16_t y = 0;    // a variable for the Joystick's y value
float angle = 0;       // The relative angle of XY
float OffCenter = 0;    // offset with the center
int speed = 0;

void setup()
{
  //initialize serial communications at 9600 bps
  Serial.begin(9600);


}


void loop()
{
 // if (speed > 0)
  {
  // read the both joystick axis values:
  x = joystick.readX();
  y = joystick.readY();
  angle = joystick.angle();
  OffCenter = joystick.OffCenter();

  // print the results to the serial monitor:
  Serial.print("Joystick X = ");
  Serial.print(x);
  Serial.print("  Joystick Y = ");
  Serial.print(y);
  Serial.print("  angle =");
  Serial.print(angle);
  Serial.print("  OffCenter= ");
  Serial.println(OffCenter);
  // wait 10 milliseconds before the next loop
  delay(1000);
  }
//  speed = zPotentiometer.read();
//  Serial.print("Speed=");
//  Serial.println(speed );
//  delay(10);
}
//end of old s015
/* */
// End Section 1


/*
 * Begin Section 2
 * Run yields
 * VRrx = 576	VRry = 590	SW = 1
 * VRrx = 576	VRry = 590	SW = 1
 * VRrx = 574	VRry = 590	SW = 1
 * VRrx = 573	VRry = 590	SW = 1
 * VRrx = 572	VRry = 590	SW = 1
 *
 */

/*
const int VRxPin = A0;
const int VRyPin = A1;
const int SWPin  = 5;

int VRx = 0;        // value read from the horizontal pot
int VRy = 0;        // value read from the vertical pot
int SW = 0;         // value read from the switch

void setup() {
  Serial.begin(9600);
  pinMode(SWPin,INPUT_PULLUP);
}

void loop() {

  VRx = analogRead(VRxPin);
  VRy = analogRead(VRyPin);
  SW = digitalRead(SWPin);

  // print the results to the Serial Monitor:
  Serial.print("VRrx = ");
  Serial.print(VRx);
  Serial.print("\tVRry = ");
  Serial.print(VRy);
  Serial.print("\tSW = ");
  Serial.println(SW);

  delay(1000);
}
 */
//end Section 2

//begin Section 3



/*

#include <Joystick.h>
#include <AxisJoystick.h>

#define SW_PIN 5
#define VRX_PIN A1
#define VRY_PIN A2
#define LOW_RANGE 0
#define HIGH_RANGE 1023
#define RANGE_DIVITION 100

Joystick* joystic;



// Return title of the input joystick move.

String moveTitle(const Joystick::Move move) {
  switch (move) {
    case Joystick::Move::NOT:
      return "NOT";
    case Joystick::Move::PRESS:
      return "PRESS";
    case Joystick::Move::UP:
      return "UP";
    case Joystick::Move::DOWN:
      return "DOWN";
    case Joystick::Move::RIGHT:
      return "RIGHT";
    case Joystick::Move::LEFT:
      return "LEFT";
    default:
      return "???";
  }
}

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  joystic = new AxisJoystick(SW_PIN, VRX_PIN, VRY_PIN);
  joystic->calibrate(LOW_RANGE, HIGH_RANGE, RANGE_DIVITION);
}

// the loop function runs over and over again forever
void loop() {
  Serial.print("| SingleRead: " + String(joystic->singleRead()));
  Serial.print(" | MultipleRead: " + String(joystic->multipleRead()));
  Serial.print(" | Press: " + String(joystic->isPress()));
  Serial.print(" | Up: " + String(joystic->isUp()));
  Serial.print(" | Down: " + String(joystic->isDown()));
  Serial.print(" | Right: " + String(joystic->isRight()));
  Serial.print(" | Left: " + String(joystic->isLeft()));
  Serial.print(" | VRx: " + String(joystic->readVRx()));
  Serial.print(" | VRy: " + String(joystic->readVRy()));
  Serial.println(" | SW: " + String(joystic->readSW()) + " |");
  delay(1000);
}
  */

//end Section 3
