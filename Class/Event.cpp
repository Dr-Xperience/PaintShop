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

#include"Button.cpp"

void ButtonPassive(int x,int y);
void ButtonPress(int x,int y);
void ButtonRelease(int x,int y);

class Event
{
protected:
//    bool isFill,isRectangle,isLine,isSphere;

public:
    bool isFill,isRectangle,isLine,isSphere;

    Event()
    {
        isFill=false;
        isRectangle=false;
        isLine=false;
        isSphere=false;

    }

};

class MouseEvent : public Event
{
    int xPressed,yPressed,xRelease,yReleased;
    bool lMB,mMB,rMB;

public:

    void mousePressed(int x,int y,bool l,bool m,bool r);
    void mouseReleased(int x,int y,bool l,bool m,bool r);
    void mouseMotion(int x,int y);
    void mousePassiveMotion(int x, int y);
};

void MouseEvent :: mousePassiveMotion(int x, int y)
{
    /*
     *	update the mouse position
     */
    TheMouse.x = (x-300);
    TheMouse.y = ((600-y)-300);

    /*
     *	Check MyButton to see if we should highlight it cos the mouse is over it
     */
    ButtonPassive(x-300,(600-y)-300);
}

void MouseEvent :: mouseMotion(int x,int y)
{
    /*
     *	Calculate how much the mouse actually moved
     */
//	int dx = x - TheMouse.x;
//	int dy = y - TheMouse.y;

    /*
     *	update the mouse position
     */
    TheMouse.x = (x-300);
    TheMouse.y = ((600-y)-300);


    /*
     *	Check MyButton to see if we should highlight it cos the mouse is over it
     */
    ButtonPassive(x-300,(600-y)-300);

    /*
     *	Force a redraw of the screen
     */

}
void MouseEvent :: mousePressed(int x, int y,bool l,bool m,bool r)
{
    lMB=l;
    mMB=m;
    rMB=r;

    TheMouse.x = (x-300.0);
    TheMouse.y = (600-y)-300.0;

    if(l)
        TheMouse.lmb = 1;
    else if(m)
        TheMouse.mmb = 1;
    else
        TheMouse.rmb = 1;


    TheMouse.xpress = x-300.0;
    TheMouse.ypress = (600-y)-300.0;

//    std::cout<<(600-y)-300<<std::endl;
//    std::cout<<"x : "<<(x)-300<<std::endl;
//    std::cout<<isDraw<<std::endl;
    if( ((600-y)-300)>-250 && (x-300)>-200)
    {

        if(isFill==true)
        {
            float pixel[3];
//            std::cout<<"hai"<<std::endl;
            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
            glReadPixels(x,600-y,1,1,GL_RGB,GL_FLOAT,pixel);
            Draw->fill(x-300,(600-y)-300,pixel);
        }
        else
        {
            xPressed=x-300;
            yPressed=(600-y)-300;
        }

    }
    else
    {
//        std::cout<<"else";
        ButtonPress(x-300,(600-y)-300);
    }


}

void MouseEvent :: mouseReleased(int x, int y,bool l,bool m,bool r)
{
    lMB=l;
    mMB=m;
    rMB=r;


    if(l)
        TheMouse.lmb = 0;
    else if(m)
        TheMouse.mmb = 0;
    else
        TheMouse.rmb = 0;

    TheMouse.x = (x-300.0);
    TheMouse.y = (600-y)-300.0;


    if( ((600-y)-300)>-250 && (x-300)>-200)
    {
        if(isFill==false)
        {
            xRelease=x-300.0f;
            yReleased=(600.0f-y)-300.0f;

            if(isRectangle)
            {
                Draw->rectangle(xPressed,yPressed,xRelease,yReleased);
            }
            if(isLine)
            {
                Draw->line(xPressed,yPressed,xRelease,yReleased);
            }
            if(isSphere)
            {
                Draw->sphere(xPressed,yPressed,xRelease,yReleased);
            }
        }

    }
    else
    {
        ButtonRelease(x-300,(600-y)-300);
    }

}


