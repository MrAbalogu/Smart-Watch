#include <Time.h>               //the Arduino): http://playground.arduino.cc/Code/Time
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

int buttonPin1 = 6;
int buttonPin2 = 8;
int buttonPin3 = 9;
int buzzerPin = 12;

int tSeconds=10, tMinutes=0, hours=0;  //this line, along with another line in void timerFunction(), is where you can adjust the amount of time that is counted down in                                                                    //the timer function
int centiseconds=0, sSeconds=0, sMinutes=0;

int button1Counter = 0;  //this section initializes the button counters, which is how the buttons can have multiple operations within the same function - more info:       
int button1State = 0;        // http://arduino.cc/en/Tutorial/ButtonStateChange
int lastButton1State = 0;
int button2Counter = 0;
int button2State = 0;
int lastButton2State = 0;


void setup()
{
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);  
  display.clearDisplay();
  setTime(14,06,00,10,06,2014);  // this is how you can set the time for the clock function (hour,minute,second,day,month,year)
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
}

void loop()
{ 
  button1State = digitalRead(buttonPin1);  //this section gives the "Mode" button the ability to have three seperate operations
  if(button1State != lastButton1State)
  {
    if(button1State = HIGH)
    {
      button1Counter++;
      button2Counter = 1;
      delay(200);
    }
  }
  lastButton1State = button1State;
  if(button1Counter > 3)
  {
    button1Counter = 1;
  }
  button2State = digitalRead(buttonPin2);  //similarly, this section allows the "Start/Stop" button the ability to have two seperate functions
  if(button2State != lastButton2State)
  {
    if(button2State = HIGH)
    {
      button2Counter++;
      delay (200);
    }
  }
  lastButton2State = button2State;
  if(button2Counter > 2)
  {
    button2Counter = 1;
  }

 switch(button1Counter)  //switch case is really nice for changing the clock between its three functions
  {
    case 1:  //the clock defaults to clockFunction()
    //display.clearDisplay();
    display.setTextColor(WHITE);
    display.setCursor(20, 10);
    display.print(":");
    display.setCursor(50, 10);
    display.print(":");
    display.display();
    clockFunction();
    delay(2000);
    break;
    case 2:  //if the "Mode" button is pressed, clock switches to timerFunction()
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setCursor(20, 10);
    display.print(":");
    display.setCursor(50, 10);
    display.print(":");
    display.display();
    timerFunction();
    delay(2000);
    break;
    case 3:  //if "Mode" is pressed again, the clock switches once more to stopwatchFunction()
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setCursor(20, 10);
    display.print(":");
    display.setCursor(50, 10);
    display.print(":");
    display.display();
    stopwatchFunction();
    delay(2000);
    break;
  }
}

void clockFunction()  //this is the time clock function; it works using the Time.h library
{
  display.setCursor(0, 0);
  display.setTextColor(WHITE);
  printDigits(month());
  display.setCursor(10, 0);
  display.print("/");
  display.setCursor(30, 0);
  printDigits(day());
  display.setCursor(50, 0);
  display.print("/");
  display.setCursor(60, 0);
  display.print(year());
  display.setCursor(0, 10);
  display.print(hour());
  display.setCursor(30, 10);
  printDigits(minute());
  display.setCursor(60, 10);
  printDigits(second());
  delay(100);
  display.display();

}

void timerFunction()  //the timer function was made with the help of this post: http://pastebin.com/f57045830
{
  if(button2Counter == 2)  //if "Start/Stop" is pressed, the timer counts down
  {
    static unsigned long lastTick = 0;
    if (tSeconds > 0)
    {
      if (millis() - lastTick >= 1000)
        {
        lastTick = millis();
        tSeconds--;
        lcdOutput();
      }
     }
    if (tMinutes > 0)
    {
      if (tSeconds <= 0)
      {
        tMinutes--;
        tSeconds = 60;
      }
    }
    if (hours > 0)
    {
      if (tMinutes <= 0)
      {
        hours--;
        tMinutes = 60;
      }
    }
  }
  else  //if "Start/Stop" is unpressed or pressed a second time, display the current time, but don't count down
  {
    lcdOutput();
  }
  if(hours == 00 && tMinutes == 00 && tSeconds == 00)  //when timer ends, the alarm goes off
  {
    button2Counter = 1;
    while(digitalRead(buttonPin3) == HIGH)  //the alarm will only go off until "Restart" is pressed
    {
      display.setCursor(0, 10);
      display.setTextColor(WHITE);
      display.print("00:00:00");
      digitalWrite(buzzerPin, HIGH);
      delay(100);
      digitalWrite(buzzerPin, LOW);
      delay(100);
    }
    if(digitalRead(buttonPin3) == LOW)  //when "Restart" is pressed, the timer resets
    {
      hours = 0;  //this part also 
      tMinutes = 0;
      tSeconds = 10;
      lcdOutput();
    }    
  }
  if(digitalRead(buttonPin3) == LOW && button2Counter == 1)  //resets the timer when "Restart" button is pressed, as long as the timer is NOT running
  {
    hours = 0;  //this part also must be changed when the timer is altered, to match the initial time
    tMinutes = 0;
    tSeconds = 10;
    delay(10);
  }
  display.display();
}

void stopwatchFunction()
{
  display.setCursor(60, 10);
  printDigits(centiseconds);
  display.setCursor(30, 10);
  printDigits(sSeconds);
  display.setCursor(0, 10);
  printDigits(sMinutes);
  display.display();
  if(button2Counter == 2)  //if the "Start/Stop" button is pressed, the time begins running
  {
    delay(5);
    centiseconds++;
  }
  if(digitalRead(buttonPin3) == LOW && button2Counter == 1)  //if the "Restart" button is pressed, as long as the timer is paused, the stopwatch resets to 00:00.00
  {
    centiseconds = 0;
    sSeconds = 0;
    sMinutes = 0;
  } 
  if(centiseconds == 100)
  {
    centiseconds = 0;
    sSeconds++;
  }
  if(sSeconds == 60)
  {
    sSeconds = 0;
    sMinutes++;
  }
  display.display();
}

void lcdOutput()  //this is just used to display the timer on the LCD
{
  display.setCursor(0, 10);
  printDigits(hours);
  display.setCursor(30, 10);
  printDigits(tMinutes);
  display.setCursor(60, 10);
  printDigits(tSeconds);
  delay(100);
  display.display();

}

void printDigits(int digits)  //this void function is really useful; it adds a "0" to the beginning of the number, so that 5 minutes is displayed as "00:05:00", rather than "00:5 :00"
{
  if(digits < 10) 
  {
    display.print("0");
    display.print(digits);
  }
  else
  {
    display.print(digits);
  }
  display.display();

}

