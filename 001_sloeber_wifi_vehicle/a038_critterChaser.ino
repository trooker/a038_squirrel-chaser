/* Copyright © 1988-2020 by Abbott Analytical Products. All Rights Reserved.
 * 200604_tr Added wifi_mgr.ino for WiFi capability.
 * 200529_tr Initial constructions.  Borrowed heavily from a034_nc_chickadee.
 *
 */


#include "a038_config.h"
#include "a038_jelly.h"

//------------------------------------------------------------------------------
// GLOBALS
//------------------------------------------------------------------------------

//int   sofar;            // how much is in the buffer
int copyMax = 1;
int copyKnt = 0;



// settings

//From GCodeParser
//#define COMMAND_SIZE 128
char commands[COMMAND_SIZE];
byte serial_count;
int no_serialData = 0;        // attempts to read Serial port
bool comment = false;



//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------


//170701_tr
rgb_lcd lcd;

int cnt = 0;
const int LED    = 2;       // the Grove port No. you attached an LED to

bool noSound = false; //true;        // turn-off sound

String readString = "";
int ledState = LOW;             // ledState used to set the LED
long previousMillis = 0;        // will store last time LED was updated
// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long interval = 1000;           // interval at which to blink (milliseconds)
int doneFlag = -1;              // think tristate
int kntr = 0;                   // loop counter
int loopknt = 0;         // count loop cycles
bool loopdebug = true;  //false;   // controls serial printing of debug info:  true is doit
int workknt = 0;         // count cycle through work
int wait4loop = 4;  //10000;
int endOn = wait4loop;         // 200;         // end work loop after
int closeLoopOn = wait4loop;   // end arduino looping after 200 cycles
// Define the delay for the "breathing" effect; change this
// to a smaller value for a faster effect, larger for slower.
const int BREATH_DELAY = 5; // milliseconds



int buzz1Knt = 1;        // Switch for happybirthday
int buzz2Knt = 1;        // Switch for shave & haircut
int buzz3Knt = 1;        // Switch for short siren
int buzz4Knt = 1;        // Switch for long siren
int buzzOff  = 2;        // Cycle play until heard x times

const int sigId_led    = 2;     //Port 3 LED   OUTPUT
const int sigId_btn    = 6;     //Port 7 btn   INPUT
const int sigId_buz    = 5;     //Port 6 buzzer OUTPUT


/*
 * wrting a string for trouble shooting loop issues
 */

void loopPrn(String msg)
{
	if (loopdebug == true)
	{
		Serial.println(msg);
	}
}



/**
 * write a string followed by a float to the serial line.  Convenient for debugging.
 * @input code the string.
 * @input val the float.
 */
void foutput(const char *code,float val)
{
 if (isReadyFlag == true)
 {
  Serial.print(code);
  Serial.print(".....");  // my test ln
  Serial.println(val,4);
 }
}


/**
 * print the current position, feedrate, and absolute mode.
 */
/* not used
void where()
{
  foutput("popped from where() of venit.ino", BADNbr);
  foutput("kntX",kntX);
  foutput("kntY",kntY);
  foutput("Distance along X Axis",kntX / x_steps2in );
  foutput("Distance along Y Axis",kntY / y_steps2in);
  foutput("x units", x_steps2in);
  foutput("y units", y_steps2in);
  foutput("a units", a_steps2deg);
  foutput("b units", b_steps2deg);
  foutput("Current_LocX",current_loc.x);
  foutput("Current_LocY",current_loc.y);
  foutput("Current_LocA",current_loc.a);
  foutput("Current_LocB",current_loc.b);


  foutput("Last fp.X requested via gCode ",fp.x);
  foutput("Last fp.Y requested via gCode ",fp.y);
  foutput("Last fp.a requested via gCode ",fp.a);
  foutput("Last fp.b requested via gCode ",fp.b);

  foutput("F",feedrate);
  foutput("E", extrude);
  foutput("abs_mode ",abs_mode);
}
*/

void help()
{
	//Serial.println("Need help with Flash memory F thingie");

	  Serial.println(F("Help Strings: "));

/*
	  Serial.print(F("GcodeCNCDemo2AxisV1 Help: "));
	  Serial.println(VERSION);
	  Serial.println(F("Unit of Measure is in Inches for Linear and Degrees for Angular"));
	  Serial.println(F("Commands:  Used by dilningc"));
	  Serial.println(F("G00 [X(steps)] [Y(steps)] [E(extrude)] [F(feedrate)]; ==> Linear"));
	  Serial.println(F("G01 [X(steps)] [Y(steps)]  [E(extrude)] [F(feedrate)]; ==> Linear"));
	  Serial.println(F("G01 G93 G53 [A(degs)]  [B(degs)][F(feedrate)]; ==> Angular"));
	  Serial.println(F("G20 Set units to inches"));
	  Serial.println(F("G04 P[seconds]; ==> delay"));
	  Serial.println(F("G90; absolute mode"));
	  Serial.println(F("G93; Angular movement of StepperA/B"));
	  Serial.println(F("G53; Machine Coordinate System. Ignored"));
	  Serial.println(F("M02; End of Program"));
	  Serial.println(F("M18; - disable motors"));
	  Serial.println(F("M100; - this help message"));
	  Serial.println(F("M114; - report position and feedrate"));
	  Serial.println(F("All commands must end with a newline."));

//	  Serial.println(F("-G02 [X(steps)] [Y(steps)] [I(steps)] [J(steps)] [E(extrude)] [F(feedrate)]; ==> clockwise arc"));
//	  Serial.println(F("-G03 [X(steps)] [Y(steps)] [I(steps)] [J(steps)] [E(extrude)] [F(feedrate)]; ==> counter-clockwise arc"));
//	  Serial.println(F("-G21 Set units to mm"));
//	  Serial.println(F("-G91; - relative mode"));
//	  Serial.println(F("-G92 [X(steps)] [Y(steps)]; - change logical position"));
//	  Serial.println(F("-M05; - Stop spindle"));
*/
}


