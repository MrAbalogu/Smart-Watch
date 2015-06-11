#include <SPI.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>
#include "I2Cdev.h"
#include "MPU6050.h"

#define OLED_RESET 8
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

#define OUTPUT_READABLE_ACCELGYRO

/* Bluetooth */
SoftwareSerial BTSerial(2, 3); //Connect HC-06. Use your (TX, RX) settings

/* time */
#define SENDING_INTERVAL 1000
#define SENSOR_READ_INTERVAL 50
unsigned long prevSensoredTime = 0;
unsigned long curSensoredTime = 0;

/* Data buffer */
#define ACCEL_BUFFER_COUNT 125
byte aAccelBuffer[ACCEL_BUFFER_COUNT];
int iAccelIndex = 2;

/* MPU-6050 sensor */
#define MPU6050_ACCEL_XOUT_H 0x3B // R
#define MPU6050_PWR_MGMT_1 0x6B // R/W
#define MPU6050_PWR_MGMT_2 0x6C // R/W
#define MPU6050_WHO_AM_I 0x75 // R
#define MPU6050_I2C_ADDRESS 0x68

typedef union accel_t_gyro_union {
	struct {
		uint8_t x_accel_h;
		uint8_t x_accel_l;
		uint8_t y_accel_h;
		uint8_t y_accel_l;
		uint8_t z_accel_h;
		uint8_t z_accel_l;
		uint8_t t_h;
		uint8_t t_l;
		uint8_t x_gyro_h;
		uint8_t x_gyro_l;
		uint8_t y_gyro_h;
		uint8_t y_gyro_l;
		uint8_t z_gyro_h;
		uint8_t z_gyro_l;
	} reg;

	struct {
		int x_accel;
		int y_accel;
		int z_accel;
		int temperature;
		int x_gyro;
		int y_gyro;
		int z_gyro;
	} value;
};


/**********************************************   
**********  Variable declarations   ***********
***********************************************/

  int i = 0;
  int j = 0;
  const int ScrollPin = 12;     
  const int EnterPin = 11;
  const int buzzerPin = 10;
  int clickLength = 0;
  int clickLength1 = 0;  
  int Press = 100;
  int press2 = 400; 
  int Click = 0;
  int ScrollClickCounts = 0;
  int EnterClickCounts = 0;
  int runFor = 60;
  int count;
  int count2;
  int x;
  int y;
  int ct = 0;
  boolean hasBeenPressed = false;
  boolean hasTiltedForward = false;
  boolean hasTiltedBackward = false;
  MPU6050 accelgyro;
  int16_t ax, ay, az;
  int16_t gx, gy, gz;


/**********************************************   
****   setup() & loop() through arduino   *****
***********************************************/  

void setup(){
  int error;
  uint8_t c;  
  
  pinMode(EnterPin, INPUT_PULLUP);   
  pinMode(ScrollPin, INPUT_PULLUP);   
  pinMode(buzzerPin, OUTPUT);
  
  Serial.begin(9600);
  Wire.begin();
  BTSerial.begin(9600);  // set the data rate for the BT port

  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.clearDisplay();
    
    //error = MPU6050_read (MPU6050_WHO_AM_I, &c, 1);
    //error = MPU6050_read (MPU6050_PWR_MGMT_2, &c, 1);
    //MPU6050_write_reg (MPU6050_PWR_MGMT_1, 0);

    //initBuffer();
    accelgyro.initialize();
    DisplayMenu();
}

