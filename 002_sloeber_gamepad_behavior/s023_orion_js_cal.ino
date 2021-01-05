/* Copyright © 1988-2020 by Abbott Analytical Products. All Rights Reserved.
 *
 * 201220_tr Continued work on adding the ability to use not only Orion but the mBot
 *           versions of Arduino UNO boards.  Continued to investigate the "flaky"
 *           MakeBlock Joystick attached to the MakeBlock Orion/mBot card.
 *           Discovered both Orion and mBot gave similar results.
 *                  Orion
 *           JS Rj25 8 excellent    jsx -13  jsy -11
 *                  mBot
 *           JS Rj25 4  excellent
 * 201212_tr This process only checks center, bottom/left, and top right.  You really need
 *           to establish those three plus center points along each edge of the js.
 *           .    .    .
 *                          for example
 *           .    +    . << center of the extreme right
 *
 *           .    .    .
 *           As such run the calibration to capture min/max for top bottom centers
 *           Then capture left/right center max values.  Note "0" values at centers.
 *
 *           See note for 200920 if using other than a Makeblock Orion style Arduino board.
 *           Added help() and retro-fitted button delay.  Set N_qty to 100 and mydelay to 100.
 * 201211_tr Rebuild from prior delete and backup copy
 *           Correct the unsigned long int which blows the mean values for all.  So changed to
 *           "long" integer since we can have both +/- for Orion Joystick values.
 *           instead.
 *           Almost seems to be working well enough to use.
 *           Added the switch case in the loop().
 * 200920_tr Borrowed from https://www.best-microcontroller-projects.com/arduino-joystick.html,
 *           https://lastminuteengineers.com/joystick-interfacing-arduino-processing/ (used link
 *           to download the Axis Joystick v.2.2.3 for non-mkeblock joysticks),
 *           https://www.instructables.com/Easy-Analog-Joystick-Calibration-and-Centering/
 *           (downloaded joystick.ino), s019joystickCalibration.ino (Makeblock example), and
 *           s016_orion_otg (Orion based gamepad prototype).  The goal is to find the average
 *           readings along the various points/sites/corners of calibrating the MeJoystick.
 *
 */

#include "Arduino.h"


//#include "MeSerial.h"
#include "MeOrion.h"
#include "MePort.h"

//MeJoystick joystick1(PORT_6);
//MeJoystick joystick1(PORT_7);
//original  orion
MeJoystick joystick1(PORT_8);    //orion
//MeJoystick joystick1(PORT_4);   //mbot/uno

//s015
int16_t jsx = 0;    /* a variable for the Joystick's x value */
int16_t jsy = 0;    /* a variable for the Joystick's y value */
float angle = 0;       /* The relative angle of XY */
float OffCenter = 0;    /* offset with the center */
int speed = 0;
int btnVal = 64;  //@
int waitkntr = 0;
const int maxwait = 200;
int N_readings = 100;             //N: Where is number of readings
                           //(Sum_ofValues_js_Read/N = Mean_value
 int mydelay = 100;        //Attempt delay to allow button to settle between presses
 int short_delay = 10;


String packageVal;
MeSerial mSerial;   //maps to default PORT D
                    //See MeSerial.h  of makeblock library. It appears to
                    // lay well with the UNO.
void Check4Joystick()
{
	jsx = 0;
	jsy = 0;
	jsx = joystick1.readX();
	delay(short_delay);
	jsy = joystick1.readY();
	delay(short_delay);
	angle = joystick1.angle();
	delay(short_delay);
	OffCenter = joystick1.OffCenter();
	delay(short_delay);
//delay(800);
	Serial.print("         ");   //jsX:");
	Serial.print(jsx);
	Serial.print("         ");   //jsY:");
	Serial.println(jsy);
	delay(mydelay);


}




// https://www.instructables.com/id/Easy-Analog-Joystick-Calibration-and-Centering/
//  **********************************************************************//
//  author: Joram de Poel                                                 //
//  date:   8-7-2016                                                      //
//  version: 1.3                                                          //
//  email: joramdepoel@gmail.com                                          //
//  **********************************************************************//
// Jram's code for a possible non-Orion caliration pathway forward.

