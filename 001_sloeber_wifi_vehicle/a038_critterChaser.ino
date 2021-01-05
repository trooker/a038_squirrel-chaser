/* Copyright © 1988-2021 by Abbott Analytical Products. All Rights Reserved.
 * 210104_tr Final tweaks to content of help() strings.
 * 210103_tr Road tested satisfacorily
 * 210102_tr Verified that getES_Flg goes straight to shutdown the Wifi and movement.
 *           added help() content.
 * 201231_tr Retrofit help() content.
 * 201230_tr Still compiles and uploads to Mega
 *           Cleanup dev comments.  Set wait4loop to 1K
 * 201222_tr Road Tested gamepad-otg-android.  The Stop and button seemed
 *           to work well.  The joystick still seemed a bit sluggish.
 *           The Compass worked well when hooked to the Wifi via udp.
 * 201122_tr Test run 1725: Compass mode worked well until return from Gamepad mode
 *           Gamepad mode acquired switch from COmpass.  But failed to acquire
 *           XY joystick changes except for "minor" right turn and halt.
 *           Note that the vehcile goes into UNK state and quits
 *           flashing LCD red.  Then it recovers and flashes but
 *           does not respond to COmpass or Gamepad inputs.
 * 201010_tr Create cloeUp() to KILLIT
 * 200925_tr Redo loop() with switch/case.  Preliminary test favorable.
 * 200914_tr Rug test showed need for kill button.  Also seems to no longer go
 *           in reverse direction.  The feedback seems to have latency that may
 *           need to be addressed.  Observed that the accelerator pushed
 *           balls to the wall is the best for recovery of active gamepad.
 *           Love the LCD red/flash during active wifi looping.
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


char commands[COMMAND_SIZE];
byte serial_count;
int no_serialData = 0;        // attempts to read Serial port
bool comment = false;



//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------


//200914
//rgb_lcd lcd;

int cnt = 0;
const int LED    = 2;       // the Grove port No. you attached an LED to

bool noSound = false; //true;        // turn-off sound

String readString = "";
int ledState = LOW;             // ledState used to set the LED
int doneFlag = -1;              // think tristate
                                //start
                                //USB setup
int kntr = 0;                   // loop counter
int loopknt = 0;         // count loop cycles
bool loopdebug = true;  //false;   // controls serial printing of debug info:  true is doit
int workknt = 0;         // count cycle through work
//int wait4loop = 10; //for dev test
//int wait4loop = 100; //for dev test
int wait4loop = 10000;  //for production use 10000;
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




int helpdelay = 100;

void help()
{
	  bool ackhelp = false;
	  char c;
	  int helpknt = 0;
	  int helpmax = 500;
	  Serial.println(F("Help Strings: "));

/* */
	  Serial.print(F("Critter Chaser Help: "));
	  Serial.println(VERSION);
	  Serial.println(F("See http://abbottanp.com/artifacts/squirrelChaser/index.html"));
	  Serial.println(F("Your Access Point Name and password must be entered in jelly.h and"));
	  Serial.println(F("   then the a038critterChaser project recompiled."));
	  Serial.println(F("Vehicle control is via Orion based prototype gamepad-otg-android."));
	  Serial.println(F("The android needs access to your Wifi Access Point so it can send"));
	  Serial.println(F("    control instructions."));
	  Serial.println(F("Compass Controls: Forward, Backward, Left, Right, Stop"));
	  Serial.println(F("Gamepad Controls implemented: XY joystick, speed, and A command"));
	  Serial.println(F("    button."));
	  Serial.println(F("LCD White: Setup underway for Mega and Wifi    "));
	  Serial.println(F("LCD Flashing Red: Awaiting Android/Gamepad Inputs    "));
	  Serial.println(F("LCD Flashing Green: Converting Inputs to Vehicle Control Instructions/Motion."));
	  Serial.println(F("LCD Dark: Vehicle sleeping needs restart to activate control."));
	  Serial.println(F("    Scrolling dot continues indicating heartbeat."));
	  Serial.println(F("    Press Vehicle/Mega on-board reset button to re-awaken."));
	  Serial.println(F("Happy Birthday: Setup running    "));
	  Serial.println(F("Shave and Haircut:    Vehicle going to sleep."));
	  Serial.println(F("Bummmp-Dat 4x: Setup complete start controlling. "));
	  Serial.println(F("    Green LED On.  LCD Flashing Red"));
	  Serial.println(F("USB Monitor/GTKterm:     "));
	  Serial.println(F("   Setup Phase: Status messages...  "));
	  Serial.println(F("msg:     .....setup() Fina..... "));
	  Serial.println(F("msg:     ..<<<<<<<<<<< Starter completed  Loop"));
	  Serial.println(F("msg:     <<<<<<<< Primer: going to wifiloop"));
	  Serial.println(F("msg:     awaiting Serial inputs > "));
	  Serial.println(F("  IDLING Phase:  Compass Mode and/or Gamepad   "));
	  Serial.println(F("     User control inputs via mode selected"));
	  Serial.println(F("msg:     Done with IDLING going to GOODBYE"));
	  Serial.println(F("msg:     runtime (sec)........xxxx.0000"));
	  Serial.println(F("msg:     yyyyyy ::... Loop count since last user inputs shutter() cChaser"));
	  Serial.println(F("msg:     Critter Chaser has been disabled."));
	  Serial.println(F("  Awaiting Reset Phase:  "));
	  Serial.println(F("msg:     ."));
	  Serial.println(F("     Vehicle power lights glow.    "));
	  Serial.println(F("     Wifly Shield Red shows slow blink.  "));
	  Serial.println(F("Pressing the ES icon on the Android       "));
	  Serial.println(F("  invokes the Emergency shut-down process   "));
	  Serial.println(F("     Vehicle motion halts, LCD goes blank,  "));
	  Serial.println(F("     LED, buzzer turned-off."));
	  Serial.println(F(" ++ Vehicle requires a system reset"));
	  Serial.println(F("     from Mega or Wifi reset button -OR "));
	  Serial.println(F("     external upload to reset "));
	  Serial.println(F(" ++ No impact on Gamepad or Android  "));
	  Serial.println(F(" ++ IDE Monitor/GTKterm display"));
	  Serial.println(F("      Emergency Stop annunication "));
	  Serial.println(F("      Then  displays . --dot-- on new line until reset."));
	  Serial.println(F("Compass Mode Stop: Press Android Stop Icon"));
	  Serial.println(F("Gamepad Mode Stop:"));
	  Serial.println(F("   Release Joystick to center -OR-"));
	  Serial.println(F("   Drop Speed to Potentiometer 0 postion"));
	  Serial.println(F("   Press Android Stop Icon"));
	  Serial.println(F("Vehicle speed resumes as user engages Compass/Gamepad"));

}


