//
//  Shape.h
//  LD26Test
//
//  Created by Evan Chapman on 4/27/13.
//  Copyright (c) 2013 Evan Chapman. All rights reserved.
//

#ifndef __LD26Test__Shape__
#define __LD26Test__Shape__

#include <iostream>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include <cmath>

const float PI = 3.14159;

enum directions{dleft,dright,dup,ddown};
enum effects{opposite,stop,sleft,sright,sup,sdown};

class Shape{
public:
    virtual void Draw() = 0;
    virtual void Update(float delta) = 0;
    virtual bool Collide(Shape& rhs) = 0;
    
    directions direction;
    
    bool flipped;
    bool shadow;
    bool inside;
    bool outside;
    bool overlapping;
    bool colliding;
    bool internalCollision;
    bool exiting;
    bool entering;
    float x,y;
    int number;
    float vx,vy;
    float size;
    float sizex;
    float sizey;
    float R,G,B;
    float fR,fG,fB;
    
    int sections;
    int currentColor;
    
    bool unique;
    bool active;
    bool isGoal;
    bool isBad;
    bool isGood;
    bool isTeleport;
    
    
};


class Circle : public Shape{
public:
    
    void Draw();
    void Update(float delta);
    bool Collide(Shape& rhs);
    float radius;
    
};

class Square : public Shape{
public:
    void Draw();
    void Update(float delta);
    bool Collide(Shape& rhs);

};



#endif /* defined(__LD26Test__Shape__) */