void loop(){ 
  /*curSensoredTime = millis();
  
  // Read from sensor
  if(curSensoredTime - prevSensoredTime > SENSOR_READ_INTERVAL) {
    readFromSensor();  // Read from sensor
    prevSensoredTime = curSensoredTime;
    
    // Send buffer data to remote
    if(iAccelIndex >= ACCEL_BUFFER_COUNT - 3) {
      sendToRemote();
      initBuffer();
      Serial.println("------------- Send 20 accel data to remote");
    }
  }*/  
  
   accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    x = map(ax, -2048, 2048, -100, 100 );
    y = map(ay, -2048, 2048, -100, 100 );

    if(x>99 && !hasTiltedForward)
    {
       ct++;
       hasTiltedForward = true;
       hasTiltedBackward = false;
    }

    if(x<80 && !hasTiltedBackward)
    {
       ct++;
       hasTiltedBackward = true;
       hasTiltedForward = false;
    }        

   while(digitalRead(ScrollPin) == LOW)
   {
      delay(100);
      count++;
      clickLength = clickLength + 100;
        if(count == 1 && !hasBeenPressed)
        {
             ScrollClickCounts++;
             hasBeenPressed = true;
        }
        else hasBeenPressed = false;
   }  
   
   while(digitalRead(EnterPin) == LOW)
   {
      delay(100);
      count2++;
      clickLength1 = clickLength1 + 100;
        if(count2 == 1 && !hasBeenPressed)
        {
             EnterClickCounts++;
             hasBeenPressed = true;
        }
        else hasBeenPressed = false;
   }     

  ScrollMenu();
  ClickMenu();
  
  clickLength = 0; 
  clickLength1 = 0;  
  count = 0;
  count2 = 0;
  
}

/**************************************************
 * BT Transaction
 **************************************************/
/*void sendToRemote() {
  // Write gabage bytes
  BTSerial.write( "accel" );
  // Write accel data
  BTSerial.write( (char*)aAccelBuffer );
  // Flush buffer
  //BTSerial.flush();
}

/**************************************************
 * Read data from sensor and save it
 **************************************************/
/*void readFromSensor() {
  int error;
  double dT;
  accel_t_gyro_union accel_t_gyro;
  
  error = MPU6050_read (MPU6050_ACCEL_XOUT_H, (uint8_t *) &accel_t_gyro, sizeof(accel_t_gyro));
  if(error != 0) {
    Serial.print(F("Read accel, temp and gyro, error = "));
    Serial.println(error,DEC);
  }

  uint8_t swap;
  #define SWAP(x,y) swap = x; x = y; y = swap
  SWAP (accel_t_gyro.reg.x_accel_h, accel_t_gyro.reg.x_accel_l);
  SWAP (accel_t_gyro.reg.y_accel_h, accel_t_gyro.reg.y_accel_l);
  SWAP (accel_t_gyro.reg.z_accel_h, accel_t_gyro.reg.z_accel_l);
  SWAP (accel_t_gyro.reg.t_h, accel_t_gyro.reg.t_l);
  SWAP (accel_t_gyro.reg.x_gyro_h, accel_t_gyro.reg.x_gyro_l);
  SWAP (accel_t_gyro.reg.y_gyro_h, accel_t_gyro.reg.y_gyro_l);
  SWAP (accel_t_gyro.reg.z_gyro_h, accel_t_gyro.reg.z_gyro_l);
  
  // Print the raw acceleration values
  Serial.print(F("accel x,y,z: "));
  Serial.print(accel_t_gyro.value.x_accel, DEC);
  Serial.print(F(", "));
  Serial.print(accel_t_gyro.value.y_accel, DEC);
  Serial.print(F(", "));
  Serial.print(accel_t_gyro.value.z_accel, DEC);
  Serial.print(F(", at "));
  Serial.print(iAccelIndex);
  Serial.println(F(""));
  
  if(iAccelIndex < ACCEL_BUFFER_COUNT && iAccelIndex > 1) {
    int tempX = accel_t_gyro.value.x_accel;
    int tempY = accel_t_gyro.value.y_accel;
    int tempZ = accel_t_gyro.value.z_accel;

    char temp = (char)(tempX >> 8);
    if(temp == 0x00)
      temp = 0x7f;
    aAccelBuffer[iAccelIndex] = temp;
    iAccelIndex++;
    temp = (char)(tempX);
    if(temp == 0x00)
      temp = 0x01;
    aAccelBuffer[iAccelIndex] = temp;
    iAccelIndex++;
    
    temp = (char)(tempY >> 8);
    if(temp == 0x00)
      temp = 0x7f;
    aAccelBuffer[iAccelIndex] = temp;
    iAccelIndex++;
    temp = (char)(tempY);
    if(temp == 0x00)
      temp = 0x01;
    aAccelBuffer[iAccelIndex] = temp;
    iAccelIndex++;
    
    temp = (char)(tempZ >> 8);
    if(temp == 0x00)
      temp = 0x7f;
    aAccelBuffer[iAccelIndex] = temp;
    iAccelIndex++;
    temp = (char)(tempZ);
    if(temp == 0x00)
      temp = 0x01;
    aAccelBuffer[iAccelIndex] = temp;
    iAccelIndex++;
  }
}*/

