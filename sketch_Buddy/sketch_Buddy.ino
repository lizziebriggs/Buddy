// Library for components
#include "Arduino.h"
#include "Adafruit_NeoPixel.h" // 8x8 LED
#include "LDR.h" // Photoresistor
#include "SoilMoisture.h" // Soil moisture
#include "DHT.h" // Temperature
#include "DCMotor.h" // Motor
#include "MPU6050.h" // Accelerometer and gyroscope
#include "Wire.h"
#include "I2Cdev.h"
#include "BTHC05.h" // Bluetooth

// == PIN DEFINITIONS ==
#define LEDMATRIXRGB_PIN_DIN 2
#define LDR_PIN_SIG A10
#define SOILMOISTURE_5V_PIN_SIG A0
#define DHT_PIN_DATA 3
#define DCMOTOR_PIN_COIL1 4
#define CAPACITIVETOUCH_PIN_SIG  3

// == GLOBAL DEFINITIONS AND DEFINES ==
#define LedMatrixRGB_NUM_LEDS 64*1
#define LedMatrixRGB_BRIGHTNESS 10

#define THRESHHOLD_ldr 100
int ldrAverageLight;

int motorSpeed, maxMotorSpeed; // Motor values

int16_t accX, accY, accZ; // Accelerometer values
int16_t gyroX, gyroY, gyroZ; // Gyroscope values

// == OBJECT INITIALIZATION ==
Adafruit_NeoPixel LedMatrixRGB = Adafruit_NeoPixel(LedMatrixRGB_NUM_LEDS, LEDMATRIXRGB_PIN_DIN, NEO_GRBW + NEO_KHZ800);
LDR ldr(LDR_PIN_SIG);
SoilMoisture soilMoisture_5V(SOILMOISTURE_5V_PIN_SIG);
DHT dht(DHT_PIN_DATA);
DCMotor dcMotor(DCMOTOR_PIN_COIL1);
MPU6050 mpu6050;
HardwareSerial& bt(Serial1);

// == STATE DEFINITIONS ==
enum States {Happy, Sad, Neutral, Watered, Thirsty, Drowned, Hot, Cold, Dark, Light, Notification, Birthday, Blushing};
enum States condition;

// == PLANT VALUES ==
int goodLight, goodWater, goodTemp;
int minLight, maxLight;
int minWater, maxWater;
int minTemp, maxTemp;


void setup() 
{
    // Set up serial for debugging
    Serial.begin(9600);
    while (!Serial) ; // wait for serial port to connect. Needed for native USB
    Serial.println("start");
    
    LedMatrixRGB.setBrightness(LedMatrixRGB_BRIGHTNESS);
    LedMatrixRGB.begin();
    LedMatrixRGB.show(); // Initialize all pixels to 'off'
    
    ldrAverageLight = ldr.readAverage();
    
    dht.begin();
    
    maxMotorSpeed = 128;
    
    Wire.begin();
    mpu6050.initialize();

    pinMode(CAPACITIVETOUCH_PIN_SIG, INPUT);

    bt.begin(9600);
    bt.println("Bluetooth on...");
}


void loop() 
{
    DisplayLED();
    
    CheckConditions();
    
    Spin();
    
    CheckStability();

    CheckTouch();

    SendData();

    ReceiveData();
}


void DisplayLED()
{
    switch (condition)
    {
        case Happy:
            DrawHappyFace();
            break;
            
        case Sad:
            DrawSadFace();
            break;

        case Neutral:
            DrawNeutral();
            break;
            
        case Watered:
            DrawHeartFilled();
            delay(300);
            condition = Neutral;
            break;
            
        case Thirsty:
            DrawWaterDroplet();
            break;
            
        case Drowned:
            DrawDrowned();
            break;
            
        case Hot:
            DrawFire();
            break;
            
        case Cold:
            DrawSnowflake();
            break;
            
        case Dark:
            DrawMoon();
            break;
            
        case Light:
            DrawSun();
            break;
            
        case Notification:
            DrawExclamation();
            break;
            
        case Birthday:
            DrawCandle();
            break;

        case Blushing:
            DrawBlushing();
            delay(500);
            condition = Neutral;
            break;
    }
}


void CheckConditions()
{
    CheckLight();
    CheckMoisture();
    CheckTemperature();
    
    if(goodLight && goodWater && goodTemp)
        condition = Happy;

    else if (!goodLight && !goodWater && !goodTemp)
        condition = Sad;
}


void CheckLight()
{
    int currentLight = ldr.read();
    
    if(currentLight > maxLight)
    {
        goodLight = false;
        condition = Light;
    }
        
    else if(currentLight < minLight)
    {
        goodLight = false;
        condition = Dark;
    }
        
    else
    {
        goodLight = true;
        condition = Happy;
    }
}


void CheckMoisture()
{
    int currentMoisture = soilMoisture_5V.read();
    
    if(currentMoisture > maxWater)
    {
        goodWater = false;
        condition = Drowned;
    }
        
    else if(currentMoisture < minWater)
    {
        goodWater = false;
        condition = Thirsty;
    }
        
    else
    {
        goodWater = true;
        condition = Watered;
    }
}


