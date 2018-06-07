#include <SSD_13XX.h>

#include "_images/hw_sm.c"
#include "_images/js_sm.c"
#include "core.h"

extern SSD_13XX mydisp;
extern volatile byte btnid;
extern byte appmode;
extern byte btncounter;

void SysInfo()
{
  mydisp.clearScreen();
  mydisp.setCursor(0, 0);
  mydisp.setTextColor(GREENYELLOW);
  mydisp.setTextScale(1);
  
  mydisp.println(F("HW-DC26 Badge"));
  mydisp.println(F("  ver 8"));
  mydisp.println(F("  rel date 2018-06-03"));
  mydisp.println(F("  src on github.com"));
  mydisp.println();
  mydisp.print(F("SDK ver: ")); mydisp.println(ESP.getSdkVersion());
}

void HWInfo()
{
  mydisp.clearScreen();
  mydisp.setCursor(0, 0);
  mydisp.setTextColor(GREENYELLOW);
  mydisp.setTextScale(1);

  mydisp.print(F("CPU: "));
  mydisp.print(ESP.getCpuFreqMHz());
  mydisp.println(F(" MHz"));
  
  mydisp.println(F("Flash Chip Size: "));
  mydisp.println(ESP.getFlashChipSize());

  mydisp.print(F("Free RAM: "));
  mydisp.println(ESP.getFreeHeap());
}

void Credits()
{
  mydisp.clearScreen();
  mydisp.setRotation(0);
  mydisp.setTextColor(GREENYELLOW);
  int h,w,buffidx,row,col;
  h = 16;
  w = 16;
  buffidx = 0;
  for (row=0; row<h; row++) {
    for (col=0; col<w; col++) { 
      mydisp.drawPixel(col, row, pgm_read_word(image_data_hw_sm + buffidx));
      buffidx++;
    } 
  }
  mydisp.setCursor(17, 4);
  mydisp.print("Hacker Warehouse");
  mydisp.setCursor(1, 20);
  mydisp.print("hackerwarehouse.com");

  mydisp.setCursor(1, 32);
  buffidx = 0;
  for (row=0; row<h; row++) {
    for (col=0; col<w; col++) { 
      mydisp.drawPixel(col, row+32, pgm_read_word(image_data_js_sm + buffidx));
      buffidx++;
    } 
  }
  mydisp.setCursor(17, 36);
  mydisp.print("Jaycon Systems");
  mydisp.setCursor(1, 52);
  mydisp.print("jayconsystems.com");
}

void Shouts() {
  appmode=1;
  byte count=1;
  byte last=0;
  btnid = 0;
  mydisp.setTextColor(WHITE);
  while (btnid != 4)
  {
    if (btnid == 3){ 
      if (count == 1) {count = 3; } 
      else {count--;}
      btnid = 0;
    }
    else if (btnid == 2){ 
      if (count == 3) {count = 1; } 
      else {count++;}
      btnid = 0;
    }
    else if (btnid == 4) {break;}

    if (count != last)
    {
      switch(count)
      {
       case 1:
          mydisp.clearScreen();
          mydisp.setCursor(0, 0);
          mydisp.setTextScale(2);
          mydisp.println(F("Thanks To:"));
          mydisp.setTextScale(1);
          mydisp.println(F("@d1g1t4l_t3mpl4r"));
          mydisp.println(F("@donds i.sheep.hunt"));
          mydisp.println(F("@rj_chap"));
          mydisp.println(F("Altezza02"));
          mydisp.println(F("b10w"));
          mydisp.println(F("djdead"));
          break;
       case 2:
          mydisp.clearScreen();
          mydisp.setTextScale(1);
          mydisp.println(F("Dj Smegma"));
          mydisp.println(F("     /w Beef Supreme"));
          mydisp.println(F("Duckie"));
          mydisp.println(F("Gater_Byte"));
          mydisp.println(F("HenrikJay"));
          mydisp.println(F("Hyr0n #badgelife"));
          mydisp.println(F("n00bhax0r"));
          mydisp.println(F("ptk2k5"));
          break;
       case 3:
          mydisp.clearScreen();
          mydisp.setTextScale(1);
          mydisp.println(F("Raitlin"));
          mydisp.println(F("sec-princess"));
          mydisp.println(F("TwinkleTwinkie"));
          mydisp.println(F("WiqWaq"));
          mydisp.println(F("...and many other"));
          mydisp.println(F("unnamed supporters"));
          break;
      }
      last=count;
    }
    delay(150);
  }
  appmode=0;
  btncounter++;
}



