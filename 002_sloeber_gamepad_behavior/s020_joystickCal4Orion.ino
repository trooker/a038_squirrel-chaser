/* Copyright © 1988-2020 by Abbott Analytical Products. All Rights Reserved.
 * 200922_tr Added the 3,4,5,7,8,9 cal runs.  Values are still way off the
 *           -500 to 500 range of the MeJoystick
 * 200921_tr Implemented switch control flow in loop. cal_1,2,6 run well
 *           Test on others soon.
 * 200920_tr Added calibration test points 1..9.
 * 200919_tr Added button to control the flow and reordered calibration
 *           to inside the loop().
 * 200918_tr Minor tweaks to Poel's work to allow for MeBlock Orion UNO
 *           and MeJoystick.  See MeJoystick.h/cpp and MePort.h/cpp
 *           for details.
 *           Added variables to adjust min/max
 * Port 8 of Makeblock Orion (UNO) Arduino
 * MePort.h
 * MePort.cpp
 * Y:  Defines int16_t aRead1(void)  Analog value from 0-1023
 * X:  Defines int16_t aRead2(void)  Analog value from 0-1023
 *     Poel uses
 *     s2  //; A1    //joystick  X-as
 *     s1  //; A0    //joystick  Y-as
 *  MeJoystick.h
 * MeJoystick.cpp <NOTE: Replaced "-" with "+" on positive
 *  x_value = analogRead(_X_port); value from(-500 to +500)
 *  y_value = analogRead(_Y_port); value from(-500 to + 500)
 *
 *
 */
#include "Arduino.h"


//#include "MeSerial.h"
#include "MeOrion.h"
#include "MePort.h"

// https://www.instructables.com/id/Easy-Analog-Joystick-Calibration-and-Centering/
//  **********************************************************************//
//  author: Joram de Poel                                                 //
//  date:   8-7-2016                                                      //
//  version: 1.3                                                          //
//  email: joramdepoel@gmail.com                                          //
//  **********************************************************************//
//
//  This code is free to use, edit and distrubate as long as you use it for non profit projects
//
//  this code is writen to use analog joysticks in the easiest way possible and with the most options
//  this code deals with ofsets, limited range, inverted axis and many other problems with joysticks
//  this code uses the map_float function, if you didnt already include this in your arduino core,
//  just uncomment the function at the bottom

//*****************define your pins here***********************************************//

//Orion UNO Port 8
//See MeJoystick.h/cpp and MePort.h/cpp
// s1 = mePort[port].s1;
// s2 = mePort[port].s2;

#define joystick_X mePort[8].s2  //; A1    //joystick  X-as
#define joystick_Y mePort[8].s1  //; A0    //joystick  Y-as
#define button_2 2       // button on d2


MePort btninput(PORT_6);           //Setup for RJ25 Adapter button
                                //Using slot 1
                                //Slot 2 held for future use

MeSerial mSerial;   //maps to default PORT D
                    //See MeSerial.h  of makeblock library. It appears to

int speed = 0;
int btnVal = 64;  //@


//const long anp_xmin = -512;  //-512;
//const long anp_xmax =  512; //1023;  //512;
//const long anp_ymin = -512;  //-512;
//const long anp_ymax =  512; //1023;  //512;
//const long anp_ycenter = 0;

//const long jsMIN = -512;
//const long jsMAX =  512;
//const long jsRANGE = 1023;


//****************define the min, max and center you want for your joystick here*******//
//long range_X_min = anp_xmin;  // 1000;           //lowest X value
//long range_X_max = anp_xmax;  //10000;          //highest X value
//long range_X_center = 0;           //X center calue
//unsigned long deadZone_X = 1;      //deadzone X, return center value for center +- deadzone(in steps of 1/jsRANGE)

//float range_Y_min = anp_ymin;      //0;             //lowest Y value
//float range_Y_max = anp_ymax;      //1023;          //highest Y value
//float range_Y_center = anp_ycenter; // 512;        //Y center value
//unsigned long deadZone_Y = 1;      //deadzone Y, return center value for center +- deadzone(in steps of 1/jsRANGE)


//***************calibration variables************************************************//
int val_X, val_Y;

