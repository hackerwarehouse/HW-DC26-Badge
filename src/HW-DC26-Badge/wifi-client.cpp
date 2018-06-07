#include <WiFi.h>
#include <WiFiUdp.h>
#include <Update.h>

#include <SSD_13XX.h>
#include <NTPClient.h>

#include "core.h"

extern SSD_13XX mydisp;
extern volatile byte btnid;
extern byte appmode;
extern byte btncounter;
extern String ISSID;
extern String IPSWD;
extern int NTP_OFFSET;
extern String GOOGLEAUTH;

extern void menu_force_escape();

//////////////////////////////////////////////////////////////////////////////

void ConnectInternet()
{
  mydisp.clearScreen();
  mydisp.setCursor(0, 0);
  mydisp.setTextColor(GREENYELLOW);
  mydisp.setTextScale(1);

  if (ISSID == NULL)
  {
    mydisp.println(F("No AP defined!"));
    mydisp.println(F("Unable to connect"));
  }
  else
  {      
    mydisp.println(F("Connecting to: "));
    mydisp.println(ISSID);

    WiFi.enableSTA(true);
    unsigned long timeout = millis();
    WiFi.begin(ISSID.c_str(), IPSWD.c_str());
    while (WiFi.status() != WL_CONNECTED) {
        if (millis() - timeout > 20000) {
          mydisp.println(F("failed :("));
          break;
        mydisp.print(F(".."));
        delay(1000);
        }
      }
    if (WiFi.status() != WL_CONNECTED)
     { 
       mydisp.println(F("trying some more...")); 
       timeout = millis();
     }
    while (WiFi.status() != WL_CONNECTED) {
        if (millis() - timeout > 15000) {
          mydisp.println(F("failed :("));
          delay(2000);
          menu_force_escape();
          return;
        }
        delay(1000);
      }
    mydisp.println();
    mydisp.println(F("connected."));
    delay(1000);
    menu_force_escape();
  }
}

//////////////////////////////////////////////////////////////////////////////

WiFiClient client;
int contentLength = 0;
bool isValidContentType = false;
String getHeaderValue(String header, String headerName) {
  return header.substring(strlen(headerName.c_str()));
}

String host = "files.hackerwarehouse.com";
int port = 80; //HTTPS doesn't work.
String bin = "/HW-DC26-Badge.ino.bin";

void execOTA() {  
  mydisp.clearScreen();
  mydisp.setCursor(0, 0);
  mydisp.setTextColor(GREENYELLOW);
  mydisp.setTextScale(1);
  
  if (client.connect(host.c_str(), port)) {
    mydisp.print(F("Fetching Bin: ")); 
    mydisp.println(String(bin));
    client.print(String("GET ") + bin + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Cache-Control: no-cache\r\n" +
                 "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        //Serial.println("Client Timeout !");
        client.stop();
        return;
      }
    }

    // Once the response is available,
    // check stuff

    while (client.available()) {
      String line = client.readStringUntil('\n');
      line.trim();

      if (!line.length()) {
        //headers ended
        break; // and get the OTA started
      }

      // Check if the HTTP Response is 200 else break and Exit Update
      if (line.startsWith("HTTP/1.1")) {
        if (line.indexOf("200") < 0) {
          mydisp.println(F("Got a non 200 status code from server. Exiting OTA Update."));
          //Serial.println("Got a non 200 status code from server. Exiting OTA Update.");
          break;
        }
      }

      // extract headers here
      // Start with content length
      if (line.startsWith("Content-Length: ")) {
        contentLength = atoi((getHeaderValue(line, "Content-Length: ")).c_str());
        //mydisp.print(F("Got "));
        //mydisp.print(String(contentLength));
        //mydisp.println(F(" bytes from server"));
        //Serial.println("Got " + String(contentLength) + " bytes from server");
      }

      // Next, the content type
      if (line.startsWith("Content-Type: ")) {
        String contentType = getHeaderValue(line, "Content-Type: ");
        //Serial.println("Got " + contentType + " payload.");
        if (contentType == "application/octet-stream") {
          isValidContentType = true;
        }
      }
    }
  } else {
    // Connect to host failed
    // May be try?
    // Probably a choppy network?
    mydisp.println(F("Connection to "));
    mydisp.println(String(host));
    mydisp.println(F(" failed. Please check your setup"));
    //Serial.println("Connection to " + String(host) + " failed. Please check your setup");
    // retry??
    // execOTA();
  }

  // Check what is the contentLength and if content type is `application/octet-stream`
  //Serial.println("contentLength : " + String(contentLength) + ", isValidContentType : " + String(isValidContentType));

  // check contentLength and content type
  if (contentLength && isValidContentType) {
    // Check if there is enough to OTA Update
    bool canBegin = Update.begin(contentLength);

    // If yes, begin
    if (canBegin) {
      //Serial.println("Begin OTA. This may take 2 - 5 mins to complete. Things might be quite for a while.. Patience!");
      // No activity would appear on the Serial monitor
      // So be patient. This may take 2 - 5mins to complete
      size_t written = Update.writeStream(client);

      if (written == contentLength) {
        mydisp.print(F("Written : "));
        mydisp.print(String(written));
        mydisp.println(F("  successfully"));
        //Serial.println("Written : " + String(written) + " successfully");
      } else {
        Serial.println("Written only : " + String(written) + "/" + String(contentLength) + ". Retry?" );
        // retry??
        // execOTA();
      }

      if (Update.end()) {
        //Serial.println("OTA done!");
        if (Update.isFinished()) {
          mydisp.println(F("Update successfully completed. Rebooting."));
          //Serial.println("Update successfully completed. Rebooting.");
          ESP.restart();
        } else {
          Serial.println("Update not finished? Something went wrong!");
        }
      } else {
        Serial.println("Error Occurred. Error #: " + String(Update.getError()));
      }
    } else {
      // not enough space to begin OTA - Understand the partitions and space availability
      //Serial.println("Not enough space to begin OTA");
      client.flush();
    }
  } else {
    //Serial.println("There was no content in the response");
    client.flush();
  }
}

