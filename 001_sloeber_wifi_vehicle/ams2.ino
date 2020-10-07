/* Copyright © 1988-2020 by Abbott Analytical Products. All Rights Reserved.
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
 *           leftdcm1 and rightdcm2.
 *
 */

#include "a038_config.h"


int ams2_SPEED = 125;  //swag at starting spped of both motors
int leftdcm1_SPEED = ams2_SPEED;
int rightdcm2_SPEED = ams2_SPEED;
int dcHold = 400;  // delay in halting
                   // 100  failed
                   // 1000 ok
                   // 500, 400 good
                   // 250, 300, 350 marginal
int dcShort = 10;   // Derived from Adafruit example for DCMotors
int vehiclePedal = ams2_SPEED;
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
   soundBuzz3();
   buzzKnt4ams2++;
 }
 soundAMS2Knt ++;
}



void chaseForward()
{
	Serial.println("chase_Forward()");
	directionFlag = FORWARD;
	rightdcm2->run(FORWARD);
	leftdcm1->run(FORWARD);
    delay(dcHold);
}

void chaseBackward()
{
	Serial.println("chase_Backward()");
	directionFlag = BACKWARD;

	leftdcm1->run(BACKWARD);
	rightdcm2->run(BACKWARD);
    delay(dcHold);

}

void chaseDCM1() //90 degrees right
{
	Serial.println("chase_Right");

	leftdcm1->run(FORWARD);
	rightdcm2->run(RELEASE);
    delay(dcHold);

}

void chaseDCM2() //90 degrees left
{
	Serial.println("chase_Left");

	leftdcm1->run(RELEASE);
	rightdcm2->run(FORWARD);
    delay(dcHold);

}


void chaseStop()
{
   Serial.println("chase_Stop()");
  directionFlag = XCARE;

   leftdcm1->run(RELEASE);
   rightdcm2->run(RELEASE);
}


/* By Joystick
 * Begin Tank Differntial Sterring */


void chase_LeftDCM_Forward()
{
	Serial.println("chase_LeftDCM_Forward()");
	leftdcm1->run(FORWARD);  //Slowdown
	rightdcm2->run(FORWARD);
    //delay(dcHold);

}

void chase_RightDCM_Forward()
{
	Serial.println("chase_RightDCM_Forward()");
	leftdcm1->run(FORWARD);
	rightdcm2->run(FORWARD);   //Slowdown
    //delay(dcHold);

}


void chase_LeftDCM_Backward()
{
	Serial.println("chase_LeftDCM_Backward()");
	leftdcm1->run(BACKWARD);
	rightdcm2->run(BACKWARD);    //Slowdown
    //delay(dcHold);

}

void chase_RightDCM_Backward()
{
	Serial.println("chase_RightDCM_Backward()");
	leftdcm1->run(BACKWARD);    //Slowdown
	rightdcm2->run(BACKWARD);
    //delay(dcHold);

}



/*
 * End Drag wheel sterring
 */


void setSpeed4Motor(int inSpeed, int motor)
{
	int jsMinSpeed = RangeMin;
	int jsMaxSpeed = RangeMax;    // -/+500  ranges
	int dcMmin     = 0;
	int dcMmax     = MotorSpeedMax;
    int setSpeed = 0;
//200926
   if (inSpeed <= 15)
		{
	        inSpeed = setSpeed;
//			leftdcm1  = inSpeed;
//			rightdcm2 = inSpeed;
		}
   setSpeed = inSpeed;
   switch (motor)
   {
   case 1: leftdcm1 ->setSpeed(setSpeed);  break;
   case 2: rightdcm2 ->setSpeed(setSpeed); break;
   default:
   	leftdcm1 ->setSpeed(inSpeed);
   	rightdcm2 ->setSpeed(inSpeed);
   	break;
   }
   Serial.print(vehiclePedal);
   Serial.print("  << vehicle speed     inSpeed = setSpeed ==> ");
   Serial.println(setSpeed);
}

double directionVector(int px, int py)
{
	double square;
	double dpx, dpy;
	dpx = (double)px;
	dpy = (double)py;
	square = sq(dpx) + sq(dpy);
    return sqrt(square);
}



