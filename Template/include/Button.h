#ifndef BUTTON_H
#define BUTTON_H
#include "Control.h"
#include <string>

class Button:public Control
{
private:
    std::string shape;
    float height;
    float width;
    float radius;


public:
    Button(float x=0.0f,float y=0.0f,float r=0.0f,float g=0.0f,float b=0.0f,std::string s="Quad",float h=0.0f,float w=0.0f);
    void setshape(std::string s,float x,float y=0.0f);
    void setheight(float x);
    void setwidth(float x);
    void setradius(float x);
    float getheight();
    float getwidth();
    float getradius();
    int getshape();
    ~Button(){};
};



#endif // BUTTON_H
