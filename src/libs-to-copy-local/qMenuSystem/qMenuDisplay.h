#include <SPI.h>

#include <SSD_13XX.h>
#ifndef qMenuDisplay_h
#define qMenuDisplay_h

class qMenuDisplay
{
  public:
    qMenuDisplay(SSD_13XX *disp);
    qMenuDisplay();
    void Begin();
    void Start();
    void Finish();
    void Title(const char text[]);
    void Item(int index,const char text[]);
    void Highlight(int index);
    void MessageBox(const char text[]);
  private:
	SSD_13XX *_disp;
       int textHeight = 14;
//    char tempBuffer[32];
};




#endif