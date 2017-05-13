#include "Stepper_8.h"
#define X_AXIS (A1) //X value = A1
#define Y_AXIS (A2) //Y value = A2
//note: for our joystick(2 potentiometers), baseline values are y=500 and x=497.

const int STEPS_PER_REVOLUTION = 256; //change this to the number of steps on your stepper motor
const int SEQUENCING = 8; //can be 8 or 4, changes the way the motors respond, 8 is more fluid i think.
float motorSpeed;
float Q; //mapped fraction multiplied by left motor steps (used to turn left)
float P; //mapped fraction multiplied by right motor steps (used to turn right)
int counter = 0;
//initialize the stepper library on pins 4-7, 8-11:
//basically, these are the arduino pins you plug your motor into.
//changing their order changes which direction the motors try to go given a particular joystick position
Stepper_8 myStepperR(STEPS_PER_REVOLUTION, SEQUENCING, 4, 5, 6, 7);
Stepper_8 myStepperL(STEPS_PER_REVOLUTION, SEQUENCING, 8, 9, 10, 11);
char pbuffer [100];
int read1, read2;
int bI = 0; //buffer index
void setup() {
  Serial.begin(9600); //make sure your bluetooth devices are both using the correct maud rate
  Serial.println("ZoomZoom"); //tells troubleshooter there's a good connection
}

void loop() {
  //my thanks to Chandler Griscom and Evan Boone for this if-statement
  //this block of code is long and weird because the bluetooth devices prefer to only send one piece of data
  //this block allows two pieces of data to be parsed out of the one
  if (Serial.available() > 0) {
    do {
      while (Serial.available() == 0) continue; //maybe gets rid of bugs
      pbuffer[bI] = Serial.read(); //gets one byte from serial buffer
      if (pbuffer[bI] == 255) continue; //fixes another bug which prevents troubleshooting and data collection
      bI++;
    }//reads seven charactesr and assigns them to pbuffer
    while (pbuffer[bI - 1] != '\n'); //indexed at bI-1

    sscanf(pbuffer, "%d,%d", &read1, &read2); //
    bI = 0; //prevents overflow of characters
    Serial.print(read1);
    Serial.print(" ");
    Serial.print(read2);
    Serial.print('\n');
    //this prints the data for data collection/troubleshooting
    //it can be removed for faster runtime
  }
  //map values to a different range:
  Q = map(read1, 498, 1023, 1, 0); //mapped fraction multiplied by left steps
  P = map(read1, 0, 497, 0, 1); //mapped fraction multiplied by right steps

  if (read2 > 499) {
    motorSpeed = map(read2, 500, 1023, 0, 200);
  }
  else {
    motorSpeed = map(read2, 499, 0, 0, 200);
  }
  //these look opposite of what they should be, so maybe that's why we had to write in a negative later
  myStepperR.setSpeed(motorSpeed);//Left motor
  myStepperL.setSpeed(motorSpeed);//Right motor

  if (motorSpeed > 0) {
    if (read2 < 500) {
      myStepperR.step(-1 * Q); //hacked to reverse as noted above
      myStepperL.step(1 * P);
    }
    else {
      myStepperR.step(1 * Q);
      myStepperL.step(-1 * P); //same as above
    }
  }
  //step one revolution (times the fraction) in one direction, negative means the opposite direction
  //motors only step if the speed is not zero

}
