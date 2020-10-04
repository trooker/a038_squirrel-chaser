/*
 * Connects to WiFi and reads/writes UDP messages.  The "read"
 * is parsed into a direction or an action. This sketch was developed
 * from s008.
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



#define SSID      _SSID  // set in a038_jelly.h
#define KEY       _PWD   // set in a038_jelly.h
// WIFLY_AUTH_OPEN / WIFLY_AUTH_WPA1 / WIFLY_AUTH_WPA1_2 / WIFLY_AUTH_WPA2_PSK
#define AUTH      WIFLY_AUTH_WPA2_PSK

unsigned int time_point = 0;


#define UDP_HOST_IP        "255.255.255.255"      // broadcast
#define UDP_REMOTE_PORT    5050 //55555
#define UDP_LOCAL_PORT     5050 //55555

#define maxBuffer  255
char packetBuffer[maxBuffer];
int packetNdx = 0;
bool isGamepadDataFlg = false;   //false: Assumes 4 flat arrows+stop
                                  //true: using Arduino JS uno syntax
                                  //!x-axis_y-axis_speed_character
                                  //numeric positive 0...999
                                  //        negavtive -999..-1
                                  //character @, A, B, X, Y
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

bool is_UDP_active = false;
bool KillwifiFlg   = false;  //on-board button kill switch

int sound2Knt = 0;
const int sound2Stopper = 10;
int buzzKnt4wifi = 0;        // Switch for shave & haircut
int buzzOff4wifi = 1;







void soundOf2()
{
if ((buzzKnt4wifi < buzzOff4wifi) && (sound2Knt < sound2Stopper))
{
   soundBuzz3(); //
   buzzKnt4wifi++;
Serial.println("Buzzer plays soundBuzz3()");
Serial.println("Played sound once.");
showDisplay("Do A() Play once.");

 }
 sound2Knt ++;
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
	Serial.println("DoDefault():  Working really hard.");
	showDisplay("DoDefault():  Working really hard.");
	;// Do something important
}

void  DoActionA()
{
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
	Serial.println(packetBuffer);

}

void sortAndroid()
{
		if (packetBuffer[packetNdx] == '\0')
		{
			//showDisplay(packetBuffer);
	    if((strcmp(packetBuffer, "forward") == 0) ||
	    		(strcmp(packetBuffer, "2") == 0))
	    {
//dev Serial.println("forward hans ");
	      display_packetBuffer();
	      forward();
	      clearPacketBuffer();
	    } else
	    if((strcmp(packetBuffer, "backward") == 0) ||
		(strcmp(packetBuffer, "4") == 0))
	    {
	      display_packetBuffer();
	      backward();
	      clearPacketBuffer();
	    } else
	    if((strcmp(packetBuffer, "stop") == 0) ||
	    	(strcmp(packetBuffer, "0") == 0))
	    {
		  display_packetBuffer();
	      stop();
	      clearPacketBuffer();
	    } else

	    if((strcmp(packetBuffer, "left") == 0) ||
	       (strcmp(packetBuffer, "3") == 0))
	    {
	      display_packetBuffer();
	      left();
	      clearPacketBuffer();
	    } else
	    if((strcmp(packetBuffer, "right") == 0) ||
	    		(strcmp(packetBuffer, "5") == 0))
	    {
	      display_packetBuffer();
	      right();
	      clearPacketBuffer();
	    }
/*	not used
//	    else

//Rootaid WiFi Command Center has error in action1 and
//  action2
//	    if(strcmp(packetBuffer, "action 1") == 0){
//	    	action1();
//	        clearPacketBuffer();
//	    }
//	    else
//	    if(strcmp(packetBuffer, "action2") == 0){
//	    	action2();
//	        clearPacketBuffer();
//	    }
 *
 */
		} //end if == '\0| lookahead
}


/* https://forum.arduino.cc/index.php?topic=186687.0
 *
 * void testChar()
{
  char *str;
  char sz[] = "will,this,work,70,4";
  char *p = sz;
  while ((str = strtok_r(p, ",", &p)) != NULL)  // Don't use \n here it fails
    {
     mySerial.println(str);
     mySerial.println("location 7");
    }
}

 *
 *
 */



typedef struct
 {
     String str_x;
     String str_y;;
     String str_speed;
     String str_btn;
 }  record_type;


//record_type elements; ;

/*
struct elements check4Eleemnts()
	{
	    record_type elements;

		char *split_str_element;
		char *p = packetBuffer;
		int kntr = 0;
		int stopper = 4;
		  while (((split_str_element = strtok_r(p, "_", &p)) != NULL) && kntr < stopper)  // Don't use \n here it fails
		    {
Serial.print(kntr);
Serial.print(" :: kntr   ");
Serial.print(split_str_element);
Serial.println("   <<<===: Value Parsed");
			  switch (kntr)
				 {
			     case 0: elements.str_x = split_str_element; break;
			     case 1: elements.str_y = split_str_element; break;
			     case 2: elements.str_speed = split_str_element; break;
			     case 3: elements.str_btn = split_str_element; break;
				 }

		    } // end of while
	    return elements;
	}
*/

