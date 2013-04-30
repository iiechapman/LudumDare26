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
    
    glColor3f(R, G, B);
    
    glVertex2f(x, y);
    glVertex2f(x, (y - sizey));
    glVertex2f(x + sizex, y - sizey);
    glVertex2f(x + sizex, y);
    
    glEnd();
    
    
    glLineWidth(4);
    glBegin(GL_LINES);
    
    glColor3f(R/2, G/2, B/2 );
    //left line
    glVertex2f(x, y);
    glVertex2f(x, y - sizey);
    
    //right line
    glVertex2f(x + sizex, y - sizey);
    glVertex2f(x + sizex, y);
    
    
    //top
    glVertex2f(x,y);
    glVertex2f(x+ sizex, y);
    
    //bottom
    glVertex2f(x, y - sizey);
    glVertex2f(x + sizex, y - sizey);
    
    glEnd();
}


void Square::Update(float delta){
    x+=vx * delta;
    y+=vy * delta;
}


bool Square::Collide(Shape &rhs){
    bool didCollide = false;
    
    //check if overlapping
    if (x + sizex >= rhs.x  &&
        x <= rhs.x + rhs.sizex &&
        y - sizey <= rhs.y &&
        y >= rhs.y - rhs.sizey){
        //cout << "overlapping" << endl;
        overlapping = true;
    } else {
        overlapping = false;
    }
    
    return didCollide;
}























