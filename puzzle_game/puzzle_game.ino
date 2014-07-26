/*
  Sliding Puzzle Game 
  Original Author: Garry Spencer
  Modified by: Richard Kirkpatrick
  Player must arrange all the blocks in the correct order to win the game.
  
  Future Upgrades:
    - Abstract out functions to library
*/

// Libraries
#include <stdint.h>  
#include <TouchScreen.h>  // Library for the Seeed Studio TFT Touch Shield 
#include <TFT.h>      // Library for the Seeed Studio TFT Touch Shield 
#include <Password.h> // http://www.arduino.cc/playground/uploads/Code/Password.zip
#include <TouchScreenGeometry.h>  // Library for drawing shapes for the touch screen
#include <TouchScreenStrings.h> // Library for drawing strings for the touch screen
#include <TouchScreenButtons.h> // Library for drawing buttons for the touch screen

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

// Pre-processor constants
#define heightButton 59
#define widthButton 59
#define noColumns 4
#define noRows 4
#define numButtons 16

// Global constants
const int xminButton[] = {1, 60, 119, 178};  // x-min for buttons
const int xmaxButton[] = {60, 119, 178, 237}; // x-max for buttons
const int yminButton[] = {26, 85, 144, 203}; // y-min for buttons
const int ymaxButton[] = {85, 144, 203, 262}; // y-max for buttons
const int xButtonText[] = {25, 85, 145, 195}; // x-coordinates for the buttons' text
const int yButtonText[] = {45, 105, 165, 225}; // y-coordinates for the buttons' text
char* buttonText[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "0"};

Button tiles[16]; 
TouchScreenString tileText[16];

void setup() 
{
  Tft.init();             // Initializes the TFT library
  Serial.begin(9600);
  titleScreen();
}

void loop() 
{
  // Intentionally left blank
}

// Displays the title screen
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

  while (!isScreenPressed()) {
    // Loops until the screen is pressed
  }
  displayGameScreen();
}

// Displays the Puzzle Game screen
void displayGameScreen()
{
  clearScreen();   
 
  // Draws some of the initial welcome screen 
  Tft.fillRectangle(1,1,237,25,RED);
  Tft.drawString("PUZZLE GAME",30,6,2,YELLOW);
  Tft.fillRectangle(1,26,237,237,WHITE);
  Tft.fillRectangle(1,263,237,57,BLUE);
  Tft.fillRectangle(40,295,160,22,CYAN);
  Tft.drawString("NEW GAME",55,300,2,BLACK);
  
  // Sets the values for the tiles and then draws them
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      tiles[4 * j + i].setValues(xminButton[i], yminButton[j], heightButton, widthButton);
      tiles[4 * j + i].setBorderColor(RED);
      tiles[4 * j + i].setFillColor(WHITE);
      tiles[4 * j + i].draw();
      tiles[4 * j + i].fill();
    }
  }
    
  while (!isNewGamePressed()) {
    // Loops until the New Game button is pressed
  }
  
  newGame();
}


void newGame()
{
  // Display "START!!!"
  Tft.drawString("START!!!", 55, 270, 2, WHITE);
  
  // Generate a random set of tiles
  // Generate a random set of 4 x 4 tiles of numbers 0 - 15.  0 representats a blank tile
  int **tiles = generateRandomTiles();
  
  // Set the tiles text and then draws the text
  drawTiles(tiles);

  boolean newGameSelected = false; // Used for determining if the user pressed the New Game button
  boolean gameWon = areTilesInOrder(tiles);
  
  while (!gameWon and !newGameSelected) {
    // A point objects holds x, y, and z coordinates
    Point p = ts.getPoint(); 
    p.x = map(p.x, TS_MINX, TS_MAXX, 240, 0);
    p.y = map(p.y, TS_MINY,TS_MAXY, 320, 0);
    int tileNumber = -1; // Resets tile number for each loop
    int tilePosNumber = -1; // Resets the tile' position number for each loop
    // If user presses the screen 
    if (p.z > ts.pressureThreshhold) { 
      // Get the selected tile
      tileNumber = getTileNumber(p.x, p.y);
      // Get the selected tile's position number
      if (tileNumber != -1) {
        Serial.print("Tile # = ");
        Serial.println(tileNumber);
      }
      tilePosNumber = getTilePosNumber(p.x, p.y);
      if (tilePosNumber != -1) {
        Serial.print("Tile Position Number = ");
        Serial.println(tilePosNumber); 
      }
      // Swap the selected tile with the blank thile
    }
    // Check to see if the player has selected the New Game button
    //newGameSelected = isNewGamePressed();
    // Check to see if the selected tile can be moved
    if (canTileMove(tileNumber, tiles)) {
      Serial.println("Tile can move");
    }
      // tiles[tileNumber].moveTile(tiles);
      // tiles = updateTiles(tileNumber, tiles);
    // playerWins = areTilesInOrder(tiles);
    // newGameSelected = isNewGamePressed();
    delay(100);
  }
}


// Clears the entire screen
void clearScreen()
{
  Tft.fillRectangle(0, 0, 240, 320, BLACK);
}

