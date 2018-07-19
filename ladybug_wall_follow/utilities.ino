//Set the speeds of the left and right motor.
//Inputs: speed1, speed2 are integers between -255 and 255
//Negative speed will set the motors in reverse, positive speed will set the
//motors foward, speed == 0 will set the motors to stop (coast).
void setSpeeds(int speed1, int speed2) {

  //make sure the speeds are in the range specified
  speed1 = constrain(speed1, -255, 255);
  speed2 = constrain(speed2, -255, 255);

  //set motor 1 direction
  if (speed1 == 0) {              //stop (coast)
    digitalWrite(PIN1A, LOW);
    digitalWrite(PIN1B, LOW);
  } else if (speed1 > 0) {        //forward
    digitalWrite(PIN1A, HIGH);
    digitalWrite(PIN1B, LOW);
  } else {                        //reverse
    digitalWrite(PIN1A, LOW);
    digitalWrite(PIN1B, HIGH);
  }

  //set motor 2 direction
  if (speed2 == 0) {              //stop (coast)
    digitalWrite(PIN2A, LOW);
    digitalWrite(PIN2B, LOW);
  } else if (speed2 > 0) {        //forward
    digitalWrite(PIN2A, HIGH);
    digitalWrite(PIN2B, LOW);
  } else {                        //reverse
    digitalWrite(PIN2A, LOW);
    digitalWrite(PIN2B, HIGH);
  }

  //set the PWM output for motor 1 and 2
  //use abs(speed) to ensure a positive number is sent to analogWrite
  analogWrite(PWM1, abs(speed1));
  analogWrite(PWM2, abs(speed2));
}

//Stop both motors and put them in brake mode
void brake() {
  analogWrite(PWM1, 0);
  analogWrite(PWM2, 0);
  digitalWrite(PIN1A, HIGH);
  digitalWrite(PIN1B, HIGH);
  digitalWrite(PIN2A, HIGH);
  digitalWrite(PIN2B, HIGH);
}

//Initialize the pin for the arduino kit
void pinInit() {
  //set motor pins to OUTPUT
  pinMode(PIN1A, OUTPUT);
  pinMode(PIN1B, OUTPUT);
  pinMode(PIN2A, OUTPUT);
  pinMode(PIN2B, OUTPUT);
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);

  //Set Servo pin to output
  pinMode(SERVO_PIN, OUTPUT);

  //Set button pin to input
  pinMode(USER_BUTTON, INPUT);
}

