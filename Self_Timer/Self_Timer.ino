#include <SPI.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>

#define OLED_RESET 8
#define LED_PIN 4
#define SCROLL_PIN 8
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

int runTimer = 1; 
int runFor = 60; // time in seconds
int buzzerPin = 4;
int relayPin=4;
int data = 0;
 
void setup() {
  
   display.begin(SSD1306_SWITCHCAPVCC, 0x3c);  // initialize with the I2C addr 0x3D (for the 128x64)
  
   pinMode(buzzerPin, OUTPUT);
   pinMode(relayPin,OUTPUT);

}
 
void loop() {
  
   if(runTimer == 1){ 
      display.clearDisplay();
      display.setCursor(0,32);
      display.setTextColor(WHITE);
      display.setTextWrap(false);
      display.setTextSize(1);
      display.print("TIMER=");
      display.display();
      timer(); 
      }
    
   runTimer = 0;
   display.clearDisplay();
   delay(250);
   
   for(int duration = 0; duration < 100; duration ++){
      digitalWrite(buzzerPin, HIGH);
      delayMicroseconds(500);
      digitalWrite(buzzerPin, LOW);
      delayMicroseconds(500);
   } 
}

void timer() {
   for(int timer = runFor; timer > 0; --timer){
   
   display.clearDisplay();
   display.setCursor(20,32);
   display.setTextColor(WHITE);
   display.setTextWrap(false);
   display.setTextSize(1);
   display.print(timer);
   display.display();
   delay(1000);
   
   display.setCursor(30,32);
   display.setTextColor(WHITE);
   display.setTextSize(1);
   display.setTextWrap(false);
   //display.print(timer);
   display.print(" SECONDs LEFT!");
   display.display();
   delay(1000);
   }
   
   display.clearDisplay();
   display.setCursor(0,0);
   display.setTextColor(WHITE);
   display.setTextSize(2);
   display.setTextWrap(false);
   display.print("TIMER ALERT!");
   display.display(); 
}
