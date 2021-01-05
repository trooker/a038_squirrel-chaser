/* Copyright © 1988-2021 by Abbott Analytical Products. All Rights Reserved.
 * Connects to WiFi and reads/writes UDP messages.  The "read"
 * is parsed into a direction or an action. This sketch was developed
 * from s008.
 * 210102_tr Tweaked Gamepad Stop.
 * 210101_tr A Sounding did not work today.  Corrected state flow
 *           issue with switching between Compass/Gamepad modes.
 *           210101_1741* testrun.
 *           Experimented with delay at bottom of while(wifly.available>...
 *           Tried 2000.  Worked better.
 * 201231_tr Restructured to use endOfMsg with states.  Had excellent bench test
 *           demonstrating control of motion, A button action, etc.
 *           Timing/Snyc still seems to be issue.
 * 201230_tr Removed extraneous comments and commented out others
 * 201222_tr Road Test: iscovered need to handle broken messages.
 * 201207_tr Seemse to work well with the a038_950_Gamepad-Otg-Wifi
 *           app.  Need to sync actions, motor control, etc.
 * 201130_tr a038_954 running throws messages for G & K playingfield
 *           Emergency and Stop buttons tested.
 * 201128_tr Employed the { } and " 0d0a" and skip adding to dataStr & packetBuffer
 * 201127_tr Begin to integrate s033_wifly_strech.ino
 * 201122_tr Added Green/Calm LCD for Wifi looping
 * 201121_tr using s033wifiy_stretch.ino to capture cUDP
 *           Removed extraneous serial_msg and actions
 * 201020_tr Experimenting with wifly "set comm match <value>"  where
 *           value  is 0x15 NAK sent by app2inv android
 *           a038_968_gamepad_otg_wifi.apk
 * 201018_tr Switch to wifly.receive() to bundle string
 * 201015_tr Experiment with RX time delays
 * 201014_tr Rebuilt the filterGamepadData() for delimiterTonFlg
 * 201011_tr Restructured
 *           see backup_withTrash
 *           /media/tom/Silo_002/TechWork/DIY_projects/p006_squirrelChaser
 *           /022_steering/002_serialMonitor_track 201010_tr Restructured
 *           sortAndroid/sortCompassData.  Added a check for primer to
 *           resolve the Wifi run/gone due to the sync with the AP at start.
 *           That was the cause of the delay. Discovered packet trash
 *           appearing as invalid joystick_speed_btn data.
 * 201009_tr Added udp_packatetDelay delay to slow the TX to WIFI app
 * 201008_tr Added wifly.flush() and Emergency stop check_ES();
 * 201007_tr Handles vehiclePedal for the Compass mode.
 * 200925_tr Discovered that 0_0_0_@ needed to be handled separately.
 * 200912_tr Taking look at parse gamepad data
 * 200911_tr a038_981_gamepad_otg_wifi.apk pairs best with this.
 *           G: Gamepad Mode
 *           K: Compass Mode
 * 200910_tr Restart using the gamepad as well as the four-post controller
 * 200604_tr Initial insertion of WiFi capability. See folder
 *           p006_squirelChaser for grub, images, and downloads of
 *           code/documents.
 *
 *
 */



//WiFiUDP Udp;
// Pins' connection
// Arduino       WiFly
//  2    <---->    TX
//  3    <---->    RX
//SoftwareSerial uart(2, 3);
//WiFly wifly(uart);
//Trie their way
//SoftwareSerial uart(15, 14);
WiFly wifly(&Serial3);


#define SSID      _SSID  // set in a038_jelly.h
#define KEY       _PWD   // set in a038_jelly.h
// WIFLY_AUTH_OPEN / WIFLY_AUTH_WPA1 / WIFLY_AUTH_WPA1_2 / WIFLY_AUTH_WPA2_PSK
#define AUTH      WIFLY_AUTH_WPA2_PSK

unsigned int time_point = 0;


#define UDP_HOST_IP        "255.255.255.255"      // broadcast
#define UDP_REMOTE_PORT    5050 //55555
#define UDP_LOCAL_PORT     5050 //55555

