/* Copyright © 1988-2020 by Abbott Analytical Products. All Rights Reserved.
 * 200604_tr Adapted the ams2.ino of a034_ project for a038
 *           replacing the steppers with dc motors
 *           leftdcm1 and rightdcm2.
 *
 */

#include "a038_config.h"


int ams2_SPEED = 50;  //swag at starting spped of both motors
int leftdcm1_SPEED = ams2_SPEED;
int rightdcm2_SPEED = ams2_SPEED;
int dcHold = 400;  // delay in halting
                   // 100  failed
                   // 1000 ok
                   // 500, 400 good
                   // 250, 300, 350 marginal
void chaseForward()
{
	rightdcm2->run(FORWARD);
	leftdcm1->run(FORWARD);
    delay(dcHold);
}

void chaseBackward()
{
	leftdcm1->run(BACKWARD);
	rightdcm2->run(BACKWARD);
    delay(dcHold);

}

void chaseDCM1()
{
	leftdcm1->run(FORWARD);
	rightdcm2->run(RELEASE);
    delay(dcHold);

}

void chaseDCM2()
{
	leftdcm1->run(RELEASE);
	rightdcm2->run(FORWARD);
    delay(dcHold);

}


void chaseStop()
{
   leftdcm1->run(RELEASE);
   rightdcm2->run(RELEASE);
}

void setup_dcm()
{
leftdcm1 ->setSpeed(125);
rightdcm2 ->setSpeed(125);
leftdcm1->run(FORWARD);
rightdcm2->run(FORWARD);
// turn on motor
leftdcm1->run(RELEASE);
rightdcm2->run(RELEASE);
delay(dcHold);
}



