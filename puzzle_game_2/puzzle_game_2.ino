/*
  Puzzle Game
*/

// Libraries
#include <stdint.h>  
#include <TouchScreen.h>  // Library for the Seeed Studio TFT Touch Shield 
#include <TFT.h>      // Library for the Seeed Studio TFT Touch Shield 
#include <Password.h> // http://www.arduino.cc/playground/uploads/Code/Password.zip
#include <TouchScreenGeometry.h>  // Library for drawing shapes for the keypad
#include <TouchScreenStrings.h> // Library for drawing strings for the keypad

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
  Tft.init();             // Initializes the TFT library
  titleScreen();
}

void loop() 
{
  // Intentionally left blank
}

void titleScreen()
{
  Tft.drawString("SLIDING PUZZLE GAME",4,10,1,YELLOW);
  Tft.drawString("by GARRY SPENCER",23,35,1,WHITE);
  Tft.drawString("& RICHARD KIRKPATRICK", 23, 50, 1, WHITE);
  Tft.drawString("ARRANGE ALL OF THE",11,80,1,CYAN);
  Tft.drawString("BLOCKS IN CORRECT",17,100,1,CYAN);
  Tft.drawString("NUMERICAL ORDER",27,120,1,CYAN);
  Tft.drawString("TOUCH THE SCREEN",20,160,1,CYAN);
  Tft.drawString("TO BEGIN PLAYING",20,180,1,CYAN);
  Tft.drawString("THE PUZZLE GAME",25,200,1,CYAN);

  //delay(1000);
  gameScreen();
}

void gameScreen()
{
  eraseScreen();
  Tft.fillRectangle(1,1,237,25,RED);
  Tft.drawString("PUZZLE GAME",30,6,2,YELLOW);
  Tft.fillRectangle(1,26,237,237,WHITE);
  Tft.drawLine(1,26,237,26,RED);
  Tft.drawLine(1,85,237,85,RED);
  Tft.drawLine(1,144,237,144,RED);
  Tft.drawLine(1,203,237,203,RED);
  Tft.drawLine(1,262,237,262,RED);
  Tft.drawLine(1,26,1,262,RED);
  Tft.drawLine(60,26,60,262,RED);
  Tft.drawLine(119,26,119,262,RED);
  Tft.drawLine(178,26,178,262,RED);
  Tft.drawLine(237,26,237,262,RED);
  Tft.fillRectangle(1,263,237,57,BLUE);
  Tft.fillRectangle(40,295,160,22,CYAN);
  Tft.drawString("NEW GAME",55,300,2,BLACK);
}

boolean screenTouched(Point p)
{
  return (p.z > ts.pressureThreshhold);
}

void eraseScreen()
{
  Tft.fillRectangle(0, 0, 240, 320, BLACK);
}

