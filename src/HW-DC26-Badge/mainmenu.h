#ifndef HW_MAINMENU_H
#define HW_MAINMENU_H

// required for "const char" and "PROGMEM"
#include <pgmspace.h>

// mnu* note - first item is menu title and it does not count toward cnt

static const char PROGMEM itmRoot[] = "HW-DC26 Badge";
static const char PROGMEM itmRootHIDarsenal[] = "HID Arsenal";
static const char PROGMEM itmRootWiFiPassive[] = "WiFi Passive Tools";
static const char PROGMEM itmRootWiFiClient[] = "WiFi Client Tools";
static const char PROGMEM itmRootWiFiServer[] = "WiFi Server Tools";
//static const char PROGMEM itmRootBLETools[] = "BLE Tools";
static const char PROGMEM itmRootBlinky[] = "Blinky";
static const char PROGMEM itmRootGraphics[] = "Graphics";
static const char PROGMEM itmRootMischief[] = "Mischief Tools";
static const char PROGMEM itmRootExtras[] = "Extras";
static const char PROGMEM itmRootSettings[] = "Settings";
static const char PROGMEM itmRootAbout[] = "About";
static const char PROGMEM itmRootDebug[] = "Debug";
const char * const mnuRoot[] PROGMEM = {
  itmRoot,
  itmRootHIDarsenal,itmRootWiFiPassive,itmRootWiFiClient,itmRootWiFiServer,itmRootBlinky,itmRootGraphics,itmRootExtras,itmRootSettings,itmRootAbout,itmRootDebug
  }; 
const int cntRoot PROGMEM = 10;

static const char PROGMEM itmHIDarsenal1[] = "Win KB Payloads";
static const char PROGMEM itmHIDarsenal2[] = "Lin KB Payloads";
static const char PROGMEM itmHIDarsenal3[] = "Mouse Jiggler";
static const char PROGMEM itmHIDarsenal4[] = "Static Strings";
static const char PROGMEM itmHIDarsenal5[] = "Google Auth (TOTP)";
static const char PROGMEM itmHIDarsenal6[] = "U2F";
const char * const mnuHIDarsenal[] PROGMEM = {
  itmRootHIDarsenal,
  itmHIDarsenal1,itmHIDarsenal2,itmHIDarsenal3,itmHIDarsenal4,itmHIDarsenal5
  };
const int cntHIDarsenal PROGMEM = 5;

static const char PROGMEM itmWiFiPassiveTools1[] = "Channel Usage";
static const char PROGMEM itmWiFiPassiveTools2[] = "AP Scanner";
static const char PROGMEM itmWiFiPassiveTools3[] = "Packet/Deauth Sniff";
static const char PROGMEM itmWiFiPassiveTools4[] = "Open AP Nearby?";
static const char PROGMEM itmWiFiPassiveTools5[] = "Deauth Monitor";
const char * const mnuWiFiPassiveTools[] PROGMEM = {
  itmRootWiFiPassive,
  itmWiFiPassiveTools1,itmWiFiPassiveTools2,itmWiFiPassiveTools3,itmWiFiPassiveTools4
  };
const int cntWiFiPassiveTools PROGMEM = 4;

static const char PROGMEM itmWiFiClient1[] = "Connect to AP";
static const char PROGMEM itmWiFiClient2[] = "ipconfig";
static const char PROGMEM itmWiFiClient3[] = "NTP Display";
static const char PROGMEM itmWiFiClient4[] = "Disconnect from AP";
const char * const mnuWiFiClient[] PROGMEM = {
  itmRootWiFiClient,
  itmWiFiClient1,itmWiFiClient2,itmWiFiClient3,itmWiFiClient4
  };  
const int cntWiFiClient PROGMEM = 4;

static const char PROGMEM itmWiFiServer1[] = "AP Start";
static const char PROGMEM itmWiFiServer2[] = "AP Info";
static const char PROGMEM itmWiFiServer3[] = "Webserver Start";
static const char PROGMEM itmWiFiServer4[] = "Webserver Info";
static const char PROGMEM itmWiFiServer5[] = "NTP Start";
static const char PROGMEM itmWiFiServer6[] = "NTP Stop";
static const char PROGMEM itmWiFiServer7[] = "AP Stop";
const char * const mnuWiFiServer[] PROGMEM = {
  itmRootWiFiServer,
  itmWiFiServer1,itmWiFiServer2,itmWiFiServer3,itmWiFiServer4,itmWiFiServer5,itmWiFiServer6,itmWiFiServer7
  };  
const int cntWiFiServer PROGMEM = 7;

//static const char PROGMEM itmBLETools1[] = "Beacon Start";
//static const char PROGMEM itmBLETools2[] = "Beacon Scanner";
//static const char PROGMEM itmBLETools3[] = "Btn Presenter Mode";
//const char * const mnuBLETools[] PROGMEM = {
//  itmRootBLETools,
//  itmBLETools1,itmBLETools2,itmBLETools3
//  };  
//const int cntBLETools PROGMEM = 3;

