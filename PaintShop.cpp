/****************************************************************************
    PaintShop (-- Orhto Projection)
    Copyright (C) 2012  Anubhav Arun <dr.xperience@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*****************************************************************************/

/** *******************************************************S'e'M****************************************************************
*   TITLE       :   PAINT SHOP ( SINGLE COMPILED FILE ) -- Orhto Projection
*   DEVELOPER   :   ASUKA ZONE , DR.XPERIEINCE
*   DATE/TIME   :   AD 2012:04:21:15:34:25
*   IDE         :   Code::Blocks 12.11
*   COMPILER    :   GCC/G++ 4.7.1
* ****************************************************************************************************************************** */

#include"Class/FloodFill.cpp"

#include<iostream>
#include<cstdlib>

GLubyte CheckImage[600][600][3];
static GLuint PixelBuffer;
Floodfill *Obj= new Floodfill(600,600,&PixelBuffer);

#include"Class/Render.cpp"
Render *Draw= new Render();

#include"Class/Event.cpp"



void InitBuffer(void)
{
    glewInit();

    for (int i = 0; i < 600; i++)
    {
        for (int j = 0; j < 600; j++)
        {
            CheckImage[i][j][0] = (GLubyte) 255;
            CheckImage[i][j][1] = (GLubyte) 255;
            CheckImage[i][j][2] = (GLubyte) 255;
        }
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 0);

    ///glGenBuffers generates a Buffer and assigns it to the variable given in its second argument
    glGenBuffers(1, &PixelBuffer);

    /// glBindBuffer binds the buffer referenced by PixelBuffer variable with actual PIXEL BUFFER
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, PixelBuffer);

    /// glBuffer data fills the buffer referenced by PixelBuffer with checkImage array
    glBufferData(GL_PIXEL_UNPACK_BUFFER,
                 3*600*600,
                 CheckImage, GL_STATIC_DRAW);

}

void DrawBuffer(void)
{
    /// Following Code is responsible for replacing the contents of PIXEL BUFFER with the contents of checkImage

    /// glRasterPos2i shifts the pointer to the raster position pointed by the pointer
    glRasterPos2i(-300, -300);

    /// glBindBuffer binds the buffer referenced by PixelBuffer variable with actual PIXEL BUFFER
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, PixelBuffer);

    /// glDrawPixels draws the texture/image to the screen
    /// in this case current buffer = buffer referenced by PixelBuffer variable
    ///              image = BUFFER_OFFSET(0) which is macro
    /// actually its just flushing the buffer on screen
    glDrawPixels(600, 600, GL_RGB,
                 GL_UNSIGNED_BYTE, BUFFER_OFFSET(0));

}

void UpdateBuffer(void)
{
    /// CODE TO READ ENTIRE SCREEN PIXELS AND STORE THEM IN checkImage ARRAY
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    glReadPixels(0, 0, 600,600, GL_RGB, GL_UNSIGNED_BYTE, CheckImage);

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, PixelBuffer);
    glBufferData(GL_PIXEL_UNPACK_BUFFER,3*600*600,CheckImage, GL_STATIC_DRAW);

}
void MouseButton(int button,int state,int x, int y)
{
    /*
     *	has the button been pressed or released?
     */
    if (state == GLUT_DOWN)
    {
        /*
         *	Which button was pressed?
         */
        switch(button)
        {
        case GLUT_LEFT_BUTTON:
            MEvent->mousePressed(x,y,true,false,false);
            break;
        case GLUT_MIDDLE_BUTTON:
            MEvent->mousePressed(x,y,false,true,false);
            break;
        case GLUT_RIGHT_BUTTON:
            MEvent->mousePressed(x,y,false,false,true);
            break;
        }

    }
    else
    {
        /*
         *	Which button was released?
         */
        switch(button)
        {
        case GLUT_LEFT_BUTTON:
            MEvent->mouseReleased(x,y,true,false,false);
            break;
        case GLUT_MIDDLE_BUTTON:
            MEvent->mouseReleased(x,y,false,true,false);
            break;
        case GLUT_RIGHT_BUTTON:
            MEvent->mouseReleased(x,y,false,true,false);
            break;
        }

    }

    /*
     *	Force a redraw of the screen
     */
    glutPostRedisplay();
}
void MouseMotion(int x, int y)
{
    MEvent->mouseMotion(x,y);
    glutPostRedisplay();
}

void MousePassiveMotion(int x, int y)
{
    MEvent->mousePassiveMotion(x,y);
}
bool IsFirst=true;

void RenderScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1.0f,1.0f,1.0f,0.0f);

    DrawBuffer();
    if(IsFirst==true)
    {

        glColor3f(0.1,0.1,0.1);
        glBegin(GL_LINE_LOOP);
        glVertex2f(-200.0f,300.0f);
        glVertex2f(300.0f,300.0f);
        glVertex2f(300.0f,-250.0f );
        glVertex2f(-200.0f,-250.0f );
        glEnd();
        glFlush();

        IsFirst=false;
    }

//
    ButtonDraw();
//
    Draw->draw();


    UpdateBuffer();
    DrawBuffer();

    glFlush();
}
void ChangeSize(int w, int h)
{

    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if (h == 0)
        h = 1;

    float ratio =  w * 1.0 / h;

    // Use the Projection Matrix
    glMatrixMode(GL_PROJECTION);

    // Reset Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set the correct perspective.

    glOrtho(-300.0,300.0,-300.0,300.0,-300.0,300.0);
//    glOrtho(0,600,0,600,-1,1);

    // Get Back to the Modelview
    glMatrixMode(GL_MODELVIEW);
}
int main(int argc,char* argv[])
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowPosition(1,1);
    glutInitWindowSize(600,600);
    glutCreateWindow("Paint Shop");


    InitButton();

    InitBuffer();

    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutMouseFunc(MouseButton);
    glutMotionFunc(MouseMotion);
    glutPassiveMotionFunc(MousePassiveMotion);


    glutMainLoop();

    return EXIT_SUCCESS;
}
