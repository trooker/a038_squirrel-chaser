/* Copyright © 1988-2020 by Abbott Analytical Products. All Rights Reserved.
 * Module assigns each "whole" number degree to a segment for steering control.
 * The range of degrees is from -179 to + 180 which are scaled to 1..24 segments.
 *
 * Reference arduino map()
 * https://www.arduino.cc/reference/en/language/functions/math/map/
 * 201213_tr Find a better segment split for controlling
 *           Working for four segments with the 0 to 180/-170 to -1 line
 *           splitting/centering the segments 1 and 3
 * 200929_tr Moved from spreadsheet analysis with 24 segments to assign
 *           turn control.
 *
*/


#include "Arduino.h"



int tstAzimuth = -179;
int seg = 3;
int adj_seg;
int const minDegree = -179;
int const maxDegree =  180;
int const minSeg    = 1;
int const maxSeg    = 5;
int const increment = 45;  // Degree increments for tstAzimuth
int adj_val = maxSeg -1;


void setup()
{
	Serial.begin(9600);
	Serial.println(" ");
	Serial.println("Mapping 360 Degrees to 4 segment of 90 degree each");
	Serial.println("where 0 degrees mid-point for segment 1.");

}


bool doneflag = false;



void loop()
{
	if (!doneflag)
	{
	int i = 0;
	while (tstAzimuth < 181)
	{
        switch (tstAzimuth)
        {
        case   1 ... 45:
        case -44 ... 0:
		   Serial.println("-44 to 45");
           seg = 1;
           break;
        case 46 ... 90:
        case 91 ... 135:
		   Serial.println("46 to 135");
           seg = 2;
           break;
        case 136 ... 180:
        case -135 ... -179:
		   Serial.println("136 to -135");
           seg = 3;
           break;
        case -134 ... -90:
        case -89 ... -45:
		   Serial.println("136 to -135");
           seg = 4;
           break;
        }


//		seg = map(tstAzimuth,minDegree, maxDegree, minSeg, maxSeg );
		Serial.print(tstAzimuth);
        Serial.print("  <== Azimuth       Segment ==> ");
		Serial.println(seg);
//		adj_seg = adj_val- seg;
//		Serial.print("    Adjusted   Segment ==> ");
//		Serial.println(adj_seg);
		tstAzimuth+=increment;
		doneflag = true;
		i++;
	}
	}
}
