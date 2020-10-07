#ifndef A038_CONFIG_H_
#define A038_CONFIG_H_


/*Copyright © 1988-2020 by Abbott Analytical Products. All Rights Reserved.
 * a038_critterChaser project
 * Header file used to centralize libraries used by project
 * 201006_tr Added XCARE for tristate behavior
 * 200914_tr Sloeber does not like rgb_lcd and has corruped showDisplay,
 * 200912_tr Added ams2 functions to external void functions list.
 * 200604_tr Added wifi_mgr.ino methods and defines.
 * 200529_tr Tinkering with SEEED WiFi shield
 *        https://wiki.seeedstudio.com/Wifi_Shield_V2.0/#example-1-send-commands-to-wifi
 *        -shield-and-receive-response-via-the-arduino-serial-monitor-window
 * 200528_tr Adapted from a034_nc_chickadee a034_config.h

//------------------------------------------------------------------------------
// 2 Axis CNC Demo
// dan@marginallycelver.com 2013-08-30
//------------------------------------------------------------------------------
// Copyright at end of file.
// please see http://www.github.com/MarginallyClever/GcodeCNCDemo for more information.
 */

//------------------------------------------------------------------------------
// CONSTANTS
//------------------------------------------------------------------------------
// supported control boards
//#define DEBUG1

#define AMS1 (1)
#define AMS2 (2)

#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_MotorShield.h>
//#include <AccelStepper.h>
#include <SPI.h>
#include <SD.h>
//#include "rgb_lcd.h"
#include <rgb_lcd.h>

#include <SoftwareSerial.h>
#include "WiFly.h"

//200914 plays better here
rgb_lcd lcd;

#define DEV_SENSORZERO 1  //shuts-off machine zero for steppers



// change this line to select a different control board for your CNC.
#define CONTROLLER AMS1

//#define DEBUG1  //verbose

/* Machine Parameters*/
// define the parameters of our machine.

/* Radioshack 2730767  5wire 1:64  5vdc
 * kiatronics 28byj-48 5v
float X_STEPS_PER_INCH = 30.72;
float X_STEPS_PER_MM = 1.2094 //this is wrong 25.6;  //????
float STEP_PER_DEG = 11.25   //https://lastminuteengineers.com/28byj48-stepper-motor-arduino-tutorial
float STEP_PER_REV = 2038  // with internal gearing11/
int X_MOTOR_STEPS   = 64;  //  5.625 degree per step

*/

/* HP J45D52HT
float Y_STEPS_PER_INCH = 96;
float Y_STEPS_PER_MM  =  3.779527559  //is wrong  80;
int Y_MOTOR_STEPS   = 200;  //  1.8 degree per step
*/


//Navaho windstrument
String targetFileName = "dilni.ngc";
bool isReadyFlag = true; //false;      // turns-off.on the foutput() after reading G20
bool isMachZeroFlag = false;  // false until machine completes zero opn



/* Airpax  	Sensata-Airpax
 * Sensata-Airpax stepper motor A82454-M3 specification sheet
 * Series 57L048B, 57M024B & 57M048B Stepper Motors
 * 7.5 degree per step
 * 48 steps per rev
 * 1706.67 steps per inch
 * 10 inches = 17,066.67 steps
 *
 * see
 * 1> 200429_pps-rpm
 * 2> https://www.google.com/search?client=ubuntu&channel=fs&q=1706.67+steps+per+inch&ie=utf-8&oe=utf-8#kpvalbx=_T_W5XoPKF8qxgge-lqH4Dw35
 * 3> https://www.youtube.com/watch?v=6vY26CIrVwg
 * show how to calculate steps per inch
 * (48 steps/rev)/(1.03358311 in/rev) =  46.4 steps per inch
 * 48 steps per rev Thompson catalog
 * 14 teeth on sprocket
 * sprocket dia is .329 in circumference is .329 x PI = 1.03358311 in/rev
 */

float minXlocation = -10;   // inches on workplate along x-axis see dilni.ngc
float maxXlocation = 0;     // inches on workplate along x-axis see dilni.ngc

