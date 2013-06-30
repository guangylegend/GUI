#include "Render.h"

/*/------------------------------------------------------------//
const GLfloat lightAmbient[4] = { 0.5, 0.5, 0.5, 1.0 };
const GLfloat lightDiffuse[4] = { 1.0, 1.0, 1.0, 1.0 };
const GLfloat lightPosition[4] = { 0.0, 0.0, 2.0, 1.0 };

const GLfloat mat_ambient[] ={0.0,0.0,0.1,1.0};
const GLfloat mat_diffuse[] ={0.1,0.5,0.8,1.0};
const GLfloat mat_specular[] ={0.0,0.0,0.0,1.0};
const GLfloat mat_shininess[] ={10.0};
const GLfloat mat_ambient_o[] ={0.2,0.2,0.2,1.0};
const GLfloat mat_diffuse_o[] ={0.8,0.8,0.8,1.0};
const GLfloat mat_specular_o[] ={0.0,0.0,0.0,1.0};
const GLfloat mat_shininess_o[] ={0.0};

const GLfloat fogColor[4]= {0.2f, 0.2f, 0.2f, 0.5f};
//-------------------------------------------------------------/*/

const GLfloat piover180 = 0.0174532925f;
const GLfloat pi = 3.141592653;
Render* renpointer;

Render::Render()
{
    Window_Height = 480;
    Window_Width = 640;
    renpointer=this;
}

Render::~Render()
{
    for(int i=0;i<(int)button.size();i++)delete button[i];
}

void Render::makelist()
{
//------------------------------------------------------------//
    GLuint temp;

    temp = glGenLists(1);
    listname.push_back(temp);
    glNewList(listname[listname.size()-1],GL_COMPILE);
    glBegin(GL_QUADS);
    glVertex3f(0.5,-0.5,0.0);
    glVertex3f(0.5,0.5,0.0);
    glVertex3f(-0.5,0.5,0.0);
    glVertex3f(-0.5,-0.5,0.0);
    glEnd();

    glEndList();

    temp = glGenLists(1);
    listname.push_back(temp);
    glNewList(listname[listname.size()-1],GL_COMPILE);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0,0.0,0.0);
    for(int i=0; i<=64; ++i)glVertex3f(cos(2*pi/64*i),sin(2*pi/64*i),0.0);
    glEnd();

    glEndList();
//------------------------------------------------------------/*/
}

static void Render::reshape(int width, int height)
{
    glViewport( 0, 0, (GLint)width, (GLint)height );
    renpointer->Window_Width=width;
    renpointer->Window_Height=height;
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D(0.0,renpointer->Window_Width,renpointer->Window_Height,0.0);
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}

void Render::loadGLTexture(std::string filename)           //only for reading BMP
{
    FIBITMAP* dib = NULL;
    char s[1000];
    GetCurrentDirectory(1000,s);
    std::string a;
    a.assign(s);
    a.append("\\"+filename);
    //FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
    //fif = FreeImage_GetFileType(a.c_str(),0);
    dib = FreeImage_Load(FIF_BMP,a.c_str(),BMP_DEFAULT);
    FIBITMAP *temp = FreeImage_ConvertTo24Bits(dib);
    FreeImage_Unload(dib);
    dib = temp;
    BYTE *bits = new BYTE[FreeImage_GetWidth(dib) * FreeImage_GetHeight(dib) * 3];
    BYTE *pixels = (BYTE*)FreeImage_GetBits(dib);
    for(int pix=0; pix<FreeImage_GetWidth(dib) * FreeImage_GetHeight(dib); pix++)
    {
        bits[pix*3+0]=pixels[pix*3+2];
        bits[pix*3+1]=pixels[pix*3+1];
        bits[pix*3+2]=pixels[pix*3+0];
    }

    GLuint i;
    glGenTextures( 1, &i );
    glBindTexture( GL_TEXTURE_2D, i );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, FreeImage_GetWidth(dib), FreeImage_GetHeight(dib), 0,GL_RGB, GL_UNSIGNED_BYTE, bits );
    texture.push_back(i);
    FreeImage_Unload(dib);
}

