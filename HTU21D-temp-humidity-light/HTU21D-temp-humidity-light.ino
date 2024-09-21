/***************************************************
  This is an example for the HTU21D-F Humidity & Temp Sensor

  Designed specifically to work with the HTU21D-F sensor from Adafruit
  ----> https://www.adafruit.com/products/1899

  These displays use I2C to communicate, 2 pins are required to
  interface
 ****************************************************/

#include <Wire.h>
#include "Adafruit_HTU21DF.h"

#define ledPin 15

long oldtime1;
long oldtime2;

#include <WiFi.h>
#include "ThingSpeak.h"

const char* ssid = "Clovis 2.4G";   // your network SSID (name) 
const char* password = "99143304";   // your network password

WiFiClient  client;

unsigned long myChannelNumber = 2515409;
const char * myWriteAPIKey = "Z3YRH78K0AFE6HLU";

// Connect Vin to 3-5VDC
// Connect GND to ground
// Connect SCL to I2C clock pin (A5 on UNO)
// Connect SDA to I2C data pin (A4 on UNO)

Adafruit_HTU21DF htu = Adafruit_HTU21DF();

void setup() {
  Serial.begin(9600);
  Serial.println("HTU21D-F test");
  pinMode(ledPin, OUTPUT);

  if (!htu.begin()) {
    Serial.println("Couldn't find sensor!");
    while (1);
  }

  WiFi.mode(WIFI_STA);   
  
  ThingSpeak.begin(client);  // Initialize ThingSpeak

}

void loop() {

    if(micros() - oldtime1 > 59990000){

      if(WiFi.status() != WL_CONNECTED){
        Serial.print("Attempting to connect");
        while(WiFi.status() != WL_CONNECTED){
          WiFi.begin(ssid, password); 
          delay(5000);     
        } 
        Serial.println("\nConnected.");
      }

      oldtime1= micros();
      float temp = htu.readTemperature();
      float rel_hum = htu.readHumidity();
      int analogVolts = analogReadMilliVolts(A0);
      Serial.print("Temp: "); Serial.print(temp); Serial.print(" C");
      Serial.print("\t\t");
      Serial.print("Humidity: "); Serial.print(rel_hum); Serial.println(" \%");
      Serial.print("Light[0-3300]: ");
      Serial.println(analogVolts);
      ThingSpeak.setField(1, temp);
      ThingSpeak.setField(2, rel_hum);
      ThingSpeak.setField(3, analogVolts);
      int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
      
    }
    if(micros() - oldtime2 > 190000){
      oldtime2= micros();
      digitalWrite(ledPin, !digitalRead(ledPin));
    }
    
    
}