//Assumes all four steppers A82454-M3 = 57L048B
float STEPS_PER_INCH = 46.4;     //steps per inch
float PPS = 320;                  // MOTOR_STEPS  pulses per second
float STEPS_PER_DEG = .13333333;  // 1/7.5
float STEPS_PER_REV = 48;
float BADNbr = 9999.82437156;              // something smells

float X_STEPS_PER_INCH =  STEPS_PER_INCH;
float X_STEPS_PER_MM = BADNbr;
int X_MOTOR_STEPS   = PPS;       //7.5 degree per step


float Y_STEPS_PER_INCH = STEPS_PER_INCH;
float Y_STEPS_PER_MM = BADNbr+1;
int Y_MOTOR_STEPS   = PPS;       // 7.5 degree per step



float Z_STEPS_PER_INCH = STEPS_PER_INCH;
float Z_STEPS_PER_MM = BADNbr+5;
int Z_MOTOR_STEPS   = PPS;       // 7.5 degree per step

//200429_pps-rpm
//degree per step = 7.5
//steps per degree = 0.133333333

float A_STEPS_PER_INCH = STEPS_PER_INCH;
float A_STEPS_PER_MM = BADNbr+9;
float A_STEPS_PER_DEG = STEPS_PER_DEG  ;
int A_MOTOR_STEPS   = PPS;  // pps ==>> 7.5 deg/step recal 200429

float B_STEPS_PER_INCH = STEPS_PER_INCH;
float B_STEPS_PER_MM = BADNbr + 13;
float B_STEPS_PER_DEG = STEPS_PER_DEG ;
int B_MOTOR_STEPS   = PPS;  //  7.5 deg/step  or 48 steps to rev
                            // confirmed by Thomson catalog spec

/*
 * The MAXIMUM FEEDRATE is calculated to be 20,000
 * see the Min/Max given near the end of the file
 */ 
//our maximum feedrates
long FAST_XY_FEEDRATE = 2000;
long FAST_Z_FEEDRATE = 2000;
long FAST_A_FEEDRATE = 2000;
long FAST_B_FEEDRATE = 2000;


// Units in curve section
//180804_tr may need to adjust for I and J
#define CURVE_SECTION_INCHES 0.019685
#define CURVE_SECTION_MM 0.5

// Set to one if sensor outputs inverting (ie: 1 means open, 0 means closed)
// RepRap opto endstops are *not* inverting.
int SENSORS_INVERTING = 1;

/****************************************************************************************
* digital i/o pin assignment
*
* this uses the undocumented feature of Arduino - pins 14-19 correspond to analog 0-5
****************************************************************************************/
// 200425 do not understand this and Adafruit Motor COntrol v2.3


// See 200428 note on LinuxCNC simulator *.ini
// Operating envelope where the limit swtichs  connect
float X_MIN_LIMIT = -2; //  inch
float X_MAX_LIMIT = 20;
float Y_MIN_LIMIT = -2; // inch
float Y_MAX_LIMIT = 20;


float A_MIN_LIMIT = 0.0;  //degree
float A_MAX_LIMIT = 90.0;
float B_MIN_LIMIT = 0.0;  //degree
float B_MAX_LIMIT = 90.0;

//Assigned by AMS and Stepper Motor
int X_DIR_PIN = 1;   //1/0 for +/-
int Y_DIR_PIN = 1;
int A_DIR_PIN = 1;
int B_DIR_PIN = 1;



// our point structure to make things nice.
struct LongPoint {
  long x;
  long y;
  long z;
  long a;
  long b;
  long i;
  long j;
};

typedef struct  {
  float x;
  float y;
  float z;
  float a;
  float b;
  float i;
  float j;
} FloatPoint_type;

  //init baby!
  FloatPoint_type fp; // gcode command string from g0, g1, g2, g3
  //fp.x derived from GCode X input
  //fp.y derived from GCode Y input
  //fp.z derived from GCode Z input


FloatPoint_type current_loc;     //current location
FloatPoint_type current_locus;   // current_locus.x = fp.x derived from ngc X value
FloatPoint_type target_locus;    // set_target(current_locus.x + fp.x,.....
FloatPoint_type delta_units;     // absolute diff between target_locus and current_locus elements

FloatPoint_type current_stepsknt;
FloatPoint_type target_stepsknt;
FloatPoint_type delta_steps;

