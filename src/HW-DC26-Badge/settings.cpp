#include "FS.h"
#include "SPIFFS.h"
#include <pgmspace.h>

#include <Adafruit_NeoPixel.h>
#include <WS2812FX.h>
#include <SSD_13XX.h>
#include <ArduinoJson.h>

#include "core.h"

extern Adafruit_NeoPixel pixels;
extern WS2812FX ws2812fx;
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
extern String GOOGLEAUTH;

byte default_oledbright = 64;
byte default_ledsbright = 4;
long default_ntp_offset = -25200;
String default_oauth = "";

extern void all_leds_off();

////////////////////////////////////////////////////////////////////////////////////////////////////////

extern char itmWinPayload1[];
extern char itmWinPayload2[];
extern char itmWinPayload3[];
extern char itmWinPayload4[];
extern char itmWinPayload5[];
extern char itmWinPayload6[];
extern char itmWinPayload7[];
extern char itmWinPayload8[];
extern char itmWinPayload9[];
extern char actualWinPayloads[9][40];
void genWinPayloadsMenu()
{
  File configFile = SPIFFS.open("/win.json", "r");
  if (!configFile) { return; }

  size_t size = configFile.size();
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
  StaticJsonBuffer<500> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());

  if (json.success()) {
    String t_name1 = json["NAMES"][0];
    itmWinPayload1[sizeof(t_name1)+1];
    strcpy(itmWinPayload1, t_name1.c_str());
    String t_file1 = json["FILES"][0];
    strcpy(actualWinPayloads[1], t_file1.c_str());

    String t_name2 = json["NAMES"][1];
    itmWinPayload2[sizeof(t_name2)+1];
    strcpy(itmWinPayload2, t_name2.c_str());
    String t_file2 = json["FILES"][1];
    strcpy(actualWinPayloads[2], t_file2.c_str());

    String t_name3 = json["NAMES"][2];
    itmWinPayload3[sizeof(t_name3)+1];
    strcpy(itmWinPayload3, t_name3.c_str());
    String t_file3 = json["FILES"][2];
    strcpy(actualWinPayloads[3], t_file3.c_str());

    String t_name4 = json["NAMES"][3];
    itmWinPayload4[sizeof(t_name4)+1];
    strcpy(itmWinPayload4, t_name4.c_str());
    String t_file4 = json["FILES"][3];
    strcpy(actualWinPayloads[4], t_file4.c_str());

    String t_name5 = json["NAMES"][4];
    itmWinPayload5[sizeof(t_name5)+1];
    strcpy(itmWinPayload5, t_name5.c_str());
    String t_file5 = json["FILES"][4];
    strcpy(actualWinPayloads[5], t_file5.c_str());

//    String t_name6 = json["NAMES"][5];
//    itmWinPayload6[sizeof(t_name6)+1];
//    strcpy(itmWinPayload6, t_name6.c_str());
//    String t_file6 = json["FILES"][5];
//    strcpy(actualWinPayloads[6], t_file6.c_str());
//
//    String t_name7= json["NAMES"][6];
//    itmWinPayload7[sizeof(t_name7)+1];
//    strcpy(itmWinPayload7, t_name7.c_str());
//    String t_file7 = json["FILES"][6];
//    strcpy(actualWinPayloads[7], t_file7.c_str());
//
//    String t_name8 = json["NAMES"][7];
//    itmWinPayload8[sizeof(t_name8)+1];
//    strcpy(itmWinPayload8, t_name8.c_str());
//    String t_file8 = json["FILES"][7];
//    strcpy(actualWinPayloads[8], t_file8.c_str());
//
//    String t_name9 = json["NAMES"][8];
//    itmWinPayload9[sizeof(t_name9)+1];
//    strcpy(itmWinPayload9, t_name9.c_str());
//    String t_file9 = json["FILES"][8];
//    strcpy(actualWinPayloads[9], t_file9.c_str());
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

extern char itmLinPayload1[];
extern char itmLinPayload2[];
extern char itmLinPayload3[];
extern char itmLinPayload4[];
extern char itmLinPayload5[];
extern char itmLinPayload6[];
extern char itmLinPayload7[];
extern char itmLinPayload8[];
extern char itmLinPayload9[];
extern char actualLinPayloads[9][40];
void genLinPayloadsMenu()
{
  File configFile = SPIFFS.open("/lin.json", "r");
  if (!configFile) { return; }

  size_t size = configFile.size();
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
  StaticJsonBuffer<500> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());

  if (json.success()) {
    String t_name1 = json["NAMES"][0];
    itmLinPayload1[sizeof(t_name1)+1];
    strcpy(itmLinPayload1, t_name1.c_str());
    String t_file1 = json["FILES"][0];
    strcpy(actualLinPayloads[1], t_file1.c_str());

    String t_name2 = json["NAMES"][1];
    itmLinPayload2[sizeof(t_name2)+1];
    strcpy(itmLinPayload2, t_name2.c_str());
    String t_file2 = json["FILES"][1];
    strcpy(actualLinPayloads[2], t_file2.c_str());

    String t_name3 = json["NAMES"][2];
    itmLinPayload3[sizeof(t_name3)+1];
    strcpy(itmLinPayload3, t_name3.c_str());
    String t_file3 = json["FILES"][2];
    strcpy(actualLinPayloads[3], t_file3.c_str());

    String t_name4 = json["NAMES"][3];
    itmLinPayload4[sizeof(t_name4)+1];
    strcpy(itmLinPayload4, t_name4.c_str());
    String t_file4 = json["FILES"][3];
    strcpy(actualLinPayloads[4], t_file4.c_str());

    String t_name5 = json["NAMES"][4];
    itmLinPayload5[sizeof(t_name5)+1];
    strcpy(itmLinPayload5, t_name5.c_str());
    String t_file5 = json["FILES"][4];
    strcpy(actualLinPayloads[5], t_file5.c_str());

//    String t_name6 = json["NAMES"][5];
//    itmLinPayload6[sizeof(t_name6)+1];
//    strcpy(itmLinPayload6, t_name6.c_str());
//    String t_file6 = json["FILES"][5];
//    strcpy(actualLinPayloads[6], t_file6.c_str());
//
//    String t_name7= json["NAMES"][6];
//    itmLinPayload7[sizeof(t_name7)+1];
//    strcpy(itmLinPayload7, t_name7.c_str());
//    String t_file7 = json["FILES"][6];
//    strcpy(actualLinPayloads[7], t_file7.c_str());
//
//    String t_name8 = json["NAMES"][7];
//    itmLinPayload8[sizeof(t_name8)+1];
//    strcpy(itmLinPayload8, t_name8.c_str());
//    String t_file8 = json["FILES"][7];
//    strcpy(actualLinPayloads[8], t_file8.c_str());
//
//    String t_name9 = json["NAMES"][8];
//    itmLinPayload9[sizeof(t_name9)+1];
//    strcpy(itmLinPayload9, t_name9.c_str());
//    String t_file9 = json["FILES"][8];
//    strcpy(actualLinPayloads[9], t_file9.c_str());
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

extern char itmStaticString1[];
extern char itmStaticString2[];
extern char itmStaticString3[];
extern char actualStaticString[3][40];
void genStaticStringsMenu()
{
  File configFile = SPIFFS.open("/static.json", "r");
  if (!configFile) { return; }

  size_t size = configFile.size();
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
  StaticJsonBuffer<300> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());

  if (json.success()) {
    String t_name1 = json["NAMES"][0];
    itmStaticString1[sizeof(t_name1)+1];
    strcpy(itmStaticString1, t_name1.c_str());
    String t_file1 = json["STRINGS"][0];
    strcpy(actualStaticString[1], t_file1.c_str());

    String t_name2 = json["NAMES"][1];
    itmStaticString2[sizeof(t_name2)+1];
    strcpy(itmStaticString2, t_name2.c_str());
    String t_file2 = json["STRINGS"][1];
    strcpy(actualStaticString[2], t_file2.c_str());

    String t_name3 = json["NAMES"][2];
    itmStaticString3[sizeof(t_name3)+1];
    strcpy(itmStaticString3, t_name3.c_str());
    String t_file3 = json["STRINGS"][2];
    strcpy(actualStaticString[3], t_file3.c_str());
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////


void loadSettings() {
  File configFile = SPIFFS.open("/config.json", "r");

  //set defaults if the file doesn't exist entirely
  if (!configFile) {
    oledbright = default_oledbright;
    mydisp.setBrightness(oledbright);
    ledsbright = default_ledsbright;
    pixels.setBrightness(ledsbright);
    ws2812fx.setBrightness(ledsbright);

    NTP_OFFSET = default_ntp_offset;
    GOOGLEAUTH = default_oauth;
    
    return;
  }

  size_t size = configFile.size();
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
  //StaticJsonBuffer<200> jsonBuffer;
  StaticJsonBuffer<500> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());

  if (json.success()) {
    oledbright = json["OLEDBRIGHT"];
    if (oledbright) { 
      mydisp.setBrightness(oledbright);
    }
    ledsbright = json["LEDSBRIGHT"];
    if (ledsbright) { 
      pixels.setBrightness(ledsbright);
      ws2812fx.setBrightness(ledsbright);
    }
    region_id = json["REGIONID"];
    region_unlocked = json["REGION_UNLOCK"];

    String t_ISSID = json["INTERNET_SSID"];
    ISSID = t_ISSID;
    String t_IPSWD = json["INTERNET_PSWD"];
    IPSWD = t_IPSWD;

    int t_NTP_OFFSET = json["NTP_OFFSET"];
    NTP_OFFSET = t_NTP_OFFSET;

    String t_GOOGLEAUTH = json["GOOGLEAUTH"];
    GOOGLEAUTH = t_GOOGLEAUTH; 
  }

  // set defaults if they don't exist in the conf file
  if (oledbright == NULL)
    { 
      oledbright = default_oledbright;
      mydisp.setBrightness(oledbright);
    }
  if (ledsbright == NULL)
    {
      ledsbright = default_ledsbright;
      pixels.setBrightness(ledsbright);
      ws2812fx.setBrightness(ledsbright);
    }
  if (NTP_OFFSET == NULL) { NTP_OFFSET = default_ntp_offset; }
  if (GOOGLEAUTH == NULL) { GOOGLEAUTH = default_oauth; }

  configFile.close();
}

void SaveSettings() {
  File configFile = SPIFFS.open("/config.json", FILE_WRITE);
  if (!configFile) {
    return;
  }

  StaticJsonBuffer<500> jsonBuffer;
  JsonObject &json = jsonBuffer.createObject();

  json["LEDSBRIGHT"] = ledsbright;
  json["OLEDBRIGHT"] = oledbright;
  json["REGIONID"] = region_id;
  json["REGION_UNLOCK"] = region_unlocked;
  json["INTERNET_SSID"] = ISSID;
  json["INTERNET_PSWD"] = IPSWD;
  json["NTP_OFFSET"] = NTP_OFFSET;
  json["GOOGLEAUTH"] = GOOGLEAUTH;

  // Serialize JSON to file
  if (json.printTo(configFile) == 0) {
    Serial.println(F("Failed to write to file"));
  }

  configFile.close();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void SaveOLEDBrightness(byte a) {
  oledbright = a;
  mydisp.setBrightness(oledbright);
}

void ShowLEDBrightness() {
  all_leds_off();
  pixels.setPixelColor(9, pixels.Color(65,105,225));
  pixels.setPixelColor(10, pixels.Color(65,105,225));
  pixels.setPixelColor(11, pixels.Color(65,105,225));  
  pixels.show();
}

void SaveLEDBrightness(byte a) {
  ledsbright = a;
  pixels.setBrightness(ledsbright); 
  ws2812fx.setBrightness(ledsbright);
}

void rotateScreen180() {
  mydisp.setRotation(2);
  //1 doesn't do anything - normal orientation
  //2 mirrors and is upside down
  //flip buttons
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

