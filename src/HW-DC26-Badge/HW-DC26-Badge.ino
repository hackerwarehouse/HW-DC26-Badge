#ifdef __AVR__
  #include <avr/power.h>
#endif
#include "FS.h"
#include "SPIFFS.h"
#include <SPI.h>
#include <WiFi.h>

#include <Adafruit_NeoPixel.h>
#include <qMenuDisplay.h>
#include <qMenuSystem.h>
#include <SSD_13XX.h>
#include <WS2812FX.h>

#include "about.h"
#include "blinky.h"
#include "core.h"
#include "debug.h"
#include "extras.h"
#include "graphics.h"
#include "hidarsenal.h"
#include "mainmenu.h"
#include "settings.h"
#include "wifi-client.h"
#include "wifi-pkt1.h"
#include "wifi-scanner.h"
#include "wifi-server.h"

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
WS2812FX ws2812fx = WS2812FX(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
SSD_13XX mydisp( /*cs*/ 15, /*dc*/ 27, /*rst*/ 26);
qMenuSystem menu=qMenuSystem(&mydisp);

const byte left = 0; 
const byte down = 16; 
const byte right = 2; 
const byte up = 4;  

volatile byte btncounter = 0;
volatile byte btnid  = 0;
byte appmode = 0;

volatile byte region_id = 1;
volatile byte oledbright;
volatile byte ledsbright;
String ISSID;
String IPSWD;
int NTP_OFFSET;
String GOOGLEAUTH;
byte region_unlocked = 0;

long debouncing_time = 250;
unsigned long last_micros = 0;

void UP(){
  if((long)(micros() - last_micros) >= debouncing_time * 1000) {
    if (appmode){      
    }
    else {
      btncounter ++;
    }
    btnid = 3;
    last_micros = micros();
  }
}

void DOWN(){
  if((long)(micros() - last_micros) >= debouncing_time * 1000) 
  {
    if (appmode){
    }
    else {
      btncounter ++;
    }
    btnid = 2;
    last_micros = micros();
  }
}

void RIGHT(){
  if((long)(micros() - last_micros) >= debouncing_time * 1000) {
    if (appmode){
    }
    else {
      btncounter ++;
    }
    btnid = 1;
    last_micros = micros();
  }
}

void LEFT(){
  if((long)(micros() - last_micros) >= debouncing_time * 1000) {
    if (appmode){
    }
    else {
      btncounter ++;
    }
    btnid = 4;
    last_micros = micros();
  }
}

void setup()
{ 
  SPI.begin( /*CLK*/ 14, /*MISO*/ 12, /*MOSI*/ 13);
  SPIFFS.begin();
  mydisp.begin();
  
  pinMode(up, INPUT_PULLUP);
  pinMode(down, INPUT_PULLUP);
  pinMode(right, INPUT_PULLUP);
  pinMode(left, INPUT);           //External pulldown resistor (required for programming)

  attachInterrupt(digitalPinToInterrupt(up), UP, FALLING);
  attachInterrupt(digitalPinToInterrupt(down), DOWN, FALLING);
  attachInterrupt(digitalPinToInterrupt(right), RIGHT, FALLING);
  attachInterrupt(digitalPinToInterrupt(left), LEFT, FALLING);
  
  pixels.begin();
  ws2812fx.init();
  ws2812fx.setSpeed(200);

  menu.InitMenu((const char **)mnuRoot,cntRoot,1);
  
  //Serial.begin(115200);
  //Serial.begin(38400); // Need slower comms between atmel 32u4 and esp32
  Serial.begin(57600); // Jay mentioned this is the fastest/reliable between the two chips

  loadSettings();
  all_leds_off();
  wifi_off();
  
  genWinPayloadsMenu();
  genLinPayloadsMenu();
  genStaticStringsMenu();
}

void menu_reset(){
  mydisp.setBrightness(oledbright);
  mydisp.setFont(&defaultFont);
  mydisp.setTextScale(1);
  //all_leds_off();
}

void all_leds_off(){
  pixels.setBrightness(ledsbright);
  for(byte i=0;i<NUMPIXELS-1;i++){
    pixels.setPixelColor(i, pixels.Color(0,0,0));
    pixels.show();
    delay(5);
  }  
}

void wifi_off(){
  WiFi.mode(WIFI_OFF);
  //WiFi.forceSleepBegin();
  delay(5);
}

void menu_force_escape(){
  btnid = 4;
  btncounter++;
}


void loop()
{ 
  int keycode=0;
  int clickedItem=0; 
  
  if (btncounter > 0)
  { 
    switch(btnid)
    {
      case 1:       
        clickedItem=menu.ProcessMenu(ACTION_SELECT);
        btncounter--;
        break;
      case 2:
        menu.ProcessMenu(ACTION_DOWN);
        btncounter--;
        break;      
      case 3:
        menu.ProcessMenu(ACTION_UP);
        btncounter--;
        break;
      case 4:
        clickedItem=menu.ProcessMenu(ACTION_BACK); 
        btncounter--;
        break;
    }
  }
  
  if (clickedItem > 0)
  {
    // Logic for root menu
    if (menu.CurrentMenu==mnuRoot)
      switch (clickedItem)
      {
        case 1:
          menu.InitMenu((const char ** )mnuHIDarsenal,cntHIDarsenal,1);
          break;
        case 2:
          menu.InitMenu((const char ** )mnuWiFiPassiveTools,cntWiFiPassiveTools,1);
          break;
        case 3:
          menu.InitMenu((const char ** )mnuWiFiClient,cntWiFiClient,1);
          break; 
        case 4:
          menu.InitMenu((const char ** )mnuWiFiServer,cntWiFiServer,1);
          break; 
//        case 5:
//          menu.InitMenu((const char ** )mnuBLETools,cntBLETools,1);
//          break;
        case 5:
          menu.InitMenu((const char ** )mnuBlinky,cntBlinky,1);
          break;
        case 6:
          menu.InitMenu((const char ** )mnuGraphics,cntGraphics,1);
          break;
//        case 6:
//          menu.InitMenu((const char ** )mnuMischief,cntMischief,1);
//          break; 
        case 7:
          menu.InitMenu((const char ** )mnuExtra,cntExtra,1);
          break;
        case 8:
          menu.InitMenu((const char ** )mnuSettings,cntSettings,1);
          break;
        case 9:
          menu.InitMenu((const char ** )mnuAbout,cntAbout,1);
          break;
        case 10:
          menu.InitMenu((const char ** )mnuDebug,cntDebug,1);
          break;
      }

    // Logic for sub menus
    else if (menu.CurrentMenu==mnuHIDarsenal)
      switch (clickedItem)
      {
        case 1:
          //win kb
          menu.InitMenu((const char ** )mnuWinPayloads,cntWinPayloads,1);
          break;
        case 2:
          //lin kb
          menu.InitMenu((const char ** )mnuLinPayloads,cntLinPayloads,1);
          break;
        case 3:
          menu.MessageBox("Moving...");
          MouseJiggler();
          break;
        case 4:
          //static strings menu
          menu.InitMenu((const char ** )mnuStaticStrings,cntStaticStrings,1);
          break;
        case 5:
          GoogleAuth();
          break;
        case 6:
          break;
      }

    else if (menu.CurrentMenu==mnuWinPayloads)
      {
        runKBPayload(actualWinPayloads[clickedItem]);
      }

    else if (menu.CurrentMenu==mnuLinPayloads)
      {
        runKBPayload(actualLinPayloads[clickedItem]);
      }

    else if (menu.CurrentMenu==mnuStaticStrings)
      {
        runStaticString(actualStaticString[clickedItem]);
      }
    
    else if (menu.CurrentMenu==mnuWiFiPassiveTools)
      switch (clickedItem)
      {
        case 1:
          menu.MessageBox("Scanning...");
          AP_Channel_Usage();
          wifi_off();
          break;
        case 2:
          menu.MessageBox("Scanning...");
          AP_Scanner();
          wifi_off();
          break;
        case 3:
          Pkt_Monitor();
          //wifi_off();
          break;
        case 4:
          menu.MessageBox("Scanning...");
          Open_AP_Scanner();
          wifi_off();
          break;
        case 5:
          //DeauthMonitor();
          wifi_off();
          break;
        case 6:
          //ClientBeacons();
          //wifi_off();
          break;
      }

    else if (menu.CurrentMenu==mnuWiFiClient)
      switch (clickedItem)
      {
        case 1:
          ConnectInternet();
          break;
        case 2:
          IPConfig();
          break;
        case 3:
          NTPdisplay();
          break;
        case 4:
          menu.MessageBox("Disconnected");
          WiFi.disconnect(true);
          break;
      }

    else if (menu.CurrentMenu==mnuWiFiServer)
      switch (clickedItem)
      {
        case 1:
          APStart();
          APDisplay();
          break;
        case 2:
          APDisplay();
          break;
        case 3:
          WebServerStart();
          WebServerDisplay();
          break;
        case 4:
          WebServerDisplay();
          break;
        case 5:
          NTPstart();
          break;
        case 6:
          NTPstop();
          break;
        case 7:
          menu.MessageBox("AP off");
          APStop();
          break;
      }

//    else if (menu.CurrentMenu==mnuBLETools)
//      switch (clickedItem)
//      {
//        case 1:
//          BeaconStart();
//          break;
//        case 2:
//          BeaconStop();
//          break;
//        case 3:
//          break;
//      }

    else if (menu.CurrentMenu==mnuBlinky)
      switch (clickedItem)
      {
        case 1:
          mydisp.clearScreen();
          LED_WS2812FX_Favorites();
          break;
        case 2:
          mydisp.clearScreen();
          LED_Flashlight();
          break;
         case 3:
          mydisp.clearScreen();
          LED_WS2812FX_FullDemo();
          break;
         default:
          mydisp.clearScreen();
          byte ledmode = clickedItem - 3;
          LED_WS2812FX(ledmode);
          break; 
      }

    else if (menu.CurrentMenu==mnuGraphics)
      switch (clickedItem)
      {
        case 1:
          SelectArtwork();
          break;
        case 2:
          GfxBenchmark();
          break;
        case 3:
          cubeDemo();
          break;
        case 4:
          clockDemo();
          break;
        case 5:
          gaugeDemo();
          break;
      }

    else if (menu.CurrentMenu==mnuMischief)
      switch (clickedItem)
      {
        case 1:
          RickRollSSID();
          break;
        default:
          menu.MessageBox("N/A");
          break;
      }

    else if (menu.CurrentMenu==mnuExtra)
      switch (clickedItem)
      {
        case 1:
          snakeGame();
          break;
        case 2:
          ButtonEcho();
          break;
        case 3:
          KeyboardDemo();
          break;
      }

    else if (menu.CurrentMenu==mnuSettings)
      switch (clickedItem)
      {
        case 1:
          menu.InitMenu((const char ** )mnuOLEDBrightness,cntOLEDBrightness,1);
          break;
        case 2:
          menu.InitMenu((const char ** )mnuLEDBrightness,cntLEDBrightness,1);
          ShowLEDBrightness();
          break;
        case 3:
          if (region_unlocked == 1) {
            menu.InitMenu((const char ** )mnuRegion,cntRegion,1);
          }
          else { menu.MessageBox("Region is Locked"); }
          break;
        case 4:
          //configuration via webserver
          mydisp.clearScreen();
          mydisp.println(F("1) Start AP"));
          mydisp.println(F("2) Start Webserver"));
          mydisp.println(F("3) Browse webserver"));
          mydisp.println(F("    and nav to config"));
          mydisp.println(F("4) Reboot after mods"));
          break;
        case 5:
          SaveSettings();
          menu.MessageBox("Settings Saved");
          delay(1000);
          menu_force_escape();
          break;
        case 6:
          break;
      }

    else if (menu.CurrentMenu==mnuOLEDBrightness)
      switch (clickedItem)
      {
        case 1:
          SaveOLEDBrightness(1);
          break;
        case 2:
          SaveOLEDBrightness(2);
          break;
        case 3:
          SaveOLEDBrightness(4);
          break;
        case 4:
          SaveOLEDBrightness(7);
          break;
        case 5:
          SaveOLEDBrightness(11);
          break;
        case 6:
          SaveOLEDBrightness(15);
          break;
      }

    else if (menu.CurrentMenu==mnuLEDBrightness)
      switch (clickedItem)
      {
        case 1:
          SaveLEDBrightness(1);
          ShowLEDBrightness();
          break;
        case 2:
          SaveLEDBrightness(10);
          ShowLEDBrightness();
          break;
        case 3:
          SaveLEDBrightness(32);
          ShowLEDBrightness();
          break;
        case 4:
          SaveLEDBrightness(64);
          ShowLEDBrightness();
          break;
        case 5:
          SaveLEDBrightness(128);
          ShowLEDBrightness();
          break;
        case 6:
          SaveLEDBrightness(184);
          ShowLEDBrightness();
          break;
        case 7:
          SaveLEDBrightness(254);
          ShowLEDBrightness();
          break;
      }
    else if (menu.CurrentMenu==mnuRegion)
      switch (clickedItem)
      {
       case 1:
          menu.MessageBox("Region Set: US");
          region_id = 1;  //US
//          if (EEPROM.read(REGION_UNLOCK_ADDR)){
//            EEPROM.write(REGION_ADDR,region_id);  
//            EEPROM.commit();
//          }
          break;
       case 2:
          menu.MessageBox("Region Set: EU");
          region_id = 2;
//          if (EEPROM.read(REGION_UNLOCK_ADDR)){
//            EEPROM.write(REGION_ADDR,region_id);  
//            EEPROM.commit();
//          }
          break;      
      }

    else if (menu.CurrentMenu==mnuAbout)
      switch (clickedItem)
      {
        case 1:
          SysInfo();
          break;
        case 2:
          HWInfo();
          break;          
        case 3:
          Credits();
          break;
        case 4:
          Shouts();
          break;
      } 

    else if (menu.CurrentMenu==mnuDebug)
      switch (clickedItem)
      {
        case 1:
          DebugInfo1();
          break;
        case 2:
          all_leds_off();
          break;
        case 3:
          wifi_off();
          break;
        case 4:
          HTTPSysUpdate();
          break;
        case 5:
          RickRollSSID();
          //rotateScreen180();
          break;
        case 6:
          //loadSettings();
          //rotateScreen180();
          break;
      } 

    //after menu function, set brightness, set default font on display. reset leds
    menu_reset();
  }

  // menu return logic
  else if(clickedItem == -1){
    if (menu.CurrentMenu==mnuRoot)
      { /*In root menu already - Do Nothing*/ }

    //2nd level menus
    else if (menu.CurrentMenu==mnuHIDarsenal)   
      { menu.InitMenu((const char ** )mnuRoot,cntRoot,1); }
    else if (menu.CurrentMenu==mnuWiFiPassiveTools)   
      { menu.InitMenu((const char ** )mnuRoot,cntRoot,2); }
    else if (menu.CurrentMenu==mnuWiFiClient)
      { menu.InitMenu((const char ** )mnuRoot,cntRoot,3); }
    else if (menu.CurrentMenu==mnuWiFiServer)
      { menu.InitMenu((const char ** )mnuRoot,cntRoot,4); }
//    else if (menu.CurrentMenu==mnuBLETools)
//      { menu.InitMenu((const char ** )mnuRoot,cntRoot,5); }
    else if (menu.CurrentMenu==mnuBlinky)
      { menu.InitMenu((const char ** )mnuRoot,cntRoot,5); }
    else if (menu.CurrentMenu==mnuGraphics)
      { menu.InitMenu((const char ** )mnuRoot,cntRoot,6); }
//    else if (menu.CurrentMenu==mnuMischief)
//      { menu.InitMenu((const char ** )mnuRoot,cntRoot,6); }
    else if (menu.CurrentMenu==mnuExtra)
      { menu.InitMenu((const char ** )mnuRoot,cntRoot,7); }
    else if (menu.CurrentMenu==mnuSettings)
      { menu.InitMenu((const char ** )mnuRoot,cntRoot,8); }
    else if (menu.CurrentMenu==mnuAbout)
      { menu.InitMenu((const char ** )mnuRoot,cntRoot,9); }
    else if (menu.CurrentMenu==mnuDebug)
      { menu.InitMenu((const char ** )mnuRoot,cntRoot,10); }

    //3rd level menus
    else if (menu.CurrentMenu==mnuOLEDBrightness)
      { menu.InitMenu((const char ** )mnuSettings,cntSettings,1); }
    else if (menu.CurrentMenu==mnuLEDBrightness)
      { menu.InitMenu((const char ** )mnuSettings,cntSettings,2); 
        all_leds_off();
      }
    else if (menu.CurrentMenu==mnuRegion)
      { menu.InitMenu((const char ** )mnuSettings,cntSettings,3); }
    else if (menu.CurrentMenu==mnuWinPayloads)
      { menu.InitMenu((const char ** )mnuHIDarsenal,cntHIDarsenal,1); }
    else if (menu.CurrentMenu==mnuLinPayloads)
      { menu.InitMenu((const char ** )mnuHIDarsenal,cntHIDarsenal,2); }
    else if (menu.CurrentMenu==mnuStaticStrings)
      { menu.InitMenu((const char ** )mnuHIDarsenal,cntHIDarsenal,4); }
    

    //after return menu, reset brightness on display
    menu_reset();

   }
   
  
}