//*****************define your pins here***********************************************//

//Orion UNO Port 8
//See MeJoystick.h/cpp and MePort.h/cpp
// s1 = mePort[port].s1;
// s2 = mePort[port].s2;

//#define joystick_X mePort[8].s2  //; A1    //joystick  X-as
//#define joystick_Y mePort[8].s1  //; A0    //joystick  Y-as
//#define button_2 2       // button on d2

//MePort btninput(PORT_1);     // mbot
MePort btninput(PORT_3);         //Yellow/Blue/White
//Original    MePort btninput(PORT_6);           //orion Setup for RJ25 Adapter button
                                //Using slot 1
                                //Slot 2 held for future use
//Ignore potentiometer speed control for now

//Note that significant chunks of original

//***************calibration variables************************************************//
int val_X, val_Y;

long X_CENTER, X_MIN, X_MAX;
long Y_CENTER, Y_MIN, Y_MAX;

long cal_X;
long cal_Y;


void calCenter()
{
	  cal_X = 0;
	  cal_Y = 0;
	  delay(2500);
	  Serial.println("calibrating center");
	  for (int i = 0; i < N_readings; i++) {
	    Serial.print(".");
	    Check4Joystick();
	    cal_X += (unsigned long)jsx ;  //analogRead(joystick_X);
	    delay(short_delay);
	    cal_Y += jsy;  //analogRead(joystick_Y);
	    delay(short_delay);
	  }
	  X_CENTER = (cal_X/N_readings);
	  Y_CENTER = (cal_Y/N_readings);
	  Serial.print("\nCorrection X: ");Serial.print(X_CENTER);
	  Serial.print("\nCorrection Y: ");Serial.println(Y_CENTER);
}

void calMin()
{
	  X_MIN = 0;    //reset the values
	  Y_MIN = 0;
	  delay(2500);
	  Serial.println("calibrating position");
	   for (int i = 0; i < N_readings; i++) {    //take 100 readings
	    Serial.print(".");
	    Check4Joystick();
	    X_MIN += jsx;  //analogRead(joystick_X);
	    delay(short_delay);
	    Y_MIN += jsy;  //analogRead(joystick_Y);
	    delay(short_delay);
	  }
	  X_MIN /= N_readings;
	  Y_MIN /= N_readings;
	  Serial.println();
	  Serial.print("minX: "); Serial.print(X_MIN);
	  Serial.print("            minY: "); Serial.println(Y_MIN);

}