void HTTPSysUpdate()
{
  ConnectInternet();
  execOTA();
}

//////////////////////////////////////////////////////////////////////////////

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "time.nist.gov");

void NTPstart()
{
  if (WiFi.status() == WL_CONNECTED)
   { 
     timeClient.begin();
     timeClient.setTimeOffset(NTP_OFFSET);
     mydisp.clearScreen();
     mydisp.println(F("NTP started"));
   }
  else
   {
     mydisp.println(F("Not online."));
     mydisp.println();
     mydisp.println(F("Unable to start"));
     mydisp.println(F("  NTP service"));
   }
  
}

void NTPstop()
{
  timeClient.end();
  mydisp.clearScreen();
  mydisp.println(F("NTP stopped"));
}

void NTPupdate()
{
  timeClient.setTimeOffset(NTP_OFFSET);
  timeClient.update();
}

void NTPdisplay() {
  appmode=1;
  byte count=1;
  byte last=0;
  btnid = 0;
  unsigned long refresh = millis();

  mydisp.clearScreen();
  mydisp.setTextColor(GREENYELLOW);
  mydisp.setTextScale(2);

  if (WiFi.status() == WL_CONNECTED)
   { 
     NTPupdate();
     while (1)
      {
    // make it an actual clock and hardcode diff timezones in there? one custom, then others are hardcoded?
    //    if (btnid == 3){ 
    //      if (count == 1) {count = 6; } 
    //      else {count--;}
    //      btnid = 0;
    //    }
    //    else if (btnid == 2){ 
    //      if (count == 6) {count = 1; } 
    //      else {count++;}
    //      btnid = 0;
    //    }
        if (btnid == 4) {break;}
        if (millis() - refresh < 1000) { delay(100); }
        else 
        { 
          mydisp.clearScreen();
          mydisp.println(timeClient.getFormattedTime());
          refresh = millis(); 
        }
        
      }
   }
  else
   {
     mydisp.setTextScale(1);
     mydisp.println(F("Not online."));
     mydisp.println();
     mydisp.println(F("Unable to fetch"));
     mydisp.println(F("  ntp data."));
   }
  
  appmode=0;
  btncounter++;
}

//////////////////////////////////////////////////////////////////////////////

#include "sha1.h"