unsigned long X_CENTER, X_MIN, X_MAX;
unsigned long Y_CENTER, Y_MIN, Y_MAX;
unsigned long jsx_1, jsx_2, jsx_3, jsx_4, jsx_5, jsx_6,jsx_7, jsx_8, jsx_9;
unsigned long jsy_1, jsy_2, jsy_3,jsy_4, jsy_5, jsy_6, jsy_7, jsy_8, jsy_9;


unsigned long cal_X;
unsigned long cal_Y;

unsigned long wrk_cal_x;
unsigned long wrk_cal_y;


void cal_pos()
{
	  cal_X = 0;
	  cal_Y = 0;
	  delay(2500);
	  Serial.print("calibrating position");

	  for (int i = 0; i < 100; i++) {
	    Serial.print(".");
	    cal_X += analogRead(joystick_X);
	    delay(5);
	    cal_Y += analogRead(joystick_Y);
	    delay(5);
	  }
	  wrk_cal_x = (cal_X/100);
	  wrk_cal_y = (cal_Y/100);

}


void displayCalValue(int inX, int inY)
{
	  Serial.print("\n X: ");
	  Serial.print(inX);
	  Serial.print(" Y: ");
	  Serial.println(inY);

}


void displayCalValues(int inX, int inY)
{
      Serial.print("   Android Input: ");
	  Serial.print(inX);
	  Serial.print("_");
	  Serial.print(inY);
	  Serial.print("_999_@");
}


void cal_1()
{

	  cal_pos();
	  jsx_1 = wrk_cal_x;
	  jsy_1 = wrk_cal_y;
	  X_CENTER = jsx_1;
	  Y_CENTER = jsx_1;
	  Serial.print("\nCorrection X: ");Serial.print(X_CENTER);
	  Serial.print("\nCorrection Y: ");Serial.println(Y_CENTER);
}

void cal_2()
{
	  X_MIN = 0;    //reset the values
	  Y_MIN = 0;
	  cal_pos();
	  jsx_2 = wrk_cal_x;
	  jsy_2 = wrk_cal_y;
	  X_MIN = jsx_2;
	  Y_MIN = jsy_2;
	  Serial.println();
	  Serial.print("minX: "); Serial.print(X_MIN);
	  Serial.print("            minY: "); Serial.println(Y_MIN);

}



void cal_3()
{
	  cal_pos();
	  jsx_3 = wrk_cal_x;
	  jsy_3 = wrk_cal_y;
	  displayCalValue(jsx_3, jsy_3);

}



void cal_4()
{
	  cal_pos();
	  jsx_4 = wrk_cal_x;
	  jsy_4 = wrk_cal_y;
	  displayCalValue(jsx_4, jsy_4);

}


void cal_5()
{
	  cal_pos();
	  jsx_5 = wrk_cal_x;
	  jsy_5 = wrk_cal_y;
	  displayCalValue(jsx_5, jsy_5);

}


void cal_6()//6
{
	  X_MAX = 0;    //reset the values
	  Y_MAX = 0;
	  cal_pos();
	  jsx_6 = wrk_cal_x;
	  jsy_6 = wrk_cal_y;
	  X_MAX = jsx_6;
	  Y_MAX = jsx_6;
	  Serial.println();
	  Serial.print("maxX: "); Serial.print(X_MAX);
	  Serial.print("          minY: "); Serial.println(Y_MAX);

}


void cal_7()
{
	  cal_pos();
	  jsx_7 = wrk_cal_x;
	  jsy_7 = wrk_cal_y;
	  displayCalValue(jsx_7, jsy_7);

}

void cal_8()
{
	  cal_pos();
	  jsx_8 = wrk_cal_x;
	  jsy_8 = wrk_cal_y;
	  displayCalValue(jsx_8, jsy_8);

}

void cal_9()
{
	  cal_pos();
	  jsx_9 = wrk_cal_x;
	  jsy_9 = wrk_cal_y;
	  displayCalValue(jsx_9, jsy_9);
}



bool checkBtn()
{
	int val = 0;
	bool statFlg = false;
	val = btninput.dRead1();   /* read SLOT1 level */
	if (val == HIGH)
    {
        statFlg = true;
     } //ifpressed
     return statFlg;
}


