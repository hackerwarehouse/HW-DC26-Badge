// remix of https://github.com/spacehuhn/wifi_ducky

#include <Keyboard.h>
#include <Mouse.h>

#define PROGPIN 7
byte microdelay = 10; //built in delay no matter what

#define KEY_MENU 0xED

String bufferStr = "";
String last = "";

void RunPayload(String _line)
{
  int firstSpace = _line.indexOf(" ");
  if(firstSpace == -1) {
    Press(_line);
    Keyboard.releaseAll(); ////
  }
  else if(_line.substring(0,firstSpace) == "STRING") {
    for(int i=firstSpace+1;i<_line.length();i++) Keyboard.write(_line[i]);
  }
  else if(_line.substring(0,firstSpace) == "REPLAY" || _line.substring(0,firstSpace) == "REPEAT") {
    int replaynum = _line.substring(firstSpace + 1).toInt();
    while(replaynum)
    {
      RunPayload(last);
      --replaynum;
    }
  } 
  else if(_line.substring(0,firstSpace) == "MOUSEMOVE") {
    int x = _line.substring(firstSpace + 1).toInt();
    int y = _line.substring(firstSpace + 2).toInt();
    Mouse.move(x,y);
  }
  else if(_line.substring(0,firstSpace) == "MOUSESCROLL") {
    int wheel = _line.substring(firstSpace + 1).toInt();
    Mouse.move(0,0,wheel);  
  }
  else if(_line.substring(0,firstSpace) == "REM"){}  //shouldn't hit this scenario b/c taken care of on esp side
  else {
    String remain = _line;
    while(remain.length() > 0) {
      int latest_space = remain.indexOf(" ");
      if (latest_space == -1) {
        Press(remain);
        remain = "";
      }
      else {
        Press(remain.substring(0, latest_space));
        remain = remain.substring(latest_space + 1);
      }
      delay(5);
    }
    Keyboard.releaseAll(); ////
  }

  //Keyboard.releaseAll();
  delay(microdelay);
}