void CheckTemperature()
{
    int currentTemperature = dht.readTempC();
    
    if(currentTemperature > maxTemp)
    {
        goodTemp = false;
        condition = Hot;
    }
        
    else if(currentTemperature < minTemp)
    {
        goodTemp = false;
        condition = Cold;
    }
        
    else
    {
        goodTemp = true;
        condition = Happy;
    }
}

void Spin()
{
    if(motorSpeed > maxMotorSpeed) return;
    if(motorSpeed == 0) dcMotor.off();
    
    dcMotor.on(motorSpeed);
}


void CheckStability()
{
    mpu6050.getMotion6(&accX, &accY, &accZ, &gyroX, &gyroY, &gyroZ);
    
    // If any of the gryo values are greater than 90
    // assume the plant has fallen over
    if(gyroX > 90 || gyroY > 90 || gyroZ > 90)
        condition = Notification;
    else
        condition = Neutral;
}


void CheckTouch()
{
  bool touched = digitalRead(CAPACITIVETOUCH_PIN_SIG);

  if(touched == 1)
      condition = Blushing);
}


void SendData()
{
  int light = ldr.read();
  int moisture = soilMoisture_5V.read();
  int temp = dht.readTempC();

  if(!bt.available()) return;
  
  bt.print(light);
  bt.print(",");
  bt.print(moisture);
  bt.print(",");
  bt.println(temp););
}


void ReceiveData()
{
  if(!bt.available()) return;

  input = bt.readStringUntil('\n');

  if(input == "") return;
  
  switch (input[0])
    {
      // L indicates light
      case 'L':
        // Calculate and set min and max value recieved
        break;

      // W indicates moisture (water)
      case 'W':
        // Calculate and set min and max value recieved
        break;
        
      // T indicates temperature
      case 'T':
        // Calculate and set min and max value recieved
        break;
        
      // M indicates motor
      case 'M':
        // Set motor speed
        break;
    }
}


