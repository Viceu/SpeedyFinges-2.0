// requires Adafruit_LEDBackpack library and the Adafruit_GFX library
// https://github.com/adafruit/Adafruit-LED-Backpack-Library
// https://github.com/adafruit/Adafruit-GFX-Library

#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include <Adafruit_GFX.h>

Adafruit_8x16matrix matrix = Adafruit_8x16matrix();

byte arbitraryCode = 97;
byte arbitraryCode2 = 67;
const int touchPin = 3;
const int button = 2;
volatile int buttonState = 0;         // variable for reading the pushbutton status
volatile int touchState = 0;
int speed = 555/8;
int pause;

int count;
int score1 = 0;
int score2 = 0;
int activeCol = 7;      // index of currently active column
int activeRow1;
int activeRow2;
float musicMap[56] ={1.5,1,1,0.5,
                   0.5,0.5,0.5,1.5,0.5,0.5,
                   0.5,0.5,1,1.5,0.5,
                   0.5,0.5,0.5,2.5,
                   1.5,2,0.5,
                   0.5,0.5,0.5,2,0.5,
                   0.5,0.5,0.5,1,1,1,
                   1,1,1.5,
                   2,1.5,2.5,
                   0.5,0.5,0.5,0.5,
                   1,1,4,
                   0.5,0.5,0.5,0.5,
                   6,0.5,0.5,0.5,0.5,
                   6
                   };

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

byte smile_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10100101,
    B10011001,
    B01000010,
    B00111100 };
byte  neutral_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10111101,
    B10000001,
    B01000010,
    B00111100 };
byte  frown_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10011001,
    B10100101,
    B01000010,
    B00111100 };


void startGame(){
    count = 0;
    speed = 555/8 * musicMap[count]; // reset speed block drops
    activeRow1 = 0;
    activeRow2 = 0;
    activeCol = 7;
    score1 = 0;  // reset level to 0
    score2 = 0;
    matrix.drawBitmap(0, 0, theGrid, 8, 16, LED_ON);

    void setNext();                 // kick off the first drop
    
}


// check if button clicked is valid -- correct answer (not early, not late)
// later: if return true, add to score
// no need check for missed blocks since missing does not lose score
boolean checkValid(int currCol) {
    if (currCol == 7) {  // hit bottom
        return true;
    }
    else {  // current block not at bottom or column, either too early or too late (next block dropped)
        return false;
    }
}


// set new column drop from top of matrix
void setNext(){
    count++;
    speed = 555/8 * musicMap[count];
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

    Serial.write(arbitraryCode2);

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
    Serial.write(arbitraryCode);
    matrix.begin(0x70);             // initialize the top matrix
    pinMode(button, INPUT);
    pinMode(touchPin, INPUT);
    attachInterrupt(2, pin_ISR, RISING);
    attachInterrupt(3, pin3_ISR, RISING);

    delay(1535);
    startGame();
}


void loop() {
    while (count < 56) {

        matrix.clear();            // clear the top matrix
        matrix.drawBitmap(0, 0, theGrid, 8, 16, LED_ON);     // draw the fixed pieces in their locations
        
        if(activeCol > 0){  
            activeCol--;
        }
        else{
            setNext();
        }
        drawActiveShape();            // draw the active piece

        matrix.writeDisplay();     // update the LEDs with the new info
        
        delay(speed);                   


        // at top of loop: if music ends, display scores
        // displayScores();
        // if (score >= count/2) {
        //     matrix.drawBitmap(0, 0, win, 8, 16, LED_ON);
        // }
        // else {
        //     matrix.drawBitmap(0, 0, lose, 8, 16, LED_ON);
        // }
    }

    matrix.clear();
    if(score1 > score2){
      matrix.drawBitmap(0, 0, smile_bmp, 8, 8, LED_ON); 
      matrix.drawBitmap(0, 8, frown_bmp, 8, 8, LED_ON);
    }
    else if(score1<score2){
      matrix.drawBitmap(0, 0, frown_bmp, 8, 8, LED_ON); 
      matrix.drawBitmap(0, 8, smile_bmp, 8, 8, LED_ON);
    }
    else{
      matrix.drawBitmap(0, 0, neutral_bmp, 8, 8, LED_ON); 
      matrix.drawBitmap(0, 8, neutral_bmp, 8, 8, LED_ON); 
    }
    matrix.writeDisplay();
    delay(2000);
    displayScores();
    delay(1000);
    exit(0);

}


void pin_ISR() {
    buttonState = digitalRead(button);
    int sensorValue = digitalRead(button);
    if (activeCol == 0) {
        score1++;
    }
}


void pin3_ISR() {
    touchState = digitalRead(touchPin);
    int sensorValue = digitalRead(touchPin);
    if (sensorValue == 1 && activeCol == 0) {
        score2++;
    }
}