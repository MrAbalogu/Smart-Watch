#include <Process.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

Process date;                 // process used to get the date
int hours, minutes, seconds;  // for the results
int lastSecond = -1;          // need an impossible value for comparison


void setup() {
  
  Bridge.begin();        // initialize Bridge

  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);  
  display.display();
  delay(1000);
  
  
  while (!Serial);              // wait for Serial Monitor to open
  display.clearDisplay();
  display.setCursor(30,0);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setTextWrap(false);
  display.print("Time Check");  // Title of sketch
  display.display();
  // run an initial date process. Should return:
  // hh:mm:ss :
  if (!date.running()) {
    date.begin("date");
    date.addParameter("+%T");
    date.run();
  }
}

void loop() {

  if (lastSecond != seconds) { // if a second has passed
    // print the time:
    if (hours <= 9) display.print("0");    // adjust for 0-9
    display.print(hours);
    display.print(":");
    if (minutes <= 9) display.print("0");  // adjust for 0-9
    display.print(minutes);
    display.print(":");
    if (seconds <= 9) display.print("0");  // adjust for 0-9
    display.println(seconds);

    // restart the date process:
    if (!date.running()) {
      date.begin("date");
      date.addParameter("+%T");
      date.run();
    }
  }

  //if there's a result from the date process, parse it:
  while (date.available() > 0) {
    // get the result of the date process (should be hh:mm:ss):
    String timeString = date.readString();

    // find the colons:
    int firstColon = timeString.indexOf(":");
    int secondColon = timeString.lastIndexOf(":");

    // get the substrings for hour, minute second:
    String hourString = timeString.substring(0, firstColon);
    String minString = timeString.substring(firstColon + 1, secondColon);
    String secString = timeString.substring(secondColon + 1);

    // convert to ints,saving the previous second:
    hours = hourString.toInt();
    minutes = minString.toInt();
    lastSecond = seconds;          // save to do a time comparison
    seconds = secString.toInt();
  }
}