MouseEvent *MEvent = new MouseEvent();


static void TheButtonCallback()
{
//    printf("Line\n");
    MEvent->isLine=true;
    MEvent->isRectangle=false;
    MEvent->isSphere=false;
    MEvent->isFill=false;
}


static void TheButtonCallback2()
{
//    printf("Circle\n");
    MEvent->isLine=false;
    MEvent->isRectangle=false;
    MEvent->isSphere=true;
    MEvent->isFill=false;
}

static void TheButtonCallback3()
{
//    printf("Rectangle\n");
    MEvent->isLine=false;
    MEvent->isRectangle=true;
    MEvent->isSphere=false;
    MEvent->isFill=false;
}

static void TheButtonCallback4()
{
//    printf("Bucket Fill\n");
    MEvent->isLine=false;
    MEvent->isRectangle=false;
    MEvent->isSphere=false;
    MEvent->isFill=true;
}

static void RedCallback()
{
    Draw->setColor(1.0f,0.0f,0.0f);
//    printf("Red Filled\n");
}

static void BlueCallback()
{
    Draw->setColor(0.0f,0.0f,1.0f);
//    printf("Blue Filled\n");
}

static void YellowCallback()
{
    Draw->setColor(1.0f,1.0f,0.0f);
//    printf("Blue Filled\n");
}

static void PinkCallback()
{
    Draw->setColor(1.0f,0.0f,1.0f);
//    printf("Blue Filled\n");
}
static void CyanCallback()
{
    Draw->setColor(0.0f,1.0f,1.0f);
//    printf("Blue Filled\n");
}

static void GreenCallback()
{
    Draw->setColor(0.0f,1.0f,0.0f);
//    printf("Green Filled\n");
}

static void BlackCallback()
{
    Draw->setColor(0.0f,0.0f,0.0f);
}
static void WhiteCallback()
{
    Draw->setColor(1.0f,1.0f,1.0f);
//    printf("White Filled\n");
}

static void Green2Callback()
{
    Draw->setColor(0.0f,0.7f,0.3f);
//    printf("Green Pastures Filled\n");
}
/*----------------------------------------------------------------------------------------
 *	\brief	This function draws a text string to the screen using glut bitmap fonts.
 *	\param	font	-	the font to use. it can be one of the following :
 *
 *					GLUT_BITMAP_9_BY_15
 *					GLUT_BITMAP_8_BY_13
 *					GLUT_BITMAP_TIMES_ROMAN_10
 *					GLUT_BITMAP_TIMES_ROMAN_24
 *					GLUT_BITMAP_HELVETICA_10
 *					GLUT_BITMAP_HELVETICA_12
 *					GLUT_BITMAP_HELVETICA_18
 *
 *	\param	text	-	the text string to output
 *	\param	x		-	the x co-ordinate
 *	\param	y		-	the y co-ordinate
 */
void Font(void *font,char *text,int x,int y)
{

glPushMatrix();
glTranslatef(x , y ,0);
glScalef(0.10f,0.10f,0);
	while( *text != '\0' )
	{
	    glutStrokeCharacter(font, *text);
		++text;
	}
glPopMatrix();
	glFlush();
}


/*----------------------------------------------------------------------------------------
 *	\brief	This function is used to see if a mouse click or event is within a button
 *			client area.
 *	\param	b	-	a pointer to the button to test
 *	\param	x	-	the x coord to test
 *	\param	y	-	the y-coord to test
 */
int ButtonClickTest(Button* b,int x,int y)
{
    if( b)
    {
        /*
         *	If clicked within button area, then return true
         */

//		 printf("\n\n x : %d b->x : %d ",x,b->x); printf(" x : %d b->x+w : %d \n",x,b->x+b->w);
//		 printf(" y : %d b->y : %d ",y,b->y);printf(" y : %d b->y+h : %d \n",y,b->y-b->h);

        if( x > b->x      &&
                x < b->x+b->w &&
                y < b->y      &&
                y > b->y-b->h )
        {
            return 1;
        }
    }

    /*
     *	otherwise false.
     */
    return 0;
}

