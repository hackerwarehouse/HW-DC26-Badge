#include <WiFi.h>

#include <Adafruit_NeoPixel.h>
#include <SSD_13XX.h>

#include "_fonts/channels.c"
#include "core.h"

extern SSD_13XX mydisp;
extern Adafruit_NeoPixel pixels;
extern byte btncounter;
extern volatile byte btnid;
extern byte appmode;
extern byte region_id;

//////////////////////////////////////////////////////////////////////////////

byte chn [14];
byte mult = 5;

void AP_Channel_Usage(){
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(1000);
  
  for (int i=0; i<NUMPIXELS;i++){ chn[i] = 0; }

  //Count number of networks in each channel
  int n = WiFi.scanNetworks();
  for (int i = 0; i < n; i++) {
    String Name = WiFi.SSID(i);
    int count = WiFi.channel(i);
    //Serial.println(Name + ": " + count);
    switch (count){
      default:
        chn[count] += 1;
        break;
      }
  }

  //Set Title, font and horizontal line 
  mydisp.clearScreen();
  mydisp.setCursor(14, 0);
  mydisp.setTextColor(CYAN);
  mydisp.print(F("AP Channel Usage"));
  mydisp.setFont(&channels);
  mydisp.setTextColor(WHITE);
  mydisp.setCursor(0,53);
  if (region_id == 1){  //US
    mydisp.print(F("1 2 3 4 5 6 7 8 9 AB"));
    mydisp.print(F(" "));
    mydisp.setTextColor(RED);
    mydisp.print(F("12 13"));
  }
  else if (region_id == 2)  //EU
    mydisp.print(F("1 2 3 4 5 6 7 8 9 AB CD"));
  mydisp.drawLine(0, 53, 95, 53, ORANGE);
  mydisp.setTextColor(WHITE);

  //Draw Bar Graph
  byte num_chns;
  if(region_id == 1) //US
     num_chns = 11;
  else if (region_id == 2) // EU
     num_chns = 13;
  for(int i=1; i<num_chns; i++){
    if(chn[i] == 0){ }
    else if(chn[i] <= 2){
      mydisp.fillRect((i-1)*6,53-chn[i]*mult,3,chn[i]*mult,GREEN);
      mydisp.setCursor((i-1)*6-1, (53-chn[i]*mult)-10);
      mydisp.print(chn[i]);
    }
    else if(chn[i] <= 4){
      mydisp.fillRect((i-1)*6,53-chn[i]*mult,3,chn[i]*mult,YELLOW);
      mydisp.setCursor((i-1)*6-1, (53-chn[i]*mult)-10);
      mydisp.print(chn[i]);
    }
    else {
      mydisp.fillRect((i-1)*6,53-chn[i]*mult,3,chn[i]*mult,RED);
      mydisp.setCursor((i-1)*6-1, (53-chn[i]*mult)-10);
      mydisp.print(chn[i]);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////

void AP_Scanner(){
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(1000);
  
  appmode=1;
  byte count=0;
  byte last=1;
  btnid = 0;

  int n = WiFi.scanNetworks();
  mydisp.clearScreen();
  mydisp.setTextColor(WHITE);
  mydisp.setTextScale(1);
  
  if (n == 0){
    mydisp.println(F("No SSIDs detected :("));
    delay(1000);
    btnid=4;
  }
  else {
  }
  
  while (btnid != 4)
  {
    if (btnid == 3){ 
      if (count == 0) {count = n-1; } 
      else {count--;}
      btnid = 0;
    }
    else if (btnid == 2){ 
      if (count == n-1) {count = 0; } 
      else {count++;}
      btnid = 0;
    }
    else if (btnid == 4){
      //left button -> promopt refresh or exit?
      break;
    }

    if (count != last)
    {
      mydisp.clearScreen();
      mydisp.print(F("....:::: SSID "));
      mydisp.print(count + 1);
      mydisp.print(F(" of "));
      mydisp.print(n);
      mydisp.println(F(" ::::...."));
      mydisp.println();

      mydisp.print(F("ssid: "));
      mydisp.println(WiFi.SSID(count));
      
      mydisp.print(F("channel: "));
      mydisp.println(WiFi.channel(count));
      mydisp.print(F("dBm: "));
      mydisp.println(WiFi.RSSI(count));
      mydisp.print(F("enc: "));
      mydisp.println((WiFi.encryptionType(count) == WIFI_AUTH_OPEN)?" ":"yes");
      last = count;
    }
    delay(100);
  }
  appmode=0;
  btncounter++;
}

//////////////////////////////////////////////////////////////////////////////

void Open_AP_Scanner(){
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(1000);
  
  byte openaps = 0;

  int n = WiFi.scanNetworks();
  mydisp.clearScreen();
  mydisp.setTextColor(WHITE);
  mydisp.setTextScale(1);
  

  if (n == 0){
    mydisp.println(F("No SSIDs detected :("));
  }
  else {
    for (int i=0; i<n; i++){
      if (WiFi.encryptionType(i) == WIFI_AUTH_OPEN)
      {
        openaps = 1;
        break;
      }
    }
  }

  if (openaps)
  {
    for (int i=0; i<n; i++){
      if (WiFi.encryptionType(i) == WIFI_AUTH_OPEN)
      {
        mydisp.print(WiFi.RSSI(i));
        mydisp.print(F(" | "));
        mydisp.println(WiFi.SSID(i));
      }
    }
  }
  else {
    mydisp.println(F("No Open APs detected :("));
  }
  
}