/**************************************************
 * MPU-6050 Sensor read/write
 **************************************************/
/*int MPU6050_read(int start, uint8_t *buffer, int size)
{
	int i, n, error;
	
	Wire.beginTransmission(MPU6050_I2C_ADDRESS);
	
	n = Wire.write(start);
	if (n != 1)
		return (-10);
	
	n = Wire.endTransmission(false); // hold the I2C-bus
	if (n != 0)
		return (n);
	
	// Third parameter is true: relase I2C-bus after data is read.
	Wire.requestFrom(MPU6050_I2C_ADDRESS, size, true);
	i = 0;
	while(Wire.available() && i<size)
	{
		buffer[i++]=Wire.read();
	}
	if ( i != size)
		return (-11);
	return (0); // return : no error
}

int MPU6050_write(int start, const uint8_t *pData, int size)
{
	int n, error;
	
	Wire.beginTransmission(MPU6050_I2C_ADDRESS);
	
	n = Wire.write(start); // write the start address
	if (n != 1)
		return (-20);
		
	n = Wire.write(pData, size); // write data bytes
	if (n != size)
		return (-21);
		
	error = Wire.endTransmission(true); // release the I2C-bus
	if (error != 0)
		return (error);
	return (0); // return : no error
}

int MPU6050_write_reg(int reg, uint8_t data)
{
	int error;
	error = MPU6050_write(reg, &data, 1);
	return (error);
}*/


/**************************************************
 * Utilities
 **************************************************/
/*void initBuffer() {
  iAccelIndex = 2;
  for(int i=iAccelIndex; i<ACCEL_BUFFER_COUNT; i++) {
    aAccelBuffer[i] = 0x00;
  }
  aAccelBuffer[0] = 0xfe;
  aAccelBuffer[1] = 0xfd;
  aAccelBuffer[122] = 0xfd;
  aAccelBuffer[123] = 0xfe;
  aAccelBuffer[124] = 0x00;
}*/


/************************************************   
***********  HIGH FUNCTION METHODS   ************
*************************************************/

void DisplayMenu(){
  
  display.clearDisplay();
  display.setCursor(30,0);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setTextWrap(false);
  display.print("MAIN MENU");
  display.setCursor(35,30);
  display.print("BEEP TEST");
  display.setCursor(30,40);
  display.print("RUN HISTORY");
  display.setCursor(32,50);
  display.print("GAIT TRACK");
  display.display();
  
}