void showCalInstructions()
{
	Serial.println("Place and hold the joystick in the top-right corner");
	Serial.println(	"Press when ready.   Wait for measurement to record.");

}

void calibrate() {

    bool calFlg = false;

    Serial.println("\nCommencing Calibration of the Joystick\n");
/* */
	Serial.println("\nStep 1: Center Position.");
	showCalInstructions();
	while (!calFlg )
	{
		calFlg = checkBtn();
		delay(100);
	}
	cal_1();

	calFlg = false;
	Serial.println("\nStep 2: Bottom-Left Corner. ");
	showCalInstructions();
	while (!calFlg )
	{
		calFlg = checkBtn();
		delay(100);
	}
	cal_2();
/*  */
	calFlg = false;
	Serial.println("\nStep 3: Bottom Center Edge. Press when ready");
	showCalInstructions();
	while (!calFlg )
	{
		calFlg = checkBtn();
		delay(100);
	}
   cal_3();

/* */
//Begin addons
   calFlg = false;
	Serial.println("\nStep 4: Bottom Right Corner Position.");
	showCalInstructions();
	while (!calFlg )
	{
		calFlg = checkBtn();
		delay(100);
	}
	cal_4();

	calFlg = false;
	Serial.println("\nStep 5: Center Right Edge. ");
	showCalInstructions();
	while (!calFlg )
	{
		calFlg = checkBtn();
		delay(100);
	}
	cal_5();
/* */
	calFlg = false;
	Serial.println("\nStep 6: Top Right Corner. Press when ready");
	showCalInstructions();
	while (!calFlg )
	{
		calFlg = checkBtn();
		delay(100);
	}
    cal_6();


	calFlg = false;
	Serial.println("\nStep 7: Top Center Position.");
	showCalInstructions();
	while (!calFlg )
	{
		calFlg = checkBtn();
		delay(100);
	}
	cal_7();


	calFlg = false;
	Serial.println("\nStep 8: Top-Left Corner. ");
	showCalInstructions();
	while (!calFlg )
	{
		calFlg = checkBtn();
		delay(100);
	}
	cal_8();

	calFlg = false;
	Serial.println("\nStep 9: Left Center. Press when ready");
	showCalInstructions();
	while (!calFlg )
	{
		calFlg = checkBtn();
		delay(100);
	}
    cal_9();


//End addons
/*  */


  if(X_MAX < X_MIN)
  {
    unsigned long val = X_MAX;
    X_MAX = X_MIN;
    X_MIN = val;
  }

 if(Y_MAX < Y_MIN){
    unsigned long val = Y_MAX;
    Y_MAX = Y_MIN;
    Y_MIN = val;
  }




  Serial.print("\nrange X  from: ");
  Serial.print(X_MIN);
  Serial.print(" to ");
  Serial.println(X_MAX);
  Serial.print("range Y  from: ");
  Serial.print(Y_MIN);
  Serial.print(" to ");
  Serial.println(Y_MAX);

  Serial.println("\n---calibration done---\n");
/*
  Serial.println("Android will handle an input looking like this:");
  Serial.println(  "         xxx_yyy_mph_@") ;

  displayCalValues(jsx_1,jsy_1);
  displayCalValues(jsx_2,jsy_2);
  displayCalValues(jsx_3,jsy_3);
  displayCalValues(jsx_4,jsy_4);
  displayCalValues(jsx_5,jsy_5);
  displayCalValues(jsx_6,jsy_6);
  displayCalValues(jsx_7,jsy_7);
  displayCalValues(jsx_8,jsy_8);
  displayCalValues(jsx_9,jsy_9);
 */


  Serial.println("\n\nAfter reviewing the results press the button to continue.");
  calFlg = false;
  while (!calFlg )
	{
		calFlg = checkBtn();
		delay(100);
	}




}


//some people added this function to the Arduino core, comment this if it's already in the Arduino core
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}



//************setup******************************************************************//