// From https://code.google.com/p/google-authenticator/source/browse/libpam/base32.c
int base32_decode(const uint8_t *encoded, uint8_t *result, int bufSize) {
  int buffer = 0;
  int bitsLeft = 0;
  int count = 0;
  for (const uint8_t *ptr = encoded; count < bufSize && *ptr; ++ptr) {
    uint8_t ch = *ptr;
    if (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n' || ch == '-') {
      continue;
    }
    buffer <<= 5;

    // Deal with commonly mistyped characters
    if (ch == '0') {
      ch = 'O';
    } else if (ch == '1') {
      ch = 'L';
    } else if (ch == '8') {
      ch = 'B';
    }

    // Look up one base32 digit
    if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) {
      ch = (ch & 0x1F) - 1;
    } else if (ch >= '2' && ch <= '7') {
      ch -= '2' - 26;
    } else {
      return -1;
    }

    buffer |= ch;
    bitsLeft += 5;
    if (bitsLeft >= 8) {
      result[count++] = buffer >> (bitsLeft - 8);
      bitsLeft -= 8;
    }
  }
  if (count < bufSize) {
    result[count] = '\000';
  }
  return count;
}

void printHash(uint8_t* hash, int keysize) 
{
  int i;
  Serial.println(F("printHash: "));
  for (i=0; i<keysize; i++) {
    Serial.print("0123456789abcdef"[hash[i]>>4]);
    Serial.print("0123456789abcdef"[hash[i]&0xf]);
  }
  Serial.println();
}

void TOTP(uint8_t* hmacKey1, int keysize, bool trig)
{
  timeClient.setTimeOffset(0);
  uint8_t byteArray[8];   
  long t_time = timeClient.getEpochTime() / 30;

  byteArray[0] = 0x00;
  byteArray[1] = 0x00;
  byteArray[2] = 0x00;
  byteArray[3] = 0x00;
  byteArray[4] = (int)((t_time >> 24) & 0xFF) ;
  byteArray[5] = (int)((t_time >> 16) & 0xFF) ;
  byteArray[6] = (int)((t_time >> 8) & 0XFF);
  byteArray[7] = (int)((t_time & 0XFF));

  uint8_t* hash;
  uint32_t a; 
  Sha1.initHmac(hmacKey1,keysize);
  Sha1.writebytes(byteArray, 8);
  hash = Sha1.resultHmac();

  int  offset = hash[20 - 1] & 0xF; 
  long truncatedHash = 0;
  int j;
  for (j = 0; j < 4; ++j) {
    truncatedHash <<= 8;
    truncatedHash  |= hash[offset + j];
  }

  truncatedHash &= 0x7FFFFFFF;
  truncatedHash %= 1000000;

  //Serial.println(truncatedHash);
  mydisp.clearScreen();
  mydisp.println(truncatedHash);

  if (trig) { Serial.println(truncatedHash); trig = 0;}
  
}

void GoogleAuth()
{
  appmode=1;
  btnid = 0;
  mydisp.setTextScale(2);
  
  unsigned long refresh = millis();
  uint8_t keyText[33];
  GOOGLEAUTH.toCharArray((char*)keyText, 33);
  uint8_t key[20];
  base32_decode(keyText, key, 20);
  //uint8_t key[]={ 0xAA, 0x00, 0xDE }; // should be 20 sets of hex if hardcoding, above process takes 32 char google string and convert to proper format
  
  TOTP(key, 20, 0);
  //printHash(key, 20);

  while (btnid != 4)
  {
    if (btnid == 1) {
      Serial.print(F("STRING "));
      TOTP(key, 20, 1);
      btnid = 0;
      }
    else if (btnid == 4) {break;}
    if (millis() - refresh < 3000) { delay(100); }
    else { 
      TOTP(key, 20, 0);
      refresh = millis(); 
    }
  }
  appmode=0;
  btncounter++;
}


//////////////////////////////////////////////////////////////////////////////

void IPConfig(){
  mydisp.clearScreen();
  mydisp.setCursor(0, 0);
  mydisp.setTextColor(GREENYELLOW);
  mydisp.setTextScale(1);
  
  mydisp.println(F("mac addy: "));
  String mac = WiFi.macAddress();
  mydisp.println("  " + mac);
  mydisp.println(F("Conn status: "));
  mydisp.print(F("  "));
  mydisp.println(WiFi.status());
  mydisp.println(F("IP addy: "));
  mydisp.print(F("  "));
  mydisp.println(WiFi.localIP());
}

//////////////////////////////////////////////////////////////////////////////

