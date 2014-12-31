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

#include<cstdio>
#include<cstdlib>
#include<cassert>
#include<cstring>

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
    while (curr!=NULL)
    {
        if (strcmp(label,curr->label)==0)
        {
            if(previous)
                previous->next = curr->next;
            else
                pButtonList = curr->next;

            if (curr->label)
            {

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

            if (curr->label)
            {

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