#define maxBuffer 255 //-xxx_-yyy_sss_A  //255
char packetBuffer[maxBuffer];
int packetNdx = 0;
bool isGamepadDataFlg = false;   //false: Assumes 4 flat arrows+stop
                                  //true: using Arduino JS uno syntax
                                  //!x-axis_y-axis_speed_character
                                  //numeric positive 0...999
                                  //        negavtive -999..-1
                                  //character @, A, B, X, Y
bool isEndMsg = false;
bool isUnderScore  = false;  //does the "_" lead a btn
char cUDP;                   //character displayed
int iUDP;                    //byte value read from Wifi
bool isValidBtn;             //True: valid F310 button
                             //received from wifi

const char baseChar = '*';

bool ES_Flg = false;         //Emergency Stop is off
bool primerFlg = false;

const int udp_packatetDelay = 500;  //100;    //added delay to slow the RX from WIFI AP
                                 //1000 to broken
                                 //500 compass works
tristate delimiterToknFlg = xcare;  //"_" underscoremay not need


bool is_UDP_active = false;
bool KillwifiFlg   = false;  //on-board button kill switch


int sound2Knt = 0;
const int sound2Stopper = 10;
int buzzKnt4wifi = 0;        // Switch for shave & haircut
int buzzOff4wifi = 1;

#define devtst 1


void setES_Flg2False()
{
	ES_Flg = false;
}

void setES_Flg2true()
{
	ES_Flg = true;
}

bool getES_Flg()
{
	return ES_Flg;
}


void write_packetBuffer()
{
	if (devtst == 1)
	{
	for (byte i = 0; i < maxBuffer-1; i++)
		  {
		    Serial.write(packetBuffer[i]);
		  }
	}
}




void soundOf2()
{
//if ((buzzKnt4wifi < buzzOff4wifi) && (sound2Knt < sound2Stopper))
{
   soundBuzz3(); //
   buzzKnt4wifi++;
//dev
//Serial.println("Buzzer plays soundBuzz3()");
//Serial.println("Do A() Play sound once..............");
//showDisplay("Do A() Play once.");

 }
// sound2Knt ++;
}




void action1()
{
	Serial.println("doing Action1");
}


void action2()
{
	Serial.println("doing Action2");
}


/*
 * Do actions on command from gamepad
 */
void DoDefault() //@ default
{
	//Serial.println("DoDefault():  Working really hard.");
	showDisplay("DoDefault(): Bizzy");
	;// Do something important
}

void  DoActionA()
{
	 Serial.println("Action A():  Sounds Once.");
	 soundOf2();
	 ;// Do something important
}

void DoActionB()
{
	 ;// Do something important
}

void DoActionX()
{
	 ;// Do something important
}

void DoActionY()
{
	 ;// Do something important
}

void forward()
{
	chaseForward();

}
void backward()
{
	chaseBackward();
}

void stop()
{
	chaseStop();
}
void left()
{
	chaseDCM1();
}

void right()
{
	chaseDCM2();
}


void display_packetBuffer()
{
	//dev
	Serial.print("packetBuffer...:: ");
	Serial.println(packetBuffer);

}

void sortCompassData(int idir_UDP)
{
   if (packetBuffer[packetNdx] == '\0')
   {

			String localdataStr = packetBuffer;
		 if (packetNdx == 1) // set the vehiclePedal speed once
		 {setSpeed4Motor(getVehiclePedal(),9);}
/*
 * 		      case 50: // 2 forward compass mode
		      case 51: // 3 left compass mode
		      case 52: // 4 reverse compass mode
		      case 53: // 5 right compass mode
 *
 */
		 switch (idir_UDP)  //201128 (tst4dir)
		 {
		 case 2: //forward
		 case 50:
		      //display_packetBuffer();
		      forward();
		      clearPacketBuffer();
			 break;
		 case 4:
		 case 52:
		      backward();
		      clearPacketBuffer();
			 break;
		 case 3:
		 case 51:
		      left();
		      clearPacketBuffer();
			 break;
		 case 5:
		 case 53:
		      right();
		      clearPacketBuffer();
			 break;
		 case 0:
		 case 48:
		 default:
		      stop();
		      clearPacketBuffer();
			 break;
		 } // end tst4dir
	} //end if == '\0 lookahead
}



