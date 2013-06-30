#include "Render.h"

int main(int argc, char *argv[])
{
    Render* r = new Render();
    r->Init(argc,argv);

    Button* bt1 = r->CreateButton(150,200);
    bt1->setshape("Circle",100);
    bt1->setcolor(1,0,0);
    bt1->setfontcolor(0,0,1);
    bt1->setfontsize(20);
    bt1->setstring("Push it!");

    Button* bt2 = r->CreateButton(450,200);
    bt2->setshape("Quad",100,250);
    bt2->setcolor(0,0,1);
    bt2->setfontcolor(1,0,0);
    bt2->setfontsize(20);
    bt2->setstring("Don't Push it!");

    r->MainLoop();
    return 0;

}



