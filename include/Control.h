#ifndef CONTROL_H
#define CONTROL_H
#include <string>

class Control
{
private:
    float px;
    float py;
    float cr;
    float cg;
    float cb;
    float fr;
    float fg;
    float fb;
    int fontsize;
    float height;
    float width;
    std::string tstring;

public:
    Control(float x=0.0f,float y=0.0f,float r=0.0f,float g=0.0f,float b=0.0f,float h=0.0f,float w=0.0f,std::string s="");
    void setposition(float x,float y);
    void setcolor(float r,float g,float b);
    void setfontcolor(float r,float g,float b);
    void setfontsize(int x);
    void setstring(std::string s);
    void setheight(float x);
    void setwidth(float x);
    float getpositionx();
    float getpositiony();
    float getcolorr();
    float getcolorg();
    float getcolorb();
    float getfontcolorr();
    float getfontcolorg();
    float getfontcolorb();
    float getheight();
    float getwidth();
    int getfontsize();
    std::string getstring();
    virtual ~Control(){};
};



#endif // CONTROL_H
