#include <SSD_13XX.h>

#include "core.h"

extern SSD_13XX mydisp;
extern byte appmode;
extern byte btncounter;
extern volatile byte btnid;

void TextInput(String header, String &a, byte maxinputlen){
  appmode=1;
  btnid = 0;

  byte count=0;
  String temp;
  char alphabet[] = {' ', 'A' , 'B' , 'C' , 'D' , 'E' , 'F' , 'G' , 'H' , 'I' , 'J' , 'K' , 'L' , 'M' , 'N' , 'O' , 'P' , 'Q' , 'R' , 'S' ,'T' , 'U' , 'V' , 'W' , 'X' , 'Y' , 'Z' , '1' , '2' ,'3' , '4' , '5' , '6' , '7' , '8' , '9' , '0', '-', '@', '/', '#' };
  byte alphalen = 40;
  
  mydisp.clearScreen();
  mydisp.println(header);

  while (1)
  {
    if (btnid == 0){}
    else
    {
      mydisp.clearScreen();
      mydisp.println(header);
      if (btnid == 3){ 
        mydisp.print(temp);
        if (count == alphalen) { count = 0; }
        else { count++; } 
        mydisp.print(alphabet[count]);
        btnid = 0;
      }
      else if (btnid == 2){ 
        mydisp.print(temp);
        if (count == 0) { count = alphalen; }
        else { count--; } 
        mydisp.print(alphabet[count]);
        btnid = 0;
      }
      else if (btnid == 1){
        temp += alphabet[count];
        count = 0;
        mydisp.print(temp);
        btnid = 0;
      }
      else if (btnid == 4) {
        byte t = temp.length();
        if (t) { temp.remove(t-1); }
        mydisp.print(temp);
        btnid = 0;
      }
      
      if (temp.length() == maxinputlen)
      {
        mydisp.println();
        mydisp.println(F("Save?:"));
        mydisp.println(temp);
        mydisp.println(F("<-= Yes | No =-> "));
        while (1)
        {
          if (btnid == 1){ 
            appmode=0;
            break;
          }
          else if (btnid == 4){ 
            a = temp;
            appmode=0;
            btncounter++;
            btnid = 0;
            break;
          }
          delay(100);
        }
        break;
      }
    }
    delay(100);
  }
}