void Render::loadGLTextures()
{
}

static void Render::idle()
{
    glutPostRedisplay();
}

void Render::testkey()
{
    if(keystate['Q']==true||keystate['q']==true||keystate[27]==true)exit(0);
}

static void Render::keydown(unsigned char key, int x, int y)
{
    if(renpointer->keystate[key]==false)renpointer->keystate[key]=true;
}

static void Render::keyup(unsigned char key, int x, int y)
{
    if(renpointer->keystate[key]==true)renpointer->keystate[key]=false;
}


static void Render::display()
{
//-----------------------------------------------------------//
    renpointer->testkey();
//-----------------------------------------------------------//
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
//-----------------------------------------------------------//
    for(int i=0;i<(int)renpointer->button.size();i++)
    {
        glPushMatrix();
        glTranslatef(renpointer->button[i]->getpositionx(),renpointer->button[i]->getpositiony(),0.0);
        glColor3f(renpointer->button[i]->getcolorr(),renpointer->button[i]->getcolorg(),renpointer->button[i]->getcolorb());
        if(renpointer->button[i]->getshape()==0)
        {
            glScalef(renpointer->button[i]->getwidth(),renpointer->button[i]->getheight(),1.0);
            glCallList(renpointer->listname[0]);
        }
        else if(renpointer->button[i]->getshape()==1)
        {
            glScalef(renpointer->button[i]->getheight(),renpointer->button[i]->getheight(),1.0);
            glCallList(renpointer->listname[1]);
        }
        glPopMatrix();

        renpointer->our_font.clean();
        renpointer->our_font.init("resources\\fonts\\test.TTF",renpointer->button[i]->getfontsize());
        glColor3f(renpointer->button[i]->getfontcolorr(),renpointer->button[i]->getfontcolorg(),renpointer->button[i]->getfontcolorb());
        freetype::print(renpointer->our_font,
        renpointer->button[i]->getpositionx()-renpointer->button[i]->getfontsize()*renpointer->button[i]->getstring().size()/3,
        renpointer->Window_Height-renpointer->button[i]->getpositiony()-renpointer->button[i]->getfontsize()/2,
        renpointer->button[i]->getstring().c_str());
    }

//-----------------------------------------------------------//

    glutSwapBuffers();
}


int Render::Init(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(Window_Width,Window_Height);
    glutInitWindowPosition(100,100);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );

    glutCreateWindow("Test");

    memset(keystate,0,sizeof(keystate));

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keydown);
    glutKeyboardUpFunc(keyup);
    glutIdleFunc(idle);

    glShadeModel( GL_SMOOTH );

    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);

    glEnable(GL_BLEND);
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glClearColor(1.0f,1.0f,1.0f,0.0f);

    glClearDepth( 1.0 );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );


    /*glLightfv( GL_LIGHT1, GL_AMBIENT, lightAmbient );
    glLightfv( GL_LIGHT1, GL_DIFFUSE, lightDiffuse );
    glLightfv( GL_LIGHT1, GL_POSITION, lightPosition );

    glEnable( GL_LIGHT1 );*/

    /*glFogi(GL_FOG_MODE, GL_EXP);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_DENSITY, 0.35f);
    glHint(GL_FOG_HINT, GL_DONT_CARE);
    glFogf(GL_FOG_START, 1.0f);
    glFogf(GL_FOG_END, 5.0f);
    glEnable(GL_FOG);*/

    makelist();
    FreeImage_Initialise();
    renpointer->our_font.init("resources\\fonts\\test.TTF", 16);

    return EXIT_SUCCESS;
}

void Render::MainLoop()
{
    glutMainLoop();
}

Button* Render::CreateButton(GLfloat x,GLfloat y,GLfloat r,GLfloat g,GLfloat b)
{
    Button* bt = new Button(x,y,r,g,b,"Quad",20,20,"");
    button.push_back(bt);
    return bt;
}