/*
 * Poor best practice.
 * i.e. Should separate the check4Eleemnts() using the struct elements
 * but it did not seem to want to smoothly pass the records values.
 * Some kind of scope issue.
 */


 //need to handle special message "0_0_0_@" such that all stops

void sortGamePadData()
{
	String test4stop = "0_0_0_S";
	String dataStr = packetBuffer;
		//dev
	Serial.print("Incoming cUDP:  ");

	display_packetBuffer();
//	Serial.println("First dataStr:  " + dataStr);
//	        record_type elements = check4Eleemnts();

    record_type elements;
    String pval_x, pval_y, pval_speed, pval_btn;  //A = 65; B = 66; X = 88; Y = 89; @ = 64
	char *split_str_element;
	char *p = packetBuffer;
	int kntr = 0;
	int stopper = 4;
	if (!(dataStr == test4stop))
	{
	  while (((split_str_element = strtok_r(p, "_", &p)) != NULL) && kntr < stopper)  // Don't use \n here it fails
	    {


//Serial.print(kntr);
//Serial.print(" :: kntr   ");
//Serial.print(split_str_element);
//Serial.println("   <<<===: Value Parsed");
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
		    default:
		    	    DoDefault();
		    	    break;
			} // end of btn action switch
		//    Serial.println("1559  x: " + pval_x  + "    y: " + pval_y + "   Speed: "  + pval_speed);
	} //not test4stop
	else
	{//special case that causes trouble.  This seems to work well by not running the chaser.
		pval_x = "0";
		pval_y = "0";
		pval_speed = "0";
		pval_btn = "64";
  	    digitalWrite((sigId_led+0),HIGH);
  	    delay(20);
  	    digitalWrite((sigId_led+0),LOW);
	}
    setSpeed4Vehicle(pval_speed.toInt());
    setDirection(pval_x.toInt(), pval_y.toInt());
    clearPacketBuffer();
}



void clearPacketBuffer()
{
	packetNdx       = 0;
	for (int i=0;i<maxBuffer;i++)
	{
	    packetBuffer[i] = {0};
	}
	//discovered a need to slowdown the clear
	delay(250);
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
}


//char cUDP;

void wifiloop()
{
	char cUDP;


 //  if (wifly.available()) {
  //  Serial.print((char)wifly.read());
 // }
	clearPacketBuffer();
	while (wifly.available())
	{

		is_UDP_active = true;
		if (checkBtn())
		{
			Serial.println("On-board Kill Switch just pressed");
			is_UDP_active = false;
			close_udp();
			KillwifiFlg = true;
			break;
		}
		cUDP = (char)wifly.read();
		packetBuffer[packetNdx] = cUDP;
		packetNdx++;

//dev		Serial.println(packetBuffer);
		if ((isGamepadDataFlg == false) && (strcmp(packetBuffer, "G") == 0))
		{
			isGamepadDataFlg = true;
			Serial.println("User Switched to Gamepad Mode");
		}
		else
			if ((isGamepadDataFlg == true) && (strcmp(packetBuffer, "K") == 0))
			{
				isGamepadDataFlg = false;
				Serial.println("User Switched to Compass Mode");
			}

		if (isGamepadDataFlg == false)
		{ sortAndroid();} //end of if isGamepadDataFlg == false
		else
		{
			String tstStr;
			tstStr = cUDP;
//			Serial.println("Incoming cUDP:  " +tstStr);
			if ((tstStr.compareTo("@") == 0)||
				(tstStr.compareTo("M") == 0)||	 // error message from extension
				(tstStr.compareTo("D") == 0)||	 // error message from A2I apk
				(tstStr.compareTo("A") == 0)||
				(tstStr.compareTo("B") == 0)||
				(tstStr.compareTo("X") == 0)||
				(tstStr.compareTo("Y") == 0))
			    {
				//	Serial.println("Incoming cUDP:  " +tstStr + " Packet going to parser");
					sortGamePadData();
			    }
		}
	  }//while available

  // send an UDP packet in every 10 second
  if ((millis() - time_point) > 10000)
  {
    time_point = millis();
    wifly.send("I'm wifly, I'm living\r\n");
  }
}





bool checkForUDPactivity()
{
	return is_UDP_active;
}

void reset_is_UDP_active()
{
	is_UDP_active = false;
}

bool Need2Killwifi()
{
	return KillwifiFlg;
}

void close_udp()
{
    wifly.clear();
	wifly.sendCommand("close\r");  // closes the TCP connection
	wifly.sendCommand("leave\r");  //disconnects the module current Access Point.

}
