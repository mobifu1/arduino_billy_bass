/*
  Make a Sound to DC Motor Move.
  This example code is in the public domain.
*/
#include <AFMotor.h>

// Select which 'port' M1, M2, M3 or M4. In this case, M1 for mouth, M2 for body and M3 for tail
AF_DCMotor Mouth_Motor(1);// define motor on channel 4 with 1KHz default PWM
AF_DCMotor Body_Motor(2);

// Some other Variables we need
int SoundInPin = A0; //sound in for OK Google
int LedPin = 13; //in case you want an LED to activate while mouth moves
int sensorValue  = 0;
int pause_counter = 0;
boolean billy_big_mouth_bass_on = true; //global on/off by playing music

//-------------------------------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);

  // Set the speed to start, from 0 (off) to 255 (max speed)
  Mouth_Motor.setSpeed(0); //mouth motor
  Mouth_Motor.run(FORWARD);
  // turn on motor
  Mouth_Motor.run(RELEASE);
  //------------
  Body_Motor.setSpeed(0); //body motor
  Body_Motor.run(FORWARD);
  // turn on motor
  Body_Motor.run(RELEASE);
  //------------
  pinMode(SoundInPin, INPUT);
  pinMode(LedPin, OUTPUT);
  //------------
  Serial.println("Setup End");
}

//-------------------------------------------------------------------------------------------------------
void loop() {

  // read the input on analog pin 0:
  sensorValue = analogRead(SoundInPin);
  //Serial.println(String(sensorValue));
  // we Map another value of this for LED that can be a integer betwen 0..255
  int LEDValue = map(sensorValue, 0, 512, 0, 255);
  // We Map it here down to the possible range of  movement.
  sensorValue = map(sensorValue, 0, 512, 0, 180);
  // note normally the 512 is 1023 because of analog reading should go so far, but I changed that to get better readings.
  int MoveDelayValue = map(sensorValue, 0, 255, 0, sensorValue);

  // maping the same reading a little bit more down to calculate the time your motor gets
  if (sensorValue > 10) { // to cut off some static readings
    delay(1);  // a static delay to smooth things out...
    if (billy_big_mouth_bass_on == true) mouth_move();
    delay(1);
    pause_counter = 0;
    //Serial.println("Reset");
  }
  else {

    if (pause_counter >= 0) {
      pause_counter++;
      //Serial.println("Incr");
    }
    if (pause_counter > 5000)motor_release();
  }
}
//-------------------------------------------------------------------------------------------------------
void mouth_move() {

  //Serial.println("Mouth Move");
  int randnumber = random(1, 4);
  analogWrite(LedPin, sensorValue);
  // now move the motor
  body_move(255);
  Mouth_Motor.run(FORWARD);
  for (int i = 140; i < 255; i++) {
    Mouth_Motor.setSpeed(i);
    delay(randnumber);//value 1-4
  }
  Mouth_Motor.run(RELEASE);//stop
  delay(50);
  analogWrite(LedPin, 0);
}
//-------------------------------------------------------------------------------------------------------
void body_move(int body_speed) {

  // now move the motor
  Body_Motor.run(FORWARD);
  Body_Motor.setSpeed(body_speed);
}
//-------------------------------------------------------------------------------------------------------
void tail_move(int tail_speed) {

  // now move the motor
  Body_Motor.run(BACKWARD);
  Body_Motor.setSpeed(tail_speed);
}
//-------------------------------------------------------------------------------------------------------
void motor_release() {

  pause_counter = -1;
  //Serial.println("Release");
  Body_Motor.run(RELEASE);//stop
}
//-------------------------------------------------------------------------------------------------------
