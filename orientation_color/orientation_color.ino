#include <M5Stack.h>
#include "utility/MPU9250.h"
#include "Math.h"

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

int accToColor(float ax, float ay, float az) {
  int r, g, b;
  uint16_t color;

  //Map acc from -1..1 to 0..1
  ax = (ax + 1)/2;
  ay = (ay + 1)/2;
  az = (az + 1)/2;

  //Map acc from 0..1 to 0..255 and make sure it stays inside those bounds
  r = constrain((int)(255 * ax), 0, 255);
  g = constrain((int)(255 * ay), 0, 255);
  b = constrain((int)(255 * az), 0, 255);

  //convert from r, g, b to 16bit color
  r = r/8;
  g = g/4;
  b = b/8;
  color = (r << 11) + (g << 5) + b;
  return color;
}

void loop() {
  updateIMU();
  uint16_t color = accToColor(IMU.ax, IMU.ay, IMU.az);

  IMU.delt_t = millis() - IMU.count;

  if (IMU.delt_t > 500) {

    M5.Lcd.setCursor(0, 0, 2);
    M5.Lcd.printf("RGB 0x%04X  ", color);

    M5.Lcd.fillRect(75, 75, 100, 100, color);
  }
}