// || b.equals("")
void Press(String b){
  if(b.length() == 1) Keyboard.press(char(b[0]));

  else if (b.equals("ENTER") || b.equals("RETURN")) Keyboard.press(KEY_RETURN);
  else if (b.equals("CTRL") || b.equals("CONTROL")) Keyboard.press(KEY_LEFT_CTRL);
  else if (b.equals("SHIFT")) Keyboard.press(KEY_LEFT_SHIFT);
  else if (b.equals("ALT") || b.equals("ALT_LEFT")) Keyboard.press(KEY_LEFT_ALT);
  else if (b.equals("ALT_RIGHT")) Keyboard.press(KEY_RIGHT_ALT);
  else if (b.equals("GUI") || b.equals("WINDOWS") || b.equals("WIN")) Keyboard.press(KEY_LEFT_GUI);
  else if (b.equals("MENU")) Keyboard.press(KEY_MENU);
  else if (b.equals("UP") || b.equals("UPARROW")) Keyboard.press(KEY_UP_ARROW);
  else if (b.equals("DOWN") || b.equals("DOWNARROW")) Keyboard.press(KEY_DOWN_ARROW);
  else if (b.equals("LEFT") || b.equals("LEFTARROW")) Keyboard.press(KEY_LEFT_ARROW);
  else if (b.equals("RIGHT") || b.equals("RIGHTARROW")) Keyboard.press(KEY_RIGHT_ARROW);
  else if (b.equals("DELETE") || b.equals("DEL")) Keyboard.press(KEY_DELETE);
  else if (b.equals("PAGEUP")) Keyboard.press(KEY_PAGE_UP);
  else if (b.equals("PAGEDOWN")) Keyboard.press(KEY_PAGE_DOWN);
  else if (b.equals("HOME")) Keyboard.press(KEY_HOME);
  else if (b.equals("ESC") || b.equals("ESCAPE")) Keyboard.press(KEY_ESC);
  else if (b.equals("BACKSPACE")) Keyboard.press(KEY_BACKSPACE);
  else if (b.equals("INSERT")) Keyboard.press(KEY_INSERT);
  else if (b.equals("TAB")) Keyboard.press(KEY_TAB);
  else if (b.equals("END")) Keyboard.press(KEY_END);
  else if (b.equals("CAPSLOCK")) Keyboard.press(KEY_CAPS_LOCK);
  else if (b.equals("F1")) Keyboard.press(KEY_F1);
  else if (b.equals("F2")) Keyboard.press(KEY_F2);
  else if (b.equals("F3")) Keyboard.press(KEY_F3);
  else if (b.equals("F4")) Keyboard.press(KEY_F4);
  else if (b.equals("F5")) Keyboard.press(KEY_F5);
  else if (b.equals("F6")) Keyboard.press(KEY_F6);
  else if (b.equals("F7")) Keyboard.press(KEY_F7);
  else if (b.equals("F8")) Keyboard.press(KEY_F8);
  else if (b.equals("F9")) Keyboard.press(KEY_F9);
  else if (b.equals("F10")) Keyboard.press(KEY_F10);
  else if (b.equals("F11")) Keyboard.press(KEY_F11);
  else if (b.equals("F12")) Keyboard.press(KEY_F12);
  else if (b.equals("SPACE")) Keyboard.press(' ');
  
  else if (b.equals("MOUSECLICKLEFT") || b.equals("MOUSE_CLICK_LEFT")) Mouse.click(MOUSE_LEFT);
  else if (b.equals("MOUSECLICKRIGHT") || b.equals("MOUSE_CLICK_RIGHT")) Mouse.click(MOUSE_RIGHT);
  else if (b.equals("MOUSECLICKMIDDLE") || b.equals("MOUSE_CLICK_MIDDLE")) Mouse.click(MOUSE_MIDDLE);
  else if (b.equals("MOUSEPRESSLEFT") || b.equals("MOUSE_PRESS_LEFT")) Mouse.press(MOUSE_LEFT);
  else if (b.equals("MOUSEPRESSRIGHT") || b.equals("MOUSE_PRESS_RIGHT")) Mouse.press(MOUSE_RIGHT);
  else if (b.equals("MOUSEPRESSMIDDLE") || b.equals("MOUSE_PRESS_MIDDLE")) Mouse.press(MOUSE_MIDDLE);
  else if (b.equals("MOUSERELEASELEFT") || b.equals("MOUSE_RELEASE_LEFT")) Mouse.release(MOUSE_LEFT);
  else if (b.equals("MOUSERELEASERIGHT") || b.equals("MOUSE_RELEASE_RIGHT")) Mouse.release(MOUSE_RIGHT);
  else if (b.equals("MOUSERELEASEMIDDLE") || b.equals("MOUSE_RELEASE_MIDDLE")) Mouse.release(MOUSE_MIDDLE);
  
  //else Serial.println("not found :'"+b+"'("+String(b.length())+")");
}


void setup() { 
  pinMode(PROGPIN, OUTPUT);
  digitalWrite(PROGPIN, HIGH);  // change the tx/rx switch to have the atmel communicate with the ESP32

  Serial1.begin(57600);
  Serial.begin(57600);
  Keyboard.begin();
  Mouse.begin();
}

void loop() {  
  while (Serial.available()) {
    String serial_link = Serial.readStringUntil('\n');
          Serial1.println(serial_link);
          //Serial.println("Relaying command to connected ESP device.");
  }
  
  while (Serial1.available()) {
    String payload_line = Serial1.readStringUntil('\n');
    
    if(payload_line.length() > 0){
      payload_line.replace("\r","\n");
      payload_line.replace("\n\n","\n");
    
      while(payload_line.length() > 0){
        int latest_return = payload_line.indexOf("\n");
        if(latest_return == -1){
          RunPayload(payload_line);
          payload_line = "";
          } 
        else {
          RunPayload(payload_line.substring(0, latest_return));
          last=payload_line.substring(0, latest_return);
          payload_line = payload_line.substring(latest_return + 1);
        }
      }
    
    payload_line = "";
    }
  }

}
