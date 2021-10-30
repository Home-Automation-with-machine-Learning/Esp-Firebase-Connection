
//FirebaseESP8266.h must be included before ESP8266WiFi.h
#include "FirebaseESP8266.h"  // Install Firebase ESP8266 library
#include <ESP8266WiFi.h>
#include <DHT.h>    // Install DHT11 Library and Adafruit Unified Sensor Library


#define FIREBASE_HOST "homeautomation-7c6bf-default-rtdb.firebaseio.com"                          // the project name address from firebase id
#define FIREBASE_AUTH "5nJ61Vj1joGhvf8XQqkBpdHZ2JpsPGYQlEPrmN1Y"            // the secret key generated from firebase
#define WIFI_SSID "press"
#define WIFI_PASSWORD "Passw0rd"

#define DHTPIN D3   // Connect Data pin of DHT to D1
#define FAN_PIN D1   // FAN RELAY
int led =D2;
int led1=D5;
int led2=D4;// Connect LED to D5

#define DHTTYPE    DHT11
DHT dht(DHTPIN, DHTTYPE);

//Define FirebaseESP8266 data object
FirebaseData firebaseData;
FirebaseData ledData;

FirebaseJson json;


void setup()
{

  Serial.begin(9600);//tells Esp8266 to get ready to exchange messages with the Serial Monitor at a data rate of 9600 bits per second

  dht.begin();
  pinMode(FAN_PIN, OUTPUT);
  digitalWrite(FAN_PIN, LOW);
  pinMode(led,OUTPUT);
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  
 
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

}

void sensorUpdate(){
  
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed 
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    
   
    return;
  }

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("C  ,"));
  Serial.print(f);
  Serial.println(F("F  "));
  digitalWrite(led2, HIGH);
     delay(1000);
  digitalWrite(led2, LOW);
     delay(1000);

  if (Firebase.pushFloat(firebaseData, "/Humidity/TEMP", t))
  {
     //Firebase.pushFloat(firebaseData,"/Temperature", t); //setup path and send readings
    Serial.println("PASSED");
   
    
  }
  else
  {
    Serial.println("FAILED");
    
  }

  if (Firebase.pushFloat(firebaseData, "/Humidity/HUM", h))
  {
     //Firebase.pushFloat("/Humidity", h);          //setup path and send readings
    Serial.println("PASSED");
  }
  else
  {
    Serial.println("FAILED");
    
  }
 
 
 
}
void loop() {
  sensorUpdate();
  float temp = dht.readTemperature();
 
   if (temp > 25)
  {
    digitalWrite(FAN_PIN, HIGH);
    Serial.println("FAN turned ON");
}
  else {
    digitalWrite(FAN_PIN, LOW);
    Serial.println("FAN turned OFF");
    
  } 
  
  if (Firebase.getString(ledData, "/LED_STATUS")){
    Serial.println(ledData.stringData());
    if (ledData.stringData() == "1") {
    digitalWrite(led, HIGH);
    }
  else if (ledData.stringData() == "0"){
    digitalWrite(led, LOW);
    }
  }
  if (Firebase.getString(ledData, "/LED_STATUS1")){
    Serial.println(ledData.stringData());
    if (ledData.stringData() == "1") {
    digitalWrite(led1, HIGH);
    }
  else if (ledData.stringData() == "0"){
    digitalWrite(led1, LOW);
    }
  }
  delay(1000);
}
//smartHomeAutomationwithMachineLearning 
