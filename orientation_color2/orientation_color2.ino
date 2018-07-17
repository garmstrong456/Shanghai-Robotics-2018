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

int accToColor(float az) {
  int hue;
  int s = 255, v = 255;  //hue varies with az, saturation and value are always max
  int r, g, b;
  uint16_t color;

  //map az from -1..1 to 0..1
  az = (az + 1) / 2;

  //map az from 0..1 to 0..255 and constrain to those bounds
  hue = constrain((int)(255 * az), 0, 255);

  //Convert hue, s, v to r, g, b
  unsigned char region, remainder, p, q, t;

  region = hue / 43;
  remainder = (hue - (region * 43)) * 6;

  p = (v * (255 - s)) >> 8;
  q = (v * (255 - ((s * remainder) >> 8))) >> 8;
  t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

  switch (region)
  {
    case 0:
      r = v; g = t; b = p;
      break;
    case 1:
      r = q; g = v; b = p;
      break;
    case 2:
      r = p; g = v; b = t;
      break;
    case 3:
      r = p; g = q; b = v;
      break;
    case 4:
      r = t; g = p; b = v;
      break;
    default:
      r = v; g = p; b = q;
      break;
  }

  //convert r, g, b to 16bit color
  r = r / 8; //5bits for red
  g = g / 4; //6bits for green
  b = b / 8; //5bits for blue

  M5.Lcd.setCursor(0, 30, 2);
  M5.Lcd.printf("r: %2d  g: %2d b: %2d", r, g, b);

  color = (r << 11) + (g << 5) + b;
  return color;
}

float az_avg = 0;

void loop() {
  updateIMU();
  az_avg = 0.05 * IMU.az + 0.95 * az_avg;

  uint16_t color = accToColor(az_avg);

  IMU.delt_t = millis() - IMU.count;

  if (IMU.delt_t > 500) {

    M5.Lcd.setCursor(0, 0, 2);
    M5.Lcd.printf("RGB  0x%04X  ", color);
    M5.Lcd.setCursor(0, 15, 2);
    M5.Lcd.printf("az  %1.3f  ", az_avg);

    M5.Lcd.fillRect(75, 75, 100, 100, color);
  }
}
