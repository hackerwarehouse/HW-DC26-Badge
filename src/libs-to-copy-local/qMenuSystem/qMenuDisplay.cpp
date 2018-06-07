#include "qMenuDisplay.h"

//DigoleSerialDisp _disp(8,9,10);
//const byte OLED_RESET               = 21;
//Adafruit_SSD1306 _disp(OLED_RESET); 

qMenuDisplay::qMenuDisplay()
{
}

qMenuDisplay::qMenuDisplay(SSD_13XX *disp)
{
  _disp=disp;

}
void qMenuDisplay::Begin()
{
  _disp->begin();
  _disp->setTextColor(WHITE);
  _disp->setTextScale(1);
}
void qMenuDisplay::Start()
{
  _disp->clearScreen();
}

void qMenuDisplay::Title(const char text[])
{
  _disp->setCursor(0, 11-textHeight/2);
  _disp->setTextColor(CYAN);
  _disp->print(text);
  _disp->drawFastHLine(0,14,96, ORANGE);
}

void qMenuDisplay::Item(int index,const char text[])
{
  _disp->setCursor(5,(index*14)+32-textHeight/2);
  _disp->setTextColor(GREENYELLOW);
  _disp->print(text);
}

void qMenuDisplay::Highlight(int index)
{
/*
_disp->setCursor(0, (index*14)+32-textHeight/2);
_disp->setTextColor(GREEN);
_disp->print("*");  
*/

_disp->drawRect(0,(index*14)+22,95,13,WHITE);

}

void qMenuDisplay::MessageBox(const char text[])
{
  int textWidth=strlen(text)*9;
  int x=64-(textWidth/2);
  int y=32;
  _disp->clearScreen();/////

//for (int i = 0; i < 21+3; i++) _disp->drawFastHLine(x-5,y-12+i,textWidth+9, BLACK);

	/*_disp->drawFastHLine(x-3,y-10,textWidth+5, WHITE);
	_disp->drawFastHLine(x-3,y-10+17+3,textWidth+5, WHITE);
	_disp->drawFastVLine(x-3,y-10,17+3, WHITE);
	_disp->drawFastVLine(x-3+textWidth+5,y-10,17+3, WHITE);*/

  _disp->setCursor(x+10,y+5-textHeight/2);
  _disp->print(text);
}


