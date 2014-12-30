                                /*** VERSION 1.5 ***/

/*----------------------------------------------------------------------------------------
 *
 *	project:	03 More than one button creation
 *	sub~author:	Asuka Zone
 *	note:
 *				Quite often you may want to draw more than one buttons.
 *				This example takes Rob's original code for creating two buttons and modifies
 *              it by adding a bunch more, for my understanding of the code.
 *
 *
 *              Buttons filled with different colors/ palette of colors created.
 *
 */

/*----------------------------------------------------------------------------------------
 *	Includes
 */


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#endif

#include <GL/glut.h>



/*----------------------------------------------------------------------------------------
 *	Global Variables
 */

/*
 *	A structure to represent the mouse information
 */
struct Mouse
{
	int x;		/*	the x coordinate of the mouse cursor	*/
	int y;		/*	the y coordinate of the mouse cursor	*/
	int lmb;	/*	is the left button pressed?		*/
	int mmb;	/*	is the middle button pressed?	*/
	int rmb;	/*	is the right button pressed?	*/

	/*
	 *	These two variables are a bit odd. Basically I have added these to help replicate
	 *	the way that most user interface systems work. When a button press occurs, if no
	 *	other button is held down then the co-ordinates of where that click occured are stored.
	 *	If other buttons are pressed when another button is pressed it will not update these
	 *	values.
	 *
	 *	This allows us to "Set the Focus" to a specific portion of the screen. For example,
	 *	in maya, clicking the Alt+LMB in a view allows you to move the mouse about and alter
	 *	just that view. Essentually that viewport takes control of the mouse, therefore it is
	 *	useful to know where the first click occured....
	 */
	int xpress; /*	stores the x-coord of when the first button press occurred	*/
	int ypress; /*	stores the y-coord of when the first button press occurred	*/
};

/*
 *	rename the structure from "struct Mouse" to just "Mouse"
 */
typedef struct Mouse Mouse;

/*
 *	Create a global mouse structure to hold the mouse information.
 */
Mouse TheMouse = {0,0,0,0,0};

/*
 *	Variables to hold the current size of the window.
 */
int winw = 600;
int winh = 600;


/*----------------------------------------------------------------------------------------
 *	Button Stuff
 */

/*
 *	We will define a function pointer type. ButtonCallback is a pointer to a function that
 *	looks a bit like this :
 *
 *	void func() {
 *	}
 */
typedef void (*ButtonCallback)();

/*
 *	This is a simple structure that holds a button.
 */
struct Button
{
	int   x;							/* top left x coord of the button */
	int   y;							/* top left y coord of the button */
	int   w;							/* the width of the button */
	int   h;							/* the height of the button */
	int	  state;						/* the state, 1 if pressed, 0 otherwise */
	int	  highlighted;					/* is the mouse cursor over the control? */
	char* label;						/* the text label of the button */
	ButtonCallback callbackFunction;	/* A pointer to a function to call if the button is pressed */

	int id;								/* A unique ID to represent this Button */

	struct Button* next;				/* a pointer to the next node in the linked list */
};
typedef struct Button Button;

/*
 *	The start of a linked list of buttons
 */
Button* pButtonList = NULL;

int GlobalRef=0;

/*
 *
 */
int CreateButton(char *label,ButtonCallback cb,int x,int y,int w,int h)
{
	Button* p = (Button*)malloc( sizeof(Button) );
	assert(p);
	memset(p,0,sizeof(Button));

	p->x = x;
	p->y = y;
	p->w = w;
	p->h = h;
	p->callbackFunction = cb;
	p->label = (char*)malloc( strlen(label)+1 );

	if(p->label)
		sprintf(p->label,label);

	p->next = pButtonList;
	pButtonList = p;

	return p->id = ++GlobalRef;
}

int DeleteButtonByName(char *label)
{
	Button* previous=NULL,*curr=pButtonList;
	while (curr!=NULL) {
		if (strcmp(label,curr->label)==0) {
			if(previous)
				previous->next = curr->next;
			else
				pButtonList = curr->next;

			if (curr->label) {

				free(curr->label);
			}
			free(curr);
			return 1;
		}
		previous = curr;
		curr = curr->next;
	}
	return 0;
}

int DeleteButtonById(int id)
{
	Button	*previous = NULL       ,
			*curr     = pButtonList;

	while (curr!=NULL)
	{
		if ( id == curr->id )
		{
			if(previous)
				previous->next = curr->next;
			else
				pButtonList = curr->next;

			if (curr->label) {

				free(curr->label);
			}
			free(curr);
			return 1;
		}
		previous = curr;
		curr = curr->next;
	}
	return 0;
}


/*----------------------------------------------------------------------------------------
 *	This is an example callback function. Notice that it's type is the same
 *	an the ButtonCallback type. We can assign a pointer to this function which
 *	we can store and later call.
 */

static void TheButtonCallback()
{
	printf("Line\n");
}


static void TheButtonCallback2()
{
    printf("Circle\n");
}

