//#include <EEPROM.h>

#include <SSD_13XX.h>

extern SSD_13XX mydisp;
extern byte appmode;
extern byte btncounter;
extern volatile byte btnid;
extern byte region_unlocked;

#include "core.h"
#include "textinput.h"

#define QUEUE_SIZE 8

void ButtonEcho(){
  appmode=1;
  btnid = 0;
  byte chars=1;
  String a = "";

  mydisp.clearScreen();
  mydisp.setCursor(0, 0);
  mydisp.setTextColor(GREENYELLOW);
  mydisp.setTextScale(1);
  
  while (1)
  {
    if (btnid == 0){}
    else
    {
      if (btnid == 3){ 
        mydisp.print(F("U"));
        a += 'U';
        chars++;
        btnid = 0;
      }
      else if (btnid == 2){ 
        mydisp.print(F("D"));
        a += 'D';
        chars++;
        btnid = 0;
      }
      else if (btnid == 1){
        mydisp.print(F("R"));
        a += 'R';
        chars++;
        btnid = 0;
      }
      else if (btnid == 4) {
        mydisp.print(F("L"));
        a += 'L';
        chars++;
        btnid = 0;
      }
      if (a.length() == QUEUE_SIZE)
      {
        String b = "";
        
        for (int i = 0; i < (sizeof(UNLOCK_KONAMI)/sizeof(UNLOCK_KONAMI[0]) - 1); i++) { b += char(pgm_read_byte_near(UNLOCK_KONAMI + i)); }
        if (a == b) { 
          mydisp.print(F("konami!")); 
          chars+=7;
        };
        b = "";

        for (int i = 0; i < (sizeof(UNLOCK_REGION)/sizeof(UNLOCK_REGION[0]) - 1); i++) { b += char(pgm_read_byte_near(UNLOCK_REGION + i)); }
        if (a == b) { 
          mydisp.print(F("region unlocked")); 
          region_unlocked = 1;
          delay(2000);
          break;
        };
        b = "";

        for (int i = 0; i < (sizeof(UNLOCK_REGIONPERM)/sizeof(UNLOCK_REGIONPERM[0]) - 1); i++) { b += char(pgm_read_byte_near(UNLOCK_REGIONPERM + i)); }
        if (a == b) { 
          mydisp.print(F("region perm unlocked"));
//          EEPROM.write(REGION_UNLOCK_ADDR,1);
//          EEPROM.commit(); 
          region_unlocked = 1;
          delay(2000);
          break;
        };
        b = "";
        
        a="";
      }
      if (chars >= 136) { mydisp.clearScreen(); chars=0;}
    }
    delay(100);
  }
  appmode=0;
  btnid = 4;
  btncounter++;
}

void KeyboardDemo(){
  appmode=1;
  btnid = 0;
  mydisp.clearScreen();
  mydisp.setCursor(0, 0);
  mydisp.setTextColor(GREENYELLOW);
  mydisp.setTextScale(1);
  String t = "";
  TextInput(F("Input text via d-pad: "), t, 8);
  if (t) { mydisp.print(t); }
  delay(2000);
  appmode=0;
  btnid = 4;
  btncounter++;
}

///////////////////////////////////////

#include <Adafruit_GFX.h>

const int D_NORTH = 1;
const int D_EAST = 2;
const int D_SOUTH = 3;
const int D_WEST = 4;

long score = 0;

byte snakePixelX[20];
byte snakePixelY[20];

int snakeX = 10;
int snakeY = 30;
int snakeLength = 1;
volatile int snakeDir = D_NORTH;

int minX = 0;
int minY = 20;
int maxX = MAX_X;
int maxY = MAX_Y;

int foodX = 0;
int foodY = 0;

bool outOfBounds() {
  return snakeX <= minX || snakeY <= minY || snakeX >= maxX || snakeY >= maxY;
}

void renderScore() {
  mydisp.fillRect(0, 0, mydisp.width()-1, 8, BLACK);
  
  mydisp.setTextScale(1);
  mydisp.setTextColor(WHITE);
  mydisp.setCursor(0,0);

  mydisp.print("Score: ");
  mydisp.print(String(score, DEC));

  // TOP
  mydisp.drawLine(0, 8, mydisp.width()-1, 8, WHITE);
  // LEFT
  mydisp.drawLine(0, 8, 0, mydisp.height()-1, WHITE);
  // RIGHT
  mydisp.drawLine(mydisp.width()-1, 8, mydisp.width()-1, mydisp.height()-1, WHITE);
  // BOTTOM
  mydisp.drawLine(0, mydisp.height()-1, mydisp.width()-1, mydisp.height()-1, WHITE);
}

void gameOver() {
  mydisp.clearScreen();
  mydisp.setCursor(5, 5);
  mydisp.print("GAME OVER :(");

  score = 0;
  snakeLength = 1;
  snakeX = mydisp.width() / 2;
  snakeY = mydisp.height() / 2;

  snakePixelX[snakeLength-1] = snakeX;
  snakePixelY[snakeLength-1] = snakeY;
  
  snakeDir = D_SOUTH;
  
  delay(2000);
  
  mydisp.clearScreen();
  renderScore();
}

// Drop food on random location
void dropFood() {
  foodX = random(minX+1, maxX-1);
  foodY = random(minY+1, maxY-1);
}

bool collectFood() {
  if (snakeX == foodX && snakeY == foodY) {
    score += 10;
    
    renderScore();
    dropFood();

    return true;
  } else {
    return false;
  }
}

bool crashedIntoSelf() {
  for(byte i = 4; i < snakeLength; i++) {
      if (snakeX == snakePixelX[i] && snakeY == snakePixelY[i]) {
        return true;
      }
  }

  return false;
}

void drawScreen() {
  bool foodCollected = false;
  
  mydisp.clearScreen();

  mydisp.drawPixel(foodX, foodY, WHITE);
  foodCollected  = collectFood();

  // Check snake position
  if (outOfBounds() || crashedIntoSelf()) {
    gameOver();
  }

  // Render the snake
  for(int i = 0; i < snakeLength; i++) {
    mydisp.drawPixel(snakePixelX[i], snakePixelY[i], WHITE);
  }
  
  // Move pixel values
  for(int i = snakeLength; i > 0; i--) {
    snakePixelX[i] = snakePixelX[i-1];
    snakePixelY[i] = snakePixelY[i-1];
  }

  if (foodCollected) {
    snakeLength += 1;
    snakePixelX[snakeLength-1] = snakeX;
    snakePixelY[snakeLength-1] = snakeY;
  }

  switch(snakeDir) {
    case D_NORTH:
      snakeY -= 1;
      break;
    case D_EAST:
      snakeX += 1;
      break;
    case D_SOUTH:
      snakeY += 1;
      break;
    case D_WEST:
      snakeX -= 1;
      break;
  }

  snakePixelX[0] = snakeX;
  snakePixelY[0] = snakeY;

  renderScore();
}

void snakeGame()
{
  appmode=1;
  btnid = 0;
  mydisp.clearScreen();

  minX = 0;
  minY = 8;
  maxX = mydisp.width()-1;
  maxY = mydisp.height()-1;

  dropFood();
  
  while (1){
    if (btnid == 1) {snakeDir = D_EAST;}
    else if (btnid == 2) {snakeDir = D_SOUTH;}
    else if (btnid == 3) {snakeDir = D_NORTH;}
    else if (btnid == 4) {snakeDir = D_WEST; }
    
    drawScreen();
   
    delay(45); //10 is too fast for snake. 300 is dirt slow. 100 is still slowish but decent
  }
  appmode=0;
  btncounter++;
}