/*----------------------------------------------------------------------------------------
 *	\brief	This function draws the specified button.
 *	\param	b	-	a pointer to the button to check.
 *	\param	x	-	the x location of the mouse cursor.
 *	\param	y	-	the y location of the mouse cursor.
 */
void ButtonRelease(int x,int y)
{
    Button* b = pButtonList;
    while(b)
    {
        /*
         *	If the mouse button was pressed within the button area
         *	as well as being released on the button.....
         */
        if( ButtonClickTest(b,TheMouse.xpress,TheMouse.ypress) &&
                ButtonClickTest(b,x,y) )
        {
            /*
             *	Then if a callback function has been set, call it.
             */
            if (b->callbackFunction)
            {
                b->callbackFunction();
            }
        }

        /*
         *	Set state back to zero.
         */
        b->state = 0;

        b=b->next;
    }
}

/*----------------------------------------------------------------------------------------
 *	\brief	This function draws the specified button.
 *	\param	b	-	a pointer to the button to check.
 *	\param	x	-	the x location of the mouse cursor.
 *	\param	y	-	the y location of the mouse cursor.
 */
void ButtonPress(int x,int y)
{
    Button* b = pButtonList;
    while(b)
    {
        /*
         *	if the mouse click was within the buttons client area,
         *	set the state to true.
         */
        if( ButtonClickTest(b,x,y) )
        {
            b->state = 1;
        }
        b=b->next;
    }
}


/*----------------------------------------------------------------------------------------
 *	\brief	This function draws the specified button.
 *	\param	b	-	a pointer to the button to check.
 *	\param	x	-	the x location of the mouse cursor.
 *	\param	y	-	the y location of the mouse cursor.
 */
void ButtonPassive(int x,int y)
{
    int needRedraw = 0;
    Button* b = pButtonList;
    while(b)
    {
        /*
         *	if the mouse moved over the control
         */
        if( ButtonClickTest(b,x,y) )
        {
            /*
             *	If the cursor has just arrived over the control, set the highlighted flag
             *	and force a redraw. The screen will not be redrawn again until the mouse
             *	is no longer over this control
             */
            if( b->highlighted == 0 )
            {
                b->highlighted = 1;
                needRedraw=1;
            }
        }
        else

            /*
             *	If the cursor is no longer over the control, then if the control
             *	is highlighted (ie, the mouse has JUST moved off the control) then
             *	we set the highlighting back to false, and force a redraw.
             */
            if( b->highlighted == 1 )
            {
                b->highlighted = 0;
                needRedraw=1;
            }
        b=b->next;
    }
    if (needRedraw)
    {
        glutPostRedisplay();
    }
}

/*----------------------------------------------------------------------------------------
 *	\brief	This function draws the specified button.
 */
