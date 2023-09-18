/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

// Import required libraries

  #include <WiFi.h>
  #include <ESPAsyncWebServer.h>
  #include <SPIFFS.h>



// Replace with your network credentials
//const char* ssid = "NBNWifi";
//const char* password = "madusha1995";
const char* ssid     = "ESP32-Access-Point";
const char* password = "123456789";

IPAddress IP(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);


// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Timer variables
unsigned long previousMillis = 0;
const long interval = 10000;  // interval to wait for Wi-Fi connection (milliseconds)



String readADC(){
  int analogValue = analogRead(36);
  int analogVolts = analogReadMilliVolts(36);

    // print out the values you read:
//  Serial.printf("ADC analog value = %d\n",analogValue);
//  Serial.printf("ADC millivolts value = %d\n",analogVolts);
  Serial.printf("%d ,%d\n",analogValue,analogVolts);
//  delay(100);  // delay in between reads for clear read from serial
  return String(analogVolts);
  
}
void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  analogReadResolution(12);
  
  // Initialize SPIFFS
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }


  WiFi.softAP(ssid, password);
  delay(500);
//  WiFi.softAPConfig(IP, gateway, subnet);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
  });

  server.serveStatic("/", SPIFFS, "/");
  
  server.on("/padwear", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readADC().c_str());
  });
//  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
//    request->send_P(200, "text/plain", readBME280Humidity().c_str());
//  });
//  server.on("/pressure", HTTP_GET, [](AsyncWebServerRequest *request){
//    request->send_P(200, "text/plain", readBME280Pressure().c_str());
//  });

  // Start server
  server.begin();

}
 
void loop(){

//  readADC();

}
