#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include <HardwareSerial.h>
HardwareSerial SerialPort(2);

String wifiSSID = "Python";
String wifiPassword = "1234arthur";

void connectWifi();
void sendToServer(String data);

void setup() {
  // Setup code here
  connectWifi();
  SerialPort.begin(115200, SERIAL_8N1, 16, 17); // Start Serial Port 2
  Serial.begin(9600); // Start the Serial communication
}

void loop() {
  static String recvBuffer = ""; // Buffer to hold the data
  while (SerialPort.available()) {
    char inChar = SerialPort.read();
    recvBuffer += inChar; // Append received char to buffer

    // If newline character detected, send data
    if (inChar == '\n' || inChar == '\r') {
      recvBuffer.trim(); // Trim whitespace
      if (!recvBuffer.isEmpty()) {
        sendToServer(recvBuffer); // Send data
        recvBuffer = ""; // Clear the buffer after sending
      }
    }
  }
}

void connectWifi() {
  Serial.println("Connecting To Wifi");
  WiFi.begin(wifiSSID.c_str(), wifiPassword.c_str());
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Wifi Connected");
}

void sendToServer(String data) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("http://192.168.43.183:5000/receive_data"); // Specify the URL
    http.addHeader("Content-Type", "application/json"); // Specify content-type header

    StaticJsonDocument<200> doc;
    doc["char_buffer"] = data;

    String requestBody;
    serializeJson(doc, requestBody);

    int httpResponseCode = http.POST(requestBody); // Send the actual POST request

    if (httpResponseCode > 0) {
      String response = http.getString(); // Get the response to the request
      Serial.println(httpResponseCode); // Print return code
      Serial.println(response); // Print request answer
    }
    else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    http.end(); // Free resources
  }
  else {
    Serial.println("WiFi Not Connected");
  }
}
