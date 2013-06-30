#include <windows.h>
#include "FreeImagePlus.h"
#include "GL/glut.h"
#include "GL/glext.h"
#include <stdlib.h>
#include <vector>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdarg.h>
#include <string>
#include "Vector3D.h"
#include "ArcBall.h"
#include "FreeType.h"
#include "Perlin.h"

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

const GLfloat piover180 = 0.0174532925f;
const double pi = 3.141592653;

const int CloudCover = 125;
const float CloudSharpness = 0.95;
const float persistence = 0.125;

PFNGLMULTITEXCOORD1FARBPROC        glMultiTexCoord1fARB    = NULL;
PFNGLMULTITEXCOORD2FARBPROC        glMultiTexCoord2fARB    = NULL;
PFNGLACTIVETEXTUREARBPROC          glActiveTextureARB      = NULL;
PFNGLCLIENTACTIVETEXTUREARBPROC    glClientActiveTextureARB= NULL;

std::vector<GLuint> listname;
std::vector<GLuint> texture;
GLfloat xRot = 0;
GLfloat yRot = 0;
GLfloat zoom = -5;
GLuint base;
HDC hDC = NULL;
HWND hWnd = NULL;
int Window_Height = 480;
int Window_Width = 640;
bool keystate[256];

char FPS[1000];

ArcBall_t* arc;
freetype::font_data our_font;

Matrix4fT   Transform   = {  1.0f,  0.0f,  0.0f,  0.0f,
                             0.0f,  1.0f,  0.0f,  0.0f,
                             0.0f,  0.0f,  1.0f,  0.0f,
                             0.0f,  0.0f,  0.0f,  1.0f };

Matrix3fT   LastRot     = {  1.0f,  0.0f,  0.0f,
                             0.0f,  1.0f,  0.0f,
                             0.0f,  0.0f,  1.0f };

Matrix3fT   ThisRot     = {  1.0f,  0.0f,  0.0f,
                             0.0f,  1.0f,  0.0f,
                             0.0f,  0.0f,  1.0f };
Point2fT    MousePt;

bool        isClicked  = false;
bool        isRClicked = false;
bool        isDragging = false;



void ShowFPS()
{
    glPushMatrix();
    glLoadIdentity();
    glColor3f(0.0,1.0,0.0);

    static float framesPerSecond    = 0.0f;
    static float lastTime           = 0.0f;

    float currentTime = GetTickCount() * 0.001f;
    ++framesPerSecond;
    /*if( currentTime - lastTime > 1.0f )
    {
        lastTime = currentTime;
        sprintf(FPS, "FPS: %d", int(framesPerSecond));
        framesPerSecond = 0;
    }*/
    //sprintf(FPS, "FPS: %.5f", PerlinNoise_2D(1000,3.0));
    //freetype::print(our_font, 0,460, FPS);
    glPopMatrix();
}

static void makelist()
{
/*/------------------------------------------------------------//
    GLuint temp;
    temp = glGenLists(1);
    listname.push_back(temp);
    glNewList(listname[listname.size()-1],GL_COMPILE);

    glBindTexture(GL_TEXTURE_2D, texture[0] );
    glEnable( GL_TEXTURE_2D );

    glEndList();
//------------------------------------------------------------/*/
}

static void reshape(int width, int height)
{
    glViewport( 0, 0, (GLint)width, (GLint)height );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 45.0, (GLfloat)width/(GLfloat)height, 0.1, 100.0 );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}

void loadGLTexture(std::string filename)
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

