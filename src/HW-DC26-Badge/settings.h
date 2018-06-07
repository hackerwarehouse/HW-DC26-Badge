#ifndef HW_SETTINGS_H
#define HW_SETTINGS_H

char itmStaticString1[] = "Static 1       ";
char itmStaticString2[] = "Static 2       ";
char itmStaticString3[] = "Static 3       ";
char* mnuStaticStrings[]  = {
  "Static Strings",
  itmStaticString1,itmStaticString2,itmStaticString3
  };            
int cntStaticStrings = 3;
char actualStaticString[3][40];

char itmWinPayload1[] = "Slot 1            ";
char itmWinPayload2[] = "Slot 2            ";
char itmWinPayload3[] = "Slot 3            ";
char itmWinPayload4[] = "Slot 4            ";
char itmWinPayload5[] = "Slot 5            ";
//char itmWinPayload6[] = "Slot 6            ";
//char itmWinPayload7[] = "Slot 7            ";
//char itmWinPayload8[] = "Slot 8            ";
//char itmWinPayload9[] = "Slot 9            ";
//char* mnuWinPayloads[]  = {
//  "Win KB Payloads",
//  itmWinPayload1,itmWinPayload2,itmWinPayload3,itmWinPayload4,itmWinPayload5,itmWinPayload6,itmWinPayload7,itmWinPayload8,itmWinPayload9
//  };
char* mnuWinPayloads[]  = {
  "Win KB Payloads",
  itmWinPayload1,itmWinPayload2,itmWinPayload3,itmWinPayload4,itmWinPayload5
  };
int cntWinPayloads = 5;
char actualWinPayloads[5][40];

char itmLinPayload1[] = "Slot 1            ";
char itmLinPayload2[] = "Slot 2            ";
char itmLinPayload3[] = "Slot 3            ";
char itmLinPayload4[] = "Slot 4            ";
char itmLinPayload5[] = "Slot 5            ";
//char itmLinPayload6[] = "Slot 6            ";
//char itmLinPayload7[] = "Slot 7            ";
//char itmLinPayload8[] = "Slot 8            ";
//char itmLinPayload9[] = "Slot 9            ";
char* mnuLinPayloads[]  = {
  "Lin KB Payloads",
  itmLinPayload1,itmLinPayload2,itmLinPayload3,itmLinPayload4,itmLinPayload5
  };
//char* mnuLinPayloads[]  = {
//  "Lin KB Payloads",
//  itmLinPayload1,itmLinPayload2,itmLinPayload3,itmLinPayload4,itmLinPayload5,itmLinPayload6,itmLinPayload7,itmLinPayload8,itmLinPayload9
//  };
int cntLinPayloads = 5;
//char actualLinPayloads[9][40];
char actualLinPayloads[5][40];

void genStaticStringsMenu();
void genLinPayloadsMenu();
void genWinPayloadsMenu();

void loadSettings();
void rotateScreen180();
void SaveOLEDBrightness(byte a);
void SaveLEDBrightness(byte a);
void ShowLEDBrightness();
void SaveSettings();

#endif