double vector_analysis(int xx, int yy)
{
	double angle, azimuth;
	const double rad2deg = 180.0/PI;
	if (xx > 0)
	   {angle = atan2(double(yy), (double) xx) * rad2deg;}
	else if ((xx < 0) && (yy >= 0))
	   {angle = (atan2(double(yy), (double) xx))* rad2deg;}
	else if ((xx < 0) && (yy < 0))
	   {angle = (atan2(double(yy), (double) xx))* rad2deg;}
	else if ((xx == 0) && (yy > 0))
		{angle = (PI/2) * rad2deg;}
	else if ((xx == 0) && (yy == 0))
		{angle = 999;} //basically undefined.

	Serial.print(angle);
	Serial.println(" ::.....atan2h angle derived");
	return azimuth = angle;
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


void setInsideTurn(int motor, int segpass)
		{
			int indx = 0;
			int indxMax = 4;
		    int mph = 0;

		  for (indx=0; indx<indxMax * segpass; indx++) {
			    mph = (indx/indxMax) * vehiclePedal;
				setSpeed4Motor(mph,2);
    		    delay(dcShort);
		  }
		  for (indx=indxMax * segpass; indx!=0; indx--) {
			    mph = (indx/(indxMax*segpass)) * vehiclePedal ;
				setSpeed4Motor(mph,2);
		    delay(dcShort);
		  }
		}



/* Joystick range
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
 *
 *
 *
 *
 */
void setDirection(int rawx, int rawy)
{
	int L = vehiclePedal; //DC motorspeed mapped to pedal
	int R = vehiclePedal;
	double azimuth;
	const int rightMotor = 1;
    const int leftMotor = 2;
	Serial.print(rawy);
	Serial.println(" :rawy value");
	Serial.print(rawx);
	Serial.println(" :rawx value");
	azimuth = vector_analysis(rawx,rawy);
	azimuthsegment = map(long(azimuth), -179, 180, 1,24  );

//Serial.print(azimuth);
//Serial.println(" :azimuth for Chaser");
Serial.print(azimuthsegment);
Serial.println(" :direction segment for Chaser");

    switch (azimuthsegment)
    {
    case 12:  // forward
		setSpeed4Motor(vehiclePedal,9);
		chaseForward();
		Serial.println(" Chaser Forward.");
    	break;
    case 1:   // reverse
	    setSpeed4Motor(vehiclePedal,9);
		chaseBackward();
    	break;
    case 6:   // hard right
    	chaseDCM1();

    	break;
    case 18:  // hard left
    	chaseDCM2();
    	break;
    case 13:  //left forward
		chase_LeftDCM_Forward();
		setSpeed4Motor(R,1);
		setInsideTurn(2,1);
    	break;
    case 14:
		chase_LeftDCM_Forward();
		setSpeed4Motor(R,1);
		setInsideTurn(2,2);
    	break;
    case 15:
		chase_LeftDCM_Forward();
		setSpeed4Motor(R,1);
		setInsideTurn(2,3);
    	break;
    case 16:
		chase_LeftDCM_Forward();
		setSpeed4Motor(R,1);
		setInsideTurn(2,4);
    	break;
    case 17:
		chase_LeftDCM_Forward();
		setSpeed4Motor(R,1);
		setInsideTurn(2,5);
    	break;
//**********
    case 19:  //left backward
//		chase_LeftDCM_Backward();
		chase_LeftDCM_Forward();
		setSpeed4Motor(R,1);
		setInsideTurn(2,7);
    	break;
    case 20:
//		chase_LeftDCM_Backward();
		chase_LeftDCM_Forward();
		setSpeed4Motor(R,1);
		setInsideTurn(2,8);

   	break;
    case 21:
//		chase_LeftDCM_Backward();
		chase_LeftDCM_Forward();
		setSpeed4Motor(R,1);
		setInsideTurn(2,9);
    	break;
    case 22:
//		chase_LeftDCM_Backward();
		chase_LeftDCM_Forward();
		setSpeed4Motor(R,1);
		setInsideTurn(2,10);
    	break;
    case 23:
//		chase_LeftDCM_Backward();
		chase_LeftDCM_Forward();
		setSpeed4Motor(R,1);
		setInsideTurn(2,11);
    	break;
    case 24:
//		chase_LeftDCM_Backward();
		chase_LeftDCM_Forward();
		setSpeed4Motor(R,1);
		setInsideTurn(2,12);
    	break;

//**********

    case 2:  //right backward
//		chase_RightDCM_Backward();
		chase_RightDCM_Forward();
		chaseRight_Forward_Backward();
		setSpeed4Motor(L,2);
		setInsideTurn(1,2);
    	break;
    case 3:
//		chase_RightDCM_Backward();
		chase_RightDCM_Forward();
		setSpeed4Motor(L,2);
		setInsideTurn(1,3);
    	break;
    case 4:
//		chase_RightDCM_Backward();
		chase_RightDCM_Forward();
		setSpeed4Motor(L,2);
		setInsideTurn(1,4);
    	break;
    case 5:
//		chase_RightDCM_Backward();
		chase_RightDCM_Forward();
		setSpeed4Motor(L,2);
		setInsideTurn(1,5);
    	break;



    case 7:     //right forward
		chase_RightDCM_Forward();
//		chase_LeftDCM_Forward();
		setSpeed4Motor(L,2);
		setInsideTurn(1,6);
    	break;
    case 8:
		chase_RightDCM_Forward();
		setSpeed4Motor(L,2);
		setInsideTurn(1,7);
    	break;
    case 9:
		chase_RightDCM_Forward();
		setSpeed4Motor(L,2);
		setInsideTurn(1,8);

    	break;
    case 10:
		chase_RightDCM_Forward();
		setSpeed4Motor(L,2);
		setInsideTurn(1,9);

    	break;
    case 11:
		chase_RightDCM_Forward();
		setSpeed4Motor(L,2);
		setInsideTurn(1,10);
    	break;


    default:
		setSpeed4Motor(0,9);
		Serial.println(" Chaser Stopped.");
		chaseStop();
    	break;
   }
    //resume speed/direction
    switch(directionFlag)
    {
        case FORWARD:
        	setSpeed4Motor(vehiclePedal ,9);
        	chaseForward();
        		break;
        case BACKWARD:
        	setSpeed4Motor(vehiclePedal ,9);
        	chaseBackward();
        		break;
        case XCARE:
        	setSpeed4Motor(0 ,9);
        	chaseStop();
        	    break;
    }
    setSpeed4Motor(vehiclePedal, 9);
	Serial.println("");
}



void setSpeed4Vehicle(int inSpeed)
{

    vehiclePedal = map(inSpeed, 0, 1024, 0, MotorSpeedMax);
	//setSpeed4Motor(vehiclePedal, 9);
    Serial.print(vehiclePedal);
    Serial.println(".....::vehiclePedal incoming");
}


int getVehiclePedal()
{
	return vehiclePedal;
}



void disable_dcm()
{
	leftdcm1->run(RELEASE);
	rightdcm2->run(RELEASE);
}

void setup_dcm()
{
leftdcm1 ->setSpeed(vehiclePedal);
rightdcm2 ->setSpeed(vehiclePedal);
leftdcm1->run(FORWARD);
rightdcm2->run(FORWARD);
// turn on motor
disable_dcm();
delay(dcHold);
}



