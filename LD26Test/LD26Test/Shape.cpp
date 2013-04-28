//
//  Shape.cpp
//  LD26Test
//
//  Created by Evan Chapman on 4/27/13.
//  Copyright (c) 2013 Evan Chapman. All rights reserved.
//

#include "Shape.h"
#include <iostream>

using namespace std;


void Square::Draw(){
    glBegin(GL_QUADS);
    
    if (active){
        glColor3f(1.0, 1.0, 1.0);
    } else {
        glColor3f(.2, .2, .2);
    }
    
    if (unique){
        glColor3f(R, G, B);
    }
    
    glVertex2f(x, y);
    glVertex2f(x, y - size);
    glVertex2f(x + size, y - size);
    glVertex2f(x + size, y);
    
    glEnd();
    
    if (flipped){
        glColor3f(1, 1, 1);
    } else {
        glColor3f(0, 0, 0);
    }

    
}


void Circle::Draw(){
    glBegin(GL_TRIANGLE_FAN);
    if (flipped){
        glColor3f(fR, fG, fB);
    } else {
        glColor3f(R, G, B);
    }
    
    float stepSize = 2*PI/32;
    glVertex2f(x, y);
    
    for (float t = 0.0f; t<=2*PI; t+=stepSize){
        float finalx = radius*cosf(t) + x;
        float finaly = radius*sinf(t) + y;
        glVertex2f(finalx, finaly);
    }
    
    
    glEnd();
}


void Circle::Update(float delta){
    x+=vx * delta;
    y+=vy * delta;
}


void Square::Update(float delta){
    x+=vx * delta;
    y+=vy * delta;
    
    if (x > 0){direction = dright;}
    if (x < 0){direction = dleft;}
    if (y > 0){direction = dup;}
    if (y < 0){direction = ddown;}
}


bool Circle::Collide(Shape &rhs){
    return false;
}


bool Square::Collide(Shape &rhs){
    bool didCollide = false;
    outside = true;
    
    if (entering && outside){
        entering = false;
    }
    
    if (inside && entering){
        exiting = true;
    }
        

        //check if overlapping
        if (x + size >= rhs.x  &&
            x <= rhs.x + rhs.size &&
            y - size <= rhs.y &&
            y >= rhs.y - rhs.size){
            //cout << "overlapping" << endl;
            overlapping = true;
        } else {
            overlapping = false;
        }
    
    
    //check for entering /exiting
    if (overlapping && outside){
        if (entering){
            exiting = true;
        }
        entering = true;
    }
    
    //check for internal collision
    if (inside){
        entering = false;
        
        if (y >= rhs.y||
            y - size <= rhs.y - rhs.size ||
            x <= rhs.x ||
            x+size >= rhs.x){
            internalCollision = true;
        }
    } else {
        internalCollision = false;
    }
    

    
    //check if inside other object
    if (x + size < rhs.x + rhs.size  &&
        x > rhs.x &&
        y - size > rhs.y - rhs.size &&
        y < rhs.y){
        //cout << "inside" << endl;
        inside = true;
        outside = false;
        
    } else {
        //cout << "outside" << endl;
        
        inside = false;
    }


    
    
    return didCollide;
}























