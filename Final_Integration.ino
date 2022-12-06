#include <LiquidCrystal_I2C.h>
#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
LiquidCrystal_I2C lcd(0x27,20,4);
//----------- Enter you Wi-Fi Details---------//
char ssid[] = "No name's Galaxy M12"; //SSID
char pass[] = "najt7317"; // Password
//-------------------------------------------//

const int trigger = D6;
const int echo = D5;
const int trig = D3;
const int ech = D4;
int relay = 13;
long T;
long T1;
float distanceCM;
float distance;
int percentage;
float AmbTemp;
float ObjTemp;
WiFiClient client;

unsigned long myChannelField = 1765720; // Channel ID
const int ChannelField = 1;
const int ChannelField2 = 2; // Which channel to write data
const char * myWriteAPIKey = "THK6SZHZ9I739M0X"; // Your write API Key

void setup()
{
  Serial.begin(230400);
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(trig, OUTPUT);
  pinMode(ech, INPUT);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
  pinMode(relay, OUTPUT);
  lcd.init();
  mlx.begin();
}
void loop()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while (WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }
  digitalWrite(trigger, LOW);
  delay(1);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  T = pulseIn(echo, HIGH);
  distanceCM = T * 0.034;
  distanceCM = distanceCM / 2;
  Serial.print("Distance in cm: ");
  Serial.println(distanceCM);
  if ( distanceCM < 15 ){
    AmbTemp = mlx.readAmbientTempC();
    ObjTemp = mlx.readObjectTempC();
    Serial.print("*C\tAmbient = "); Serial.print(mlx.readAmbientTempC()); Serial.println("*C");
    Serial.print("*C\tObject = "); Serial.print(mlx.readObjectTempC()); Serial.println("*C");
    ThingSpeak.writeField(myChannelField, ChannelField, ObjTemp, myWriteAPIKey);
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print(" Object Temp :");
    lcd.print(mlx.readObjectTempC());
    lcd.println("*F");
    digitalWrite (relay, HIGH);
    Serial.print("relay on");
    delay(200);
    digitalWrite (relay, LOW);
    delay(3000);
    digitalWrite(trig, LOW);
    delay(1);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
    T1 = pulseIn(ech, HIGH);
    distance = T1 * 0.034;
    distance = distance / 2;
    percentage = map(distance,8, 2, 0, 100);
    if(percentage<0)
    {
      percentage = 0;
    }
    else if(percentage > 100)
    {
      percentage = 100;
    }
    Serial.print("percentage: ");
    Serial.print(percentage);
    ThingSpeak.writeField(myChannelField, ChannelField2, percentage, myWriteAPIKey);
    delay(5000); 
    Serial.print("% Distance: ");
    Serial.print(distance);
    Serial.println("cm");
    }
    else{  
      digitalWrite (relay, LOW);
    } 
}
