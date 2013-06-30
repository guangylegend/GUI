#ifndef CONTROL_H
#define CONTROL_H

class Control
{
private:
    float px;
    float py;
    float cr;
    float cg;
    float cb;

public:
    Control(float x=0.0f,float y=0.0f,float r=0.0f,float g=0.0f,float b=0.0f);
    void setposition(float x,float y);
    void setcolor(float r,float g,float b);
    float getpositionx();
    float getpositiony();
    float getcolorr();
    float getcolorg();
    float getcolorb();
    virtual ~Control(){};
};



#endif // CONTROL_H
