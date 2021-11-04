
//FirebaseESP8266.h must be included before ESP8266WiFi.h
#include "FirebaseESP8266.h"  // Install Firebase ESP8266 library
#include <ESP8266WiFi.h>
#include <DHT.h>    // Install DHT11 Library and Adafruit Unified Sensor Library
#include <UniversalTelegramBot.h>

#define BOTtoken "1985276546:AAHO4y_IrsKZg31q0K1ziFKIuQBhxi2AGxw" // your Bot Token (Get from Botfather)
#define FIREBASE_HOST "homeautomation-7c6bf-default-rtdb.firebaseio.com"                          // the project name address from firebase id
#define FIREBASE_AUTH "5nJ61Vj1joGhvf8XQqkBpdHZ2JpsPGYQlEPrmN1Y"            // the secret key generated from firebase
#define WIFI_SSID "press"
#define WIFI_PASSWORD "Passw0rd"

//Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

#define DHTPIN D1   // Connect Data pin of DHT to D1
#define FAN_PIN D2   // FAN RELAY
int AC=D0;
int lights =D3;
int led2=D4;//dht record led Connect LED to D5
int rgb =D5;
int led1=D6; //greenBulb
int led =D7;//whiteBulb
int buzzer=D8;



int D2Status = 0;
int D3Status = 0;
int D4Status = 0;




#define DHTTYPE    DHT11
DHT dht(DHTPIN, DHTTYPE);

//Define FirebaseESP8266 data object
FirebaseData firebaseData;
FirebaseData ledData1;
FirebaseData ledData2;


FirebaseJson json;
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";

   
    if (text == "/White_bulb_ON") {
      digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
       D2Status = 1;
       bot.sendMessage(chat_id, "Hello the white bulb is ON", "");
    }

    if (text == "/White_bulb_OFF") {
       D2Status = 0;
      digitalWrite(led, LOW);    // turn the LED off (LOW is the voltage level)
      bot.sendMessage(chat_id, "OK the white bulb turned OFF", "");
    }
     if (text == "/RGB_ON") {
      digitalWrite(rgb,HIGH);
      D3Status = 1;
      bot.sendMessage(chat_id, "rgb is HIGH", "");
    }

    if (text == "/RGB_OFF") {
      D3Status = 0;
      digitalWrite(rgb, LOW);
      bot.sendMessage(chat_id, "rgb is LOW", "");
    }

    if (text == "/Exrta_LIGHTs_ON") {
      digitalWrite(lights,HIGH);
      D4Status = 1;
      bot.sendMessage(chat_id, "extra light is HIGH", "");
    }

    if (text == "/Extra_Lights_OFF") {
      D4Status = 0;
      digitalWrite(lights, LOW);
      bot.sendMessage(chat_id, "extra light is LOW", "");
    }

   
   

    if (text == "/start") {
      String welcome = "Welcome To Smart Home Automation Bot.Lets control your appliances from here, " + from_name + ".\n";
      welcome += "Lets tap on this commands to change the statuses of your home.\n\n";
      welcome += "/White_bulb_ON : to Turn ON The White Bulb\n";
      welcome += "/White_bulb_OFF : to Turn OFF The White Bulb\n";
      welcome += "/RGB_ON : to Turn ON  RGB \n";
      welcome += "/RGB_OFF : to Turn OFF RGB \n";
      welcome += "/Exrta_LIGHTs_ON : to Turn ON The Extra Lighting system \n";
      welcome += "/Exrta_LIGHTs_OFF : to Turn OFF The Extra Lighting system \n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }
  }
}



void setup()
{

  Serial.begin(9600);//tells Esp8266 to get ready to exchange messages with the Serial Monitor at a data rate of 9600 bits per second

  dht.begin();
  pinMode(FAN_PIN, OUTPUT);
  digitalWrite(FAN_PIN, LOW);
  pinMode(led,OUTPUT);
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(AC,OUTPUT);


  client.setInsecure();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  digitalWrite(buzzer, HIGH);
     delay(1000);
  digitalWrite(buzzer, LOW);
     delay(1000);
 
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
     if (temp < 24)
  {
    digitalWrite(AC, HIGH);
    Serial.println("AC turned ON");
}
  else {
    digitalWrite(AC, LOW);
    Serial.println("AC turned OFF");
    
  } 
  if (Firebase.getString(ledData1, "/BEDROOM/BEDROOM")){
    Serial.println(ledData1.stringData());
    if (ledData1.stringData() == "1") {
    digitalWrite(led1, LOW);
    }
  else if (ledData1.stringData() == "0"){
    digitalWrite(led1, HIGH);
    }
    }
  if (Firebase.getString(ledData2, "/DINNINGROOM/DINNINGROOM")){
    Serial.println(ledData2.stringData());
    if (ledData2.stringData() == "1") {
    digitalWrite(led, LOW);
    }
  else if (ledData2.stringData() == "0"){
    digitalWrite(led, HIGH);
    }
     }
    delay(1000);
 
 if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    lastTimeBotRan = millis();
  }
   
}
//smartHomeAutomationwithMachineLearning 