typedef struct
 {
     String str_x;
     String str_y;;
     String str_speed;
     String str_btn;
 }  record_type;


 /*
  * 	Need on the fly test for valid data
 		Gamepad Incoming cUDP:  packetBuffer...::  58_105_77_155_194166_1290233_324224_315130_77_--5_-124_13_-106472_17_69130_133163_174175_198180_202_174_186153_14142_126_133_10668_-19_35_-75_66_-115_690_@

 		&&&&*&(&(validData
 		-xxx_-yyy_1024_@ABXYMD
 		_66_-115_690_@

        need to handle special message "0_0_0_@" such that all stops
*/

void sortGamePadData() //was int ibtn_UDP)
{
	bool isStopFlg = false;
	String test4stop = "0_0_0_S";
	String localdataStr = packetBuffer;
		//dev
    record_type elements;
    String pval_x, pval_y, pval_speed, pval_btn;  //A = 65; B = 66; X = 88; Y = 89; @ = 64
	char *split_str_element;
	char *p = packetBuffer;
	int kntr = 0;
	int stopper = 4;
	if (!(localdataStr == test4stop))
	{
	  while (((split_str_element = strtok_r(p, "_", &p)) != NULL) && kntr < stopper)  // Don't use \n here it fails
	    {

		  switch (kntr)
			 {
		     case 0: //elements.str_x = split_str_element;
		             pval_x = split_str_element;
		             break;
		     case 1: //elements.str_y = split_str_element;
		     	 	 pval_y = split_str_element;
		     	 	 break;
		     case 2: //elements.str_speed = split_str_element;
             	 	 pval_speed = split_str_element;
		     	 	 break;
		     case 3: //elements.str_btn = split_str_element;
             	 	 pval_btn = split_str_element;

		     	 	 break;
			 }
             kntr++;

	    } // end of while

		    char c = pval_btn.charAt(0);

		    switch (c)
		    {
		    case 64: //@
		    	   DoDefault();
		    	   break;
		    case 65:  //A
//dev
Serial.print(c);
Serial.println(" ::.....c   About to execute button A Action.");
		    	    DoActionA();
		    	    break;
		    case 66:  //B
		    	    DoActionB();
		    	    break;
		    case 88:  //X
		    	    DoActionX();
		    	    break;
		    case 89:  //Y
		    	    DoActionY();
		    	    break;
		    case 83:
		    	isStopFlg = true;
		    	break;
		    default:
		    	    DoDefault();
		    	    break;
			} // end of btn action switch
		//    Serial.println("      x: " + pval_x  + "    y: " + pval_y + "   Speed: "  + pval_speed);
	} //not test4stop
	else
	{//special case that causes trouble.  This seems to work well by not running the chaser.
		pval_x = "0";
		pval_y = "0";
		pval_speed = "0";
		pval_btn = "83";
  	    digitalWrite((sigId_led+0),HIGH);
  	    delay(20);
  	    digitalWrite((sigId_led+0),LOW);
	}
//old version     setSpeed4Vehicle(pval_speed.toInt());
	setUserDefinedInputSpeed4Vehicle(pval_speed.toInt());
    if (pval_speed.toInt() > 50)
    {
        setDirection(pval_x.toInt(), pval_y.toInt());
    } else if (isStopFlg)
    	setDirection(0,0);
    clearPacketBuffer();
}


void clearPacketBuffer()
{
	packetNdx       = 0;
	for (int i=0;i<maxBuffer;i++)
	{
	    packetBuffer[i] = {0};
	}
	delay(250);
    wifly.flush();
}