void ScrollMenu(){  
  if(ScrollClickCounts > 0 && EnterClickCounts == 0)
  {
        if(clickLength >= Press) 
        {
            i=i+1;
            if ( i == 4 )
            {
               i = 1;
            }
            display.drawRect(32,28,59,12,BLACK); 
            display.drawRect(28,38,70,12,BLACK);
            display.drawRect(28,48,66,12,BLACK);
            switch(i)
            {
               case 1 :  display.drawRect(32,28,59,12,WHITE); break;
               case 2 :  display.drawRect(28,38,70,12,WHITE); break;
               case 3 :  display.drawRect(28,48,66,12,WHITE); break;
            }   
            display.display();  
       }
  }
  if( EnterClickCounts > 0 && ScrollClickCounts > 0 )
  {
         if(clickLength >= Press)
         {
            j=j+1;
            if ( j == 3 )
            {
               j = 1;
            }            
            display.drawRect(32,28,62,12,BLACK); 
            display.drawRect(8,48,40,12,BLACK);
            switch(j)
            {
               case 1 :  display.drawRect(32,28,62,12,WHITE); break;
               case 2 :  display.drawRect(8,48,40,12,WHITE); break;
            }   
            display.display();         
         }
  }
}
   
void ClickMenu(){
  if( EnterClickCounts == 1)
  { 
        if (clickLength1 >= Press) 
        {    
          switch (i)
          {
             case 1: BeepTest(); break;
             case 2: RunHistory(); break;
             case 3: countSteps(); break;
          } 
        }    
          display.display();  
  }
  if(EnterClickCounts > 1)
  {
        if (clickLength1 >= Press) 
        {
           switch (j)
           {
             case 1: StartBeepTest(); break;
             case 2: DisplayMenu(); break;
           }  
        }  
    }  
}
  
/************************************************   
***********  LOW FUNCTION METHODS   *************
*************************************************/
 
void BeepTest(){

  display.clearDisplay();
  display.setCursor(35,30);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setTextWrap(false);
  display.print("START TEST");
  display.setCursor(10,50);
  display.print("BACK");
  display.display();
  
}

void RunHistory(){
  display.clearDisplay();
  display.setCursor(10,0);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setTextWrap(false);
  display.print("Last run:  ");
  display.setCursor(10,20);
  display.print("Total run distance:  ");
  display.setCursor(10,50);
  display.print("BACK");
  display.display();
}

void CaloriesBurntToday(){
  display.clearDisplay();
  display.setCursor(0,10);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setTextWrap(false);
  display.print(" Calories Burnt:   ");
  display.setCursor(10,50);
  display.print("BACK");
  display.display();
}

void StartBeepTest() {     
  
    display.clearDisplay();
    display.setCursor(50,10);
    display.setTextColor(WHITE);
    display.setTextSize(5);
    display.setTextWrap(false);
    display.print("3");
    display.display();
    delay(1000);
   
    display.clearDisplay();
    display.setCursor(50,10);
    display.setTextColor(WHITE);
    display.setTextSize(5);
    display.setTextWrap(false);
    display.print("2");
    display.display(); 
    delay(1000);
    
    display.clearDisplay();
    display.setCursor(50,10);
    display.setTextColor(WHITE);
    display.setTextSize(5);
    display.setTextWrap(false);
    display.print("1");
    display.display(); 
    delay(1000);

          for (runFor=runFor; runFor > 10; runFor=runFor-10 )
          {
             
              for(int timer = runFor; timer > 0; --timer)
              {   
                     display.clearDisplay();
                     display.setCursor(20,32);
                     display.setTextColor(WHITE);
                     display.setTextWrap(false);
                     display.setTextSize(1);
                     display.print(timer);
                     display.display();
                     delay(1000);
                     
              }
              
          digitalWrite(buzzerPin, HIGH);
          delay(1000);
          digitalWrite(buzzerPin, LOW);
               
          display.setCursor(30,18);
          display.setTextColor(WHITE);
          display.setTextSize(1);
          display.setTextWrap(false);
          display.print("NEXT LAP");
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
   delay(2000);
   
}

void countSteps(){
  
                display.clearDisplay();
                display.setTextSize(1);
                display.setTextColor(WHITE);
                display.setCursor(0,0);
                display.print("Steps: ");
                display.println(ct);
                display.display(); 
}
