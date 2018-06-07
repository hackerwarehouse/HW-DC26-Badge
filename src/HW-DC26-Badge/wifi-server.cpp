#include <WiFi.h>
#include "FS.h"
#include "SPIFFS.h"

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

extern void SaveSettings();

//////////////////////////////////////////////////////////////////////////////////

String APssid;
String APpassword;

void APDisplay()
{
  mydisp.clearScreen();
  mydisp.setCursor(0, 0);
  mydisp.setTextColor(WHITE);
  mydisp.setTextScale(1);
  mydisp.println(F("Connect to..."));
  mydisp.print(F("ssid: "));
  mydisp.println(APssid);
  mydisp.print(F("password: "));
  mydisp.println(APpassword);
}

void APStart() { 
  //APssid = "HW-1234";
  APssid = "HW-";
  APssid += random(0,9);
  APssid += random(0,9);
  APssid += random(0,9);
  APssid += random(0,9);
  //APpassword = "badge1234";
  APpassword = "badge";
  APpassword += random(0,9);
  APpassword += random(0,9);
  APpassword += random(0,9);
  APpassword += random(0,9);
  APpassword += random(0,9);

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(IPAddress(10,0,5,1), IPAddress(10,0,5,1), IPAddress(255,255,255,0));
  WiFi.softAP(APssid.c_str(), APpassword.c_str(), 9, 0);
}

void APStop(){
  WiFi.softAPdisconnect(1);  
}
//////////////////////////////////////////////////////////////////////////////////

// webserver functions remix of https://github.com/spacehuhn/wifi_ducky

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFSEditor.h>
#include <Update.h>
#include <FS.h>

#include "update.h"
#include "hidarsenal.h"

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
AsyncEventSource events("/events");

char *multi_tok(char *input, char *delimiter) {
    static char *string;
    if (input != NULL)
        string = input;

    if (string == NULL)
        return string;

    char *end = strstr(string, delimiter);
    if (end == NULL) {
        char *temp = string;
        string = NULL;
        return temp;
    }

    char *temp = string;

    *end = '\0';
    string = end + strlen(delimiter);
    return temp;
}

void WebServerStart()
{
  server.addHandler(&ws);

  events.onConnect([](AsyncEventSourceClient *client){
    client->send("hello!",NULL,millis(),1000);
  });
  //server.addHandler(&events);

  server.addHandler(new SPIFFSEditor(SPIFFS,APssid.c_str(),APpassword.c_str()));

  server.on("/heap", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(ESP.getFreeHeap()));
  });

  server.on("/update", HTTP_POST, [](AsyncWebServerRequest *request){
    if(request->hasArg("confirm")) {
      bool confirm = request->arg("confirm");
      if (confirm) 
      {
        request->send(200, "text/plain", "Updating...");
        updateFromFS(SPIFFS);  
      }
    }
    else {
      AsyncWebServerResponse *response = request->beginResponse(302, "text/plain", "");
      response->addHeader("Location", "/");
      request->send(response);
    }
  });

  server.on("/runscript", HTTP_POST, [](AsyncWebServerRequest *request) {
    if(request->hasArg("name")) {
      String file = request->arg("name");
      if (file != "" and SPIFFS.exists(file)) {
        request->send(200, "text/plain", "true");
        runpayload(file);
      }
      else {
        request->send(200, "text/plain", "Nothing to run");
      }
    }
    else {
      request->send(200, "text/plain", "Nothing to run");
    }
  });

  server.on("/runtext", HTTP_POST, [](AsyncWebServerRequest *request) {
    if(request->hasArg("text")) {
      request->send(200, "text/plain", "Running");
      Serial.println(request->arg("text"));

      char input[1024];
      int defaultdelay = 2000;
      strcpy(input, request->arg("text").c_str());
      char *token = multi_tok(input, "@@@");
      while (token != NULL) {
        char *i;
        String line = token;
        String cmd;
        String cmdinput;
        cmd = String(strtok_r(token," ",&i));

        if(cmd == "REM") { } // nothing
        else if(cmd == "DEFAULTDELAY" || cmd == "DEFAULT_DELAY") {
          cmdinput = String(strtok_r(NULL," ",&i));
          String newdefaultdelay = cmdinput;
          defaultdelay = newdefaultdelay.toInt();
        }
        else if(cmd == "DELAY") {
          cmdinput = String(strtok_r(NULL," ",&i));
          String newdefaultdelay = cmdinput;
          int tempdelay = newdefaultdelay.toInt();
          delay(tempdelay);
        }
        else {
          Serial.println(line);
        }
        delay(defaultdelay); //delay between lines in payload, I found running it slower works best
        
        token = multi_tok(NULL, "@@@");
      }
    }
    else {
      request->send(200, "text/plain", "Nothing to run");
    }
  });

  //for spiffseditor
  server.serveStatic("/ace.js.gz", SPIFFS, "/www/ace.js.gz");
  server.serveStatic("/ext-searchbox.js.gz", SPIFFS, "/www/ext-searchbox.js.gz");
  server.serveStatic("/mode-css.js.gz", SPIFFS, "/www/mode-css.js.gz");
  server.serveStatic("/mode-html.js.gz", SPIFFS, "/www/mode-html.js.gz");
  server.serveStatic("/mode-javascript.js.gz", SPIFFS, "/www/mode-javascript.js.gz");
  server.serveStatic("/worker-html.js.gz", SPIFFS, "/www/worker-html.js.gz");

  server.serveStatic("/functions.js", SPIFFS, "/www/functions.js");
  server.serveStatic("/skeleton.min.css", SPIFFS, "/www/skeleton.min.css");
  server.serveStatic("/style.css", SPIFFS, "/www/style.css");

  server.serveStatic("/win.json", SPIFFS, "/win.json");
  server.serveStatic("/lin.json", SPIFFS, "/lin.json");

  server.serveStatic("/WebSocketTester.html", SPIFFS, "/www/WebSocketTester.html").setAuthentication(APssid.c_str(), APpassword.c_str());
  server.serveStatic("/update.html", SPIFFS, "/www/update.html").setAuthentication(APssid.c_str(), APpassword.c_str());
  server.serveStatic("/storedpayloads.html", SPIFFS, "/www/storedpayloads.html").setAuthentication(APssid.c_str(), APpassword.c_str());
  server.serveStatic("/livepayload.html", SPIFFS, "/www/livepayload.html").setAuthentication(APssid.c_str(), APpassword.c_str());
  server.serveStatic("/liveinput.html", SPIFFS, "/www/liveinput.html").setAuthentication(APssid.c_str(), APpassword.c_str());
  //server.serveStatic("/", SPIFFS, "/www/").setDefaultFile("index.html");
  server.serveStatic("/", SPIFFS, "/www/").setDefaultFile("index.html").setAuthentication(APssid.c_str(), APpassword.c_str());

  server.onFileUpload([](AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final){  });
  server.onRequestBody([](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){  });
  
  server.begin();
}

void WebServerStop()
{
  //there is no service stop!
  //server.stop();
}

void WebServerDisplay()
{
  mydisp.clearScreen();
  mydisp.setTextColor(WHITE);
  mydisp.setTextScale(1);
  mydisp.println(F("Browse to 10.0.5.1"));
  mydisp.print(F("u: "));
  mydisp.println(APssid);
  mydisp.print(F("p: "));
  mydisp.println(APpassword);
}

//////////////////////////////////////////////////////////////////////////////////


