/*
 * Melted s010 and s015 into joystick/potentiometer controller
 * for the a038 critterChaser
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
//MeJoystick joystick2(PORT_6);
MePotentiometer zPotentiometer(PORT_7);

//s015
int16_t x = 0;    /* a variable for the Joystick's x value */
int16_t y = 0;    /* a variable for the Joystick's y value */
float angle = 0;       /* The relative angle of XY */
float OffCenter = 0;    /* offset with the center */
int speed = 0;
int btnVal = 64;  //@

String packageVal;
MeSerial mSerial;   //maps to default PORT D

//s010
// Any pin that supports PWM can also be used:

int haltflg  =0;     //3<: fires no msg in RX Serial unavailable
int waitflg  =0;     //= 0: fires msg from loop
int stopper  = 3;
int loopknt = 0;
int cycleBtn = 0;

unsigned char inByte=0, outByte=48;
unsigned char testByte = 0;



void setup()
{
  /* initialize serial communications at 9600 bps */
  mSerial.begin(9600);
  //Serial.begin(9600);
}



void Check4Button()
{   //add command buttons
	//Assume btnA pressed
	//Only one button press per serial print
    cycleBtn;
	int btnPressed = random(0,6);  //cycleBtn;
	cycleBtn++;

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
	  x = joystick1.readX();
	  y = joystick1.readY();
	  angle = joystick1.angle();
	  OffCenter = joystick1.OffCenter();
	  speed = zPotentiometer.read();
      packageVal = x;
      packageVal = packageVal  + "_" + y;
//     packageVal = packageVal + "_" + angle;
//     packageVal = packageVal + "_" + OffCenter;
      packageVal = packageVal + "_" + speed;
	  }
	  delay(800);
}



void Check4serialEvent()
{
 //  mSerial.println("Open serial event");
//   if (Serial.available() > 0)
   {
	        mSerial.printf("%s \r\n","Open serial event");
	        //Serial.println("Open serial event");
            Check4Joystick();
			Check4Button();
 	        //
//			mSerial.printf("%s \r\n",packageVal);
//			char * cstr;
//			cstr = (char)*packageVal[0];
			//mSerial.sendString(cstr);
			//mSerial.printf(cstr);
			  mSerial.printf("%s\r\n","just for test");
			  mSerial.printf("%d,0x%x \r\n",123,0x123);
			  mSerial.printf("%d_,%d_,%d_,%c \r\n",x, y,speed,btnVal);

			  delay(100);

 	}
}


void loop()
{
	if (waitflg < stopper)
	{
	  mSerial.printf("%s \r\n","looping");
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
