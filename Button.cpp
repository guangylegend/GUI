#include "Button.h"

Button::Button(float x,float y,float r,float g,float b,std::string s,float h,float w,std::string t):Control(x,y,r,g,b,0,0,t)
{
    if(s=="Quad")
    {
        shape=s;
        setheight(x);
        setwidth(y);
    }
    else if(s=="Circle")
    {
        shape=s;
        setheight(x);
    }
}
void Button::setshape(std::string s,float x,float y)
{
    if(s=="Quad")
    {
        shape=s;
        setheight(x);
        setwidth(y);
    }
    else if(s=="Circle")
    {
        shape=s;
        setheight(x);
    }
}

void Button::setradius(float x)
{
    setheight(x);
    setwidth(x);
}

int Button::getshape()
{
    if(shape=="Quad")return 0;
    else if(shape=="Circle")return 1;
    return -1;
}
