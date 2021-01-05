/* Copyright © 1988-2021 by Abbott Analytical Products. All Rights Reserved.
 * 210102_tr Removed extraneous direction msg.
 * 201230_tr Removed extraneous comments and commented out others
 * 201227_tr Restart the integrate again.
 * 201226_tr Tried to integrate s035.  Created a real mess
 * 201222_tr Correct soft right/left
 * 201213_tr Integrated s031_mapAzimuth2seg.ino
 * 201121_tr Restart effort.  Remove troubleshhoting serial_msg
 * 201009_tr Correct map_turntime using motor instead of "2"
 * 201007_tr Added getvehiclepedal() and set ams2_SPEED to 125;
 *           altered setup_dcm() to use vehiclePedal
 * 201006_tr Incorporate roadtest observations on turning speed and settling.
 *           See adafruit DCMotor example and pieslice segments.
 * 200928_tr Revisit based upon road-testing.  Use the incremental pie sections to reduce
 *           delay and improve divergent behavior:  -x/-y read as positive quad,
 * 200926_tr Special case setSpeed when input speed is 0.
 * 200915_tr atan2h may be wrong y and x should be flipped to obtain same results as
 *           spreadsheet.
 * 200914_tr This file deals primarily with the dc motors run in parallel
 *           to drive a pair of tracks for steering.
 * 200604_tr Adapted the ams2.ino of a034_ project for a038
 *           replacing the steppers with dc motors
 *           dcmotor1 and dcmotor2.
 *
 */


#include "a038_config.h"


int ams2_SPEED = 125;  //swag at starting spped of both motors
int vehiclePedal = ams2_SPEED;
int mapRawSpeed = 0;   //until user inputs received.


 int dcmotor1_SPEED = vehiclePedal;
 int dcmotor2_SPEED = vehiclePedal;

int dcHold = 400;  // delay in halting
                   // 100  failed
                   // 1000 ok
                   // 500, 400 good
                   // 250, 300, 350 marginal
int dcShort = 10;   // Derived from Adafruit example for DCMotors
const int deadZone = 10;
const int deadMin  = -deadZone;
const int deadMax  = deadZone;
//Makeblock Joystick
//Ranges -500 to 500 or 1000
//MeJoystick range
const int RangeMin = -500;
const int RangeMax = 500;   // -/+
//Backward MeDCMotor
// NOT USED const int MotorSpeedMIN = -255;     // -255....0
//Forward MeDCMotor
const int MotorSpeedMax = 255;     // 0 .. 255

int azimuthsegment = 0;  // 1..24 segments assigned

int directionFlag = XCARE;   // constrain(dirFlg, XCARE, BACKWARD);
                             //-1: XCARE,   0: Not used,
                             // 1: FORWARD, 2: BACKWARD
int soundAMS2Knt = 0;
const int soundAMS2Stopper = 3;
int buzzKnt4ams2 = 0;        // Switch for shave & haircut
int buzzOff4ams2 = 1;



void soundOfAMS2()
{
if ((buzzKnt4ams2 < buzzOff4ams2) && (soundAMS2Knt < soundAMS2Stopper))
{
  //not needed for spike   soundBuzz3();
   buzzKnt4ams2++;
 }
 soundAMS2Knt ++;
}

const int delay4_90 =2000;


void releaseMotors()
{
    delay(delay4_90);
	dcmotor1->run(RELEASE);   //testing
	dcmotor2->run(RELEASE);   //testing
}


void chaseForward()
{
//	Serial.println("chase_Forward()");
	directionFlag = FORWARD;
	dcmotor2->run(FORWARD);
	dcmotor1->run(FORWARD);
	releaseMotors();
}

void chaseBackward()
{
//	Serial.println("chase_Backward()");
	directionFlag = BACKWARD;
	dcmotor1->run(BACKWARD);
	dcmotor2->run(BACKWARD);
	releaseMotors();


}




void chaseDCM2() //90 degrees right
{
	dcmotor1->run(FORWARD);
	dcmotor2->run(RELEASE);
    delay(delay4_90);
	dcmotor1->run(RELEASE);   //testing
}

void chaseDCM1() //90 degrees left
{
	dcmotor1->run(RELEASE);
	dcmotor2->run(FORWARD);
    delay(delay4_90);
    dcmotor2->run(RELEASE);  //for test

}


void chaseStop()
{
  directionFlag = XCARE;
   dcmotor1->run(RELEASE);
   dcmotor2->run(RELEASE);
}


/* By Joystick
 * Begin Tank Differntial Sterring */

void chase_RightDCM_Forward()
{
//	Serial.println("chase_RightDCM_Forward()");
	dcmotor1->run(RELEASE);
	dcmotor2->run(FORWARD);
    delay(delay4_90/2);
	dcmotor2->run(RELEASE);

}

