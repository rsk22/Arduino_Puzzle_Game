/*
 Title: Puzzle Game for Touch Screen
 Author: Richard Kirkpatrick
 Date: 10 June 2014
 Description:
 Uses the Seeed Studio TFT Touch Screen (Version 1) with Arduino Uno to play the puzzle game.
 This puzzzle game was based on Nuts & Volts May 2014 issue.  The main difference is that this 
 version tries to use my personal libraries for a more Object Oriented Programming approach.
 Step 1:
   - Make a program that can move an object based on user input
*/

#include <stdint.h>
#include <TouchScreen.h> 
#include <TFT.h>
#include <TouchScreenGeometry.h> // Library for drawing shapes for the keypad

#ifdef SEEEDUINO
  #define YP A2   // must be an analog pin, use "An" notation!
  #define XM A1   // must be an analog pin, use "An" notation!
  #define YM 14   // can be a digital pin, this is A0
  #define XP 17   // can be a digital pin, this is A3 
#endif

#ifdef MEGA
  #define YP A2   // must be an analog pin, use "An" notation!
  #define XM A1   // must be an analog pin, use "An" notation!
  #define YM 54   // can be a digital pin, this is A0
  #define XP 57   // can be a digital pin, this is A3 
#endif 

//Measured ADC values for (0,0) and (210-1,320-1)
//TS_MINX corresponds to ADC value when X = 0
//TS_MINY corresponds to ADC value when Y = 0
//TS_MAXX corresponds to ADC value when X = 240 -1
//TS_MAXY corresponds to ADC value when Y = 320 -1
#define TS_MINX 140
#define TS_MAXX 900

#define TS_MINY 120
#define TS_MAXY 940

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

void setup() 
{  
  Serial.begin(9600);
  Tft.init();          // Initializes TFT library
  
  const int xminButton = 12;  // x-min for button
  const int yminButton = 70; // y-min for button
  const int heightButton = 45; // height for button
  const int widthButton = 45; // width for button
  Rectangle r1(xminButton, yminButton, heightButton, widthButton);
  r1.draw();           // Draws the rectangle
  drawObjects(r1);     // Draws the objects to the screen
}

void loop() 
{ 
  // Intentionally left blank 
}

void drawObjects(Rectangle r1)
{
  while (true) {
    // A point objects holds x, y, and z coordinates
    Point p = ts.getPoint(); //
    p.x = map(p.x, TS_MINX, TS_MAXX, 240, 0);
    p.y = map(p.y, TS_MINY,TS_MAXY, 320, 0);
    
    if (p.z > ts.pressureThreshhold) {
      if (inBoundaries(r1, p))
        r1.move(p.x - r1.getXStart(), p.y - r1.getYStart());
    }
    delay(10);
  }
}

boolean inBoundaries(Rectangle rect, Point p) 
{
  return ((rect.getWidth() + p.x < 239) && (rect.getHeight() + p.y < 319));
}