void setupUDP(const char *host_ip, uint16_t remote_port, uint16_t local_port)
{
  char cmd[32];
  wifly.sendCommand("set w j 1\r", "AOK");   // enable auto join

  wifly.sendCommand("set i p 1\r", "AOK");
  snprintf(cmd, sizeof(cmd), "set i h %s\r", host_ip);
  wifly.sendCommand(cmd, "AOK");
  snprintf(cmd, sizeof(cmd), "set i r %d\r", remote_port);
  wifly.sendCommand(cmd, "AOK");
  snprintf(cmd, sizeof(cmd), "set i l %d\r", local_port);
  wifly.sendCommand(cmd, "AOK");
  wifly.sendCommand("save\r");
  wifly.sendCommand("reboot\r");
}

void wifisetup() {
	clearPacketBuffer();
  Serial.println("--------- WIFLY UDP --------");

  Serial3.begin(9600);     // WiFly UART Baud Rate: 9600
  wifly.init();
  wifly.reset();

  while (1) {
    Serial.println("Try to join " SSID );
    if (wifly.join(SSID, KEY, AUTH)) {
      Serial.println("Join succeeded  " SSID);
      wifly.clear();
      break;
    } else {
      Serial.println("Failed to join " SSID);
      Serial.println("Wait 1 second and try again...");
      delay(1000);
    }
  }

  setupUDP(UDP_HOST_IP, UDP_REMOTE_PORT, UDP_REMOTE_PORT);

  delay(1000);
  wifly.clear();
  //210101 insure we are in Compass mode at startup-primer
  isGamepadDataFlg = false;
}


String fillBuffer(String dataStr)
{
	String holdStr = dataStr;
	  packetBuffer[packetNdx] = cUDP;
	  packetNdx++;
	  holdStr = packetBuffer;
	return holdStr;
}


void branch2parse(int ibtn_UDP)
{
	//Serial.print(ibtn_UDP);
	//Serial.println(" ::.....ibtn_UDP   Check for A Btn press branch2parse");
					switch (ibtn_UDP)
					{
				      case 64:  //@ default
				      case 65:  //A btn
				      case 66:  //B btn
				      case 88:  //X btn
				      case 89:  //Y btn
				      case 68:  //D error message from A2I apk
				      case 77:  //M error message from extension
				      case 83:  //S stop for Gamepad
				            {
  					            sortGamePadData(); //ibtn_UDP);
				            }
				            break;
				      default:
				            break;
					}

}


bool rightTheShip()
{  //handle broken faulty messages
				  clearPacketBuffer();
				  wifly.flush();
		    	  return true;
}



bool prep2KillOffWifi()
		{  //"t"  gamepad   "v": compass
			Serial.println("The Emergency Stop Switch just pressed");
			is_UDP_active = false;
			wifly.flush();
			wifly.clear();
			wifly.leave();
			ES_Flg = true;
			return true;
		}


bool setSkipFillBuffer()
{
	bool rtnbool = false;
	if (isValidBtn)
	{
		  rtnbool = true;
	}
	return rtnbool;
}


int const FOUND_ZERO         = 0;
int const FOUND_LEFTPAREN    = 1; //begin token
int const FOUND_DATA         = 2; //data read looking for data end token
int const FOUND_RIGHTPAREN   = 3; //pending end-of-line markers
int const FOUND_0d           = 4; //pending closure
int const FOUND_0a           = 5; //valid message
int const FOUND_DIR          = 9; //non actionable input
int const FOUND_FLIPMODE     = 6; //flip between Compass and Gamepad
int const FOUND_KILL         = 99;

int endOfMsg ;          // checks for 0d0a