void chase_LeftDCM_Forward()
{
//	Serial.println("chase_LefttDCM_Forward()");
	dcmotor1->run(FORWARD);
	dcmotor2->run(RELEASE);
    delay(delay4_90/2);
	dcmotor1->run(RELEASE);

}


void chase_LeftDCM_Backward()
{
//	Serial.println("chase_LeftDCM_Backward()");
	dcmotor1->run(BACKWARD);
	dcmotor2->run(RELEASE);
    delay(delay4_90/2);
	dcmotor1->run(RELEASE);

}

void chase_RightDCM_Backward()
{
//201121	Serial.println("chase_RightDCM_Backward()");
	dcmotor1->run(RELEASE);
	dcmotor2->run(BACKWARD);
    delay(delay4_90/2);
	dcmotor2->run(RELEASE);

}



/*
 * End Drag wheel sterring
 */

int getVehiclePedal()
{
	return vehiclePedal;
}

void setMappedRawSpeed()
{
	mapRawSpeed = getVehiclePedal();
//    Serial.print(mapRawSpeed);
//    Serial.println(".....:: mapRawSpeed");

}

int getMappedRawSpeed()
{
	return mapRawSpeed;
}

void  resetVehiclePedal()
{
	vehiclePedal = getMappedRawSpeed();
}





void setUserDefinedInputSpeed4Vehicle(int inSpeed)
{
/* https://www.arduino.cc/reference/en/language/functions/math/map/
 * map(value, fromLow, fromHigh, toLow, toHigh)
Parameters

value: the number to map.
fromLow: the lower bound of the value’s current range.
fromHigh: the upper bound of the value’s current range.
toLow: the lower bound of the value’s target range.
toHigh: the upper bound of the value’s target range.
 *
 */
      vehiclePedal = map(inSpeed, 0, 1024, 0, MotorSpeedMax);
 //     Serial.print(vehiclePedal);
 //     Serial.println(".....::vehiclePedal post mapping");
      setMappedRawSpeed();
	//setSpeed4Motor(vehiclePedal, 9);
}



void setSpeed4Motor(int inSpeed, int motor)
{
//	int jsMinSpeed = RangeMin;
//	int jsMaxSpeed = RangeMax;    // -/+500  ranges
//	int dcMmin     = 0;
//	int dcMmax     = MotorSpeedMax;
    int setVelocity = 0;
//200926
   if (inSpeed <= 15)
		{
	        inSpeed = setVelocity;
//			dcmotor1  = inSpeed;
//			dcmotor2 = inSpeed;
		}
   setVelocity = inSpeed;
   switch (motor)
   {
   case 1: dcmotor1 ->setSpeed(setVelocity);  break;
   case 2: dcmotor2 ->setSpeed(setVelocity); break;
   default:
   	dcmotor1 ->setSpeed(inSpeed);
   	dcmotor2 ->setSpeed(inSpeed);
   	break;
   }
//   Serial.print(getVehiclePedal());
//   Serial.print("  << vehicle pedal     inSpeed = setVelocity ==> ");
//   Serial.print(setVelocity);
//   Serial.print("    motor:");
//   Serial.println(motor);

}



/*
* NOTE: 201226_1020
* See https://www.medcalc.org/manual/atan2_function.php
* Added (xx <= 0) and (yy < 0)  was (xx < 0)
* Now chases_Hard_Right
*/
double vector_analysis(int xx, int yy)
{
	double angle = 0.0 ;  // as azimuth off forward line from origin of motion
	const double rad2deg = 180.0/PI;
	if (xx > 0)
	   {angle = atan2(double(yy), (double) xx) * rad2deg;}
	else if ((xx < 0) && (yy >= 0))
	   {angle = (atan2(double(yy), (double) xx))* rad2deg;}
	else if ((xx <= 0) && (yy < 0))   //201225
	   {angle = (atan2(double(yy), (double) xx))* rad2deg;}
	else if ((xx == 0) && (yy > 0))
		{angle = (PI/2) * rad2deg;}
	else if ((xx == 0) && (yy == 0))
		{angle = 999;} //basically undefined.

//	Serial.print(angle);
//	Serial.println(" ::.....atan2h angle derived");
	return angle;
}

void chaseLeft_Forward_Backward()
{
    switch(directionFlag)
    {
        case FORWARD:
        	chase_LeftDCM_Forward();
        	break;
        case BACKWARD:
        	chase_LeftDCM_Backward();
        	break;
        case XCARE:
        	setSpeed4Motor(0 ,9);
        	chaseStop();
        	    break;
    }

}



void chaseRight_Forward_Backward()
{
    switch(directionFlag)
    {
        case FORWARD:
        	chase_RightDCM_Forward();
        	break;
        case BACKWARD:
        	chase_RightDCM_Backward();
        	break;
        case XCARE:
        	setSpeed4Motor(0 ,9);
        	chaseStop();
        	    break;
    }

}