static const char PROGMEM itmBlinky[] = "Blinky Modes";
static const char PROGMEM itmBlinky0[] = "Favorites";
static const char PROGMEM itmBlinky1[] = "Flashlight";
static const char PROGMEM itmBlinky2[] = "Full Demo";
static const char PROGMEM itmBlinky3[] = "Blink";
static const char PROGMEM itmBlinky4[] = "Breath";
static const char PROGMEM itmBlinky5[] = "Color Wipe";
static const char PROGMEM itmBlinky6[] = "Color Wipe Rndm";
static const char PROGMEM itmBlinky7[] = "Random Color";
static const char PROGMEM itmBlinky8[] = "Single Dynamic";
static const char PROGMEM itmBlinky9[] = "Multi Dynamic";
static const char PROGMEM itmBlinky10[] = "Rainbow";
static const char PROGMEM itmBlinky11[] = "Rainbow Cycle";
static const char PROGMEM itmBlinky12[] = "Scan";
static const char PROGMEM itmBlinky13[] = "Dual Scan";
static const char PROGMEM itmBlinky14[] = "Fade";
static const char PROGMEM itmBlinky15[] = "Theater Chase";
static const char PROGMEM itmBlinky16[] = "Theater Chase Rnbw";
static const char PROGMEM itmBlinky17[] = "Running Lights";
static const char PROGMEM itmBlinky18[] = "Twinkle";
static const char PROGMEM itmBlinky19[] = "Twinkle Random";
static const char PROGMEM itmBlinky20[] = "Twinkle Fade";
static const char PROGMEM itmBlinky21[] = "Twinkle Fade Rndm";
static const char PROGMEM itmBlinky22[] = "Sparkle";
static const char PROGMEM itmBlinky23[] = "Flash Sparkle";
static const char PROGMEM itmBlinky24[] = "Hyper Sparkle";
static const char PROGMEM itmBlinky25[] = "Strobe";
static const char PROGMEM itmBlinky26[] = "Strobe Rainbow";
static const char PROGMEM itmBlinky27[] = "Multi Strobe";
static const char PROGMEM itmBlinky28[] = "Blink Rainbow";
static const char PROGMEM itmBlinky29[] = "Chase White";
static const char PROGMEM itmBlinky30[] = "Chase Color";
static const char PROGMEM itmBlinky31[] = "Chase Random";
static const char PROGMEM itmBlinky32[] = "Chase Rainbow";
static const char PROGMEM itmBlinky33[] = "Chase Flash";
static const char PROGMEM itmBlinky34[] = "Chase Flash Rndm";
static const char PROGMEM itmBlinky35[] = "Chase Rnbw White";
static const char PROGMEM itmBlinky36[] = "Chase Blackout";
static const char PROGMEM itmBlinky37[] = "Chase Blckot Rnbw";
static const char PROGMEM itmBlinky38[] = "Color Sweep Rndm";
static const char PROGMEM itmBlinky39[] = "Running Color";
static const char PROGMEM itmBlinky40[] = "Running Red Blue";
static const char PROGMEM itmBlinky41[] = "Running Random";
static const char PROGMEM itmBlinky42[] = "Larson Scanner";
static const char PROGMEM itmBlinky43[] = "Comet";
static const char PROGMEM itmBlinky44[] = "Fireworks";
static const char PROGMEM itmBlinky45[] = "Fireworks Random";
static const char PROGMEM itmBlinky46[] = "Merry xmas";
static const char PROGMEM itmBlinky47[] = "Fire Flicker";
static const char PROGMEM itmBlinky48[] = "Fire Flicker Soft";
const char * const mnuBlinky[] PROGMEM = {
  itmBlinky,
  itmBlinky0,itmBlinky1,itmBlinky2,itmBlinky3,itmBlinky4,itmBlinky5,itmBlinky6,itmBlinky7,itmBlinky8,itmBlinky9,
  itmBlinky10,itmBlinky11,itmBlinky12,itmBlinky13,itmBlinky14,itmBlinky15,itmBlinky16,itmBlinky17,itmBlinky18,itmBlinky19,
  itmBlinky20,itmBlinky21,itmBlinky22,itmBlinky23,itmBlinky24,itmBlinky25,itmBlinky26,itmBlinky27,itmBlinky28,itmBlinky29,
  itmBlinky30,itmBlinky31,itmBlinky32,itmBlinky33,itmBlinky34,itmBlinky35,itmBlinky36,itmBlinky37,itmBlinky38,itmBlinky39,
  itmBlinky40,itmBlinky41,itmBlinky42,itmBlinky43,itmBlinky44,itmBlinky45,itmBlinky46,itmBlinky47,itmBlinky48
  };  
const int cntBlinky PROGMEM = 49;

