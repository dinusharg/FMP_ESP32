/***************************************************
  This is an example for the Adafruit Thermocouple Sensor w/MAX31855K

  Designed specifically to work with the Adafruit Thermocouple Sensor
  ----> https://www.adafruit.com/products/269

  These displays use SPI to communicate, 3 pins are required to
  interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <SPI.h>
#include "Adafruit_MAX31855.h"

 #include <SPIFFS.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
// Default connection is using software SPI, but comment and uncomment one of
// the two examples below to switch between software SPI and hardware SPI:

// Example creating a thermocouple instance with software SPI on any three
// digital IO pins.
#define MAXDO   23
#define MAXCS   22
#define MAXCLK  21

const char* ssid     = "ESP32-Access-Point";
const char* password = "123456789";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);


// initialize the Thermocouple
Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);

// Example creating a thermocouple instance with hardware SPI
// on a given CS pin.
//#define MAXCS   10
//Adafruit_MAX31855 thermocouple(MAXCS);

// Example creating a thermocouple instance with hardware SPI
// on SPI1 using specified CS pin.
//#define MAXCS   10
//Adafruit_MAX31855 thermocouple(MAXCS, SPI1);

String ADC_val="";
String Temp="";

void readTemp(){
   // basic readout test, just print the current temp
//   Serial.print("Internal Temp = ");
//   Serial.println(thermocouple.readInternal());

   double c = thermocouple.readCelsius();
   if (isnan(c)) {
//     Serial.println("Thermocouple fault(s) detected!");
//     uint8_t e = thermocouple.readError();
//     if (e & MAX31855_FAULT_OPEN) Serial.println("FAULT: Thermocouple is open - no connections.");
//     if (e & MAX31855_FAULT_SHORT_GND) Serial.println("FAULT: Thermocouple is short-circuited to GND.");
//     if (e & MAX31855_FAULT_SHORT_VCC) Serial.println("FAULT: Thermocouple is short-circuited to VCC.");
   Temp=String(-1000);
   } else {
    Temp=String(c);
//     Serial.print("C = ");
//     Serial.println(c);
   }
   //Serial.print("F = ");
   //Serial.println(thermocouple.readFahrenheit());
}

void readADC(){
  int analogValue = analogRead(36);
  int analogVolts = analogReadMilliVolts(36);

    // print out the values you read:
//  Serial.printf("ADC analog value = %d\n",analogValue);
//  Serial.printf("ADC millivolts value = %d\n",analogVolts);
//  Serial.printf("%d ,%d\n",analogValue,analogVolts);
//  delay(100);  // delay in between reads for clear read from serial

    ADC_val=String(analogVolts);
//  return String(analogVolts);
  
}

String getADCVal(){
//  Serial.println("ADC val");
  return ADC_val;
}

String getTemp(){
  return Temp;
}

void printVal(){
   Serial.print("#,"+ADC_val+",");
   Serial.print(Temp+"\n");
}
void setup() {
  Serial.begin(9600);
  analogReadResolution(12);

  while (!Serial) delay(1); // wait for Serial on Leonardo/Zero, etc

  Serial.println("MAX31855 test");
  // wait for MAX chip to stabilize
  delay(500);
  Serial.print("Initializing sensor...");
  if (!thermocouple.begin()) {
    Serial.println("ERROR.");
    while (1) delay(10);
  }

  // OPTIONAL: Can configure fault checks as desired (default is ALL)
  // Multiple checks can be logically OR'd together.
  // thermocouple.setFaultChecks(MAX31855_FAULT_OPEN | MAX31855_FAULT_SHORT_VCC);  // short to GND fault is ignored

  Serial.println("DONE.");

   // Initialize SPIFFS
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  WiFi.softAP(ssid, password);
  delay(500);

   IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

    // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
  });

  server.serveStatic("/", SPIFFS, "/");

    server.on("/padwear", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getADCVal().c_str());
  });

    server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getTemp().c_str());
  });

   // Start server
  server.begin();
}

void loop() {
 
readTemp();
  readADC();
  printVal();
   delay(1000);
}