/* disregard
void map_turntime(int motor, int segpass)
		{
			int indx = 0;
			int indxMax = 4;
		    int mph = 0;
          switch (motor)
	      {
          case 1:
    		  for (indx=0; indx<indxMax * segpass; indx++) {
    			    mph = (indx/(indxMax*segpass)) * vehiclePedal;
    				setSpeed4Motor(mph,motor);
        		    delay(dcShort);
    		  } //for motor 2 s
              break;
          case  2:
    		  for (indx=indxMax * segpass; indx!=0; indx--) {
    			    mph = (indx/(indxMax*segpass)) * vehiclePedal ;
    				setSpeed4Motor(mph,motor);
    		    delay(dcShort);
    		  }
    		  break;

		  }
		}
*/


/* Joystick range  Makeblock JS only
 * rawx ranges -500 to 0 to 500  by test/observe 200913
 * rawy ranges -500 to 0 to 500  by test/observe 200913
 * dead zone: -100 to 100 for both  //assumed
 *
 * calculations normalized to 100%
 * DCMotors States:
 * FORWARD
 * BACKWARD
 * RELEASE
 * Plus Speed control by motor 0 to 255
 */
const int js_deadspot = 200;     //Set deadspot for allowing joystick to return to rest
const int js_movement_pos =  js_deadspot;   //ignore change in steering if less than
const int js_movement_neg = -(js_deadspot);   //ignore change in steering if more than


void setDirection(int rawx, int rawy)
{
	int L = getVehiclePedal(); //DC motorspeed mapped to pedal
	int R = getVehiclePedal();
//	double azimuth;
//	const int rightMotor = 1;
//    const int leftMotor  = 2;
    if ((( rawx > js_movement_neg) && (rawx < js_movement_pos)) && (( rawy > js_movement_neg) && (rawy < js_movement_pos)))
	{
		setSpeed4Motor(0,9);
//		Serial.print(rawx);
//		Serial.print("  ...rawx          rawy.... ");
//		Serial.println(rawy);
//		Serial.println(" Box-Out Chaser Stopped.");
		chaseStop();


 	}
    else
    { // begin steering
/*
       reset vehicle speed from rawSpeed to original user
       defined velocity and map to vehiclePedal
*/
    	resetVehiclePedal();
    	setSpeed4Motor(getVehiclePedal(),9);
//		Serial.print(rawx);
//		Serial.print("  ...rawx          rawy.... ");
//		Serial.print(rawy);
//		Serial.print("  ......Speed..:: ");
//		Serial.println(getVehiclePedal());
    int tstAzimuth = (int)vector_analysis(rawx,rawy);
//    Serial.print(tstAzimuth);
//   Serial.println(" ::.....  tstAzimuth");
//	Serial.print("  ......Speed..:: ");
//	Serial.println(getVehiclePedal());
    switch (tstAzimuth)
    {
    case   1 ... 30:
    case -29 ... 0:
//	   Serial.println("-29 to 30 Forward");
       azimuthsegment = 1;
 		setSpeed4Motor(vehiclePedal,9);
		chaseForward();
      break;
    case 121 ... 180:
    case -179 ... -120:
//	   Serial.println("120 to -120  Backwards");
       azimuthsegment = 4;
	   setSpeed4Motor(vehiclePedal,9);
	   chaseBackward();
       break;
    case 61 ... 90:
    case 91 ... 120:
//	   Serial.println("61 to 120  Hard Left");
       azimuthsegment = 3;
   	   chaseDCM2();
		setSpeed4Motor(vehiclePedal,9);
		chaseForward();

       break;
    case -119 ... -90:
    case -89 ... -60:
//	   Serial.println("119 to -60  Hard Right");
       azimuthsegment = 5;
       chaseDCM1();
		setSpeed4Motor(vehiclePedal,9);
		chaseForward();
       break;
    case 31 ... 60:
//	   Serial.println("31 to 60  Soft Left");
       azimuthsegment = 2;
	   setSpeed4Motor(R,1);
	   //chase_LeftDCM_Forward();
	   chaseLeft_Forward_Backward();
		setSpeed4Motor(vehiclePedal,9);
		chaseForward();

       break;
    case -59 ... -30:
//	   Serial.println("59 to -30  Soft Right");
       azimuthsegment = 6;
	   setSpeed4Motor(L,2);
       chaseRight_Forward_Backward();
	   setSpeed4Motor(vehiclePedal,9);
	   chaseForward();
       break;
    }  // end of 6 segment switch
//    ****** Start of old clipped from here
    } // super else for switch
    //resume speed/direction  switch(directionFlag)
}







void disable_dcm()
{
	dcmotor1->run(RELEASE);
	dcmotor2->run(RELEASE);
}

void setup_dcm()
{
dcmotor1 ->setSpeed(vehiclePedal);
dcmotor2 ->setSpeed(vehiclePedal);
dcmotor1->run(FORWARD);
dcmotor2->run(FORWARD);
// turn on motor
disable_dcm();
delay(dcHold);
}