/*
long joy_X(long X) {
  X += (X_CENTER-jsMAX);   //should always be 512
  if (X <= jsMAX) {
    X = mapfloat(X, X_MIN, X_CENTER, range_X_min, range_X_center);   //min,center to range min, range center
    return X;
  }
  else if (X > jsMAX) {
    X = mapfloat(X, jsMAX, jsRANGE + X_CENTER, range_X_center, range_X_max);
    return X;
  }
}

long joy_Y(long Y) {
  Y += (Y_CENTER-jsMAX);
  if (Y <= range_Y_center) {
    Y = mapfloat(Y, Y_CENTER, jsMAX, 0, jsMAX);
    return Y;
  }
  else if (Y > range_Y_center) {
    Y = mapfloat(Y, jsMAX, jsRANGE + Y_CENTER, jsMAX, jsMAX);
    return Y;
  }
}

void set_range_X(float min, float max, float center) {
  range_X_min = min;
  range_X_max = max;
  range_X_center = center;
}

void set_range_Y(float min, float max, float center) {
  range_Y_min = min;
  range_Y_max = max;
  range_Y_center = center;
}
*/


void setup() {

  Serial.begin(9600);
  pinMode(joystick_X, INPUT);
  pinMode(joystick_Y, INPUT);
  pinMode(button_2, INPUT_PULLUP);    //we pull this pin high to avoid a floating pin
//  calibrate();
}


int kntr = 0;
int stateFlg = 0; //0: throw first message
int changeFlg = 0;
int const STARTER = 0;
int const CALFINA = 1;
int const GOODBYE = 5;
int const IDLING = 9999;
int const XCARE  = 9;
int calmFinaMsg = 0;
int const maxkntr = 100;

int liveOrDie(int inFlg)
{
  int testFlg;

  if ((kntr < maxkntr))// && (stateFlg != GOODBYE))
  {
	  testFlg = inFlg;
      if ((digitalRead(button_2) == LOW) || (checkBtn()) && inFlg != GOODBYE)
      {
    	 Serial.println("\n\nUser requested a pass through the calibration cycle.");
         calibrate();    //you can call any function you want here, this is just to show the possibilities
         testFlg = CALFINA;
      }
  }
  else //if ((kntr == maxkntr) || (maxkntr - kntr == 0) || (kntr > maxkntr))
  {switch (inFlg)
  {
  case GOODBYE:
     testFlg = XCARE;
     break;
  case IDLING:
	  testFlg = GOODBYE;
	  break;
  default:
	  testFlg = GOODBYE;
	  break;
  } //end of short switch
  }
  return testFlg;
}



void loop()
{
	switch (stateFlg)
	{
	case IDLING:
 		Serial.print(maxkntr - kntr);
 		Serial.print(" :kntr      ");
 		Serial.println("Count-down to 0. Press button to start another cycle of calibration.");
 		break;
	case STARTER:
	{
		Serial.println("\n\nPress the joystick button or the button attached to Port 8 slot1");
        Serial.println("shortly after the slight delay and as the kntr message becomes active.");
		delay(2000);
		   for (int i = 0; i < 100; i++)
		   {    //take 100 readings
		    Serial.print(".");
		   }
		stateFlg = IDLING;
		break;
	}
	case CALFINA:
		kntr = 0;
		stateFlg = IDLING;
		Serial.println("\n\nAfter reviewing the results press the button to continue.");
		break;
	case GOODBYE:
		stateFlg = XCARE;

//		Serial.print(stateFlg);
//		Serial.println("  stateFlg");
		break;
	case XCARE:
		if (calmFinaMsg == 0)
		{
			Serial.println("\n\n\n\n......Calibration done,  Good-bye......");
			calmFinaMsg++;
		}
	    break;
	default:
		break;
	} //end of switch (stateFlg)
	// Check to see if user wants to calibrate or quit.
	if (stateFlg == GOODBYE)
	 {stateFlg = XCARE;}
	stateFlg = liveOrDie(stateFlg);
	kntr++;

//Do not need
//  val_X = analogRead(joystick_X);
//  Serial.print(joy_X(val_X)); Serial.print(" : ");
//  delay(50);
//  val_Y = analogRead(joystick_Y);
//  Serial.println(joy_Y(val_Y));
 // delay(50);


}