static void TheButtonCallback3()
{
    printf("Rectangle\n");
}

static void TheButtonCallback4()
{
    printf("Bucket Fill\n");
}

static void RedCallback()
{
    printf("Red Filled\n");
}

static void BlueCallback()
{
    printf("Blue Filled\n");
}

static void GreenCallback()
{
    printf("Green Filled\n");
}

static void WhiteCallback()
{
    printf("White Filled\n");
}

static void Green2Callback()
{
    printf("Green Pastures Filled\n");
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
	glRasterPos2f(x/300.0f, y/300.0f);

	while( *text != '\0' )
	{
		glutBitmapCharacter( font, *text );
		++text;
	}
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
			y > b->y-b->h ) {
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
			if (b->callbackFunction) {
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
			if( b->highlighted == 0 ) {
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
	if (needRedraw) {
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

                if(b->x==-200 && b->y==-280)
                    glColor3f(1.0f,0.0f,0.0f);

                else if (b->x==-175 && b->y==-280)
                    glColor3f(0.0f,1.0f,0.0f);

                else if (b->x==-150 && b->y==-280)
                    glColor3f(0.0f,0.0f,1.0f);

                else if (b->x==-125 && b->y==-280)
                    glColor3f(1.0f,1.0f,1.0f);

                else if (b->x==-100 && b->y==-280)
                    glColor3f(0.0f,0.7f,0.3f);


//printf(" height = %f \n", ((b->x)+(b->h))/300.0f  );

			glVertex2f( b->x/300.0f     , b->y /300.0f     );
			glVertex2f( b->x/300.0f     , ((b->y)-(b->h))/300.0f );
			glVertex2f( (b->x+b->w)/300.0f,(b->y-b->h)/300.0f );
			glVertex2f( (b->x+b->w)/300.0f, b->y/300.0f      );
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
			glVertex2f( (b->x+b->w)/300.0f, b->y/300.0f      );
			glVertex2f( b->x /300.0f     , b->y/300.0f      );
			glVertex2f( b->x /300.0f     , (b->y-b->h)/300.0f );
		glEnd();

		if (b->state)
			glColor3f(0.8f,0.8f,0.8f);
		else
			glColor3f(0.4f,0.4f,0.4f);

		glBegin(GL_LINE_STRIP);
			glVertex2f( b->x/300.0f     , (b->y-b->h)/300.0f );
			glVertex2f( (b->x+b->w) /300.0f, (b->y-b->h)/300.0f );
			glVertex2f( (b->x+b->w) /300.0f, b->y/300.0f      );
		glEnd();

		glLineWidth(1);


		/*
		 *	Calculate the x and y coords for the text string in order to center it.
		 */
		fontx = b->x + (b->w - glutBitmapLength(GLUT_BITMAP_HELVETICA_10,b->label)) / 2 ;
		fonty = b->y - (b->h+10)/2;

		/*
		 *	if the button is pressed, make it look as though the string has been pushed
		 *	down. It's just a visual thing to help with the overall look....
		 */
		if (b->state) {
			fontx+=2;
			fonty+=2;
		}

		/*
		 *	If the cursor is currently over the button we offset the text string and draw a shadow
		 */
		if(b->highlighted)
		{
			glColor3f(0,0,0);
			Font(GLUT_BITMAP_HELVETICA_10,b->label,fontx,fonty);
			fontx--;
			fonty--;
		}

		glColor3f(1,1,1);
		Font(GLUT_BITMAP_HELVETICA_10,b->label,fontx,fonty);

		b = b->next;
	}
}


/*----------------------------------------------------------------------------------------
 *	\brief	This function is called to initialise opengl.
 *  It creates buttons at specified x-y coordinates,
 *  of specified width and height,
 *  and relates them to the different callback functions.
 */

void Init()
{
    //for 3D scenes, no effect of light on button
	//glEnable(GL_LIGHT0);

	CreateButton("Line",TheButtonCallback,-300,300,100,30);
	CreateButton("Circle",TheButtonCallback2,-300,268,100,30);
	CreateButton("Rectangle",TheButtonCallback3,-300,236,100,30);
	CreateButton("Bucket Fill",TheButtonCallback4,-300,0,100,30);

	CreateButton("",RedCallback,-200,-280,20,20);
	CreateButton("",GreenCallback,-175,-280,20,20);
	CreateButton("",BlueCallback,-150,-280,20,20);
	CreateButton("",WhiteCallback,-125,-280,20,20);
	CreateButton("",Green2Callback,-100,-280,20,20);
}

/*----------------------------------------------------------------------------------------
 *	This function will be used to draw the 3D scene
 */
void Draw3D()
{
}

/*----------------------------------------------------------------------------------------
 *	This function will be used to draw an overlay over the 3D scene.
 *	This will be used to draw our fonts, buttons etc......
 */
void Draw2D()
{
	ButtonDraw();
}

/*----------------------------------------------------------------------------------------
 *	This is the main display callback function. It sets up the drawing for
 *	The 3D scene first then calls the Draw3D() function. After that it switches to
 *	an orthographic projection and calls Draw2D().
 */
void Draw()
{
	/*
	 *	Clear the background
	 */
	glClear( GL_COLOR_BUFFER_BIT |
			 GL_DEPTH_BUFFER_BIT );
	/*
	 *	Draw the 2D overlay
	 */
	Draw2D();

	/*
	 *	Bring the back buffer to the front and vice-versa.
	 */
	glutSwapBuffers();
}

/*----------------------------------------------------------------------------------------
 *	This function is called when the window is resized. All this does is simply
 *	store the new width and height of the window which are then referenced by
 *	the draw function to set the correct viewing transforms
 */
void Resize(int w, int h)
{
	winw = w;
	winh = h;

	/*
	 *	Allow drawing in full region of the screen
	 */
	glViewport(0,0,w,h);
}


/*----------------------------------------------------------------------------------------
 *	\brief	This function is called whenever a mouse button is pressed or released
 *	\param	button	-	GLUT_LEFT_BUTTON, GLUT_RIGHT_BUTTON, or GLUT_MIDDLE_BUTTON
 *	\param	state	-	GLUT_UP or GLUT_DOWN depending on whether the mouse was released
 *						or pressed respectivly.
 *	\param	x		-	the x-coord of the mouse cursor.
 *	\param	y		-	the y-coord of the mouse cursor.
 */
void MouseButton(int button,int state,int x, int y)
{
	/*
	 *	update the mouse position
	 */
	TheMouse.x = (x-300.0);
	TheMouse.y = (600-y)-300.0;

	/*
	 *	has the button been pressed or released?
	 */
	if (state == GLUT_DOWN)
	{
		/*
		 *	This holds the location of the first mouse click
		 */
		//if ( !(TheMouse.lmb || TheMouse.mmb || TheMouse.rmb) ) {
			TheMouse.xpress = x-300.0;
			TheMouse.ypress = (600-y)-300.0;
		//}

		/*
		 *	Which button was pressed?
		 */
		switch(button)
		{
		case GLUT_LEFT_BUTTON:
			TheMouse.lmb = 1;
			ButtonPress(x-300,(600-y)-300);
		case GLUT_MIDDLE_BUTTON:
			TheMouse.mmb = 1;
			break;
		case GLUT_RIGHT_BUTTON:
			TheMouse.rmb = 1;
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
			TheMouse.lmb = 0;
			ButtonRelease(x-300,(600-y)-300);
			break;
		case GLUT_MIDDLE_BUTTON:
			TheMouse.mmb = 0;
			break;
		case GLUT_RIGHT_BUTTON:
			TheMouse.rmb = 0;
			break;
		}
	}

	/*
	 *	Force a redraw of the screen. If we later want interactions with the mouse
	 *	and the 3D scene, we will need to redraw the changes.
	 */
	glutPostRedisplay();
}

/*----------------------------------------------------------------------------------------
 *	\brief	This function is called whenever the mouse cursor is moved AND A BUTTON IS HELD.
 *	\param	x	-	the new x-coord of the mouse cursor.
 *	\param	y	-	the new y-coord of the mouse cursor.
 */
void MouseMotion(int x, int y)
{
	/*
	 *	Calculate how much the mouse actually moved
	 */
//	int dx = x - TheMouse.x;
//	int dy = y - TheMouse.y;

	/*
	 *	update the mouse position
	 */
	TheMouse.x = x;
	TheMouse.y = y;


	/*
	 *	Check MyButton to see if we should highlight it cos the mouse is over it
	 */
	ButtonPassive(x,y);

	/*
	 *	Force a redraw of the screen
	 */
	glutPostRedisplay();
}

/*----------------------------------------------------------------------------------------
 *	\brief	This function is called whenever the mouse cursor is moved AND NO BUTTONS ARE HELD.
 *	\param	x	-	the new x-coord of the mouse cursor.
 *	\param	y	-	the new y-coord of the mouse cursor.
 */
void MousePassiveMotion(int x, int y)
{
	/*
	 *	Calculate how much the mouse actually moved
	 */
//	int dx = (x-300) - TheMouse.x;
//	int dy = ((600-y)-300) - TheMouse.y;

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
	 *	Note that I'm not using a glutPostRedisplay() call here. The passive motion function
	 *	is called at a very high frequency. We really don't want much processing to occur here.
	 *	Redrawing the screen every time the mouse moves is a bit excessive. Later on we
	 *	will look at a way to solve this problem and force a redraw only when needed.
	 */
}

/*----------------------------------------------------------------------------------------
 *
 */
int main(int argc,char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH|GLUT_DOUBLE);
	glutInitWindowSize(winw,winh);
	glutInitWindowPosition(200,100);
	glutCreateWindow("Two Buttons- Two Callbacks");

	glutDisplayFunc(Draw);
	glutReshapeFunc(Resize);
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(MousePassiveMotion);

	Init();

	glutMainLoop();
}








