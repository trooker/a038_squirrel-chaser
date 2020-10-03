/* Copyright © 1988-2020 by Abbott Analytical Products. All Rights Reserved.
 * 200925_tr Redo loop() with switch/case.  Preliminary test favorable.
 * 200914_tr Rug test showed need for kill button.  Also seems to no longer go
 *           in reverse direction.  The feedback seems to have latency that may
 *           need to be addressed.  Observed that the accelerator pushed
 *           balls to the wall is the best for recovery of active gamepad.
 *           Love the LCD red/flash during active wifi looping.
 * 200913_tr Make wifiloop() available and waiting for
 *           10,000 with not activity.
 * 200912_tr Revisited the buzzer soundings. Added clear Base Shield Port ID
 *           for LED, btn, buzzer.  Sound ques
 *           Happy Birthday:  Ready
 *           Buzzer Cycle:  WIFI UDP setup complete> Take control or lose it.
 *           ShaveNHaircut:  About to halt active WIFI UDP.
 * 200907_tr Verified UDP functional and communicates with Android a038_rc_wifi app
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
//200914
//rgb_lcd lcd;

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
                                //start
                                //USB setup
int kntr = 0;                   // loop counter
int loopknt = 0;         // count loop cycles
bool loopdebug = true;  //false;   // controls serial printing of debug info:  true is doit
int workknt = 0;         // count cycle through work
int wait4loop = 1000; //100;//10000; //4;  //10000;
int endOn = wait4loop;         // 200;         // end work loop after
int closeLoopOn = wait4loop;   // end arduino looping after 200 cycles
bool shutdownWifi = false;

// Define the delay for the "breathing" effect; change this
// to a smaller value for a faster effect, larger for slower.
const int BREATH_DELAY = 5; // milliseconds



int buzz1Knt = 0;        // Switch for happybirthday
int buzz2Knt = 0;        // Switch for shave & haircut
int buzz3Knt = 0;        // Switch for short siren
int buzz4Knt = 0;        // Switch for long siren
int buzzOff  = 1;        // Cycle play until heard x times - 1

const int sigId_led    = 3;     //Base Shield Port 3 LED   OUTPUT
const int sigId_btn    = 6;     //Base Shield Port 6 btn   INPUT
const int sigId_buz    = 5;     //Base Shiled Port 5 buzzer OUTPUT


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
	bool statFlg = false;
 if (digitalRead(sigId_btn)==HIGH)
  {
	 loopknt = closeLoopOn+1; //kill switch mode of operation
     showDisplay0_0();
	 readString = "Kill Switch Pressed";
     showDisplayDebug(readString);
     Serial.println(readString);
     showDisplay0_1();
     readString = "Kill Switch";
     showDisplayDebug(readString);
     digitalWrite((sigId_btn+0),LOW);
     digitalWrite((sigId_led+0),LOW);
Serial.println("soundBuzz3");
//     soundBuzz3(); //siren
     statFlg = true;
  } //ifpressed
  else
  {
//	  Serial.println(loopknt);
	  digitalWrite((sigId_btn+0),LOW);
      if (shutdownWifi)
    	  digitalWrite((sigId_led+0),LOW);
      else {digitalWrite((sigId_led+0),HIGH);}
	  digitalWrite((sigId_buz+0),LOW);
	  statFlg = false;
  }
  return statFlg;
}



void setupCommo()
{
//not used	  doneFlag = 0;
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
   while ((buzz4Knt < buzzOff) && ( !noSound))
   {
	   soundBuzz4(); //siren
	   digitalWrite((sigId_buz+0),LOW);
	   buzz4Knt++;
//	   Serial.print(buzz4Knt);
//	   Serial.println("  : Buzz4Knt");
   }
   digitalWrite((sigId_buz+0),LOW);
   buzz4Knt = 0;

}

void shutter()
 {
            showMills();
    // keep LCD active or kill it
            calmShowDisplay();
    	    allstop();
    	    delay(1000);
            disable_dcm();  //shutdown dc motors
       	    close_udp();    //turn-off wifi-TCP
            Serial.print(loopknt);
//           Serial.println("  :loopknt at shutdown");
//           Serial.print(doneFlag);
//           Serial.println("  :doneFlag");
            Serial.println("Critter Chaser has been disabled. ");
      	    digitalWrite((sigId_led+0),LOW);
            shutdownWifi = true;
 }


int stateFlg = 0; //0: throw first message
int const XCARE = -1;

int const STARTER = 0;
int const PRIMER  = 1;
int const GOODBYE = 5;
int const KILLIT  = 7;
int const DEAD    = 9;
int const IDLING = 9999;


void setup()
{
      stateFlg = XCARE;
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

//not used 	  init_parse_cmdln(); //process string
	  init_steppers();   //ams1 Steppers and DC Motors
//not used	  setup_sd();
      setupCommo();
      showSetup();  //fires LCD message
      stateFlg = STARTER;

}


// The loop function is called in an endless loop
void loop()
{
// works from here
//	wifiloop();
    if (checkBtn())
    	{stateFlg = KILLIT;}
//    if ((stateFlg != KILLIT) && (stateFlg != DEAD))
//    {
//    	Serial.print(stateFlg);
//        Serial.println(" Entering switch loop");
//    }
    switch (stateFlg)
    {
    case IDLING:
        loopknt++;
        flipShowDisplay2Red();
	    calmShowDisplay();
        wifiloop();
        if ((checkForUDPactivity())&& (!Need2Killwifi()))
        {
    	  loopknt = 2; //need to skip past dowork() and buzzer
    	  reset_is_UDP_active();
        }
//        Serial.print(loopknt);
//        Serial.println("  :loopknt wifiloop cycle finished");
        Serial.println(".");
	    if (loopknt >= closeLoopOn+1)
	    	{stateFlg = GOODBYE;}
        break;
    case STARTER:
	   // if (buzz1Knt < buzzOff) //play tune once
	    {
		  soundBuzz1(); //happy birthday
	      buzz1Knt++;
	    }
	    disable();  //not used steppers
		Serial.println("\ngo to dowork");
		dowork();  //sound buzzer4
		loopknt++;
		stateFlg = IDLING;
        break;

    case GOODBYE:
	    soundBuzz2(); //shave haircut
	    buzz2Knt++;
        if (!shutdownWifi)
        {shutter();}
        stateFlg = DEAD;
        break;
    case DEAD:
    case KILLIT:
	    break;
    case XCARE:
	    break;
    default:
    	Serial.println("default");
    	break;
    } // end of switch


//	if (buzz1Knt < buzzOff) //play tune once
//	{
//		soundBuzz1(); //happy birthday
//	    buzz1Knt++;
//	}
//	if ((loopknt < 1) ) // not used&& (doneFlag <= 2))
//	{
//	  Serial.print("");
//	  Serial.println("go to dowork");
//	  dowork();  //sound buzzer4
//	}
//	else
//	{
//	    if ((loopknt >= closeLoopOn)|| (doneFlag >= 2))
//	    {
//	    	disable();  //not used steppers
//	    }
//    }
//    if (loopknt < closeLoopOn+1)
//    {
        //not used doneFlag++;
//    	loopknt++;
 //       flipShowDisplay2Red();
 //   	calmShowDisplay();

//	    wifiloop();
//	    if ((checkForUDPactivity())&& (!Need2Killwifi()))
//	    {
//	    	loopknt = 2; //need to skip past dowork() and buzzer
//	    	reset_is_UDP_active();
//	    }
//       Serial.print(loopknt);
 //       Serial.println("  :loopknt wifiloop cycle finished");

//    }
//    else
//    {

//        if (buzz2Knt < buzzOff)
//        {
//    	    soundBuzz2(); //shave haircut
//    	    buzz2Knt++;
//        }
//        if (!shutdownWifi)
//        {
//        	shutter();
//       }

 //   }
}
