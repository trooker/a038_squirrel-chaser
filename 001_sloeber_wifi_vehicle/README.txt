1> WIFI ready feature:
1.1> The a038_critterChaser applicaiton addresses the WIFI access point and seeks a connection 
based upon user defined authentication credentials that must be changed to suit the user's 
specific WIFI access point.  The credentials can be modifed in the a038_jelly.h header file.
Once the change has been made, built, and uploaded to the vehicle's Mega256 Arduino board.

1.2> The a038_critterChaser applicaiton handles two different types of messages.  The first is 
information that tell the application which operational mode (Compass or Gamepad) the user has
selected.  The type of message is a package that contains encode vehcile control information.  
Predicated upon the user's selectioin of playingfield

2> Vehicle Control
2.1> The a038_critterChase application employs the standard Arduino setup and loop program flow.
Application activities that require a one-time-only initialization to known states, values, 
conditions, or functional readiness are completed in in this phase.  Immediately after the setup 
completes the looping phase begins.  Within the looping phase there is a "watchdog time" that
that recognizes when the "user" has not interfaced with the looping for a significant amount of 
time.  The "watchdog-timer" throws the "off-switch" by playing the tune "shave and haircut" then 
begins the power-down of motors, displays, led, WIFI connection, etc.  The Mega256 continues to 
loop aimlessly with no activity until power is physically removed.

2.2> The a038_critterChase application's WIFI manager reads incoming WIFI UDP messages and parses
the appropriate playing field and motion requested by the Android user.  That information is 
adapted to the appropriate motor control and speed.  The button command is interperted in a similar
manner depending upon the playingfield.  
   
