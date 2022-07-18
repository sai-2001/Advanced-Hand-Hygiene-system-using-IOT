#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
#include <Adafruit_MLX90614.h>
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const char* ssid = "No name's Galaxy M12";
const char* password = "najt7318";
WiFiClient client;
unsigned long myChannelNumber = 1765720;
const char * myWriteAPIKey = "JVLA037GD9BTGZPU";
const char* server = "api.thingspeak.com";

unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

float temperatureC;

int trigPin1 = 12 ;
int echoPin1 = 14;
int relay = 13;
int buzzer = 2;
long duration1, dist1, average1;
long aver1[3];

void setup() {
  Serial.begin(115200);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(relay, OUTPUT);
  pinMode(buzzer, OUTPUT);
  lcd.init();
  mlx.begin(); 
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
  }
void measure() {
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(15);
  digitalWrite(trigPin1, LOW);
  pinMode(echoPin1, INPUT);
  duration1 = pulseIn(echoPin1, HIGH);
  dist1 = (duration1 / 2) / 29.1;
  }
void loop() {
  /*if(WiFi.status() != WL_CONNECTED){
  Serial.print("Attempting to connect");
  while(WiFi.status() != WL_CONNECTED){
    WiFi.begin(ssid, password); 
    delay(5000);     
  } 
  Serial.println("\nConnected.");
 }*/
  for (int i = 0; i <= 2; i++) { //average distance
    measure();
    aver1[i] = dist1;
    delay(10);              //delay between measurements
  }
  dist1 = (aver1[0] + aver1[1] + aver1[2]) / 3;
  Serial.print("dist1 :");
  Serial.println(dist1);
  if ( dist1 < 15 ) {
    temperatureC = mlx.readObjectTempC();
    Serial.print("Temp = "); Serial.print(mlx.readObjectTempC());
    lcd.backlight();
    lcd.setCursor(0,1);
    lcd.print("Temp = ");
    lcd.println(mlx.readObjectTempC());
    int i=0;
    if (mlx.readObjectTempC() > 34){
      tone(buzzer, 200);
      delay(1000);
      noTone(buzzer);
      lcd.setCursor(1,0);
      lcd.print("Suspect = ");
      ++i;
      lcd.print(i);  
    }
    ThingSpeak.writeField(myChannelNumber,1,temperatureC, "myWriteAPIKey");
    digitalWrite (relay, HIGH);
    Serial.print("relay on");
    delay(700);
    digitalWrite (relay, LOW);
    delay(700);
    }
  else{  
    digitalWrite (relay, LOW);
  } 
 }   
