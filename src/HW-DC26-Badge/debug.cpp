#include <SSD_13XX.h>

#include "core.h"

extern SSD_13XX mydisp;
extern volatile byte btnid;
extern byte appmode;
extern byte btncounter;

extern byte oledbright;
extern byte ledsbright;
extern byte region_id;
extern byte region_unlocked;

extern String ISSID;
extern String IPSWD;
extern int NTP_OFFSET;

void DebugInfo1()
{
  mydisp.clearScreen();
  mydisp.setCursor(0, 0);
  mydisp.setTextColor(GREENYELLOW);
  mydisp.setTextScale(1);
//  mydisp.print(F("oled: "));
//  mydisp.println(oledbright);
//  mydisp.print(F("leds: "));
//  mydisp.println(ledsbright);
//  mydisp.print(F("region_id: "));
//  mydisp.println(region_id);
  mydisp.print(F("region_unlocked: "));
  mydisp.println(region_unlocked);

  mydisp.print(ISSID);
  mydisp.print(F(" / "));
  mydisp.println(IPSWD);
  
}


