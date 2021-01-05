/* Copyright © 1988-2020 by Abbott Analytical Products. All Rights Reserved.
 * 201227_tr Used the wifly udp example cut for a038 to verify reading
 *           from AP.  Commented-out the original tray at controllong
 *           movement of vehicle.
 * 201207_tr WOrks with 954 but not 953.
 * 201130_tr works well wit a038_954_gamepad_otg_wifi to mega
 *           now reads the G and K playingflied change.
 * 201127_tr WOrks well with the Android -Omega gamepad and
 *           a038_659_gamepad_otg_wifi to Mega256
 * 201125_tr
 * ==>arduino alternate ascii characters
 * https://forum.arduino.cc/index.php?topic=278393.0
 *
 *
 * runs well with a038_969_gamepd_otg_wifi
 * not so well with 968 and the 21/0x15 NAK termiator
 * https://forum.arduino.cc/index.php?topic=109308.0
 *
* WiFly_Device Test Platform
* A simple tester for communicating with the WiFly GSx RN-131b/g series.
* LGPL 2.0
* Tom Waldock, 2011
*/

#include <Arduino.h>
#include <Streaming.h>
#include <SoftwareSerial.h>
#include "WiFly.h"
#include "MemoryFree.h"
#include "Credentials.h"
#include "a038_jelly.h"


// Pins are 3 for INCOMING TO Arduino, 5 for OUTGOING TO Wifly
// Arduino       WiFly
//  2 - receive  TX   (Send from Wifly, Receive to Arduino)
//  3 - send     RX   (Send from Arduino, Receive to WiFly)
//WiFlySerial WiFly(2,3);
WiFly wifly(&Serial3);


#define SSID      _SSID  // set in a038_jelly.h
#define KEY       _PWD   // set in a038_jelly.h
// WIFLY_AUTH_OPEN / WIFLY_AUTH_WPA1 / WIFLY_AUTH_WPA1_2 / WIFLY_AUTH_WPA2_PSK
#define AUTH      WIFLY_AUTH_WPA2_PSK

unsigned int time_point = 0;


#define UDP_HOST_IP        "255.255.255.255"      // broadcast
#define UDP_REMOTE_PORT    5050 //55555
#define UDP_LOCAL_PORT     5050 //55555

#define maxBuffer 255  //201124   15 //-xxx_-yyy_sss_A  //255
char packetBuffer[maxBuffer];
int packetNdx = 0;


const char baseChar = '*';
/*

enum tristate {
  low,
  high,
  xcare
};

tristate terminateToknFlg;



//s014_stringCharacter2Serial
#define STRINGER(s)   #s
#define MAKE_STRING(s) STRINGER(s) //see a004_table_ascii_keyboard.pdf
#define _NAK_ MAKE_STRING(021)    //NAK negative ack  https://www.asciitable.com/
#define _ALPHA_ MAKE_STRING(224)
#define _OMEGA_ MAKE_STRING(234)

unsigned char inByte=0;




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
//  wifly.sendCommand("set comm match 21\r");  // sets the terminator character to NAK
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




void setup() {

  Serial.begin(9600);
//  inByte = MAKE_STRING(\021);

  Serial.println(F("Starting WiFly Tester." ) );
  Serial << F("Free memory:") << freeMemory() << endl;
  wifly.init();
  wifisetup();
  Serial << F("Post Setup UDP Free memory:") << freeMemory() << endl;
  wifly.flush();
  while (wifly.available() )
    wifly.read();

}

bool isGamepadDataFlg = false;
bool isEndMsg = false;
bool isUnderScore  = false;  //does the "_" lead a btn
char cUDP;
int iUDP;
bool isValidBtn;


void doWork(String dataStr)
{
	Serial.println("\n dataStr ....::  " + dataStr);

}

String fillBuffer(String dataStr)
{
	String holdStr = dataStr;
	  packetBuffer[packetNdx] = cUDP;
	  packetNdx++;
	  holdStr = packetBuffer;
	return holdStr;
}


void loop() {
	String tst4ASCIIchar;
	int bytesSent;
    int incomingByte = 0; // for incoming serial data
	char btnChar;   //@,A, B, X, Y for F310
	String dataStr = "";
	bool isSkipFillBuffer = false;

//201226   Serial.println("*****");
  while(wifly.available() > 0)
  {
	   //201124
	//  while (wifly.receive((uint8_t *)&incomingByte, maxBuffer, 300) > 0)
	  {//start while read
//		cUDP = (char)incomingByte;
	  iUDP = wifly.read();
	  cUDP = (char) iUDP;
	  isSkipFillBuffer = false;
	  Serial.print("WiFly.read()....:: ");
	  Serial.println(iUDP);
      switch(iUDP)
      {
      case 10:
      case 13:
      case 32:
//    	  Serial.println("pre (isValBtn)");
    	  if (isValidBtn)
    	  {
    		  isSkipFillBuffer = true;
//        	  Serial.println("post (isValBtn)  isSkipFillBuffer = true");
    	  }
     	  break;
      case 95:
    	  isUnderScore = true;
    	  isValidBtn = false;
    	  break;
      case 75:  //K Compass mode
          Serial.println("***** Compass Mode Ignore for now ***********") ;
          isGamepadDataFlg = false;
          isUnderScore = false;
    	  break;
      case 71:  //Gamepad mode
          Serial.println("*********** Gamepad Mode Monitor ************") ;
          isGamepadDataFlg = false;
          isUnderScore = false;
   	  break;
      case 64:  //@ default
      case 65:  //A btn
      case 66:  //B btn
      case 88:  //X btn
      case 89:  //Y btn
//probably need a isValidBtn
    	  if (isUnderScore)
    	  {
    		  btnChar = cUDP;
    		  isValidBtn = true;
    	  }
    	  else
    	  {
    		  isValidBtn = false;
    		  btnChar = baseChar;
    	  }
		  isUnderScore = false;
    	  break;
      case 123:
    	  terminateToknFlg = low;
	       clearPacketBuffer();
	       dataStr = "";
    	   isEndMsg = false;
    	  break;
      case 125:
    	   terminateToknFlg = high;
    	   isEndMsg = true;
    	   wifly.flush();
    	  break;
      default:
    	  terminateToknFlg = xcare;
    	  isUnderScore = false;
    	  break;
      }
      if (!isSkipFillBuffer)
      {
          dataStr = fillBuffer(dataStr);

      }
      else
      {
    	 // dataStr remains as shown
      }

	  switch (terminateToknFlg)

	  {
	  case  high:
     	  doWork(dataStr);
		  clearPacketBuffer();
		  dataStr = "";
		  wifly.flush();
    	  terminateToknFlg = xcare;
          break;
	  case low:
	      break;
	  case xcare:
	  default:
	           break;
	  } //switch

	  } //while reading
  }
} //loop
*/



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

void setup() {

  Serial.begin(9600);
  Serial.println("--------- WIFLY UDP --------");

  Serial.begin(9600);     // WiFly UART Baud Rate: 9600
  wifly.reset();

  while (1) {
    Serial.println("Try to join " SSID );
    if (wifly.join(SSID, KEY, AUTH)) {
      Serial.println("Succeed to join " SSID);
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

//unsigned int time_point = 0;

void loop() {
  if (wifly.available()) {
    Serial.print((char)wifly.read());
  }

  // send an UDP packet in every 10 second
  if ((millis() - time_point) > 10000) {
    time_point = millis();

    wifly.send("I'm wifly, I'm living\r\n");
  }
}