static const char PROGMEM itmGraphics[] = "Graphic Modes";
static const char PROGMEM itmGraphics1[] = "Artwork";
static const char PROGMEM itmGraphics2[] = "Benchmark";
static const char PROGMEM itmGraphics3[] = "3d Cube Demo";
static const char PROGMEM itmGraphics4[] = "Clock Demo";
static const char PROGMEM itmGraphics5[] = "Gauge Demo";
const char * const mnuGraphics[] PROGMEM = {
  itmGraphics,
  itmGraphics1,itmGraphics2,itmGraphics3,itmGraphics4,itmGraphics5
  };  
const int cntGraphics PROGMEM = 5;

static const char PROGMEM itmMischief1[] = "SSID Rickroll";
static const char PROGMEM itmMischief2[] = "Karma/Pineapple lite";
static const char PROGMEM itmMischief3[] = "Broadcast Packets";
static const char PROGMEM itmMischief4[] = "Deauther";
static const char PROGMEM itmMischief5[] = "SSID Havoc";
const char * const mnuMischief[] PROGMEM = {
  itmRootMischief,
  itmMischief1
  };  
const int cntMischief PROGMEM = 1;

static const char PROGMEM itmExtra1[] = "Snake Game";
static const char PROGMEM itmExtra2[] = "Button Echo";
static const char PROGMEM itmExtra3[] = "Keyboard Demo";
const char * const mnuExtra[] PROGMEM = {
  itmRootExtras,
  itmExtra1,itmExtra2,itmExtra3
  };  
const int cntExtra PROGMEM = 3;

static const char PROGMEM itmSettings1[] = "OLED Brightness";
static const char PROGMEM itmSettings2[] = "LED Brightness";
static const char PROGMEM itmSettings3[] = "Region";
static const char PROGMEM itmSettings4[] = "Other via WiFi";
static const char PROGMEM itmSettings5[] = "Save Settings";
const char * const mnuSettings[]PROGMEM  = {
  itmRootSettings,
  itmSettings1,itmSettings2,itmSettings3,itmSettings4,itmSettings5
  }; 
const int cntSettings PROGMEM = 5;

static const char PROGMEM itmOLEDBrightness[] = "OLED Brightness";
static const char PROGMEM itmOLEDBrightness1[] = "Lowest";
static const char PROGMEM itmOLEDBrightness3[] = "Low";
static const char PROGMEM itmOLEDBrightness4[] = "Low/Medium";
static const char PROGMEM itmOLEDBrightness5[] = "Medium";
static const char PROGMEM itmOLEDBrightness6[] = "High";
static const char PROGMEM itmOLEDBrightness7[] = "MAX";
const char * const mnuOLEDBrightness[] PROGMEM = {
  itmOLEDBrightness,
  itmOLEDBrightness1,itmOLEDBrightness3,itmOLEDBrightness4,itmOLEDBrightness5,itmOLEDBrightness6,itmOLEDBrightness7
  };  
const int cntOLEDBrightness PROGMEM = 6;

static const char PROGMEM itmLEDBrightness[] = "LEDs Brightness";
static const char PROGMEM itmLEDBrightness2[] = "Lower";
const char * const mnuLEDBrightness[] PROGMEM = {
  itmLEDBrightness,
  itmOLEDBrightness1,itmLEDBrightness2,itmOLEDBrightness3,itmOLEDBrightness4,itmOLEDBrightness5,itmOLEDBrightness6,itmOLEDBrightness7
  };  
const int cntLEDBrightness PROGMEM = 7;

static const char PROGMEM itmRegion[] = "Region Setting";
static const char PROGMEM itmRegion1[] = "United States";
static const char PROGMEM itmRegion2[] = "European Union";
const char * const mnuRegion[] PROGMEM = {
  itmRegion,
  itmRegion1,itmRegion2
  };  
const int cntRegion PROGMEM = 2;

static const char PROGMEM itmAbout1[] = "System Info";
static const char PROGMEM itmAbout2[] = "Hardware Info";
static const char PROGMEM itmAbout3[] = "Credits";
static const char PROGMEM itmAbout4[] = "Early Supporters";
const char * const mnuAbout[] PROGMEM = {
  itmRootAbout,
  itmAbout1,itmAbout2,itmAbout3,itmAbout4
  };  
const int cntAbout PROGMEM = 4;

static const char PROGMEM itmDebug1[] = "Debug 1 Output";
static const char PROGMEM itmDebug2[] = "LEDs off";
static const char PROGMEM itmDebug3[] = "WIFI off";
static const char PROGMEM itmDebug4[] = "HTTP System Update";
static const char PROGMEM itmDebug5[] = "meh";
static const char PROGMEM itmDebug6[] = "Rotate Screen 180";
static const char PROGMEM itmDebug7[] = "";
static const char PROGMEM itmDebug8[] = "";
static const char PROGMEM itmDebug9[] = "";
const char * const mnuDebug[] PROGMEM = {
  itmRootDebug,
  itmDebug1,itmDebug2,itmDebug3,itmDebug4
  };  
const int cntDebug PROGMEM = 4;

#endif
