// Nathan's tetris ideas basics

// Also required are the Adafruit_LEDBackpack library and the Adafruit_GFX library
// https://github.com/adafruit/Adafruit-LED-Backpack-Library
// https://github.com/adafruit/Adafruit-GFX-Library

#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include <Adafruit_GFX.h>

Adafruit_8x16matrix matrix = Adafruit_8x16matrix();

const int TouchPin = 8;
const int button = 2;
volatile int buttonState = 0;         // variable for reading the pushbutton status
int speed = random(70, 500);
int pause;

int count;
int level;
int score1 = 0;
int score2 = 0;
int activeCol = 7;      // index of currently active column
int activeRow1;
int activeRow2;

// The fixed grid pieces
byte theGrid[] = {   
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000
};

byte lose[] = {
    B10000001,
    B01000010,
    B00100100,
    B00011000,
    B00011000,
    B00100100,
    B01000010,
    B10000001,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000
};

byte win[] = {   
    B00000000,
    B00000000,
    B00100100,
    B00000000,
    B01000010,
    B00111100,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000
};


void startGame(){
    speed = random(70, 500); // reset speed block drops
    activeRow1 = 0;
    activeRow2 = 0;
    activeCol = 7;
    level = 1;  // reset level to 1
    score1 = 0;  // reset level to 0
    score2 = 0;
    matrix.drawBitmap(0, 0, theGrid, 8, 16, LED_ON);

    void setNext();                 // kick off the first drop
    
}

// check if button clicked is valid -- correct answer (not early, not late)
// *** later: if return true, add to score
boolean checkValid(int currCol) {
    if (currCol == 7) {  // hit bottom
        return true;
    }
    else {  // current block not at bottom or column, either too early or too late (next block dropped)
        return false;
    }
}

// no need check for missed blocks since missing does not lose score


// set new column drop from top of matrix
void setNext(){
    count++;
    speed = random(70, 500);
    activeRow1 = random(7);     // pick a random col
    activeRow2 = random(7);
    activeCol = 7;
}



void displayScores(){
    matrix.clear();
    matrix.writeDisplay();
    matrix.setTextSize(1);
    matrix.setTextWrap(false);  // we don't want text to wrap, so it scrolls nicely
    //  matrix.setTextColor(LED_ON);
    
    String pointString1=String(score1); // convert the score integer to a string
    String scoreOne=String("Points: " + pointString1);  // build a string containing the word "points" + the point value
    int scoreOneLength = 44 + (7 * pointString1.length()); // estimating the pixel width of the string
    
    String pointString2=String(score2); // convert the score integer to a string
    String scoreTwo=String("Points: " + pointString2);  // build a string containing the word "points" + the point value
    int scoreTwoLength = 44 + (7 * pointString2.length()); // estimating the pixel width of the string
    
    for (int8_t x=8; x>= -(scoreOneLength); x--) {       // start offscreen and scroll the level from right to left
        matrix.clear();
        matrix.setCursor(x,0);
        matrix.print(scoreOne);
        matrix.writeDisplay();
        matrix.setCursor(x,8);
        matrix.print(scoreTwo);
        matrix.writeDisplay();

        delay(100);
    }

}


void drawActiveShape(){
    matrix.drawPixel(activeCol, activeRow1, LED_ON);
    matrix.drawPixel(activeCol, 8 + activeRow2, LED_ON);
}


void setup() {
    Serial.begin(9600); 
 
    matrix.begin(0x70);             // initialize the top matrix
   
    pinMode(button, INPUT);
    attachInterrupt(2, pin_ISR, RISING);

    startGame();
}


void loop() {
    while (score1 < 5 && score2 < 5) {

        matrix.clear();            // clear the top matrix
        matrix.drawBitmap(0, 0, theGrid, 8, 16, LED_ON);     // draw the fixed pieces in their locations
        
        if(activeCol > 0){  
            activeCol--;
            pause = speed;          // wait pause ms before change to next row
        }
        else{
            setNext();
            pause = 100;             // wait 30ms before next drop
        }
        drawActiveShape();            // draw the active piece

        matrix.writeDisplay();     // update the LEDs with the new info
        
        delay(pause);                   


        // at top of loop: if music ends, display scores
        // displayScores();
        // if (score >= count/2) {
        //     matrix.drawBitmap(0, 0, win, 8, 16, LED_ON);
        // }
        // else {
        //     matrix.drawBitmap(0, 0, lose, 8, 16, LED_ON);
        // }
    }
    displayScores();

}


void pin_ISR() {
    buttonState = digitalRead(button);
    int sensorValue = digitalRead(button);
    if (sensorValue == 1 && activeCol == 0) {
        Serial.println("score");
        score1++;
    }
}