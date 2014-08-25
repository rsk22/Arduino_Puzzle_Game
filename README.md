Puzzle Game for the Arduino and Seeed Studio TFT Touch Screen
===================

Original Author: Garry Spencer

Modified by: Richard Kirkpatrick

Player must arrange all the blocks in the correct order to win the game.
Order of tiles must be:
  
  1 2 3 4
  
  5 6 7 8
  
  9 10 11 12
  
  13 14 15 "blank"

Original game idea came from an article from "Nuts and Volts", May 2014 by Garry Paul Spencer.
Game concept was modified to be more readable and make use of my personal libraries. 
  
To run the game, you will need to install the following libraries located on my Github (rsk22) page:

1. Arduino TouchScreenGeometry
2. Arduino TouchScreenStrings
3. Arduino TouchScreenButtons

You will need the following hardware to run the game:

1. Seeed Studio TFT Touch Screen (Version 1).  Version 2 will work but you will need to modify 
the code (upgrade in near future).
2. Arduino microcontroller compatabile with Seeed Studio TFT Touch Screen (Arduino Uno or Arduino Mega)
  
Future Upgrades:
  - Abstract out functions to library
  - Add an automatic puzzle solver

