/*
 * Connects to WiFi and reads/writes UDP messages.  The "read"
 * is parsed into a direction or an action. This sketch was developed
 * from s008.
 * 200604_tr Initial insertion of WiFi capability. See folder
 *           p006_squirelChaser for grub, images, and downloads of
 *           code/documents.
 *
 *
 */


#define SSID      _SSID
#define KEY       _PWD
// WIFLY_AUTH_OPEN / WIFLY_AUTH_WPA1 / WIFLY_AUTH_WPA1_2 / WIFLY_AUTH_WPA2_PSK
#define AUTH      WIFLY_AUTH_WPA2_PSK

#define UDP_HOST_IP        "255.255.255.255"      // broadcast
#define UDP_REMOTE_PORT    5050 //55555
#define UDP_LOCAL_PORT     5050 //55555

#define maxBuffer  255
char packetBuffer[maxBuffer];
int packetNdx = 0;

//WiFiUDP Udp;



int ledPin = 6;

// Pins' connection
// Arduino       WiFly
//  2    <---->    TX
//  3    <---->    RX
//SoftwareSerial uart(2, 3);
//WiFly wifly(uart);
//Trie their way
//SoftwareSerial uart(15, 14);
WiFly wifly(&Serial3);


void action1()
{
	Serial.println("doing Action1");
}


void action2()
{
	Serial.println("doing Action2");
}


void forward()
{
//	showDisplayDebug(" Moving Forward");
	chaseForward();
//digitalWrite(10, HIGH);
//digitalWrite(11, LOW);
//digitalWrite(12, HIGH);
//digitalWrite(13, LOW);
//digitalWrite(ledPin, HIGH);
//Serial.println(" Moving Forward");

}
void backward()
{
//	showDisplayDebug(" Moving Backward");
	chaseBackward();
//digitalWrite(10, LOW);
//digitalWrite(11, HIGH);
//digitalWrite(12, LOW);
//digitalWrite(13, HIGH);
//digitalWrite(ledPin, HIGH);
//Serial.println("Moving Backward");
}

void stop()
{
	//showDisplayDebug(" Stop Movement");
	chaseStop();

//digitalWrite(10, LOW);
//digitalWrite(11, LOW);
//digitalWrite(12, LOW);
//digitalWrite(13, LOW);
//digitalWrite(ledPin, LOW);
//Serial.println("Stop");
}
void left()
{
	//showDisplayDebug(" Rotate Left");
	chaseDCM1();
//digitalWrite(10, HIGH);
//digitalWrite(11, LOW);
//digitalWrite(12, LOW);
//digitalWrite(13, HIGH);
//digitalWrite(ledPin, HIGH);
//Serial.println(" Rotate Left");
}
void right()
{
	//showDisplayDebug(" Rotate Right");
	chaseDCM2();
//digitalWrite(10, LOW);
//digitalWrite(11, HIGH);
//digitalWrite(12, HIGH);
//digitalWrite(13, LOW);
//digitalWrite(ledPin, HIGH);
//Serial.println(" Rotate Right");
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

	pinMode (10, OUTPUT);
	pinMode (11, OUTPUT);
	pinMode (12, OUTPUT);
	pinMode (13, OUTPUT);
	pinMode(ledPin, OUTPUT);

	clearPacketBuffer();

//Serial primary set up earlier    Serial.begin(9600);
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

unsigned int time_point = 0;

char cUDP;


void wifiloop()
{

 //  if (wifly.available()) {
  //  Serial.print((char)wifly.read());
 // }
	clearPacketBuffer();
	while (wifly.available())
	{
		cUDP = (char)wifly.read();
		packetBuffer[packetNdx] = cUDP;
		packetNdx++;
//dev		Serial.println(packetBuffer);
		if (packetBuffer[packetNdx] == '\0')
		{
			//dev
			Serial.println(packetBuffer);
			//showDisplay(packetBuffer);
	    if((strcmp(packetBuffer, "forward") == 0) ||
	    		(strcmp(packetBuffer, "2") == 0))
	    {
//dev Serial.println("forward hans ");
	      forward();
	      clearPacketBuffer();
	    } else
	    if((strcmp(packetBuffer, "backward") == 0) ||
		(strcmp(packetBuffer, "4") == 0))
	    {
	      backward();
	      clearPacketBuffer();
	    } else
	    if((strcmp(packetBuffer, "stop") == 0) ||
	    	(strcmp(packetBuffer, "0") == 0))
	    {
	      stop();
	      clearPacketBuffer();
	    } else

	    if((strcmp(packetBuffer, "left") == 0) ||
	       (strcmp(packetBuffer, "3") == 0))
	    {
	      left();
	      clearPacketBuffer();
	    } else
	    if((strcmp(packetBuffer, "right") == 0) ||
	    		(strcmp(packetBuffer, "5") == 0))
	       {
	      right();
	      clearPacketBuffer();
	    }
	    else
//Rootaid WiFi Command Center has error in action1 and
//  action2
	    if(strcmp(packetBuffer, "action 1") == 0){
	    	action1();
	        clearPacketBuffer();
	    }
	    else
	    if(strcmp(packetBuffer, "action2") == 0){
	    	action2();
	        clearPacketBuffer();
	    }
		} //if \0

	  }//while available

  // send an UDP packet in every 10 second
  if ((millis() - time_point) > 10000)
  {
    time_point = millis();
    wifly.send("I'm wifly, I'm living\r\n");
  }
}