void gentexture()
{
    int textureWidth = 256;
    int textureHeight = 256;

    static GLubyte textureColors [256][256][4];
    for(int i=0;i<textureHeight;i++)
    {
        for(int j=0;j<textureWidth;j++)
        {
            float c256 = ((Perlin::PerlinNoise_2D((float)i,(float)j,persistence)+1.0)/2)*255;
            float c128 = ((Perlin::PerlinNoise_2D((float)i/2,(float)j/2,persistence)+1.0)/2)*255;
            float c64 = ((Perlin::PerlinNoise_2D((float)i/4,(float)j/4,persistence)+1.0)/2)*255;
            float c32 = ((Perlin::PerlinNoise_2D(((float)i)/8,((float)j)/8,persistence)+1.0)/2)*255;
            int c = (2.0/3)*(c32+c64/4+c128/8+c256/8);
            c -= CloudCover;
            if(c<0) c=0;
            c = 255 - (pow(CloudSharpness,c) * 255) ;

            textureColors[i][j][0] = (GLubyte)255;
            textureColors[i][j][1] = (GLubyte)255;
            textureColors[i][j][2] = (GLubyte)255;
            textureColors[i][j][3] = (GLubyte)c;
        }
    }
    GLuint i;
    glGenTextures( 1, &i );
    glBindTexture( GL_TEXTURE_2D, i );
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0,GL_RGBA, GL_UNSIGNED_BYTE, textureColors );
    texture.push_back(i);

}

static void loadGLTextures()
{
}

static void idle(void)
{
    glutPostRedisplay();
}

void testkey()
{
    if(keystate['Q']==true||keystate['q']==true||keystate[27]==true)exit(0);
    if(keystate['D']==true||keystate['d']==true)
    {
        yRot+=2;
    }
    if(keystate['A']==true||keystate['a']==true)
    {
        yRot-=2;
    }
    if(keystate['W']==true||keystate['w']==true)
    {
        xRot-=2;
    }
    if(keystate['S']==true||keystate['s']==true)
    {
        xRot+=2;
    }
    if(keystate['O']==true||keystate['o']==true)
    {
        zoom++;
    }
    if(keystate['P']==true||keystate['p']==true)
    {
        zoom--;
    }
}

static void keydown(unsigned char key, int x, int y)
{
    if(keystate[key]==false)keystate[key]=true;


}

static void keyup(unsigned char key, int x, int y)
{
    if(keystate[key]==true)keystate[key]=false;

}

void testdrag()
{
    if (isRClicked)													// 如果右键按下，这重置所有的变量
    {
		Matrix3fSetIdentity(&LastRot);
		Matrix3fSetIdentity(&ThisRot);
        Matrix4fSetRotationFromMatrix3f(&Transform, &ThisRot);
    }

    if (!isDragging)
    {
        if (isClicked)
        {
			isDragging = true;
			LastRot = ThisRot;
			arc->click(&MousePt);
        }
    }
    else
    {
        if (isClicked)
        {
            Quat4fT     ThisQuat;

            arc->drag(&MousePt, &ThisQuat);
            Matrix3fSetRotationFromQuat4f(&ThisRot, &ThisQuat);
            Matrix3fMulMatrix3f(&ThisRot, &LastRot);
            Matrix4fSetRotationFromMatrix3f(&Transform, &ThisRot);
        }
        else
            isDragging = false;
    }
}


void mouse(int button,int state,int x,int y)
{

    if(button==GLUT_LEFT_BUTTON&&state==GLUT_UP)
    {
        isClicked = false;
    }
    else if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN)
    {
        isClicked = true;
    }
    else if(button==GLUT_RIGHT_BUTTON&&state==GLUT_UP)
    {
        isRClicked = false;
    }
    else if(button==GLUT_RIGHT_BUTTON&&state==GLUT_DOWN)
    {
        isRClicked = true;
    }

    MousePt.s.X = (GLfloat)x;
    MousePt.s.Y = (GLfloat)y;
}

void mousemove(int x,int y)
{
    MousePt.s.X = (GLfloat)x;
    MousePt.s.Y = (GLfloat)y;
}