/**
 * prepares the input buffer to receive a new message and tells the serial connected device it is ready for more.
 */
void ready_USBinputs()
{
//  sofar=0;  // clear input buffer
  Serial.print(F("awaiting Serial inputs >"));  // signal ready to receive input
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
     statFlg = true;
  } //ifpressed
  else
  {
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
	  ready_USBinputs(); // ready to pass USB Serial traffic  to loop()
	  show2Reset();
}




void dowork()
{

   breatherLED();
   setupCommo();
   //un-needed siren sound
   foutput("\nbuzz4Knt..: ", buzz4Knt);
   while ((buzz4Knt < buzzOff) && ( !noSound))
   {
	   soundBuzz4(); //siren
	   digitalWrite((sigId_buz+0),LOW);
	   buzz4Knt++;
   }
   digitalWrite((sigId_buz+0),LOW);
   buzz4Knt = 0;

}

void closeUp()
{
    calmShowDisplay();
    allstop();
	close_udp();    //turn-off wifi-TCP
    shutdownWifi = true;
    Serial.println("Emergency Stop.  Restart Everybody");
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
            Serial.print(loopknt);   //trouble shooting
            Serial.println(" ::... Loop count since last user inputs shutter() cChaser");
            Serial.println("Critter Chaser has been disabled. ");
            shutdownWifi = true;
 }


int stateFlg = 0; //0: throw first message

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
	  help();
      wifisetup();
	  pinMode((sigId_led+0),INPUT);    //LED   _
	  pinMode((sigId_btn+0),INPUT);    //btn
	  pinMode((sigId_buz+0),OUTPUT);   //buzzer
	  allstop();
	  setup_controller();  //ams1
	  init_steppers();   //ams1 Steppers and DC Motors
      setupCommo();
      showSetup();  //fires LCD message
      stateFlg = STARTER;

      Serial.println("..............setup() Fina...........");

}


//Neds some thinking

int prep2RunWifiLoop4Record()
{
    if ((checkForUDPactivity())&& (!Need2Killwifi()))
    {
      loopknt = 2; //need to skip past dowork() and buzzer
    }
    return 2;
}


//need to test the getES_Flg for stopping the wifi commo

void loop()
{
    if ((stateFlg == DEAD) || (stateFlg == KILLIT))
    {
    	;
    }
    else
    {
	    if (((checkBtn()) || (Need2Killwifi()))&& (stateFlg < IDLING)|| (getES_Flg()))
	    	{
	    	  stateFlg = KILLIT;
	  	      allstop();
	    	}
    }
    switch (stateFlg)
    {
    case IDLING:
        loopknt++;    // maybe better to place just above the if for loopkn > closeLoopOn + 1
        reset_is_UDP_active2false();
        if (getPrimerFlg())
        {
           flipShowDisplay2Red();
	       calmShowDisplay();
	       if (!getES_Flg())
	       {
              wifiloop();
              prep2RunWifiLoop4Record();
	       }
        }
        else
        {
        	Serial.println("Houston the WIFI has a problem!");
        	showDisplay("WIFI Issue");
        }
	    if (loopknt >= closeLoopOn+1)
	    	{
	    	Serial.println("Done with IDLING going to GOODBYE");
	    	stateFlg = GOODBYE;
	    	}
        break;
    case STARTER:
	    {
		  soundBuzz1(); //happy birthday
	      buzz1Knt++;
	    }
	    setES_Flg2False();
	    disable();  //not used steppers
		Serial.println("\n...<<<<<<<<<<< Starter completed  Loop");
		loopknt++;
		stateFlg = PRIMER;
        break;
    case PRIMER:
		Serial.println("\n<<<<<<<< Primer: going to wifiloop");
        wifiloop();
        prep2RunWifiLoop4Record();
		setPrimer();
		dowork();  //sound buzzer4
		stateFlg = IDLING;
    	break;
    case GOODBYE:
	    soundBuzz2(); //shave haircut
	    buzz2Knt++;
        if (!shutdownWifi)
        {shutter();}
        stateFlg = DEAD;
        break;
    case KILLIT:
    	stateFlg = DEAD;
	    break;
    case DEAD:
    case XCARE:
        Serial.println(".");
	    break;
    default:
    	Serial.println("default");
    	break;
    } // end of switch

}
