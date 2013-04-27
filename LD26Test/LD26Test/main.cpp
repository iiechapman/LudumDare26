//
//  main.cpp
//  GLUT Circle Move
//
//  Created by Evan Chapman on 2/24/13.
//  Copyright (c) 2013 Evan Chapman. All rights reserved.
//


/*
 =================
 GLUT Circle Move
 Creates GLUT Window that displays a circle you can move with mouse or keyboard
 
 Controls:
 WASD  - Move circle up down left and right
 Left Mouse Button/Right Mouse Button - Expand/Shrink Circle
 +-    - Change direction angle of line
 
 =================
 */

#include <iostream>
#include <cmath>
#include <OpenGL/OpenGL.h>
#include <SFML/Audio.hpp>
#include <SFML/Audio/SoundSource.hpp>
#include <GLUT/GLUT.h>

const float PI = 3.14159;

float VIEW_HEIGHT = 20.0f;  //Height Ratio Lock
float VIEW_WIDTH  = 0.0f;   //Width Ratio Calculated later

bool LEFT_MOUSE_DOWN;
bool RIGHT_MOUSE_DOWN;
bool PRESS_UP;
bool PRESS_DOWN;
bool PRESS_LEFT;
bool PRESS_RIGHT;
bool PRESS_PLUS;
bool PRESS_MINUS;

float cx,cy,cr,cspeed;
float angle;

void Display();
void Idle();
void Reshape(int w, int h);
void OnKeyPress(unsigned char key, int x , int y);
void OnKeyUp(unsigned char key, int x, int y);
void OnMouseClick(int button, int state, int x, int y);

void UpdateScene();
void DrawCircle(float cx, float cy, float r, float R, float G, float B);
void DrawDisk(float cx, float cy, float r, float R, float G, float B);

int main(int argc, char** argv){
    std::cout << "Initializing GLUT...\n";
    
    cx      =   1;
    cy      =   0;
    cr      =   2;
    cspeed  =   .0025f;
    angle   =   0.0f; // "North"
    
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(640, 480);
    glutCreateWindow("LD26 Minimalism");
    
    //Set GLUT Utility Functions
    glutIdleFunc(Idle);
    glutReshapeFunc(Reshape);//Set resize function
    glutKeyboardFunc(OnKeyPress);
    glutKeyboardUpFunc(OnKeyUp);
    glutMouseFunc(OnMouseClick);
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glutDisplayFunc(Display); //Set display function
    sf::Music music;
    

    glutMainLoop();
    return 0;
}


void Display(){
    glClear(GL_COLOR_BUFFER_BIT);
    
    float timeElapsed = glutGet(GLUT_ELAPSED_TIME) * 0.001f;
    float finalRadius = cr * cosf(timeElapsed);
    
    int sections = 5;
    float R,G,B;
    R = 1.0f;
    G = 0.2f;
    B = 0.2f;
    
    for (float i = sections; i > 0; i--) {
        DrawDisk(cx, cy,
                 finalRadius*(i/sections),
                 R*(i/sections), G*(i/sections), B*(i/sections));
    }
    
    
    //Draw Line Showing orientation of circle
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex2f(cx, cy);
    glVertex2f(cx + (finalRadius*cos(angle)), (cy + (finalRadius * sin(angle))));
    glEnd();
    
    char string[] = "LD26 Minimalism";
    char* buffer = string;
    
    glColor3f(1.0f,1.0f,1.0f);
    glRasterPos2f(-13, 9);
    
    for (int i = 0; i < strlen(buffer); i ++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,buffer[i]);
    }
    
    //GL END
    glutSwapBuffers();
}

void Idle(){
    UpdateScene();
    glutPostRedisplay();
}

void Reshape(int w, int h){
    float aspectRatio = (float)w/h;
    VIEW_WIDTH = aspectRatio * VIEW_HEIGHT;
    
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-VIEW_WIDTH/2,VIEW_WIDTH/2,
            -VIEW_HEIGHT/2,VIEW_HEIGHT/2,
            -10.0,10.0f);
}

