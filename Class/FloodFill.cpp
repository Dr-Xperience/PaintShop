/****************************************************************************
    PaintShop (-- Orhto Projection)
    Copyright (C) 2012  Anubhav Arun

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

#define BUFFER_OFFSET(bytes)  ((GLubyte*) NULL + (bytes))
#include<queue>
#include<GL/glew.h>
#define GLAPI extern
#include<GL/glut.h>
//using namespace std;

class Floodfill
{
private:
    struct pt
    {
        int x;
        int y;
    };

    GLubyte *** image,*imageTemp;
    GLuint *pixelBuffer;
    int width,height;

protected:
    bool testPixel(int x, int y, float testColor[3]);
public:

    bool flag;
    void fastFill(int x, int y,float fillColor[3],float oldColor[3]);

    Floodfill(int vWidth,int vHeight,GLuint *buffer)
    {
        width=vWidth;
        height=vHeight;

        pixelBuffer=buffer;
        image=new GLubyte**[height];

//        glewInit();

        for(int i=0; i<height; ++i)
        {
            image[i]=new GLubyte*[width];

            for(int j=0; j<height; ++j)
            {
                image[i][j]=new GLubyte[3];

                image[i][j][0] = (GLubyte)0;
                image[i][j][1] = (GLubyte)0;
                image[i][j][2] = (GLubyte)0;


            }
        }


        flag=false;
    }

};

bool Floodfill::testPixel(int x, int y,float testColor[3])
{
    float pixels[3];
    pixels[0]=(float)image[y+(height/2)][x+(width/2)][0]/255.0f;
    pixels[1]=(float)image[y+(height/2)][x+(width/2)][1]/255.0f;
    pixels[2]=(float)image[y+(height/2)][x+(width/2)][2]/255.0f;

    if((testColor[0] == pixels[0])&&(testColor[1]== pixels[1])&&(testColor[2] == pixels[2]))
        return true;
    else
        return false;
}


void Floodfill::fastFill(int x, int y,float fillColor[3],float oldColor[3])
{
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

    imageTemp = new GLubyte[width*height*3];

    glReadPixels(0, 0, width,height, GL_RGB, GL_UNSIGNED_BYTE, imageTemp);


    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, *pixelBuffer);
    glBufferData(GL_PIXEL_UNPACK_BUFFER,
                 3*width*height,
                 imageTemp, GL_STATIC_DRAW);



    for(int i=0; i<height; ++i)
    {
        for(int j=0; j<width; ++j)
        {
            image[i][j][0]=imageTemp[(i*width+j)*3+0];
            image[i][j][1]=imageTemp[(i*width+j)*3+1];
            image[i][j][2]=imageTemp[(i*width+j)*3+2];
        }
    }

    delete imageTemp;

if(testPixel(x,y,fillColor)==true)
return;

    std::queue<pt> ptQueue;

    pt seed;
    seed.x=x;
    seed.y=y;
    ptQueue.push(seed);

    pt pixel,temp;

    std::queue<pt> ptQueueE;
    std::queue<pt> ptQueueW;
    std::queue<pt> ptTempE,ptTempW;

    while(!ptQueue.empty())
    {

        pixel = ptQueue.front();

        ptQueue.pop();

        if(testPixel(pixel.x,pixel.y,oldColor))
        {

            for(int i=pixel.x; i<(width/2) && testPixel(i,pixel.y,oldColor); ++i)
            {
                temp.x = i;
                temp.y = pixel.y;
                ptQueueW.push(temp);

            }

            for(int i=pixel.x; i>-(width/2) && testPixel(i,pixel.y,oldColor); --i)
            {
                temp.x = i;
                temp.y = pixel.y;
                ptQueueE.push(temp);
            }

            ptTempE=ptQueueE;
            ptTempW=ptQueueW;


            while(!ptQueueE.empty())
            {
                temp = ptQueueE.front();

                image[temp.y+(height/2)][temp.x+(width/2)][0]=fillColor[0]*255;
                image[temp.y+(height/2)][temp.x+(width/2)][1]=fillColor[1]*255;
                image[temp.y+(height/2)][temp.x+(width/2)][2]=fillColor[2]*255;
                ptQueueE.pop();
            }


            while(!ptQueueW.empty())
            {
                temp = ptQueueW.front();

                image[temp.y+(height/2)][temp.x+(width/2)][0]=fillColor[0]*255;
                image[temp.y+(height/2)][temp.x+(width/2)][1]=fillColor[1]*255;
                image[temp.y+(height/2)][temp.x+(width/2)][2]=fillColor[2]*255;

                ptQueueW.pop();
            }

            while(!ptTempE.empty())
            {
                temp = ptTempE.front();
                if(temp.y-1>-(height/2) &&testPixel(temp.x,temp.y-1,oldColor))
                {
                    temp.x = temp.x;
                    temp.y = temp.y-1;
                    ptQueue.push(temp);
                }
                temp = ptTempE.front();
                if(temp.y+1<(height/2) && testPixel(temp.x,temp.y+1,oldColor))
                {
                    temp.x = temp.x;
                    temp.y = temp.y+1;
                    ptQueue.push(temp);
                }
                ptTempE.pop();
            }

            while(!ptTempW.empty())
            {
                temp = ptTempW.front();
                if(temp.y-1>=-(height/2) &&testPixel(temp.x,temp.y-1,oldColor))
                {
                    temp.x = temp.x;
                    temp.y = temp.y-1;
                    ptQueue.push(temp);
                }
                temp = ptTempW.front();
                if(temp.y+1<=(height/2) &&testPixel(temp.x,temp.y+1,oldColor))
                {
                    temp.x = temp.x;
                    temp.y = temp.y+1;
                    ptQueue.push(temp);
                }
                ptTempW.pop();
            }

        }

    }

    imageTemp = new GLubyte[width*height*3];

    for(int i=0; i<height; ++i)
    {
        for(int j=0; j<width; ++j)
        {
            imageTemp[(i*width+j)*3+0]=image[i][j][0];
            imageTemp[(i*width+j)*3+1]=image[i][j][1];
            imageTemp[(i*width+j)*3+2]=image[i][j][2];
        }
    }


    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, *pixelBuffer);
    glBufferData(GL_PIXEL_UNPACK_BUFFER,
                 3*width*height,
                 imageTemp, GL_STATIC_DRAW);

    glRasterPos2i(-width/2, -height/2);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, *pixelBuffer);
    glDrawPixels(width, height, GL_RGB,
                 GL_UNSIGNED_BYTE, BUFFER_OFFSET(0));

    flag=true;

    delete imageTemp;
//    std::cout<<"DONE"<<std::endl;
}