boolean abs_mode = false;   //0 = incremental; 1 = absolute

//poor use of term "units"
// x/y/x_steps2in ==> stepsPerInch of linear movement
// a/b_steps2deg   ==> stepsPerDegree of rotation
//default to inches for units
float x_steps2in = X_STEPS_PER_INCH;  //X_STEPS_PER_MM;
float y_steps2in = Y_STEPS_PER_INCH;  //Y_STEPS_PER_MM;
float z_steps2in = Z_STEPS_PER_INCH;  //Z_STEPS_PER_MM;
float a_steps2deg = A_STEPS_PER_DEG;  //A_STEPS_PER_MM;
float b_steps2deg = B_STEPS_PER_DEG;  //B_STEPS_PER_MM;


float curve_section = CURVE_SECTION_MM;

//our direction vars
byte x_direction = 1;
byte y_direction = 1;
byte z_direction = 1;
byte a_direction = 1;
byte b_direction = 1;

//our feedrate variables.
float feedrate = 0.0;
long feedrate_micros = 0;

float extrude = 0.0;


//Dev
/*
 * kntX is used to track step movement
 * forwardstep1()  ==>> kntX++
 * backwardstep1() ==>> kntX--
 */


int kntX = 0;
int kntY = 0;
int kntZ = 0;
int kntA = 0;
int kntB = 0;



//init our variables
long max_delta;
long x_counter;
long y_counter;
long z_counter;
long a_counter;
long b_counter;


bool x_can_step;
bool y_can_step;
bool z_can_step;
bool a_can_step;
bool b_can_step;
int milli_delay;




//For dev 170331
#define FORWARD 1
#define BACKWARD 2
//201006
//201006
#define XCARE -1


#define MICROSTEP 4



#define VERSION        (1)  // firmware version
#define BAUD           (9600)  // was 11How fast is the Arduino talking?
#define MAX_BUF        (64)  // What is the longest message Arduino can store?
#define STEPS_PER_TURN (48)  //(320)  // depends on your stepper motor.  most are 200.
//#define MIN_STEP_DELAY (50.0) // begins to vibrate not step
//#define MIN_STEP_DELAY (2000)//works but begins to show signs of jumping/skipping
#define MIN_STEP_DELAY (5000) // (10) (100) (250) (500)(1000) flakie
                              // (1500)(2000)
                              //(3000) works well with great rev rate.
//                              Without concern for leadscrew
//                              4min 26sec to cycle startup and one loop of dilni.ngc
//                              13.86 10-inch strokes per hour
//#define MIN_STEP_DELAY (4000) //works
//#define MIN_STEP_DELAY (10000)//works
//#define MIN_STEP_DELAY (20000)//works
#define MAX_FEEDRATE   (1000000.0/MIN_STEP_DELAY)
#define MIN_FEEDRATE   (0.01)


#define COMMAND_SIZE 128



// for arc directions
#define ARC_CW          (1)
#define ARC_CCW         (-1)
// Arcs are split into many line segments.  How long are the segments?
#define MM_PER_SEGMENT  (10)

int sderrFlg = -1;  // -1: XCare 0: okay  1: Err state


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------
//ams1
extern void m1step(int dir);
extern void m2step(int dir);
extern void m3step(int dir);
extern void disable();
extern void setup_controller();
extern void run_steppers();
extern void init_steppers(); //ams1


//ams2
//extern void setSpeedjs(int inSpeed);
extern void setDirection(int rawx, int rawy);
extern void disable_dcm();
extern int getVehiclePedal();


extern void parse_cmdln();
extern void init_process_string(); //process_string

//------------------------------------------------------------------------------
// Wifi METHODS
//------------------------------------------------------------------------------
extern void wifisetup();
extern void wifiloop();
extern void close_udp();
extern bool checkForUDPactivity();
extern void reset_is_UDP_active();
extern bool Need2Killwifi();


/**
* This file is part of GcodeCNCDemo.
*
* GcodeCNCDemo is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* GcodeCNCDemo is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Foobar. If not, see <http://www.gnu.org/licenses/>.
*/
#endif /* A038_CONFIG_H_ */