void calMax()
{
	  X_MAX = 0;    //reset the values
	  Y_MAX = 0;
	  delay(2500);
	  Serial.println("calibrating position");
	   for (int i = 0; i < N_readings; i++)
	   {    //take 100 readings
	    Serial.print(".");
	    Check4Joystick();
	    X_MAX += jsx;  //analogRead(joystick_X);
	    delay(short_delay);
	    Y_MAX += jsy;  //analogRead(joystick_Y);
	    delay(short_delay);
	//	  Serial.print("rawX: "); Serial.print(X_MAX);
	//	  Serial.print("           rawY: "); Serial.println(Y_MAX);
	    }
	  X_MAX /=  N_readings;
	  Y_MAX /=  N_readings;
	  Serial.println();
	  Serial.print("maxX: "); Serial.print(X_MAX);
	  Serial.print("               minY: "); Serial.println(Y_MAX);

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


void calibrate() {
    bool calFlg1 = false;
    bool calFlg2 = false;
    bool calFlg3 = false;


    bool ack1 = false;  //proceed after review of calibration
    Serial.println("\nCommencing Calibration of the Joystick\n");
	Serial.println("\nStep 1: Center Position.");
	Serial.println("Allow joystick to freely center itself.");
	Serial.println(	"Press when ready.  Wait for measurement to record.");

	while (!calFlg1 )
	{
		calFlg1 = checkBtn();
		delay(mydelay);
	}
	calCenter();
	Serial.println("\nStep 2: Bottom-Left Corner. ");
    Serial.println("Place and hold the joystick in the bottom-left corner");
	Serial.println(	"Press when ready.   Wait for measurement to record.");
	while (!calFlg2 )
	{
		calFlg2 = checkBtn();
		delay(mydelay);
	}
	calMin();
	Serial.println("\nStep 3: Top Right. Press when ready");
	Serial.println("Place and hold the joystick in the top-right corner");
	Serial.println(	"Press when ready.   Wait for measurement to record.");
	while (!calFlg3 )
	{
		calFlg3 = checkBtn();
		delay(mydelay);
	}
    calMax();

  if(X_MAX < X_MIN){
    long val = X_MAX;
    X_MAX = X_MIN;
    X_MIN = val;
  }
  if(Y_MAX < Y_MIN){
    long val = Y_MAX;
    Y_MAX = Y_MIN;
    Y_MIN = val;
  }

  Serial.print("\nrange X: from ");Serial.print(X_MIN); Serial.print(" to ");Serial.println(X_MAX);
  Serial.print(  "range Y: from ");Serial.print(Y_MIN); Serial.print(" to ");Serial.println(Y_MAX);
  Serial.println("\n---calibration done---\n");


  Serial.println("Android will handle an input looking like this:");
  mSerial.printf("%d_%d_%d_%c \r\n",jsx, jsy,speed,btnVal);
  Serial.println("\n\nReview and record the results.  Then press the button to resume.");

	while (!ack1)
	{
		  ack1 = checkBtn();
          delay(mydelay);
	}
}



int const STARTER = 0;
int const PRIMER  = 1;
int const GOODBYE = 5;    //setup for final loop activity
int const KILLIT  = 7;
int const DEAD    = 9;    // go into dead loop
int const IDLING = 9999;   //calibrating

int stateFlg = IDLING; //0: throw first message
//see a038_config.h int const XCARE = -1;


void help()
{
	  bool ackhelp = false;
	  Serial.print(F("\ns023_orion_js_cal.ino: "));
	  Serial.println("This program makes 100 readings sequntially");
	  Serial.println(F("from the center, bottom/left, and top/right"));
	  Serial.println(F("as well was the min/max x, y values for the"));
	  Serial.println(F("the joystick attached to the Makeblock Orion"));
	  Serial.println(F("Arduino board.  See s015_orion_js for a work-"));
	  Serial.println(F("around for other boards."));
	  Serial.println(F("\nThe cycling using the button may be out-of-wack."));
	  Serial.println(F("If so adjust the --delay(mydelay)--_ to suit"));
	  Serial.println(F("\nWhen in dev/test use 'N: number of values read' = 10 or so"));
	  Serial.println(F("\n\nPress the button when done with help."));
      while(!ackhelp)
      {
    	  ackhelp = checkBtn();
		  delay(2*mydelay);
      }
}


void setup() {

  Serial.begin(9600);
  help();
}




void loop()
{

    bool ack2 = false;  //select recalibrate or retunr to loop
    bool ack3 = false;
	switch (stateFlg)
	{
	case IDLING:
	{
		calibrate();
		waitkntr = 0;
		stateFlg = PRIMER;
		delay(mydelay);
	} //IDLING
	break;
	case PRIMER:
		Serial.println("\n\nWhen the ticks starts scrolling press the button to restart the calibration");
		Serial.print("or just wait to return to the program loop.");
        delay(500);
		while (!ack2)
		{
	 		ack2 = checkBtn();
			delay(mydelay);
			waitkntr++;
			Serial.print(waitkntr);
			Serial.println(" ...:: tick");
			if (waitkntr >= maxwait)
			{
				stateFlg = GOODBYE;
						break;  // wfrom while
			}
			else
			{
				stateFlg = IDLING;
			}
		} //while
    break;
	case GOODBYE:
	{
		Serial.println("Goodbye");
		Serial.println("To acknowledge goodbye press the button.");
		while (!ack3)
		{
			ack3 = checkBtn();
			delay(mydelay);
		}
        stateFlg = DEAD;
	}
	break;
	case DEAD:
	{
	    Serial.println(".");
	}
	break;
    }  // end of switch
// dev test point    Serial.println("End of loop()");

}
