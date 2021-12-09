
#include <DHT.h>
#include <Wire.h>
#include <BH1750.h>
#include <SoftwareSerial.h>
#include <MHZ.h>
#include <WiFi.h>
#include "ThingSpeak.h"
//https://thingspeak.com/channels/1602959
const char* ssid = "MYWIFI";   // your network SSID (name)
const char* password = "MYPASS";   // your network password  

WiFiClient  client;

unsigned long myChannelNumber = 3;
const char * myWriteAPIKey = "MYAPI";

#define DHTPIN 33
#define DHTTYPE    DHT11
#define CO2_IN 25
#define MH_Z19_RX 1  // D7
#define MH_Z19_TX 3  // D6

int sensorValue1, sensorValue2, sensorValue3;
DHT dht(DHTPIN, DHTTYPE);
BH1750 lightMeter;
MHZ co2(MH_Z19_RX, MH_Z19_TX, CO2_IN, MHZ19B);

void setup() {
  dht.begin();
  Serial.begin(9600);
  Wire.begin();
  lightMeter.begin();
  pinMode(CO2_IN, INPUT);

  WiFi.mode(WIFI_STA); 
   
  ThingSpeak.begin(client);
}


void loop() {

  // Connect or reconnect to WiFi
    if(WiFi.status() != WL_CONNECTED){
      Serial.print("Attempting to connect");
      while(WiFi.status() != WL_CONNECTED){
        WiFi.begin(ssid, password); 
        delay(5000);     
      } 
      Serial.println("\nConnected.");
    }

  //CO Alkohol Föld Humi Hőm Lux Co2

  sensorValue1 = analogRead(A6);
  Serial.print(sensorValue1); Serial.print("\t");

  sensorValue2 = analogRead(A7);
  Serial.print(sensorValue2); Serial.print("\t");

  sensorValue3 = analogRead(A4);
  Serial.print(sensorValue3); Serial.print("\t");

  float h = dht.readHumidity();
  Serial.print(h); Serial.print("\t");
  
  float t = dht.readTemperature();
  Serial.print(t); Serial.print("\t");

  float lux = lightMeter.readLightLevel();
  Serial.print(lux); Serial.print("\t");

  int ppm_pwm = co2.readCO2PWM();
  Serial.print(ppm_pwm); Serial.print("\t");

  ThingSpeak.setField(1, sensorValue1);
  ThingSpeak.setField(2, sensorValue2);
  ThingSpeak.setField(3, sensorValue3);
  ThingSpeak.setField(4, h);
  ThingSpeak.setField(5, t);
  ThingSpeak.setField(6, lux);
  ThingSpeak.setField(7, ppm_pwm);
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
      Serial.println("Channel update successful.");
    }
    else{
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
  Serial.print("\n");
  delay(15000);        
}
