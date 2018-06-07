#include "FS.h"
#include "SPIFFS.h"

#include <SSD_13XX.h>

#include "core.h"

extern SSD_13XX mydisp;
extern volatile byte btnid;
extern byte appmode;
extern byte btncounter;
extern volatile byte btnid;

extern void menu_force_escape();

////////////////////////////////////////////////////////////////////////////////////////////

void runpayload(String filepath) {
    int defaultdelay = 2000;
    
    File f = SPIFFS.open(filepath, "r");
    
    while(f.available()) {
      mydisp.clearScreen();
      String line = f.readStringUntil('\n');
      //line.replace("&lt;", "<");

      String fullkeys = line;
      int str_len = fullkeys.length();
      char keyarray[str_len+1];
      fullkeys.toCharArray(keyarray, str_len);

      char *i;
      String cmd;
      String cmdinput;
      cmd = String(strtok_r(keyarray," ",&i));
     
      if(cmd == "REM") {
        mydisp.println(line);
      }
      else if(cmd == "DEFAULTDELAY" || cmd == "DEFAULT_DELAY") {
        cmdinput = String(strtok_r(NULL," ",&i));
        String newdefaultdelay = cmdinput;
        defaultdelay = newdefaultdelay.toInt();
        mydisp.println(line);
      }
      else if(cmd == "DELAY") {
        cmdinput = String(strtok_r(NULL," ",&i));
        String newdefaultdelay = cmdinput;
        int tempdelay = newdefaultdelay.toInt();
        mydisp.println(line);
        delay(tempdelay);
      }
      else {
        Serial.println(line);
        mydisp.println(line);
        //mydisp.println(String()+"cmd:"+cmd);
        //mydisp.println(String()+"cmdin:"+cmdinput);
      }

      delay(defaultdelay); //delay between lines in payload, I found running it slower works best
    }
    f.close();
}

void runKBPayload(String file)
{
  mydisp.clearScreen();
  mydisp.setTextColor(GREENYELLOW);
  mydisp.setTextScale(1);

  if (file != "" and SPIFFS.exists(file)) {
    mydisp.println(F("Running:"));
    mydisp.println(file);
    //test if file exists before execute?
    runpayload(file);
    mydisp.clearScreen();
    mydisp.println(F("done"));
    menu_force_escape();
  }
  else {
    mydisp.println(F("Error: "));
    mydisp.println(F("No such file."));
    delay(1500);
    menu_force_escape();
  }
}

void Payload1()
{
  mydisp.clearScreen();
  mydisp.setTextColor(GREENYELLOW);
  mydisp.setTextScale(1);
  mydisp.println(F("Running:"));
  mydisp.println(F("/w/notepad-hello.txt"));
  runpayload("/w/notepad-hello.txt");
  //runpayload("/w/cmd-ipconfig-systeminfo.txt");
  //runpayload("/l/xterm-id-ifconfig.txt");
  //runpayload("/l/xterm-nano-hello.txt");
  mydisp.clearScreen();
  mydisp.println(F("done"));
}

////////////////////////////////////////////////////////////////////////////////////////////

void runStaticString(String payload)
{
  Serial.print(F("STRING "));
  Serial.println(payload);
}

////////////////////////////////////////////////////////////////////////////////////////////

void MouseJiggler()
{
  appmode=1;
  byte count=7;
  byte last=0;
  btnid = 0;

  unsigned long refresh = millis();
  while (btnid != 4)
  {
    if (btnid == 4) {break;}
    
    if (millis() - refresh < 5000) { delay(100); } else { refresh = millis(); Serial.println(F("MOUSEMOVE 10 0"));}
    if (millis() - refresh < 5000) { delay(100); } else { refresh = millis(); Serial.println(F("MOUSEMOVE -10 0")); }
    if (millis() - refresh < 5000) { delay(100); } else { refresh = millis(); Serial.println(F("MOUSEMOVE 0 10")); }
    if (millis() - refresh < 5000) { delay(100); } else { refresh = millis(); Serial.println(F("MOUSEMOVE 0 -10")); }
    
  }

  appmode=0;
  btncounter++;
}

////////////////////////////////////////////////////////////////////////////////////////////


