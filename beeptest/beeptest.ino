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

int j = 0;
int runFor = 10; // time in seconds
//int laps = 5;

void setup(){

  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.clearDisplay();
  
  pinMode(SCROLL_PIN, INPUT); //Set pin for time/date set button to input
  digitalWrite(SCROLL_PIN, HIGH); //Turn on pullup resistors
  
  pinMode(LED_PIN, OUTPUT);
  
}

void loop(){
  
    BeepTest();
    ScrollMenu();
    timer();

}


void ScrollMenu(){
  
      if ( digitalRead(SCROLL_PIN)) {
        do {} while (digitalRead(SCROLL_PIN));
        StartTest();
        digitalWrite(LED_PIN, HIGH);
        delay(1000);
        digitalWrite(LED_PIN, LOW);
        
      }
}

void BeepTest(){
  
    display.clearDisplay();
    display.setCursor(35,30);
    display.setTextColor(WHITE);
    //display.setTextColor(uint16_t color, uint16_t backgroundcolor);
    display.setTextSize(1);
    display.setTextWrap(false);
    display.print("Start Test");
    display.display();
    
    display.setCursor(0,50);
    display.setTextColor(WHITE);
    //display.setTextColor(uint16_t color, uint16_t backgroundcolor);
    display.setTextSize(1);
    display.setTextWrap(false);
    display.print("Back");
    display.display();

}

void StartTest(){
  
    display.clearDisplay();
    display.setCursor(50,10);
    display.setTextColor(WHITE);
    //display.setTextColor(uint16_t color, uint16_t backgroundcolor);
    display.setTextSize(5);
    display.setTextWrap(false);
    display.print("3");
    display.display();
    delay(1000);
   
    display.clearDisplay();
    display.setCursor(50,10);
    display.setTextColor(WHITE);
    //display.setTextColor(uint16_t color, uint16_t backgroundcolor);
    display.setTextSize(5);
    display.setTextWrap(false);
    display.print("2");
    display.display(); 
    delay(1000);
    
    display.clearDisplay();
    display.setCursor(50,10);
    display.setTextColor(WHITE);
    //display.setTextColor(uint16_t color, uint16_t backgroundcolor);
    display.setTextSize(5);
    display.setTextWrap(false);
    display.print("1");
    display.display(); 
    delay(1000);

}

void timer() {     
  
    for (runFor=runFor; runFor > 4; runFor=runFor-2 ){
       
        for(int timer = runFor; timer > 0; --timer){
   
               display.clearDisplay();
               display.setCursor(20,32);
               display.setTextColor(WHITE);
               display.setTextWrap(false);
               display.setTextSize(1);
               display.print(timer);
               display.display();
               delay(1000);
               
               /*display.setCursor(30,32);
               display.setTextColor(WHITE);
               display.setTextSize(1);
               display.setTextWrap(false);
               //display.print(timer);
               display.print(" SECONDs LEFT!");
               display.display();
               delay(1000);*/
               
              }
         digitalWrite(LED_PIN, HIGH);
         delay(1000);
         digitalWrite(LED_PIN, LOW);
        /* display.setCursor(20,18);
         display.setTextColor(WHITE);
         display.setTextWrap(false);
         display.setTextSize(1);
         display.print(x);
         display.display();*/
         //delay(1000);
         
         
               display.setCursor(30,18);
               display.setTextColor(WHITE);
               display.setTextSize(1);
               display.setTextWrap(false);
               //display.print(timer);
               display.print("NEXT LAP");
               display.display();
               delay(1000);
               }
   
     /*case 1:
        for(int tmer = runFor2; tmer > 0; --tmer){
                   
                   display.clearDisplay();
                   display.setCursor(20,32);
                   display.setTextColor(WHITE);
                   display.setTextWrap(false);
                   display.setTextSize(1);
                   display.print(tmer);
                   display.display();
                   delay(1000);
                   
                   display.setCursor(30,32);
                   display.setTextColor(WHITE);
                   display.setTextSize(1);
                   display.setTextWrap(false);
                   //display.print(timer);
                   display.print(" SECONDs LEFT! ");
                   display.display();
                   delay(1000);
                   
                  }
         digitalWrite(LED_PIN, HIGH);
         delay(1000);
         digitalWrite(LED_PIN, LOW);
        /* display.setCursor(20,18);
         display.setTextColor(WHITE);
         display.setTextWrap(false);
         display.setTextSize(1);
         display.print(x);
         display.display();*/
         //delay(1000);
         //break;
 
   display.clearDisplay();
   display.setCursor(0,0);
   display.setTextColor(WHITE);
   display.setTextSize(2);
   display.setTextWrap(false);
   display.print("TIMER ALERT!");
   display.display(); 
   delay(2000);
   
}
