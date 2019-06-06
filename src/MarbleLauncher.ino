/*
 * Code Description: This code is for a marble launcher project I created for my physics class at school. Basically it 
 * just calculates the angle the marble launcher is at using the GY-521 sensor and prints it to a 16x2 LCD attached to 
 * the Arduino.
 * 
 * By: Darian Benam
 * Date: 2019-05-20
 */

#include <LiquidCrystal.h>
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);  
MPU6050 accelerometerGyro;

bool errorMsgPrinted = false; // States whether the error message about the GY-521 not being found has been printed or not.
bool runLoop = false; // States whether the loop() method should run or not.
int lcdContrast = 125; // The contrast value of the 16x2 LCD.

/*
 * The setup method for the Arduino.
 */
void setup() 
{
  Serial.begin(9600);
  Wire.begin();
  
  // Initialize the LCD:
  lcd.begin(16, 2);
  
  // Adjust the LCD contrast:
  analogWrite(6, lcdContrast);

  // Write some text on the first row of the LCD:
  lcd.setCursor(0,0);
  lcd.print("Launcher Angle:");

  // Initalize the accelerometer:
  accelerometerGyro.initialize();
  
  if (accelerometerGyro.testConnection())
  {
    Serial.println("Accelerometer connection successful! :)"); 
    runLoop = true;
  }
  else
  {
    Serial.println("GY-521 connection failed. :(");
  }
}

/*
 * The loop method for the Arduino.
 */
void loop()
{
  // Don't run the loop if the accelerometer isn't found:
  if (!runLoop)
  {
    if (!errorMsgPrinted)
    {
      printErrorMessage();
    }
    return;
  }
  
  int angle = getAngle();
  updateLCD(angle);
  
  delay(1000); // Wait 1 second before rerunning the loop.
}

/*
 * Calculates the angle of the GY-521 accelerometer.
 */
int getAngle()
{
  int ax;
  int accelAngle;
  int accelReading;
  int accelCorrected;
  
  accelerometerGyro.getMotion6(&ax, 0, 0, 0, 0, 0);

  // Calculate the angle for the X axis is at on the GY-521 accelerometer:
  accelCorrected = ax - 200;
  accelCorrected = map(accelCorrected, -16800, 16800, -90, 90);
  accelCorrected = constrain(accelCorrected, -90, 90);
  
  accelAngle = abs(accelCorrected); // Get the absoloute value of the angle.
  
  return accelAngle;
}

/*
 * Updates the bottom row text on the LCD with the angle the marble launcher is at:
 */
void updateLCD(int angle)
{
  lcd.setCursor(0,1);
  lcd.print(angle);
  lcd.print(" degrees");
}

/*
 * Prints error text onto the 16x2 LCD.
 */
void printErrorMessage()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Error. :(");

  errorMsgPrinted = true;
}
