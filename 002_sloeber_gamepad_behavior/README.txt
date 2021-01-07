Contents:

The first three sktechs originally began in the the Sloeber Arduino IDE environment as 
spkies to support technical design issues that arose development of the "primary"
a038_critterChaser project.  


s016_orion_otg.ino: The sketch desinged around using the Makeblock Orion (a version  of
the Arduino UNO employing RJ25 cable connections).  Ths sketch interperts joystick X/Y 
axis directions, accelerator-speed information, and command button push into a message 
package that is placed on the OTG-USB-Serial cable port.


s20_joystickCal4Orion.ino: <Eventually discarded but it may be useful to someone.> The 
sketch "adjust" readings of joystick X/Y axis directions to a meaningful rendering 
that is closer to F310 style joystick data.


s031_mapAzimuth2seg.ino: Originally the sketch was used to derive a "common sense" cutting 
of the 360 degree 'pie-slices" and on-the-fly workable differential steering.  BUT testing
showed that the hystersis between gamepad mode inputs and the vehicle response was 
unacceptable.  Testing also showed that having a tight 15 degree segment was probably too
much precision for a "chase" vehicle that had to adjust as the target critter moved.  As a 
results a six segment sloopy steering differential was developed and employed.  Test supported
this imprecise steering as satisfactory.


Later Spikes:

Joystick Calibration:
s023_orion_js_cal.ino (Recommended for Orion boards)
s015_orion_js.ino (Limited value but helpful)
The need to look into calibration of the Joysticks used on the prototype gamepad fostered the 
evolution of these two sketches.

WiFly Playground Sketch:
s033_wifly_stretch.ino allowed experimentation without too much overhead.  It is supported by the 
a039_jelly.h header file for defining Wifi authentication pairs.

