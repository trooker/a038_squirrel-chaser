# a038_squirrel-chaser
A WIFI remote control ground mobile vehicle for harmlessly engaging squirrels, chipmunks, and other small critter.

The purpose of this vehicle is to encourage ground critters to migrating to a more suitable region has been developed.  This ground vehicle
is controlled by a gamepad using joystick motion, a speed control, and buttons for commanding flashes, buzzers, LEDs, etc.  The user remotely controls the vehicle over WIFI via a gamepad-OTG-Android system component.  The vehicle itself receives its WIFI directions/instructions via 
an on-board WIFI shield-Arduino Mega256.

A full technical presentation of the project can be found at http://abbottanp.com.  From the Learn: section of the web page select "Knowledge Center".  That will provide a table of projects available as OpenSource of which a038_squirrel-chaser is near the top for 2020.

The squirrel-chaser design was compartmentized into a Android-WIFI-Arduino vehicle baranch and a gamepad-Arduino-OTG-Android branch.  Both branches have matured to the point were integration is the next step.  Both branches and the integration required the tools shown below.  


Development Tools used include: 
Sloeber for Arduino *.ino, *.h.  
Eclipse-Java for MIT App2Inventor extensions and build
MIT App2Inventor for creation of Android *.apk and *.aia
Android Studion SDK was used early for various test spikes into the Android WIFI and Gamepad-OTG-Android 