void wifiloop()
{
//	char btnChar;   //@,A, B, X, Y for F310
	endOfMsg = 0;
	int ibtnUDP = -9;
	int idirUDP = -9;
	String dataStr = "";
	bool isSkipFillBuffer = false;
	clearPacketBuffer();
	endOfMsg = 0;
	while ((wifly.available()>0) && (endOfMsg != FOUND_KILL) && (!ES_Flg))
	{
		iUDP = wifly.read();
		cUDP = (char)iUDP;
        is_UDP_active = true;
  	    isSkipFillBuffer = false;
  	    if(getPrimerFlg())
		{

 /* dev - troubleshooting
  	    	        Serial.print(iUDP, DEC);
  	    			Serial.print("  ::.. iUDP as DEC   ");
  	    			Serial.print(cUDP);
 	    			Serial.println("  ::.... ASCII character");
 */
			  flipShowDisplay2Green();
			  calmShowDisplay();
			  flipShowDisplay2Green();
			  calmShowDisplay();
		      switch(iUDP)
		      {
		      case 116:   // t kill for compass
		      case 118:   // v kill for gamepad
		    	  if (endOfMsg == FOUND_LEFTPAREN)
		    	  {	  endOfMsg = FOUND_KILL;
	    	          KillwifiFlg = prep2KillOffWifi();
	    	          setES_Flg2true();
					  break;
		    	  }
		    	  break;
		      case 10:     //0x0a
		    	  if (endOfMsg == FOUND_0d)
		    	  {
		    		  endOfMsg = FOUND_0a;
		    	      isSkipFillBuffer = setSkipFillBuffer();
		    	  }
		    	  break;
		      case 13:     //0x0d
		    	  if (endOfMsg == FOUND_DATA)
		    	  {	  endOfMsg = FOUND_0d;}
		    	  isSkipFillBuffer = setSkipFillBuffer();
		    	  break;
		      case 32: //blank space
		    	  if (endOfMsg == FOUND_DATA)
		    	  {
		    	  isSkipFillBuffer = setSkipFillBuffer();
		    	  }
		     	  break;
		      case 95:  //"_"
		    	  if (endOfMsg == FOUND_DATA)
		    	  {
		    	  isUnderScore = true;
		    	  isValidBtn = false;
		    	  }
		    	  break;
		      case 75:  //K Compass mode
		    	  if (endOfMsg == FOUND_LEFTPAREN)
		    	  {
		    		  endOfMsg = FOUND_FLIPMODE;
		              Serial.println("***** Compass Mode ***********") ;
		              isUnderScore = false;
		             isSkipFillBuffer = true;
		             clearPacketBuffer();   //Redundant
			//		if ((isGamepadDataFlg == true))
					{
						isGamepadDataFlg = false;
						clearPacketBuffer();
					}
		    	  }
		    	  break;
		      case 71:  // G Gamepad mode
		    	  if (endOfMsg == FOUND_LEFTPAREN)
		    	  {
		    		  endOfMsg = FOUND_FLIPMODE;

		    	      Serial.println("*********** Gamepad Mode Monitor ************") ;
		              isUnderScore = false;
		              isSkipFillBuffer = true;
				      //  if ((isGamepadDataFlg == false))
					  {
						isGamepadDataFlg = true;
						clearPacketBuffer();
					  }
		    	  }
		   	  break;
		      case 64:  //@ default
		      case 65:  //A btn
		      case 66:  //B btn
		      case 88:  //X btn
		      case 89:  //Y btn
		      case 68:  //D error message from A2I apk
		      case 77:  //M error message from extension
		      case 83:  //S stop for gamepad
		    	  if (endOfMsg == FOUND_DATA)
		    	  {
//Serial.println("A Btn pressed   wifiloop()");
		    	  if (isUnderScore)
		    	  {
//Serial.println("A Btn valid   wifiloop()");

	//	    		  btnChar = cUDP;
		    		  ibtnUDP = iUDP;
		    		  isValidBtn = true;
		    	  }
		    	  else
		    	  {
		    		  isValidBtn = false;
	//	    		  btnChar = baseChar;
		    	  }
				  isUnderScore = false;
		    	  }
		    	  break;
		      case 123:  //"{"   begin msg
		    	  if (endOfMsg == FOUND_ZERO)
		    	  {
		    	   endOfMsg = FOUND_LEFTPAREN;
	//	    	   terminateToknFlg = low;
			       clearPacketBuffer();
			       dataStr = "";
			       isSkipFillBuffer = true;  //201128
		    	   isEndMsg = false;
		    	  }
		    	  break;
		      case 125:  //"}"   end msg  after "32" and 0D0A
		    	         // {x} where x = 0, 2,3,4,5
//Serial.print(endOfMsg);
//Serial.println("  Top of Case for endOfMsg for } right paren");


		    	  if ((endOfMsg == FOUND_0a)|| (endOfMsg == FOUND_DIR) || (endOfMsg == FOUND_FLIPMODE))
		    	  {
		    	   endOfMsg = FOUND_RIGHTPAREN;
//		    	   terminateToknFlg = high;
		    	   isEndMsg = true;
		    	   isSkipFillBuffer = true;   //201128
		    	   wifly.flush();
//Serial.print(endOfMsg);
//Serial.println("  Switch for endOfMsg set to FOUND_RIGHTPAREN");
		    	  }
		    	  break;
		      case 48: // 0 stop compass mode
		      case 50: // 2 forward compass mode
		      case 51: // 3 left compass mode
		      case 52: // 4 reverse compass mode
		      case 53: // 5 right compass mode
//Serial.print(endOfMsg);
//Serial.println("   << endOfMsg   top dir 0..5");

		    	  if ((endOfMsg == FOUND_LEFTPAREN) && (!isGamepadDataFlg))
		    	  {
		    		  endOfMsg = FOUND_DIR;
		    	      idirUDP = iUDP;
		    	      ibtnUDP = iUDP;
//Serial.print(endOfMsg);
//Serial.println("   << endOfMsg   FOUND_DIR just set");

		    	  }
		    	  else
		    	  {
		    		  endOfMsg = FOUND_DATA;
//Serial.print(endOfMsg);
//Serial.println("   << endOfMsg  else  FOUND_DATA just set");
		    	  }
		    	  break;

		      default:
		    	  if (endOfMsg == FOUND_ZERO)
		    	  {
		    	   endOfMsg = FOUND_ZERO;

//		    	  terminateToknFlg = xcare;
		    	  isUnderScore = false;
		    	  }
		    	  break;
		      }
		      if (!isSkipFillBuffer)
		      {
		    	  // adds last character read from wifi if not space of )D OA
		    	  dataStr = fillBuffer(dataStr);
		    	  isSkipFillBuffer = false;     //201128
		      }
		      else
		      {
			       dataStr = "";  //201222 // dataStr remains as shown
		      }

			  switch (endOfMsg)

			  {
			  case FOUND_KILL:
				  break;
			  case  FOUND_RIGHTPAREN:   // end bracket +  0d0a  found = end of message+}

				  display_packetBuffer();
				  switch (isGamepadDataFlg)
		     	  {
		     	  case true:

		     		  branch2parse(ibtnUDP);
		     		  break;
		     	  case false:
		     		  sortCompassData(idirUDP);
		     		  break;
		     	  }
		     	  delay(100);
		     	 endOfMsg = FOUND_ZERO;   //201231
//		     	  if (rightTheShip())
//		     		{dataStr = "";}

		    	   ibtnUDP = -9;  //reset for next valid msg
		  		   idirUDP = -9;  //reset for next valid msg

			      break;
			  default:
			      break;
			  } //switch
		} // if(getPrimerFlg())
 // 	    delay(500); //experiment with delay
	}//while available
  // send an UDP packet in every 10 second
  if ((millis() - time_point) > 10000)
  {
    time_point = millis();
    wifly.send("I'm wifly, I'm living\r\n");
  }


}


bool getPrimerFlg()
	{
		return primerFlg;
	}

void setPrimer()
	{
		primerFlg = true;
	}


bool checkForUDPactivity()
{
	return is_UDP_active;
}

void reset_is_UDP_active2false()
{
	is_UDP_active = false;
}

bool Need2Killwifi()
{
	return KillwifiFlg;
}


void close_udp()
{
//	Serial.println("close_udp()   wifi_mg");
	wifly.flush();
    wifly.clear();
	wifly.sendCommand("close\r");  // closes the TCP connection
	wifly.sendCommand("leave\r");  //disconnects the module current Access Point.

}
