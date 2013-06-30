#ifndef BUTTON_H
#define BUTTON_H
#include "Control.h"

class Button:public Control
{
private:
    std::string shape;

public:
    Button(float x=0.0f,float y=0.0f,float r=0.0f,float g=0.0f,float b=0.0f,std::string s="Quad",float h=0.0f,float w=0.0f,std::string t="");
    void setshape(std::string s,float x,float y=0.0f);
    void setradius(float x);
    int getshape();
    ~Button(){};
};



#endif // BUTTON_H
