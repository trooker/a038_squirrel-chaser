/* Copyright © 1988-2020 by Abbott Analytical Products. All Rights Reserved.
 * Melted s010 and s015 into joystick/potentiometer controller
 * for the a038 critterChaser
 * 201222_tr Road Tested gamepad-otg-android.  The Stop and button seemed
 *           to work well.  The joystick still seemed a bit sluggish.
 *           The Compass worked well when hooked to the Wifi via udp.
 * 201221_tr Runs great with the SLOT2 note.  need to wire wrap of Dupont cable
 *           the switch to RJ24 adapter.
 *           Adjusted js and btn read delays.
 * 201220_tr Reset Orion RJ25 connections
 *           joystick      => RJ25 8
 *           potentiometer => RJ25 6
 *           button        => RJ25 3
 * 201017_tr Need to slow down the number of messages being sent to the
 *           Android for processing.  The msg are overwhelming the
 *           Making the message JS readings too long causes junk on the
 *           vehicle because of message collision.  Return to single 800
 *           delay.
 *           vehicle processing.
 * 201015_tr Experiment with slowing down the Serial messaging loop.
 *           No real difference in behavior at the vehicle end was observed.
 *           Reverted to the 800 value.  Best scrolling of Serial Monitor
 *           and Android Gamepad mode.
 *           Remove extraneous development comments
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
MePotentiometer zPotentiometer(PORT_7);     // was 7
//MeJoystick joystick2(PORT_6);

//NOTE: 201221_tr slot2
//Using slot2 jumper
//yellow to s2
//black  to GND
MeLimitSwitch btn_input(PORT_6,SLOT2);   //(uint8_t port, uint8_t slot);
//MePort btn_input(PORT_6);
//MePort btn_input(PORT_3); // was 6 works well on 6 but not 5
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
                    //play well with the UNO.
int const delay_rd_btn = 10;
int const delay4js_moves = 800;//1800; //1600; // 800;
//201214
// 1600 => 3 ticks
// 1800 => 7 ticks
// 2400 => 9 ticks
//201017 replaced 800;
// discarded 1600 which causes message collision on the vehicle.
//tried		7500;500;//250; //100; //800; //10000; //800;
//good 900; 750; 800;

//s010
// Any pin that supports PWM can also be used:

int haltflg  =0;     //3<: fires no msg in RX Serial unavailable
int waitflg  =0;     //= 0: fires msg from loop
int stopper  = 3;
int loopknt  = 0;

unsigned char inByte=0, outByte=48;
unsigned char testByte = 0;



void setup()
{
  /* initialize serial communications at 9600 bps */
  mSerial.begin(9600);
}


int ReadRJ25_btn()
{
    bool ack_btnRd = false;  //proceed after review of calibration
    int waitkntr	= 0;
    int maxwait = 2;
	bool val1 = false; //, val2;
	int rsvp = 0;
  /* read SLOT1 level */
	while (!ack_btnRd)
	{
  		val1 = btn_input.touched();   // for MePort  dRead1();
  	    if (val1 == true)
  	    {
  	    	 rsvp = 1;    //A button pressed
  	    	 ack_btnRd = true;
  	    	 break;
  	    }
  	    else
  	    {
  		//delay(delay_rd_btn);
		waitkntr++;
		if (waitkntr >= maxwait)
		{
			rsvp = 0;
			break;  // wfrom while
		}
  	    } // end else
  //	    val1 = false;
	} //while
//	delay(delay_rd_btn);
	 return rsvp;
}

void Check4Button()
{   //add command buttons
	//Assume btnA pressed
	//Only one button press per serial print
//	int btnPressed = ReadRJ25_Port_6(); // = random(0,6);  //cycleBtn;
	int btnPressed = ReadRJ25_btn(); // = random(0,6);  //cycleBtn;
//	Serial.print(btnPressed);
//	Serial.println(" ::... button pressed  Check4Button()");
	switch (btnPressed)
	{
	case 1:
	btnVal = 65;   //A
	break;
	case 2:
	btnVal = 66;   //B
	break;
	case 3:
	btnVal = 88;   //X
	break;
	case 4:
	btnVal = 89;  //Y
	break;
	case 0:
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
//redundant	  speed = zPotentiometer.read();
	  }
//just use the Check4Serial Event	  delay(delay4js_moves);
}



void Check4serialEvent()
{
   {
            Check4Joystick();
			Check4Button();
			mSerial.printf("%d_%d_%d_%c \r\n",jsx, jsy,speed,btnVal);
     		delay(delay4js_moves);  // was 100
     		btnVal = 64;

 	}
}


void loop()
{
	if (waitflg < stopper)
	{
      packageVal = "";
      Check4serialEvent();
	}
    if (mSerial.available() > 0)
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

