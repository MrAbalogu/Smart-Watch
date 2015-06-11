#include <SPI.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>

#define OLED_RESET 8
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

int i = 0;



void setup () {
  pinMode(9, INPUT); //Set pin for time/date set button to input
  digitalWrite(9, HIGH); //Turn on pullup resistors

  pinMode(8, INPUT); //Set pin for time/date set button to input
  digitalWrite(8, LOW); //Turn on pullup resistors
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);  // initialize with the I2C addr 0x3D (for the 128x64)

  display.clearDisplay();
  display.setCursor(30,10);
  display.setTextColor(WHITE);
  //display.setTextColor(uint16_t color, uint16_t backgroundcolor);
  display.setTextSize(1);
  display.setTextWrap(false);
  display.print("Ipalibo Whyte");
  display.display();
  
  display.setCursor(30,30);
  display.setTextColor(WHITE);
  //display.setTextColor(uint16_t color, uint16_t backgroundcolor);
  display.setTextSize(1);
  display.setTextWrap(false);
  display.print("Chinedu Abalogu");
  display.display();
  
  
}

void loop () {
  
  while (1) {
    
    if ( digitalRead(9) ) {
      
      while ( digitalRead(9) ){
         //NULL   
      }
      
      i = i + 1;
      
      if ( i == 4) {
        i = 1;
      }
      
      display.drawRect(32,28,59,12,BLACK); 
      display.drawRect(32,38,59,12,BLACK) ;
      display.drawRect(32,48,59,12,BLACK);
    
      switch (i) {
        
        case 1 :
          display.drawRect(32,28,59,12,WHITE); 
          break;
          
        case 2 :
          display.drawRect(32,38,59,12,WHITE); 
          break;
          
        case 3 :
          display.drawRect(32,48,59,12,WHITE); 
          break;
          
      };display.display();
  
    }
   display.print("russ Whyte");
   display.display()

  }

}
