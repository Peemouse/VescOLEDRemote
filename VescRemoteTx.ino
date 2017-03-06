/*********************************************************************
Monitoring Remote for VESC
by Clément Le Priol
March 2017

*********************************************************************/

#include <SPI.h>
#include <Wire.h>
//#include "VescUart.h"
//#include "datatypes.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSans18pt7b.h>

// If using software SPI (the default case):
#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

/* Uncomment this block to use hardware SPI
#define OLED_DC     6
#define OLED_CS     7
#define OLED_RESET  8
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);
*/

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

/*
#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif */
//HW config
uint8_t pinBT = 2;

//build config
uint8_t wheelDiam = 203; //Wheel diameter in mm
uint8_t motorPulley = 12; //Motor pulley tooth number
uint8_t drivePulley = 50; //Drive pulley tooth number
uint8_t motorPoles = 14; //Number of motor poles
uint16_t capacity = 215; //Capacity of the battery in Wh (nS*3,7*cap in mAh)

//working variables
long lastTime;
float maxCurrent=0.0;
float distance=0.0;
float maxSpeed=0;
float conso=0.0;

void setup()   {                
  Serial.begin(9600);
  pinMode(pinBT, INPUT_PULLUP);
  display.begin(SSD1306_SWITCHCAPVCC);
  delay(100);

  display.clearDisplay();
  lastTime = millis();
}


void loop() {
/*  display.invertDisplay(true);
  delay(1000); 
  display.invertDisplay(false);
  delay(1000); 
*/

//Data

int sensorValue = analogRead(A2); //Test with a potentiometer
float erpm = map(sensorValue, 0, 1023, 0, 60000); //To be connected to the VESC
float instantSpeed = erpm/motorPoles*motorPulley/drivePulley*wheelDiam/5305.16; //exact formula : erpm/motorPoles/60*motorPulley/drivePulley*wheelDiam/1000*3.14159*3.6

maxSpeed = max(instantSpeed,maxSpeed);

float batVoltage=38.6; //To be connected to the VESC

distance += instantSpeed/3600.0*float(millis()-lastTime)/1000.0;
lastTime=millis();

float current; //To be connected to the VESC
maxCurrent=max(current,maxCurrent);

uint8_t tempFet=46;//To be connected to the VESC

uint8_t whTotal; //To be connected to the VESC
//conso = whTotal/distance;
conso=11.5;

float range = float(capacity)/conso;

//Display
  //Header
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setFont();
  display.setCursor(0,0);
  display.print(batVoltage,1);
  display.print("V");
  display.setCursor(35,0);
  display.print(maxCurrent,1);
  display.print("Amax");
  display.setCursor(95,0);
  display.print(tempFet);
  display.print("C");
  if (digitalRead(pinBT)) {
  display.setCursor(120,0); //Symbol BT
  display.write(146);
  }
    /*Body (layout 1)
  display.drawFastHLine(0,9, 128,WHITE);
  display.setFont(&FreeSans18pt7b);
  display.setTextSize(1);
  display.setCursor(30,39);
  display.print(instantSpeed,1);
  display.setFont();
  display.setTextSize(1);
  display.print(" km/h");
  display.setCursor(0,45);
  display.print("Max:");
  display.print(maxSpeed,1);
  display.setCursor(65,45);
  display.print("Dist:");
  display.print(distance,2); */
  
  //Body (layout 2)
  display.drawFastHLine(0,9, 128,WHITE);
  display.setFont(&FreeSans18pt7b);
  display.setTextSize(1);
  display.setCursor(0,40);
  display.print(instantSpeed,1);
  display.setFont();
  display.setTextSize(1);
  display.setCursor(70,13);
  display.print("Max:  ");
  display.print(maxSpeed,1);
  display.setCursor(70,22);
  display.print("Dist:");
  if (distance>=10.00) { display.print(distance,1);}
  else {display.print(distance,2);}
  display.setCursor(70,31);
  display.print("Cons:");
  display.print(conso,1);
  display.setCursor(70,40);
  display.print("Auto:");
  display.print(range,1);
  display.setCursor(20,45);
  display.print("km/h");
  
  //Footer
  display.drawFastHLine(0,54, 128,WHITE);
  display.setCursor(40,57);
  display.print("NO FAULT");
  //display.setCursor(40,40);
  //display.println("Lake");
  display.display();
  //display.dim(true);
  //display.dim(false);
  delay(100);
}