void DrawHappyFace()
{
    int yellowLED[] = {2, 5, 10, 13, 18, 21, 32, 39, 41, 46, 50, 53, 59, 60};
    
    for(uint16_t i = 0; i < LedMatrixRGB.numPixels(); i++)
    {
        if(IsInArray(i, yellowLED[])
            SetYellow(i);
            
        else
            SetWhite(i);
    }
    
    LedMatrixRBG.show();
}


void DrawSadFace()
{
    int blueLED[] = {2, 5, 10, 13, 18, 21, 35, 36, 42, 45, 49, 54, 56, 63};
    
    for(uint16_t i = 0; i < LedMatrixRGB.numPixels(); i++)
    {
        if(IsInArray(i, blueLED[])
            SetBlue(i);

        else
            SetWhite(i);
    }
    
    LedMatrixRBG.show();
}


void DrawNeutralFace()
{
  
}


void DrawHeartOutline()
{
    int redLED[] = {1, 2, 5, 6, 8, 11, 12, 15, 16, 23, 24, 31, 32, 39, 41, 46, 50, 53, 59, 60};
    
    for(uint16_t i = 0; i < LedMatrixRGB.numPixels(); i++)
    {
        if(IsInArray(i, redLED[])
            SetRed(i);

        else
            SetWhite(i);
    }
    
    LedMatrixRBG.show();
}


void DrawHeartFilled()
{
    int whiteLED[] = {0, 3, 4, 7, 40, 47, 48, 49, 54, 55, 56, 57, 58, 61, 62, 63};
    
    for(uint16_t i = 0; i < LedMatrixRGB.numPixels(); i++)
    {
        if(IsInArray(i, whiteLED[])
            SetWhite(i);
        
        else
            SetRed(i);
    }
    
    LedMatrixRBG.show();
}

void DrawWaterDroplet()
{
    int whiteLED[] = {0, 1, 2, 5, 6, 7, 8, 9, 11, 14, 15, 16, 18, 23, 25, 40, 47, 48, 55, 56, 57, 62, 63};
    
    for(uint16_t i = 0; i < LedMatrixRGB.numPixels(); i++)
    {
        if(IsInArray(i, whiteLED[])
            SetWhite(i);
        
        else
            SetBlue(i);
    }
    
    LedMatrixRBG.show();
}


void DrawDrowned()
{
  int redLED[] = {0, 7, 9, 14, 18, 21, 27, 28, 35, 36, 42, 45, 49, 54, 56, 63};
  int whiteLED[] = {1, 2, 5, 6, 8, 11, 15, 16, 23, 25, 40, 47, 48, 55, 57, 62};
  
  for(uint16_t i = 0; i < LedMatrixRGB.numPixels(); i++)
    {
        if(IsInArray(i, redLED[])
            SetRed(i);
        
        else if(IsInArray(i, whiteLED[])
            SetWhite(i);

        else
            SetBlue(i);
    }
    
    LedMatrixRBG.show();
}


void DrawFire()
{
    int yellowLED[] = {1, 3, 5, 10, 12, 14, 17, 19, 21, 23};
    int orangeLED[] = {9, 11, 13, 16, 18, 20, 22, 24, 25, 26, 27, 28, 29, 30, 31, 34, 36, 38, 41, 43, 45};
    int redLED[] = {32, 33, 35, 37, 39, 40, 42, 44, 46, 47, 49, 50, 51, 52, 53, 54, 58, 59, 60, 61};
    
    for(uint16_t i = 0; i < LedMatrixRGB.numPixels(); i++)
    {
        if(IsInArray(i, yellowLED[])
            SetYellow(i);
        
        else if(IsInArray(i, orangeLED[])
            SetOrange(i);
        
        else if(IsInArray(i, redLED[])
            SetRed(i);

        else
            SetWhite(i);
    }
    
    LedMatrixRBG.show();
}


void DrawSnowflake()
{
    int blueLED[] = {2, 5, 11, 12, 16, 19, 20, 23, 25, 26, 27, 28, 29, 30, 33, 34, 35, 36, 37, 38, 40, 43, 44, 47, 51, 52, 58, 61};
    
    for(uint16_t i = 0; i < LedMatrixRGB.numPixels(); i++)
    {
        if(IsInArray(i, blueLED[])
            SetBlue(i);

        else
            SetWhite(i);
    }
    
    LedMatrixRBG.show();
}


void DrawMoon()
{
    int blueLED[] = {2, 3, 4, 5, 9, 10, 11, 14, 16, 17, 18, 19, 23, 24, 25, 26, 27, 32, 33, 34, 35, 40, 41, 42, 43, 47, 49, 50, 51, 54, 58, 59, 60, 61};
    
    for(uint16_t i = 0; i < LedMatrixRGB.numPixels(); i++)
    {
        if(IsInArray(i, blueLED[])
            SetBlue(i);

        else
            SetWhite(i);
    }
    
    LedMatrixRBG.show();
}


void DrawSun()
{
    int whiteLED[] = {1, 2, 4, 5, 6, 8, 10, 12, 13, 15, 16, 17, 22, 23, 30, 31, 32, 33, 40, 41, 46, 47, 48, 50, 51, 53, 55, 57, 58, 59, 61, 62};
    
    for(uint16_t i = 0; i < LedMatrixRGB.numPixels(); i++)
    {
        if(IsInArray(i, whiteLED[])
            SetWhite(i);
        
        else
            SetYellow(i);
    }
    
    LedMatrixRBG.show();
}


void DrawExclamation()
{
    int redLED[] = {3, 4, 11, 12, 19, 20, 27, 28, 35, 36, 51, 52, 59, 60};
    
    for(uint16_t i = 0; i < LedMatrixRGB.numPixels(); i++)
    {
        if(IsInArray(i, redLED[])
            SetRed(i);

        else
            SetWhite(i);
    }
    
    LedMatrixRBG.show();
}


void DrawCandle()
{
    int yellowLED[] = {3, 4};
    int orangeLED[] = {11, 12};
    int blueLED[] = {19, 28, 35, 44, 51, 60};
    int redLED[] = {20, 27, 36, 43, 52, 59};
    
    for(uint16_t i = 0; i < LedMatrixRGB.numPixels(); i++)
    {
        if(IsInArray(i, yellowLED[])
            SetYellow(i);
        
        else if(IsInArray(i, orangeLED[])
            SetOrange(i);
        
        else if(IsInArray(i, blueLED[])
            SetBlue(i);
        
        else if(IsInArray(i, redLED[])
            SetRed(i);

        else
            SetWhite(i);
    }
    
    LedMatrixRBG.show();
}


void DrawBlushing()
{
    int yellowLED[] = {2, 5, 10, 13, 18, 21};
    int redLED[] = {33, 34, 37, 38};
    
    for(uint16_t i = 0; i < LedMatrixRGB.numPixels(); i++)
    {
        if(IsInArray(i, yellowLED[])
            SetYellow(i);
        
        else if(IsInArray(i, redLED[])
            SetRed(i);

        else
            SetWhite(i);
    }
    
    LedMatrixRBG.show();
}


bool IsInArray(int value, int array[])
{
    for(int i = 0; i < sizeof(array); i++)
    {
        if(array[i] == value) return true;
    }
    
    return false;
}


void SetRed(int ledNum)
{
    LedMatrixRGB.setPixelColor(ledNum, 255, 46, 31);
}


void SetOrange(int ledNumint ledNum)
{
    LedMatrixRGB.setPixelColor(ledNum, 252, 150, 25);
}


void SetYellow(int ledNum)
{
    LedMatrixRGB.setPixelColor(ledNum, 255, 245, 46);
}


void SetGreen(int ledNum)
{
    LedMatrixRGB.setPixelColor(ledNum, 53, 240, 24);
}


void SetBlue(int ledNum)
{
    LedMatrixRGB.setPixelColor(ledNum, 24, 217, 214);
}


void SetWhite(int ledNum)
{
    LedMatrixRGB.setPixelColor(ledNum, 0, 0, 0);
}
