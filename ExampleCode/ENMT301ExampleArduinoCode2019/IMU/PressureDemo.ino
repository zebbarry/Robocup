#include <DFRobot_BMP280.h>

DFRobot_BMP280 bmp;

void setup() {
  Serial.begin(115200);
  while (!bmp.begin()) {  
    Serial.println(F("Error initializing BMP280!"));
    delay(1000);
  }
}

void loop() {
  Serial.print("Temperature:");
  Serial.println(bmp.readTemperatureValue());
  Serial.print("Pressure:");
  Serial.println(bmp.readPressureValue());
  Serial.print("Altitude:");
  Serial.println(bmp.readAltitudeValue());
  /* use a sampling time of at least 38ms */
  delay(50);
}