static void display(void)
{
//-----------------------------------------------------------//
    testkey();
    testdrag();
//-----------------------------------------------------------//
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glTranslatef(0.0,0.0, zoom );
    glRotatef( xRot,  1.0,  0.0,  0.0 );
    glRotatef( yRot,  0.0,  1.0,  0.0 );
    glMultMatrixf(Transform.M);

//-----------------------------------------------------------//


    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glPushMatrix();
    //glScalef(2.0,1.0,1.0);
    glBegin(GL_QUADS);
    glColor4f(0.2,0.2,0.2,1.0);glVertex3f(-1.0, -1.0, 1.0);
    glColor4f(0.2,0.2,0.2,1.0);glVertex3f(1.0, -1.0,  1.0 );
    glColor4f(0.0,0.0,1.0,1.0);glVertex3f(1.0,  1.0,  1.0);
    glColor4f(0.0,0.0,1.0,1.0);glVertex3f(-1.0,  1.0,  1.0);
    glEnd();
    glPopMatrix();
//------------------------------------------------------------//

    glEnable(GL_LIGHTING);
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    glEnable(GL_TEXTURE_2D);
    //glScalef(2.0,1.0,1.0);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);glVertex3f(-1.0, -1.0, 1.0);
    glTexCoord2f(1.0, 0.0);glVertex3f(1.0, -1.0,  1.0 );
    glTexCoord2f(1.0, 1.0);glVertex3f(1.0,  1.0,  1.0);
    glTexCoord2f(0.0, 1.0);glVertex3f(-1.0,  1.0,  1.0);
    glEnd();
    glPopMatrix();




//----------------------平面数据处理-------------------------//

    glMatrixMode( GL_PROJECTION );
    glDisable(GL_LIGHTING);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0,Window_Width,Window_Height,0.0);
    glMatrixMode( GL_MODELVIEW );

    ShowFPS();

    glMatrixMode( GL_PROJECTION );
    glPopMatrix();
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

//------------------------------------------------------------//
    glutSwapBuffers();
}


int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(Window_Width,Window_Height);
    glutInitWindowPosition(100,100);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );

    glutCreateWindow("GLUT");

    hWnd = FindWindow(NULL,"GLUT");
    hDC = GetDC(hWnd);

    glMultiTexCoord1fARB    = (PFNGLMULTITEXCOORD1FARBPROC)        wglGetProcAddress("glMultiTexCoord1fARB");
    glMultiTexCoord2fARB    = (PFNGLMULTITEXCOORD2FARBPROC)        wglGetProcAddress("glMultiTexCoord2fARB");
    glActiveTextureARB        = (PFNGLACTIVETEXTUREARBPROC)        wglGetProcAddress("glActiveTextureARB");
    glClientActiveTextureARB= (PFNGLCLIENTACTIVETEXTUREARBPROC)    wglGetProcAddress("glClientActiveTextureARB");

    memset(keystate,0,sizeof(keystate));

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keydown);
    glutKeyboardUpFunc(keyup);
    glutMouseFunc(mouse);
    glutMotionFunc(mousemove);
    glutIdleFunc(idle);

    loadGLTextures();
    glShadeModel( GL_SMOOTH );
    glShadeModel( GL_SMOOTH );
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);

    glClearColor(0.0f,0.0f,0.0f,0.0f);

    glLightfv( GL_LIGHT1, GL_AMBIENT, lightAmbient );
    glLightfv( GL_LIGHT1, GL_DIFFUSE, lightDiffuse );
    glLightfv( GL_LIGHT1, GL_POSITION, lightPosition );

    glEnable( GL_LIGHT1 );

    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    /*glFogi(GL_FOG_MODE, GL_EXP);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_DENSITY, 0.35f);
    glHint(GL_FOG_HINT, GL_DONT_CARE);
    glFogf(GL_FOG_START, 1.0f);
    glFogf(GL_FOG_END, 5.0f);
    glEnable(GL_FOG);*/

    glClearDepth( 1.0 );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );


    makelist();
    gentexture();
    FreeImage_Initialise();
    our_font.init("resources\\fonts\\test.TTF", 16);
    arc = new ArcBall_t(640.0f, 480.0f);

    glutMainLoop();

    return EXIT_SUCCESS;
}
