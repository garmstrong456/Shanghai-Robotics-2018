#include <M5Stack.h>
#include "utility/MPU9250.h"

MPU9250 IMU;

void setup() {
  M5.begin();
  Wire.begin();

  IMU.MPU9250SelfTest(IMU.SelfTest);
  IMU.calibrateMPU9250(IMU.gyroBias, IMU.accelBias);
  IMU.initMPU9250();
}

void updateIMU() {
  if (IMU.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01) {
    IMU.readAccelData(IMU.accelCount);
    IMU.getAres();
    IMU.ax = (float)IMU.accelCount[0] * IMU.aRes;
    IMU.ay = (float)IMU.accelCount[1] * IMU.aRes;
    IMU.az = (float)IMU.accelCount[2] * IMU.aRes;
  }
}

uint16_t color = BLACK;
uint16_t lastColor = BLACK;

void loop() {
  updateIMU();
  double aTotal = sqrt(sq(IMU.ax) + sq(IMU.ay) + sq(IMU.az));

  IMU.delt_t = millis() - IMU.count;

  if (IMU.delt_t > 100) {

    M5.Lcd.setCursor(0, 0, 2);
    M5.Lcd.printf("  %4f    ", aTotal);

    if (aTotal < 0.25) {
      color = BLUE;
    } else {
      color = BLACK;
    }
  }

  if (color != lastColor) {
    M5.Lcd.fillScreen(color);
    lastColor = color;
  }
}
