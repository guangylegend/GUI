#include "Control.h"

Control::Control(float x,float y,float r,float g,float b,float h,float w,std::string s)
{
    px=x;
    py=y;
    cr=r;
    cg=g;
    cb=b;
    height=h;
    width=w;
    tstring=s;
    fr=0.0f;
    fg=1.0f;
    fb=0.0f;
    fontsize=16;
}

void Control::setposition(float x,float y)
{
    px=x;
    py=y;
}

void Control::setcolor(float r,float g,float b)
{
    cr=r;
    cg=g;
    cb=b;
}
void Control::setfontcolor(float r,float g,float b)
{
    fr=r;
    fg=g;
    fb=b;
}

void Control::setstring(std::string s)
{
    tstring=s;
}

void Control::setheight(float x)
{
    height=x;
}
void Control::setwidth(float x)
{
    width=x;
}
void Control::setfontsize(int x)
{
    fontsize=x;
}
float Control::getpositionx()
{
    return px;
}
float Control::getpositiony()
{
    return py;
}
float Control::getcolorr()
{
    return cr;
}
float Control::getcolorg()
{
    return cg;
}
float Control::getcolorb()
{
    return cb;
}
float Control::getfontcolorr()
{
    return fr;
}
float Control::getfontcolorg()
{
    return fg;
}
float Control::getfontcolorb()
{
    return fb;
}
std::string Control::getstring()
{
    return tstring;
}
float Control::getheight()
{
    return height;
}
float Control::getwidth()
{
    return width;
}
int Control::getfontsize()
{
    return fontsize;
}