/**
 * prepares the input buffer to receive a new message and tells the serial connected device it is ready for more.
 */
void ready_USBinputs()
{
//  sofar=0;  // clear input buffer
  Serial.print(F(">"));  // signal ready to receive input
}

void mgtCmdLn(char c)
{
	    no_serialData = 0;
	    //newlines are ends of commands.
	    if (c != '\n')
	    {
	      if(c==0x18)
	      {
	        Serial.println("Grbl 1.0");
	      }
	      else
	      {
             if ((c == ';')or (c == '('))
	         {comment = true;}
	         // If we're not in comment mode, add it to our array.
	         if (!comment)
	         {
	           commands[serial_count] = c;
	           serial_count++;
	         }
	         if (c == ')')
	         { comment = false; }// End of comment - start listening again
	       } //
	    }
	 //}
      else
	  {
	    no_serialData++;
	    delayMicroseconds(100);


foutput("serial_count....: ", serial_count);


	  //if theres a pause or we got a real command, do it
	  if (serial_count && ((c == '\n'  || (c == 0x0A) ) || no_serialData > 100))
	  {
	    //process our command!
	    Serial.print("ln 2 cmd..: ");
	    Serial.println(commands);
	    showInstruction();
//	    parse_cmdln(commands, serial_count);
	    //clear command.
//	    init_parse_cmdln();
	  }
	  }
	  }	   //mgtCmdLn()


int do_serial_read()
{
	  char c;
	  bool readchar = false;
	  //read in characters if we got them.
	  if (Serial.available() > 0)
	  {
	    c = Serial.read();
        mgtCmdLn(c);
        return 999;
      }
	  return 1;
}

void  allstop()
{
   digitalWrite((sigId_led+0),LOW);   //LED
   digitalWrite((sigId_btn+0),HIGH);  //btn
   digitalWrite((sigId_buz+0),LOW);   //buzzer
}

void breatherLED()
{
    for(int i=0; i<256; i++)
    {
        analogWrite(sigId_led, i);
        delay(BREATH_DELAY);
    }
    delay(100);

    for(int i=254; i>=0; i--)
    {
        analogWrite(sigId_led, i);
        delay(BREATH_DELAY);
    }

}

bool checkBtn()
{
 if (digitalRead(sigId_btn)==HIGH)
  {
     lcd.setCursor(0, 0);
     readString = "Button Pressed";
     showDisplayDebug(readString);
     lcd.setCursor(0, 1);
     readString = "Buzz On LED Off";
     showDisplayDebug(readString);
     digitalWrite((sigId_btn+0),HIGH);
     digitalWrite((sigId_led+0),LOW);
 soundBuzz3(); //siren
     return true;
  } //ifpressed
  else
  {
	  digitalWrite((sigId_btn+0),LOW);
	  digitalWrite((sigId_led+0),HIGH);
	  digitalWrite((sigId_buz+0),LOW);
	  return false;

  }
}



void setupCommo()
{
	  doneFlag = 0;
//	  ready_file4reading();
	  ready_USBinputs(); // ready to pass USB Serial traffic  to loop()
	  show2Reset();
}




void dowork()
{

   breatherLED();
   setupCommo();
   //un-needed siren sound
   foutput("buzz4Knt..: ", buzz4Knt);
   if ((buzz4Knt < buzzOff) && ( !noSound))
   {
	   soundBuzz4(); //siren
	   buzz4Knt++;
   }
}


void setup()
{
	  Serial.begin(BAUD);  // open coms
	  Serial.println("Serial is running");
      wifisetup();
	  help();  // say hello
	  pinMode((sigId_led+0),INPUT);    //LED   _
	  pinMode((sigId_btn+0),INPUT);    //btn
	  pinMode((sigId_buz+0),OUTPUT);   //buzzer
	  allstop();

	  setup_controller();  //ams1
//Skip for now not using steppers
//	  set_position(0,0,0,0,0);  // set staring position
	 //init_steppers does this feedrate((MAX_FEEDRATE + MIN_FEEDRATE)/2);  // set default speed

// 	  init_parse_cmdln(); //process string
	  init_steppers();   //ams1 Steppers and DC Motors
//	  setup_sd();
      setupCommo();
      showSetup();  //fires LCD message


}


// The loop function is called in an endless loop
void loop()
{
// works from here
	wifiloop();


	if (buzz1Knt < buzzOff) //play tune once
	{
		soundBuzz1(); //happy birthday
	    buzz1Knt++;
	}
	if ((loopknt < 1) && (doneFlag <= 2))
	{
	  Serial.print("");
	  Serial.println("go to dowork");
	  dowork();
	 // dead stick here wifiloop();

	}
	else
	{
		if (doneFlag < 3)
		{
	    //foutput("loop loopknt..: ", loopknt);
	    //foutput("loop doneFlag..: ", doneFlag);
		}
	    if ((loopknt >= closeLoopOn)|| (doneFlag >= 2))
	    {
	        disable();
	        showMills();
// keep LCD active or kill it
	        showDone();
		    allstop();
	        if (buzz2Knt < buzzOff)
	        {
	    	   soundBuzz2(); //shave haircut
	    	   buzz2Knt++;
	         }
	    }

    }
    if (loopknt < closeLoopOn+1)
    {
       loopknt++;
       doneFlag++;
    }


}
