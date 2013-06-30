#ifndef RENDER_H
#define RENDER_H
#include "Button.h"
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
#include "FreeType.h"
#include "Perlin.h"

class Render
{
public:
    Render();
    int Init(int argc, char *argv[]);
    void MainLoop();

    Button* CreateButton(GLfloat x,GLfloat y,GLfloat r=0.0f,GLfloat g=0.0f,GLfloat b=0.0f);
private:

    std::vector<GLuint> listname;
    std::vector<GLuint> texture;
    std::vector<Button*> button;
    int Window_Height;
    int Window_Width;
    bool keystate[256];

    freetype::font_data our_font;

    static void reshape();
    static void idle();
    static void display();
    static void keydown(unsigned char key, int x, int y);
    static void keyup(unsigned char key, int x, int y);
    void testkey();
    void loadGLTextures();
    void loadGLTexture(std::string filename);
    static void reshape(int width, int height);
    void makelist();
    void ShowFPS();

    ~Render();
};






#endif // RENDER_H
