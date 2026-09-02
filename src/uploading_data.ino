#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

HardwareSerial sim800l(2);
const char* ssid = "WIFI_SSID_HERE";
const char* password = "WIFI_PASSWORD_HERE"; 

String message; float distance;
void setup() {
  Serial.begin(115200);  // USB Serial for debugging
  Serial1.begin(9600, SERIAL_8N1, 16, 17);  // Serial1 on GPIO16 (RX), GPIO17 (TX)

   WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.println("Connected to Wi-Fi");
}


void loop() {
  // put your main code here, to run repeatedly:
  if (WiFi.status() == WL_CONNECTED) {
    uploadData();
  }

}

void uploadData(){
  if (Serial1.available() > 0) {
      Serial.println(Serial1.readString());  // Read the incoming message
       message = Serial1.readStringUntil('\n');
      String val[3];
      int index = 0;    
      // Iterate over the data and split by commas
      int prevIndex = 0;
      for (int i = 0; i < message.length(); i++) {
        if (message.charAt(i) == ',') {
          val[index++] = message.substring(prevIndex, i);
          prevIndex = i + 1;
        }
      }
      val[index] = message.substring(prevIndex);  // Last value after the last comma
      // Convert to float and print values
      String data = val[0];
     distance = val[1].toFloat();
      float sensor_val = val[2].toFloat();
      Serial.print("To post: ");
      Serial.println(data);
      Serial.print("Sensor 2: ");
      Serial.println(distance);
      Serial.print("Sensor 3: ");
      Serial.println(sensor_val);

      if((data != "no data available") && (sensor_val == 1)){
           HTTPClient http;
            String url = "https://Domain_here/API_here"; // RESTAPI for uploading data
            http.begin(url); // URL
            http.addHeader("Content-Type", "application/x-www-form-urlencoded"); 
            Serial.println("Sending data: " + data); 
            int httpResponseCode = http.POST(data);
            if (httpResponseCode > 0) {
              String response = http.getString(); // Get response from the server
              // String msg = http.getString();
              Serial.println("Server Response: " + response);
            } else {
              Serial.print("Error in HTTP request: ");
              Serial.println(httpResponseCode);
            }
            // Close the HTTP connection
            http.end();
      }
     
      else{
        Serial.print("invalid!"); 
      }delay(2500);
    }
}


