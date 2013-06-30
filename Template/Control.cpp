#include "Control.h"

Control::Control(float x,float y,float r,float g,float b)
{
    px=x;
    py=y;
    cr=r;
    cg=g;
    cb=b;
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
