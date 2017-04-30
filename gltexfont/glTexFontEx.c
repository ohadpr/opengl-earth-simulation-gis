/*
    Copyright (c) 1999 Nate Miller
    
    Notice: Usage of any code in this file is subject to the rules
    described in the LICENSE.TXT file included in this directory.
    Reading, compiling, or otherwise using this code constitutes
    automatic acceptance of the rules in said text file.

    File        -- glTexFontEx.c
    Date        -- 5/31/99
    Author      -- Nate 'm|d' Miller
    Contact     -- vandals1@home.com
    Web         -- http://members.home.com/vandals1
*/
#include <glTexFont.h>
//#include "glTexFont.h"
#include <stdio.h>
#include <GL/glut.h>

int winW = 640;
int winH = 480;
float rot[3] = {0,0,0};
char *glExten;

float cube[][3] = 
{
	{-10,-10,10},
	{10,-10,10},
	{10,10,10},
	{-10,10,10},
	{-10,-10,-10},
	{10,-10,-10},
	{10,10,-10},
	{-10,10,-10},
	{-10,-10,-10},
	{-10,-10,10},
	{-10,10,10},
	{-10,10,-10},
	{10,-10,-10},
	{10,-10,10},
	{10,10,10},
	{10,10,-10},
	{-10,10,10},
	{10,10,10},
	{10,10,-10},
	{-10,10,-10},
	{-10,-10,10},
	{10,-10,10},
	{10,-10,-10},
	{-10,-10,-10}
};
float colors[][3] = 
{
	{1,0,0},
	{0,1,0},
	{0,0,1},
	{1,1,0},
	{1,0,1},
	{0,1,1}
};

void drawTest (void)
{
	int i;

	glBegin (GL_QUADS);
		for (i = 0; i < 24; i ++)
		{
			glColor3fv (colors[(i % 7)]);
			glVertex3fv (cube[i]);	
		}
	glEnd ();
}
void glutDisplay (void) 
{
    int size;
    float pink[] = {1, 0, 0.5};

    glClearColor (0,0,0,1);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	glTranslatef (0, 0, -50);
	glRotatef (rot[0], 1.0, 0.0, 0.0);
	glRotatef (rot[1], 0.0, 1.0, 0.0);
	glRotatef (rot[2], 0.0, 0.0, 1.0);

    drawTest ();

    /* There is an unreasonable amount of text below */
    fontSize (10);
    fontShadow ();
    fontShadowColor (0.5, 0.5, 0.5);
    fontColor (1,0,0);
    fontDrawString (0,winH - 10,"OpenGL Extensions:");

    fontSize (10);
    fontColor (1,1,1);
    fontRegion (10, winH - 10, 10 * 50, 10 * 10);
    // Please note that position is ignored with a region established 
    fontDrawString (-1,-1,glExten);

    fontSize (14);
    fontGradient ();
    fontGradientColor (1,1,1);
    fontDrawString (0, 0, "\\c(64 128 128)\\i+glTexFont Library Release \\i-\\b+\\c(128 0 128)%d", GL_TEX_FONT_VERSION);
    
    fontSize (20);
    fontShadow ();
    fontShadowColor (0.5,0.5,0.5);
	fontDrawString (winW - 20,winH - 20, "\\c(255 255 0)T\ne\nx\nt\n \n\\c(0 255 128)o\nn\n\\c(0 0 255) \nt\nh\ne\n\\c(0 128 128) \ns\ni\nd\ne\n!");

    size = 12;
    fontShadow ();
    fontSize (size);
    fontColorp (pink);
    fontRegion (winW - (size * 15), (size * 10), (size * 15), (size * 10));
    // Please note that position is ignored with a region established 
    fontDrawString (0,0,"A text region that is 15 chars by 10 chars, neat!");

    fontSize (20);
	fontDrawString (winW / 2 - 100, winH / 2, "\\c(255 255 0)\"\\a(255 0 0 128)glTexFont\\c(255 255 0)\"");

    fontSize (14);
    fontDrawString (0, winH / 2 - 120, "\\c(255 0 0)\\b+Bold\\b-, \\c(0 255 0)\\i+Italic\\i-, \\c(0 0 255)Normal");
    glutSwapBuffers();
}
void glutIdle (void)
{
	int i;

	rot[0] += 0.5;
	rot[1] -= 0.25;
	rot[2] += 1.5;
	
	for (i = 0; i < 3; i ++)
		if (rot[i] > 360 || rot[i] < -360)
			rot[i] = 0;
	glutPostRedisplay ();
}
void glutResize (int w, int h)
{	
	if (!h)
	{
		glutIdleFunc (NULL);
		return;
	}
	
	winW = w;
	winH = h;
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport (0, 0, winW, winH);

	glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();

	gluPerspective (90, winW / winH, 1, 9999);

	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	glutPostRedisplay ();
}
void glInit (void)
{
	glEnable (GL_DEPTH_TEST);
    glExten = (char *) glGetString(GL_EXTENSIONS);
}
void glutVisible (int state)
{
	if (state == GLUT_VISIBLE)
		glutIdleFunc (glutIdle);
}
void glutKeyboard (unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'q':
        case 'Q':
        case 27:
            exit (1);
        break;
    }   
}
void main (void)
{
	glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize (winW,winH);
	glutCreateWindow ("Font Test");
	glutDisplayFunc (glutDisplay);
	glutReshapeFunc (glutResize);
	glutIdleFunc (glutIdle);
	glutVisibilityFunc (glutVisible);
    glutKeyboardFunc (glutKeyboard);
	glInit ();

    // normal font
	//fontLoad ("font.tga");
    // gray scale font
    fontLoad ("fontGray.tga");

	glutMainLoop();
}
