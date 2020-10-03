/* Copyright © 1988-2020 by Abbott Analytical Products. All Rights Reserved.
 * Melted s010 and s015 into joystick/potentiometer controller
 * for the a038 critterChaser
 * 200925_tr Testing showed a need to implement some type of GUI to
 *           warn that 0_0_0_@ is a valid functioning condition that needs to be
 *           handled in a038 and maybe here as well.
 * 200922_tr Using the USB and no OTG/Android the x,y readings are still
 *           highly un-senstive after spraying the JS with Blow-Off Duster.
 *           Bottom-line it looks like junk.
 * 200918_tr Calibration work to reconcile readings on UNO/Orion MeJoystick
 * 200909_tr_Verified build and upload.  Turn-off test/dev mserial print
 *           statements.  Added note about MeSerial and the Write()
 *           issue.  All seem to play well as is.
 *           Added MeRJ25 Slot1/2 for two buttons.  Tested between @ and A
 *           switching.  TIme for the big show. Reset delimiter to "_".
 *           Commented-out packageval.
 * 200819_tr Actively testing and altering to use MeSerial
 *           library.
 * 200816_tr
 *     Tst:  See p001_proto_ngjs_200816.png.  Shows
 *           orion-js communicating via otg with
 *           Android (Serial USB Terminal apk)
 *     Dev:  Return from F310 deadend.
 *           MBlock has wrong syntax for axis values
 *           X/Y axis both range from -500 to 500
 *           Speed ranges 0 to 1000
 *           Need to match the Android  A2I Clock looping
 *           to support the flow.
 *           See s012_serialEvent for looping thoughts
 *
 * 200719_tr initial
 */

#include "Arduino.h"

#include "MeSerial.h"
#include "MeOrion.h"

MeJoystick joystick1(PORT_8);
MePotentiometer zPotentiometer(PORT_7);
//MeJoystick joystick2(PORT_6);
MePort input(PORT_6); // works well on 6 but not 5
                      // Setup two buttons per port

//s015
int16_t jsx = 0;    /* a variable for the Joystick's x value */
int16_t jsy = 0;    /* a variable for the Joystick's y value */
float angle = 0;       /* The relative angle of XY */
float OffCenter = 0;    /* offset with the center */
int speed = 0;
int btnVal = 64;  //@

String packageVal;
MeSerial mSerial;   //maps to default PORT D
                    //See MeSerial.h  of makeblock library. It appears to
                    // lay well with the UNO.



//s010
// Any pin that supports PWM can also be used:

int haltflg  =0;     //3<: fires no msg in RX Serial unavailable
int waitflg  =0;     //= 0: fires msg from loop
int stopper  = 3;
int loopknt = 0;
//int cycleBtn = 0;

unsigned char inByte=0, outByte=48;
unsigned char testByte = 0;



void setup()
{
  /* initialize serial communications at 9600 bps */
  mSerial.begin(9600);
  //Serial.begin(9600);
}


int ReadRJ25_Port_6()
{
	int val1, val2;
	int rsvp = 0;
	val1 = input.dRead1();   /* read SLOT1 level */
	val2 = input.dRead2();    /* read SLOT2 level */
	if (val1 == HIGH)
		{rsvp = val1;}
	else if (val2 == HIGH)
	     {rsvp = val2;}
	 return rsvp;
}

void Check4Button()
{   //add command buttons
	//Assume btnA pressed
	//Only one button press per serial print
//    cycleBtn;
	int btnPressed = ReadRJ25_Port_6(); // = random(0,6);  //cycleBtn;
//	cycleBtn++;

	switch (btnPressed)
	{
	case(1):
	btnVal = 65;   //A
	break;
	case(2):
	btnVal = 66;   //B
	break;
	case(3):
	btnVal = 88;   //X
	break;
	case(4):
	btnVal = 89;  //Y
	break;
	default: // no buttons pressed
	btnVal = 64;    //@
	break;

	} // end of switch
}

void Check4Joystick()
{
    speed = zPotentiometer.read();
	if (speed > 0)
	  {
	  /* read the both joystick axis values: */
	  jsx = joystick1.readX();
	  jsy = joystick1.readY();
	  angle = joystick1.angle();
	  OffCenter = joystick1.OffCenter();
	  speed = zPotentiometer.read();
//      packageVal = x;
//     packageVal = packageVal  + "_" + y;
//     packageVal = packageVal + "_" + angle;
//     packageVal = packageVal + "_" + OffCenter;
//      packageVal = packageVal + "_" + speed;
	  }
	  delay(800);
}



void Check4serialEvent()
{
 //  mSerial.println("Open serial event");
//   if (Serial.available() > 0)
   {
//	        mSerial.printf("%s \r\n","Open serial event");
	        //Serial.println("Open serial event");
            Check4Joystick();
			Check4Button();
 	        //
//			mSerial.printf("%s \r\n",packageVal);
//			char * cstr;
//			cstr = (char)*packageVal[0];
			//mSerial.sendString(cstr);
			//mSerial.printf(cstr);
//			  mSerial.printf("%s\r\n","just for test");
//			  mSerial.printf("%d,0x%x \r\n",123,0x123);
			  mSerial.printf("%d_%d_%d_%c \r\n",jsx, jsy,speed,btnVal);

			  delay(100);

 	}
}


void loop()
{
	if (waitflg < stopper)
	{
//	  mSerial.printf("%s \r\n","looping");
	  //Serial.println("looping");
      packageVal = "";
      Check4serialEvent();
	}
    if (mSerial.available() > 0)
    //if (Serial.available() > 0)
    {
	        haltflg = 0;
    }
    else
    {
    	if (haltflg < 3)
    	{
	      haltflg++;
    	}
    	else
    	{   if (waitflg < 1)
    	    {
    		waitflg++;
    	    }
    	}
    }

}

