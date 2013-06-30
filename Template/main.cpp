#include "Render.h"

int main(int argc, char *argv[])
{
    Render* r = new Render();
    r->Init(argc,argv);

    Button* bt1 = r->CreateButton(200,200);
    bt1->setshape("Circle",100);
    bt1->setcolor(1,0,0);
    r->MainLoop();
    return 0;

}



