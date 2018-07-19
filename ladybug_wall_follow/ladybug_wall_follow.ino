#include "defines.h"
#include <NewPing.h>

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

//*************//
//PID Constants//
float kp = 3.0;
float kd = 0;
//*************//

//More PID constants
int setpoint = 20;
int baseSpeed = 150;
int maxSpeed = 255;
int minSpeed = 0;

//PID variables
int pos, error, lastError;
int correction;
int motor1Speed, motor2Speed;

void setup() {
  Serial.begin(115200);

  //initialize the pins
  pinInit();

  //wait for the user to press the button
  while (digitalRead(USER_BUTTON)) {}
}

void loop() {
  kp = (float)analogRead(KNOB_PIN) / 100;

  pos = sonar.ping_cm();
  error = pos - setpoint;
  correction = (int)(kp * (float)error + kd * (float)(error - lastError));
  lastError = error;

  motor1Speed = baseSpeed + correction;
  motor2Speed = baseSpeed - correction;

  motor1Speed = constrain(motor1Speed, minSpeed, maxSpeed);
  motor2Speed = constrain(motor2Speed, minSpeed, maxSpeed);

  setSpeeds(motor1Speed, motor2Speed);

  Serial.print("po: "); Serial.print(pos); Serial.print("\t");
  Serial.print("er: "); Serial.print(error); Serial.print("\t");
  Serial.print("co: "); Serial.print(correction); Serial.print("\t");
  Serial.print("m1: "); Serial.print(motor1Speed); Serial.print("\t");
  Serial.println();
}
