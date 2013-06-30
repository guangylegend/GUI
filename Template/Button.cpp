#include "Button.h"

Button::Button(float x,float y,float r,float g,float b,std::string s,float h,float w):Control(x,y,r,g,b)
{
    if(s=="Quad")
    {
        height=h;
        width=w;
        shape=s;
    }
    else if(s=="Circle")
    {
        radius=h;
        shape=s;
    }
}
void Button::setshape(std::string s,float x,float y)
{
    if(s=="Quad")
    {
        height=x;
        width=y;
        shape=s;
    }
    else if(s=="Circle")
    {
        radius=x;
        shape=s;
    }
}
void Button::setheight(float x)
{
    height=x;
}
void Button::setwidth(float x)
{
    width=x;
}
void Button::setradius(float x)
{
    radius=x;
}
float Button::getheight()
{
    return height;
}
float Button::getwidth()
{
    return width;
}
float Button::getradius()
{
    return radius;
}
int Button::getshape()
{
    if(shape=="Quad")return 0;
    else if(shape=="Circle")return 1;
    return -1;
}