void ButtonDraw()
{
    int fontx;
    int fonty;

    Button* b = pButtonList;
    while(b)
    {
        /*
         *	We will indicate that the mouse cursor is over the button by changing its
         *	colour.
         */
        if (b->highlighted)
            glColor3f(0.7f,0.7f,0.8f);
        else
            glColor3f(0.6f,0.6f,0.6f);

        /*
         *	draw background for the button.
         */
        glBegin(GL_QUADS);

        if(b->x==-198 && b->y==-280)
            glColor3f(1.0f,1.0f,1.0f);

        else if (b->x==-173 && b->y==-280)
            glColor3f(0.0f,0.0f,0.0f);

        else if (b->x==-148 && b->y==-280)
            glColor3f(1.0f,0.0f,.0f);

        else if (b->x==-123 && b->y==-280)
            glColor3f(0.0f,1.0f,0.0f);

        else if (b->x==-98 && b->y==-280)
            glColor3f(0.0f,0.7f,0.3f);

            else if (b->x==-73 && b->y==-280)
            glColor3f(0.0f,0.0f,1.0f);

            else if (b->x==-48 && b->y==-280)
            glColor3f(1.0f,1.0f,0.0f);

            else if (b->x==-23 && b->y==-280)
            glColor3f(1.0f,0.0f,1.0f);

            else if (b->x==3 && b->y==-280)
            glColor3f(0.0f,1.0f,1.0f);




//printf(" height = %f \n", ((b->x)+(b->h))   );

        glVertex2f( b->x      , b->y       );
        glVertex2f( b->x      , ((b->y)-(b->h))  );
        glVertex2f( (b->x+b->w) ,(b->y-b->h)  );
        glVertex2f( (b->x+b->w) , b->y       );
        glEnd();

        /*
         *	Draw an outline around the button with width 3
         */
        glLineWidth(3);

        /*
         *	The colours for the outline are reversed when the button.
         */
        if (b->state)
            glColor3f(0.4f,0.4f,0.4f);
        else
            glColor3f(0.8f,0.8f,0.8f);

        glBegin(GL_LINE_STRIP);
        glVertex2f( (b->x+b->w) , b->y       );
        glVertex2f( b->x       , b->y       );
        glVertex2f( b->x       , (b->y-b->h)  );
        glEnd();

        if (b->state)
            glColor3f(0.8f,0.8f,0.8f);
        else
            glColor3f(0.4f,0.4f,0.4f);

        glBegin(GL_LINE_STRIP);
        glVertex2f( b->x      , (b->y-b->h)  );
        glVertex2f( (b->x+b->w)  , (b->y-b->h)  );
        glVertex2f( (b->x+b->w)  , b->y       );
        glEnd();

        glLineWidth(1);


        /*
         *	Calculate the x and y coords for the text string in order to center it.
         */

        fontx = b->x + 5;//(b->w - glutBitmapLength(GLUT_BITMAP_HELVETICA_10,b->label)) / 2 '
        fonty = b->y - (b->h+10)/2;

        /*
         *	if the button is pressed, make it look as though the string has been pushed
         *	down. It's just a visual thing to help with the overall look....
         */
        if (b->state)
        {
            fontx+=2;
            fonty+=2;
        }

        /*
         *	If the cursor is currently over the button we offset the text string and draw a shadow
         */
        if(b->highlighted)
        {
            glColor3f(0,0,0);
            Font(GLUT_STROKE_ROMAN,b->label,fontx,fonty);
            fontx--;
            fonty--;
        }

        glColor3f(1,1,1);
        Font(GLUT_STROKE_ROMAN,b->label,fontx,fonty);

        b = b->next;
    }
}


/*----------------------------------------------------------------------------------------
 *	\brief	This function is called to initialise opengl.
 *  It creates buttons at specified x-y coordinates,
 *  of specified width and height,
 *  and relates them to the different callback functions.
 */

void InitButton()
{
    //for 3D scenes, no effect of light on button
    //glEnable(GL_LIGHT0);

    CreateButton("Line",TheButtonCallback,-300,300,98,30);
    CreateButton("Circle",TheButtonCallback2,-300,268,98,30);
    CreateButton("Rectangle",TheButtonCallback3,-300,236,98,30);
    CreateButton("Bucket Fill",TheButtonCallback4,-300,0,98,30);

    CreateButton("",WhiteCallback,-198,-280,20,20);
    CreateButton("",BlackCallback,-173,-280,20,20);
    CreateButton("",RedCallback,-148,-280,20,20);
    CreateButton("",GreenCallback,-123,-280,20,20);
    CreateButton("",Green2Callback,-98,-280,20,20);
    CreateButton("",BlueCallback,-73,-280,20,20);
    CreateButton("",YellowCallback,-48,-280,20,20);
    CreateButton("",PinkCallback,-23,-280,20,20);
    CreateButton("",CyanCallback,3,-280,20,20);


}