void OnKeyPress(unsigned char key , int x, int y){
    std::cout << "Key: " << key << "\n";
    switch(key){
        case 'w':
            PRESS_UP = true;
            break;
            
        case 'a':
            PRESS_LEFT = true;
            break;
            
        case 's':
            PRESS_DOWN  = true;
            break;
            
        case 'd':
            PRESS_RIGHT = true;
            break;
            
        case '=':
            PRESS_PLUS  = true;
            break;
            
        case '-':
            PRESS_MINUS = true;
            break;
    }
}

void OnKeyUp(unsigned char key, int x, int y){
    std::cout << "Released Key: " << key << "\n";
    switch (key){
        case 'w':
            PRESS_UP = false;
            break;
            
        case 'a':
            PRESS_LEFT = false;
            break;
            
        case 's':
            PRESS_DOWN  = false;
            break;
            
        case 'd':
            PRESS_RIGHT = false;
            break;
            
        case '=':
            PRESS_PLUS  = false;
            break;
            
        case '-':
            PRESS_MINUS = false;
            break;
    }
}


void OnMouseClick(int button,int state,int x, int y){
    if (button == GLUT_LEFT_BUTTON){
        if (state == GLUT_DOWN){
            LEFT_MOUSE_DOWN = true;
            std::cout << "Left Mouse Button Pressed at X: "
            << x << " Y: " << y << "\n";
            
        }else if (state == GLUT_UP){
            LEFT_MOUSE_DOWN = false;
            std::cout << "Left mouse button released!\n" ;
        }
    }
    
    if(button == GLUT_RIGHT_BUTTON){
        if (state == GLUT_DOWN){
            RIGHT_MOUSE_DOWN = true;
            std::cout << "Right Mouse Button Pressed at X: "
            << x << " Y: " << y << "\n";
            
        }else if (GLUT_UP){
            RIGHT_MOUSE_DOWN = false;
            std::cout << "Right Mouse button released \n";
        }
    }
}

void DrawCircle(float cx, float cy, float r, float R, float G, float B){
    glBegin(GL_LINE_LOOP);
    glColor3f(R, G, B);
    
    float stepSize = 2*PI/32;
    
    for(float t = 0.0f; t < 2*PI; t+=stepSize){
        float x = r*cosf(t) + cx;
        float y = r*sinf(t) + cy;
        
        glVertex2f(x, y);
    }
    glEnd();
}



void DrawDisk(float cx, float cy, float r, float R, float G, float B){
    glBegin(GL_TRIANGLE_FAN);
    
    glColor3f(R, G, B);
    
    float stepSize = 2*PI/32;
    glVertex2f(cx, cy);
    
    for (float t = 0.0f; t<=2*PI; t+=stepSize){
        float x = r*cosf(t) + cx;
        float y = r*sinf(t) + cy;
        glVertex2f(x, y);
    }
    
    glEnd();
}




void UpdateScene(){
    if (PRESS_UP){
        cy += cspeed;
    }
    if (PRESS_DOWN){
        cy -= cspeed;
    }
    if (PRESS_LEFT){
        cx -= cspeed;
    }
    if (PRESS_RIGHT){
        cx += cspeed;
    }
    
    if (LEFT_MOUSE_DOWN){
        cr += cspeed;
    }
    
    if(RIGHT_MOUSE_DOWN){
        cr -= cspeed;
    }
    
    if (PRESS_PLUS) {
        angle -= cspeed/10;
        fmod(angle, 360.0f);
        std::cout << "Angle: " << angle << "\n";
    }
    
    if (PRESS_MINUS){
        angle += cspeed/10;
        fmod(angle, 360.0f);
        std::cout << "Angle: " << angle << "\n";
    }
    
}










