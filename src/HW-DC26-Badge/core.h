#include <pgmspace.h>

#ifndef HW_CORE_H
#define HW_CORE_H

#define ESP32

#define PIN            23
#define NUMPIXELS      14

#define MAX_X 96
#define MAX_Y 64

//#define REGION_ADDR 1           //1 byte - 1->US, 2->EU, 3->JP
//#define PIXELBRIGHT_ADDR 2      //1 byte
//#define MYDISPBRIGHT_ADDR 3     //1 byte
//#define WS2812FXBRIGHT_ADDR 4   //1 byte
//#define ARTWORK_ADDR 5          //1 byte
//#define REGION_UNLOCK_ADDR 6    //1 byte

static const char UNLOCK_KONAMI[] PROGMEM  = {"UUDDLRLR"};
static const char UNLOCK_REGION[] PROGMEM  = {"RLRLLRDU"};
static const char UNLOCK_REGIONPERM[] PROGMEM  = {"UDDDUULR"};

#endif