// Returns true if the New Game button is pressed; false otherwise
boolean isNewGamePressed()
{
  // A point objects holds x, y, and z coordinates
  Point p = ts.getPoint(); 
  p.x = map(p.x, TS_MINX, TS_MAXX, 240, 0);
  p.y = map(p.y, TS_MINY,TS_MAXY, 320, 0);
  
  if (p.z > ts.pressureThreshhold) {
    Tft.drawString("NEW GAME",55,300,2,RED);
    delay(100);
    Tft.drawString("NEW GAME",55,300,2,BLACK);
    return ((p.x > 40 && p.x < 200) && (p.y > 295 && p.y < 317));
  } else {
    return false;
  }
}

// Returns true if the screen is pressed; false otherwise
boolean isScreenPressed()
{
  // A point objects holds x, y, and z coordinates
  Point p = ts.getPoint(); 
  p.x = map(p.x, TS_MINX, TS_MAXX, 240, 0);
  p.y = map(p.y, TS_MINY,TS_MAXY, 320, 0);
  
  return (p.z > ts.pressureThreshhold);
}

// Returns a randomly shuffled 4 x 4 array of the numbers 0 - 15, where 0
// represents a blank tile, and the remaining numbers represents numbered tiles.
int **generateRandomTiles()
{
  // Provide a random seed for the button selection
  randomSeed(analogRead(0));  
  
  // Generate the tile numbers 0 - 15
  int **tiles = new int*[4];  
  int k = 0;
  for (int i = 0; i < 4; i++) {
    tiles[i] = new int[4];    
    for (int j = 0; j < 4; j++) {
      tiles[i][j] = k; 
      k++;
    }
  }
  
  // Shuffle the tiles
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      int randomI = random(0, 4);
      int randomJ = random(0, 4);
      int temp = tiles[i][j];
      tiles[i][j] = tiles[randomI][randomJ];
      tiles[randomI][randomJ] = temp;
    }
  }
  return tiles;
}

void freeTiles(int **tiles)
{
  for (int i = 0; i < 16; i++) {
      delete [] tiles[i];
  }
  delete [] tiles;
}

// Returns true if the tiles are in order; false otherwise
boolean areTilesInOrder(int **tiles)
{
  int j = 0;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (tiles[i][j] != j)
        return false;
      j++;
    }
  }
  return true;
}

// Draws the tiles
void drawTiles(int **tiles)
{
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      tileText[4 * j + i].setValues(buttonText[tiles[i][j]], xButtonText[i], yButtonText[j], 1, BLACK);
      tileText[4 * j + i].drawText();
    }
  }
}

// Returns the tile number that was pressed
int getTileNumber(int xInput, int yInput)
{
  for (int i = 0; i < 16; i++) {
    if ((xInput > tileText[i].getXStart() - 30 && xInput < tileText[i].getXStart() + 30)
       && (yInput > tileText[i].getYStart() - 30 && yInput < tileText[i].getYStart() + 30)) {
       int tileNumber = String((tileText[i].getText())).toInt();
       return tileNumber ; // Returns the tile number
    }
  }
  return -1; // Return -1 if no tile was pressed
}

// rReturns the tile's position number
int getTilePosNumber(int xInput, int yInput)
{
  for (int i = 0; i < 16; i++) {
    if ((xInput > tileText[i].getXStart() - 30 && xInput < tileText[i].getXStart() + 30)
       && (yInput > tileText[i].getYStart() - 30 && yInput < tileText[i].getYStart() + 30)) {
         int posNumber = i;
         return i;
       }
  }
  return -1; // Return -1 if no tile was pressed
}

boolean canTileMove(int tileNumber, int **tiles)
{  
  // legalTileShifts[][] is used to determine if the tile can move
  int legalTileShifts[16][4] = {
                               {1, 4, -1, -1}, // Tile #0
                               {0, 2, 6, -1}, // Tile #1
                               {1, 3, 7, -1}, // Tile #2
                               {2, 7, -1, -1}, // Tile #3
                               {0, 5, 8, -1}, // Tile #4
                               {1, 4, 6, 9}, // Tile #5
                               {2, 5, 7, 10}, // Tile #6
                               {3, 6, 11, -1}, // Tile #7
                               {4, 9, 12, -1}, // Tile #8
                               {5, 8, 10, 13}, // Tile #9
                               {6, 9, 11, 14}, // Tile #10
                               {7, 10, 15, -1}, // Tile #11
                               {8, 13, -1, -1}, // Tile #12
                               {9, 12, 14, -1}, // Tile #13
                               {10, 13, 15, -1}, // Tile #14
                               {11, 14, -1, -1}, // Tile #15
                               };
  
  // Using legalTileShifts, determine if the tile can move.  One of the adjacent tiles must contain zero (blank tile).
  for (int i = 0; i < 4; i++) {
    if (tiles[legalTileShifts[tileNumber][i]] == 0)
      return true;  // Return true if the adjacent tile is zero  
  }
  return false; // Else return false                         
}

void swapTiles(int tileSelected)
{
  // Get the selected tile's information
  
}



