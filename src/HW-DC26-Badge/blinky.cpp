//#include <EEPROM.h>

#include <Adafruit_NeoPixel.h>
#include <WS2812FX.h>

#include "core.h"

extern Adafruit_NeoPixel pixels;
extern WS2812FX ws2812fx;
extern byte btncounter;
extern volatile byte btnid;
extern byte appmode;
extern byte ledsbright;

extern void all_leds_off();

void LED_Flashlight(){
  appmode=1;
  byte count=7;
  byte last=0;
  btnid = 0;
  while (btnid != 4)
  {
    if (btnid == 3){ 
      if (count == 1) {count = 1; } 
      else {count--;}
      btnid = 0;
    }
    else if (btnid == 2){ 
      if (count == 7) {count = 7; } 
      else {count++;}
      btnid = 0;
    }
    else if (btnid == 4) {break;}

    if (count != last)
    {
      switch(count)
      {
       case 1:
          pixels.setBrightness(255);
          break;
       case 2:
          pixels.setBrightness(184);
          break;
       case 3:
          pixels.setBrightness(128);
          break;
       case 4:
          pixels.setBrightness(64);
          break;
       case 5:
          pixels.setBrightness(32);
          break;
       case 6:
          pixels.setBrightness(10);
          break;
       case 7:
          pixels.setBrightness(4);
          break;          
      }
      for(byte i=0;i<NUMPIXELS;i++){
        pixels.setPixelColor(i, pixels.Color(63,63,63)); 
        pixels.show(); 
      }
      last=count;
    }
    delay(100);
  }
  appmode=0;
  btncounter++;
  all_leds_off();
}

#define TIMER_MS 7000
void LED_WS2812FX_FullDemo(){
  unsigned long last_change = 0;
  unsigned long now = 0;
  ws2812fx.setMode(FX_MODE_RANDOM_COLOR);
  ws2812fx.start();

  while (btncounter == 0 or btnid != 4)
    {
      now = millis();
      ws2812fx.service();
      if(now - last_change > TIMER_MS) {
        ws2812fx.setMode((ws2812fx.getMode() + 1) % ws2812fx.getModeCount());
        last_change = now;
      }
      delay(50);
    }
  all_leds_off();
}

void LED_WS2812FX_Favorites(){
  unsigned long last_change = 0;
  unsigned long now = 0;
  ws2812fx.setMode(FX_MODE_TWINKLE_RANDOM);
  ws2812fx.start();

  //byte favs[] = { 7, 40, 43, 10, 17, 8, 14, 19, 41 }; //dc25 favorites
  byte favs[] = { 7, 9, 14, 17, 19, 41, 43 }; //dc26 favorites in a circle pattern
  byte favs_cnt = 6;
  byte favs_current = 0;
  while (btncounter == 0 or btnid != 4)
    {
      now = millis();
      ws2812fx.service();
      if(now - last_change > TIMER_MS) {
        ws2812fx.setMode(favs[favs_current]);
        if (favs_current == favs_cnt) { favs_current = 0; }
        else { favs_current = favs_current + 1; }
        last_change = now;
      }
      delay(50);
    }
  all_leds_off();
}

void LED_WS2812FX(byte ledmode){
  ws2812fx.setMode(ledmode);
  ws2812fx.start();
  while (btncounter == 0 or btnid != 4)
  {
    ws2812fx.service();
    delay(50);
  }
 all_leds_off();
}

