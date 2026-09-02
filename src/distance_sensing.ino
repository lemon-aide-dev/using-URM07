#include "DFRobot_URM07.h"
#include <DHT.h>
#define DHT_SENSOR_PIN 10
#define DHT_SENSOR_TYPE DHT11
DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);
int floatsensor = 5;
int buttonState ;
#if (!defined ARDUINO_ESP32_DEV) && (!defined __SAMD21G18A__)
/**
 * UNO:pin_3-----RX
 *     pin_4-----TX
 */
SoftwareSerial softserial(3,4);
DFRobot_URM07_SoftwareSerial URM07(&softserial,19200,0x11);
// #else
//   DFRobot_URM07_HardwareSerial URM07(&Serial2,19200,0x11); //ESP32HardwareSerial
#endif
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(floatsensor, INPUT_PULLUP);
  dht_sensor.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  buttonState = digitalRead(floatsensor);
  float humi  = dht_sensor.readHumidity();
  float temp = dht_sensor.readTemperature();
  uint16_t Urm1 = URM07.gainDistance();


  float compute = 900 - Urm1;
  if (buttonState == HIGH) {
      
      Serial.print("distance="+ String(compute) + "&temperature=" + String(temp) + "&humidity=" + String(humi) + "&button=0");
      Serial.print(",");
      Serial.print(compute);
      Serial.print(",");
      Serial.println("0");
  }
  else {
    Serial.print("distance="+ String(compute) + "&temperature=" + String(temp) + "&humidity=" + String(humi) + "&button=1");
    Serial.print(",");
    Serial.print(compute);
    Serial.print(",");
    Serial.println("1");
  }
  delay(3000);
}
