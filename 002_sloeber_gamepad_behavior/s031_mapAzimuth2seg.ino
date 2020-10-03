/* Copyright © 1988-2020 by Abbott Analytical Products. All Rights Reserved.
 * Module assigns each "whole" number degree to a segment for steering control.
 * The range of degrees is from -179 to + 180 which are scaled to 1..24 segments.
 *
 * Reference arduino map()
 * https://www.arduino.cc/reference/en/language/functions/math/map/
 *
 * 200929_tr Moved from spreadsheet analysis with 24 segments to assign
 *           turn control.
 *
*/


#include "Arduino.h"
//The setup function is called once at startup of the sketch
void setup()
{
	Serial.begin(9600);

// Add your initialization code here
}

int tstAzimuth = -180;
int seg;
int const minDegree = -179;
int const maxDegree =  180;
int const minSeg    = 1;
int const maxSeg    = 24;
bool doneflag = false;


// The loop function is called in an endless loop
void loop()
{
	if (!doneflag)
	{
	int i = 0;
	while (tstAzimuth < 181)
	{
		seg = map(tstAzimuth,minDegree, maxDegree, minSeg, maxSeg );
		Serial.print(tstAzimuth);
		Serial.print("  <== Azimuth       Segment ==> ");
		Serial.println(seg);
		tstAzimuth++;
		doneflag = true;
		i++;
	}
	}
}
