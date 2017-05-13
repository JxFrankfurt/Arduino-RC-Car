
float stepFraction;
float motorSpeedScalar;
char pbuffer [100];

void setup() {
  Serial.begin(9600);

}

void loop() {
  // read the sensor values
  stepFraction = analogRead(A1); //X value = A1 is used for mapping Q,P
  motorSpeedScalar = analogRead(A2); //Y value = A2 is speed(number of steps) both positive and negaive
  sprintf(pbuffer, "%d,%d\n",(int)stepFraction,(int)motorSpeedScalar); //set pbuffer as two int values separated by a comma
  Serial.print(pbuffer); //print pbuffer's data to the Serial(which goes to bluetooth)

}
