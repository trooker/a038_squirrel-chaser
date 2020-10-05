Contents:

These three sktechs originally began in the the Sloeber Arduino IDE environment as 
spkies to support technical design issues that arose development of the "primary"
a038_critterChaser project.  


s016_orion_otg.ino: The sketch desinged around using the Makeblock Orion (a version  of
the Arduino UNO employing RJ25 cable connections).  Ths sketch interperts joystick X/Y 
axis directions, accelerator-speed information, and command button push into a message 
package that is placed on the OTG-USB-Serial cable port.


s20_joystickCal4Orion.ino: The sketch "adjust" readings of joystick X/Y 
axis directions to a meaningful rendering that is closer to F310 style joystick data.


s031_mapAzimuth2seg.ino: The sketch was used to derive a "common sense" cutting of the 
360 degree 'pie-slices" and on-the-fly workable differential steering.  

