The a038_973_gamepad_otg_wifi.apk Android app installation package will install
tje a038_973_gamepad_otg_wifi application.   

1> Vehicle Control Featrue:
1.1> The a038_973_gamepad_otg_wifi provides the Android with two modes of operation for
controlling vehicles: Compass Mode and Gamepad Mode.  These modes are selectable "on-the-fly".
The playing field of each is slightly different but intuitive.  

1.2> The first mode is the Compass Mode:  The Compass Mode provides a reliable set 
compass points for froward, backward, left, and right directrional control movement icons.
A  stop icon "halts".  This mode refelcts the user's directional/motion changes faster 
than the second mode.  But there is no means to alter speed, rate of turnning, 
or command buttons.


1.3> The second mode is the Gamepad Mode: This is a non-standard gamepad that relies upon
the Arduino UNO board and its associated listing in the USBSerial.jar library.  This application 
provides the interface between the Orion (Makeblock version of Arduino UNO board) used to 
interpert Abbottanp Gamepad) with the OTG-USB attached Android.  The OTG-USB
attached Android operates as a Host by communicating wtih  the Orion-Gamepad as a client. 

2> WIFI ready feature:
2.1> The a038_973_gamepad_otg_wifi applicaiton address the WIFI access point using UDP.  
The connection is predicated upon the existing WIFI connection process of the Android
to which the Gamepad is attached.ilt, and uploaded to the  

2.2> The a038_973_gamepad_otg_wifi does not "care" which mode the user employs at any specific
time slice.  As the playingfield is changed by the user, the application sense the change 
and sends a message via WIFI to the vehicle to prepare to recieve control information 
originating from the mode of operation the user currently has selected.  Regardless of 
the operational mode (Compass or Gamepad) the application creates a package for transmission 
to the WIFI access point.  

3> General Operation
3.1> Regardless of the operational mode selected, the WIFI UDP connection details and status 
information is provided just above the playingfield displayed.  The WIFI connection and 
basic motion reset can be invoked by pressing the bttons indicated.

3.2> The playing field can be switched as desired using the appropriate "Start Gamepad" or 
"Halt Gamepad" buttons found along the botton of the playing field.  When all four of the LED
icons appear as yellow that indicates that Compass Mode playingfield is active.  When the LED 
icons are red that indicates that the Gamepad Mode is active.  One of the four red LED icons
may be switch to green to indicated the button selected.  Currtently, the "default: @" or 
"A" buttons are implemented.

3.3> Five labels may be observed.  The X axis value, Y axis value, speed and button.  The X/Y 
distance can be expressed as an integer withot or with a "-" indicating the direction.  The
speed will be expressed as an integer value between 0 and 1000.  The "package transmitted
is shown as a string xxx_yyy_spe_@ format.

        |            |
        |     /\     |
____X___|____________|____Y______
        |            |
    <   |            |    >
        |            |
____S___|____________|___X_Y_S @_
        |            |
        |     \/     |
        |            |
        |      @     |
           o o o o
