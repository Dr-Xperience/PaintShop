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

#include<cmath>

void drawHollowCircle(GLfloat x, GLfloat y, GLfloat radius)
{
    radius=abs(radius);

    int i;
    int lineAmount = 500; //# of triangles used to draw circle

    //GLfloat radius = 0.8f; //radius
    GLfloat twicePi = 2.0f * (22.0d/7.0d);

    glBegin(GL_LINE_LOOP);
    for(i = 0; i <= lineAmount; i++)
    {
        if(((y + (radius* sin(i * twicePi / lineAmount))) ) > -250)
        glVertex2f( (x + (radius * cos(i *  twicePi / lineAmount))) ,(y + (radius* sin(i * twicePi / lineAmount))) );
    }
    glEnd();
}
class Render
{

    bool isFill,isSphere,isRectangle,isLine;
    int xStart,yStart;
    int xEnd,yEnd;
    float fillColor[3],oldColor[3];

public:
    Render()
    {
        isFill=false;
        isSphere=false;
        isRectangle=false;
        isLine=false;
        fillColor[0]=0.0f;
        fillColor[1]=0.0f;
        fillColor[2]=0.0f;
    }
    void setColor(float r,float g, float b);
    void fill(int x, int y,float oldColor[3]);
    void sphere(int xS,int yS,int xE,int yE);
    void rectangle(int xS,int yS,int xE,int yE);
    void line(int xS,int yS,int xE,int yE);
    void draw(void);
};

void Render::setColor(float r,float g, float b)
{
    fillColor[0]=r;
    fillColor[1]=g;
    fillColor[2]=b;
}
void Render::fill(int x, int y,float oColor[3])
{
    xStart=x;
    yStart=y;
    oldColor[0]=oColor[0];
    oldColor[1]=oColor[1];
    oldColor[2]=oColor[2];
    isFill=true;
    //glutPostRedisplay();
}

void Render::sphere(int xS, int yS, int xE, int yE)
{
    xStart=xS;
    yStart=yS;
    xEnd=xE;
    yEnd=yE;
    isSphere=true;
}

void Render:: rectangle(int xS, int yS, int xE, int yE)
{
    xStart=xS;
    yStart=yS;
    xEnd=xE;
    yEnd=yE;
    isRectangle=true;
}

void Render:: line(int xS, int yS, int xE, int yE)
{
    xStart=xS;
    yStart=yS;
    xEnd=xE;
    yEnd=yE;
    isLine=true;
}


void Render::draw(void)
{
    glColor3fv(fillColor);


    glBegin(GL_QUADS);
    glVertex2i(-300,-250);
    glVertex2i(-200,-250);
    glVertex2i(-200,-300);
    glVertex2i(-300,-300);
    glEnd();

    glLineWidth(3);

    glColor3f(0.5,0.5,0.5);
    glBegin(GL_LINE_STRIP);
    glVertex2i(-300,-250);
    glVertex2i(-200,-250);
    glVertex2i(-200,-300);
    glEnd();

    glLineWidth(1);
    glColor3fv(fillColor);



    if(isFill)
    {
        Obj->fastFill(xStart,yStart,fillColor,oldColor);
        isFill=false;
    }

    if(isSphere)
    { int xCod,yCod,rad;

        if(xEnd==xStart)
        {

            drawHollowCircle(xStart,yStart-((yStart-yEnd)/2),((yStart-yEnd)/2));

        }
        else if(yStart==yEnd)
        {
            drawHollowCircle((  xStart+((xEnd-xStart)/2) )   ,yStart,( (xEnd-xStart)/2 ));
        }
        else
        {
            if( abs((xEnd-xStart)/2) > abs((yStart-yEnd)/2) )
            {

                drawHollowCircle( (  xStart+((xEnd-xStart)/2) ),yStart-((yStart-yEnd)/2),(xEnd-xStart)/2)  ;
            }
            else
            {
                drawHollowCircle( (  xStart+((xEnd-xStart)/2) ),yStart-((yStart-yEnd)/2),(yStart-yEnd)/2) ;
            }
        }
        isSphere=false;
    }

    if(isRectangle)
    {
        if(yEnd==yStart)
        {
            glBegin(GL_LINE_LOOP);
            glVertex2f(xStart ,yStart );
            glVertex2f(xEnd ,yEnd );
            glVertex2f(xEnd ,(yEnd-50.0f) );
            glVertex2f(xStart ,(yStart-50.0f) );
            glEnd();

        }
        else if(xEnd==xStart)
        {
            glBegin(GL_LINE_LOOP);
            glVertex2f(xStart ,yStart );
            glVertex2f((xEnd-50.0f) ,yEnd );
            glVertex2f((xEnd-50.0f) ,yEnd );
            glVertex2f(xStart ,yStart );
            glEnd();
        }
        else
        {
            glBegin(GL_LINE_LOOP);
            glVertex2f(xStart ,yStart );
            glVertex2f(xEnd ,yStart );
            glVertex2f(xEnd ,yEnd );
            glVertex2f(xStart ,yEnd );
            glEnd();

        }

        isRectangle=false;
    }

    if(isLine)
    {

        glBegin(GL_LINES);
        glVertex2f(xStart ,yStart );
        glVertex2f(xEnd ,yEnd );
        glEnd();
        isLine=false;
    }
}

