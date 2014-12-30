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
        if(((y + (radius* sin(i * twicePi / lineAmount)))/300.0f) > -0.833)
        glVertex2f( (x + (radius * cos(i *  twicePi / lineAmount)))/300.0f,(y + (radius* sin(i * twicePi / lineAmount)))/300.0f);
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
    glVertex2f(-300.0f/300.0f,-250.0f/300.0f);
    glVertex2f(-200.0f/300.0f,-250.0f/300.0f);
    glVertex2f(-200.0f/300,-300.0f/300);
    glVertex2f(-300.0f/300,-300.0f/300);
    glEnd();

    glLineWidth(3);

    glColor3f(0.5,0.5,0.5);
    glBegin(GL_LINE_STRIP);
    glVertex2f(-300.0f/300.0f,-250.0f/300.0f);
    glVertex2f(-200.0f/300.0f,-250.0f/300.0f);
    glVertex2f(-200.0f/300,-300.0f/300);
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
            glVertex2f(xStart/300.0f,yStart/300.0f);
            glVertex2f(xEnd/300.0f,yEnd/300.0f);
            glVertex2f(xEnd/300.0f,(yEnd-50.0f)/300.0f);
            glVertex2f(xStart/300.0f,(yStart-50.0f)/300.0f);
            glEnd();

        }
        else if(xEnd==xStart)
        {
            glBegin(GL_LINE_LOOP);
            glVertex2f(xStart/300.0f,yStart/300.0f);
            glVertex2f((xEnd-50.0f)/300.0f,yEnd/300.0f);
            glVertex2f((xEnd-50.0f)/300.0f,yEnd/300.0f);
            glVertex2f(xStart/300.0f,yStart/300.0f);
            glEnd();
        }
        else
        {
            glBegin(GL_LINE_LOOP);
            glVertex2f(xStart/300.0f,yStart/300.0f);
            glVertex2f(xEnd/300.0f,yStart/300.0f);
            glVertex2f(xEnd/300.0f,yEnd/300.0f);
            glVertex2f(xStart/300.0f,yEnd/300.0f);
            glEnd();

        }

//        glTranslatef(-100.0f,-50.0f,0);
        isRectangle=false;
    }

    if(isLine)
    {
//        glTranslatef(100.0f/300.0f,50.0f/300.0f,0);

        glBegin(GL_LINES);
        glVertex2f(xStart/300.0f,yStart/300.0f);
        glVertex2f(xEnd/300.0f,yEnd/300.0f);
        glEnd();
        isLine=false;

//        glTranslatef(-100.0f/300.0f,-50.0f/300.0,0);
    }
}

