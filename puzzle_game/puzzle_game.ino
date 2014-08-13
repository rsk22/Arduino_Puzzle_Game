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

// Global instances
Button tiles[16]; 
TouchScreenString tileText[16];

void setup() 
{
  Tft.init();             // Initializes the TFT library
  Serial.begin(9600);     // Used for debugging
  randomSeed(analogRead(0)); // Used for shuffling the tiles 
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
  // Clear the screen 
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
  // Reset the tile text for each game
  char* buttonText[16] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "0"}; 

  // Display "START!!!"
  Tft.drawString("START!!!", 55, 270, 2, WHITE);
  
  // Shuffle the tiles and then draw them
  shuffleTiles(buttonText, 16);
  drawTiles(buttonText);
  
  // Loop until game has been won or player selects new game
  boolean gameWon = false;
  while (!gameWon) {
    // A point objects holds x, y, and z coordinates
    Point p = ts.getPoint(); 
    p.x = map(p.x, TS_MINX, TS_MAXX, 240, 0);
    p.y = map(p.y, TS_MINY,TS_MAXY, 320, 0);
    // Resets tile number and tile position number for each loop
    int tileNumber = -1; 
    int tilePosNumber = -1; 
    // If user presses the screen 
    if (p.z > ts.pressureThreshhold) { 
      // Get the selected tile
      tileNumber = getTileNumber(p.x, p.y);
      // Get the selected tile's position number
      tilePosNumber = getTilePosNumber(p.x, p.y);
    }
    if (canTileMove(tilePosNumber) && tilePosNumber != -1) {
      // Swap the tiles  
      swapTiles(tilePosNumber);
    }
    // Check to see if the player has selected the New Game button
    if (isNewGamePressed()) 
      displayGameScreen();
    // Check to see if the player has won
    gameWon = areTilesInOrder(buttonText);
    delay(100);
  }
  // Check to see if the game was won.  If so, print "YOU WIN!"
  if (gameWon) 
    playerWins();
  // Restart the game
  displayGameScreen();
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
    if ((p.x > 40 && p.x < 200) && (p.y > 295 && p.y < 317)) {
      Tft.drawString("NEW GAME",55,300,2,RED);
      delay(100);
      Tft.drawString("NEW GAME",55,300,2,BLACK);
      return true;
    } else {
      return false;
    }
  }
  return false;
}

// Returns true if the screen is pressed; false otherwise
boolean isScreenPressed()
{
  // A point objects holds x, y, and z coordinates
  Point p = ts.getPoint(); 
  return (p.z > ts.pressureThreshhold);
}

// Utility function to swap char*
void swap(char **a, char **b)
{
  char* temp = *a;
  *a = *b;
  *b = temp;
}

// Shuffles the tiles
void shuffleTiles(char** tiles, int n)
{
  // Start from the last element and swap one by one.  
  for (int i = n - 1; i > 0; i--) {
    // Pick a random index from 0 to i
    int j = random() % (i+1);
    // Swaw the elements
    swap(&tiles[i], &tiles[j]);
  }
}


// Returns true if the tiles are in order; false otherwise
boolean areTilesInOrder(char** tiles)
{
  // Go through each tile and determine if it is in order
  for (int i = 0; i < 15; i++) {
    int temp = String(tiles[i]).toInt();
    Serial.println("Tile: ");
    Serial.println(temp);
    if (temp != i + 1)
        return false;
  }
  // Make sure last tile is blank
  int temp = String(tiles[15]).toInt();
  Serial.println("Tile: ");
  Serial.println(temp);
  if (temp == 0)
    return true;
  else 
    return false;
}

// Draws the tiles
void drawTiles(char** tiles)
{
  int k = 0;
  for (int j = 0; j < 4; j++) {
    for (int i = 0; i < 4; i++) {
      int temp = String(tiles[k]).toInt();
      if (temp != 0) {
        tileText[k].setValues(tiles[k], xButtonText[i], yButtonText[j], 1, BLACK);
        tileText[k].drawText();
      }
      else {
        tileText[k].setValues("", xButtonText[i], yButtonText[j], 1, BLACK);
        tileText[k].drawText();
      }
      k++;
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

// Returns the tile's position number
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

// Returns true if the tile can move; false otherwise
boolean canTileMove(int tilePosNumber)
{  
  // If tile number is zero return false
  if (tilePosNumber == getBlankTilePosNumber())
    return false;  
  
  // legalTileShifts[][] is used to determine if the tile can move.  -1 indicates it cannot move
  int legalTileShifts[16][4] = {
                               {1, 4, -1, -1}, // Tile #0
                               {0, 2, 5, -1}, // Tile #1
                               {1, 3, 6, -1}, // Tile #2
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
    int tileNumber = String(tileText[legalTileShifts[tilePosNumber][i]].getText()).toInt();
    if (tileNumber == 0 && legalTileShifts[tilePosNumber][i] != -1)
      return true;  // Return true if the adjacent tile is zero  
  }
  return false; // Else return false                         
}

// Get the blank tile's position number
int getBlankTilePosNumber()
{
  for (int i = 0; i < 16; i++) {
    int tileNumber = String((tileText[i].getText())).toInt(); 
    if (tileNumber == 0)
      return i;
  }
}

// Swaps the specified tile with the blank tile
void swapTiles(int positionNumber)
{
  // Determine blank tile's position number
  const int blankTilePosNum = getBlankTilePosNumber();
  
  // Erase both of the tiles
  tileText[positionNumber].setTextColor(WHITE);
  tileText[blankTilePosNum].setTextColor(WHITE);
  tileText[positionNumber].drawText();
  tileText[blankTilePosNum].drawText();
  
  // Set both tile's text color back to black
  tileText[positionNumber].setTextColor(BLACK);
  tileText[blankTilePosNum].setTextColor(BLACK);
  
  // Get the text from the selected
  char *tempText = tileText[positionNumber].getText();
  
  // Set the blank tile to swap tile's text and draw it
  tileText[blankTilePosNum].setText(tempText);
  tileText[blankTilePosNum].drawText();
  
  // Update the swap tile's text and draw it
  tileText[positionNumber].setText("");
  tileText[positionNumber].drawText();
}

// Displays "YOU WIN!!!" 
void playerWins()
{
  Tft.drawString("START!!!", 55, 270, 2, BLUE); 
  Tft.drawString("YOU WIN!", 55, 270, 2, WHITE);
  delay(3000);
}


