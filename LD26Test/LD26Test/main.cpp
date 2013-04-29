/*
 =================
LD 26 Minimalism 
 By Evan Chapman
 
 =================
 */

#include <iostream>
#include <cmath>
#include <OpenGL/OpenGL.h>
#include <SFML/Audio.hpp>
#include <SFML/Audio/SoundSource.hpp>
#include <GLUT/GLUT.h>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "Shape.h"

using namespace std;


enum GameState{intro,splashludum,splashlogo,title,levelselect,game,win,lose,quit};
GameState gameState = game;
bool gameFlipped = false;
int level = 1;
int difficulty = 4;
int gameSpeed = difficulty * 150;

unsigned int combo;
unsigned int maxCombo;

bool movingRight;
bool movingLeft = true;

int sections = 5 * difficulty;

enum colors {white,red,green,blue,purple};
int selectColor;



colors currentColor = white;

float timerR , timerG , timerB;

float fullTime = 1;
bool resetTimer;
float timer;

float lineFullTime = 1;
bool lineReset;
float lineTimer;

float VIEW_HEIGHT = 20.0f;  //Height Ratio Lock
float VIEW_WIDTH  = 0.0f;   //Width Ratio Calculated later

int WINDOWWIDTH = 1024;
int WINDOWHEIGHT = 768;
float centerLane = WINDOWWIDTH/2;

float rad = 0;
int numberOfShapes;

sf::Music music;
sf::Music powerTune;
sf::Music beep;
sf::Music powerDown;
sf::Music boop;
sf::Music highBeep;
sf::Music screechRight;
sf::Music screechLeft;

bool LEFT_MOUSE_DOWN;
bool HELD_MOUSE;
bool PRESS_MOUSE;
bool RIGHT_MOUSE_DOWN;
bool PRESS_UP;
bool PRESS_DOWN;
bool PRESS_LEFT;
bool PRESS_RIGHT;
bool PRESS_PLUS;
bool PRESS_MINUS;
bool PRESS_SPACE;
bool PRESS_ESCAPE;

bool pup,pdown,pleft,pright;
bool fullscreen = false;

float cx,cy,cr,cspeed,cradius;
float angle= 0;
int par = 10;
float delta;
float timeChunk = .001;
float timeOfLastDraw;

//Game Objects
vector<Shape*> objects;
Square* player;
Square* testSquare;

//Function declaration
void Display();
void Init();
void Reshape(int w, int h);
void OnKeyPress(unsigned char key, int x , int y);
void OnKeyUp(unsigned char key, int x, int y);
void OnMouseClick(int button, int state, int x, int y);
void CheckCollisions();
void GameLoop();
void UpdateScene();
void DrawTimer();
void GetCurrentTime();
void CalculateDrawTime();
void LoadObject();
void DrawCircle(float cx, float cy, float r, float R, float G, float B);
void DrawDisk(float cx, float cy, float r, float R, float G, float B);

int main(int argc, char** argv){
    std::cout << "Initializing GLUT...\n";
    cx      =   100;
    cy      =   WINDOWHEIGHT/2;
    cr      =   500;
    cspeed  =   .0025f;
    angle   =   0.0f; // "North"
    
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(400, 400);
    glutInitWindowSize(WINDOWWIDTH, WINDOWHEIGHT);
    glutCreateWindow("LD26 Minimalism");

    
    //Set GLUT Utility Functions

    glutReshapeFunc(Reshape);//Set resize function
    glutKeyboardFunc(OnKeyPress);
    glutKeyboardUpFunc(OnKeyUp);
    glutMouseFunc(OnMouseClick);
    glutIdleFunc(GameLoop);
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glutDisplayFunc(Display); //Set display function

    if (fullscreen){
        glutFullScreen();
    }
    
    if (!powerTune.openFromFile("powerUp.ogg")) {
        cout << "Error loading song..." << endl;
    }


    if (!music.openFromFile("jam.ogg")) {
        cout << "Error loading song..." << endl;
    }
    
    music.setLoop(true);
    
    if (!beep.openFromFile("beep.ogg")) {
        cout << "Error loading song..." << endl;
    }
    
    if (!powerDown.openFromFile("powerDown.ogg")) {
        cout << "Error loading song..." << endl;
    }
    
    if (!boop.openFromFile("boop.ogg")) {
        cout << "Error loading song..." << endl;
    }
    
    if (!highBeep.openFromFile("highBeep.ogg")) {
        cout << "Error loading song..." << endl;
    }
    
    
    if (!screechRight.openFromFile("screechRight.ogg")) {
        cout << "Error loading song..." << endl;
    }
    
    
    if (!screechLeft.openFromFile("screechLeft.ogg")) {
        cout << "Error loading song..." << endl;
    }
    
    music.play();
    
    Init();
    glutMainLoop();
    return 0;
}

void Init(){
    srand((unsigned int)time(NULL));
    player = new Square();
    player->sections = 4;
    player->x = WINDOWWIDTH/2;
    player->sizex = 40;
    player->sizey = 80;
    player->y = player->sizey + 10;
    player->active = true;
    player->unique = true;
    player->R = 1.0;
    player->G = 1.0f;
    player->B = 1.0f;
    player->vy = 0;
    player->vx = 0;
    
    objects.push_back(player);
    
    srand((unsigned int)time(NULL));
    
    int randomize = 0 + (int) (100.0 * (rand() / (RAND_MAX + 1.0)));
    
    for (int i = 0; i < randomize ; i ++){
        rand();
    }

    selectColor = 0 + (int) (difficulty * (rand() / (RAND_MAX + 1.0)));
    
    
    currentColor = (colors)selectColor;
    
}


void GameLoop(){
    //Calculate time before draw
    CalculateDrawTime();
    
    if (PRESS_ESCAPE){
        glutDestroyWindow(1);
        exit(0);
    }
    
    switch (gameState) {
            
        case intro:
        
            break;
            
        case title:
            break;
            
        case splashludum:
            break;
            
        case splashlogo:
            break;
            
        case levelselect:
            break;
            
        case game:
            UpdateScene();
            CheckCollisions();
            break;
            
        case win:
            break;
            
        case lose:
            break;
            
        case quit:
            break;
            
            
        default:
           // cout << "GAME STATE LOOP" << endl;
            break;
    }
    
    //cout << "Direction :" << player->direction << endl;
//    cout << "-----------------------------------" << endl;
//    cout << "Overlap " << player->overlapping << endl;
//    cout << "Enter " << player->entering << endl;
//    cout << "Exit " << player->exiting << endl;
//    cout << "Inside " << player->inside << endl;
//    cout << "Outside " << player->outside << endl;
//    cout << "In Coll " << player->internalCollision << endl;
//    cout << "Mouse " << LEFT_MOUSE_DOWN << endl;
//    cout << "HOLD M " << HELD_MOUSE << endl;
//    cout << "PRESS M " << PRESS_MOUSE << endl;
//cout << "X " << player->x << " y " << player->y << endl
    //cout << "timer " << timer << endl;
    //cout << "reset timer " << resetTimer << endl;
  // cout << "angle :"  << angle << endl;
    

    glutPostRedisplay();
    
    //Calculate time after draw
    GetCurrentTime();
}

void CalculateDrawTime() {
    delta = glutGet( GLUT_ELAPSED_TIME ) * timeChunk - timeOfLastDraw;
    if (delta == 0){
        delta = timeChunk;
    }
    //cout << "Delta Time: " << delta << endl;
}

void GetCurrentTime(){
    timeOfLastDraw = glutGet( GLUT_ELAPSED_TIME ) * timeChunk;
}

void LoadObject(){
    
}

void DrawTimer(){
    
    //Draw circle code
    float finalRadius = cr * cosf(1);


    for (float i = sections; i > 0; i--) {
        DrawDisk(cx, cy,
                 finalRadius*(i/sections),
                 timerR*(i/sections), timerG*(i/sections), timerB*(i/sections));
    }
    
    //Draw Line Showing orientation of circle

    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(10);
    glBegin(GL_LINES);
    glVertex2f(cx, cy);
    glVertex2f(cx + (finalRadius*cos(angle)), (cy + (finalRadius * sin(angle))));
    glEnd();

}

void Display(){
    
    if (gameFlipped) {
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    } else {
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    }
    glClear(GL_COLOR_BUFFER_BIT);

    DrawTimer();

    for (auto i = objects.begin() ; i != objects.end() ; i++){
        (*i)->Draw();
    }

    player->Draw();
   
    
    
    if (combo > maxCombo){
        maxCombo = combo;
    }
    
    //Draw text
    char string[120];
    char * buffer;
    
    glColor3f(1.0, 1.0, 1.0);

    
    if (combo > 0){
        sprintf(string,"%d", combo);
        buffer = string;
        glRasterPos2f(WINDOWWIDTH/2 - 2, WINDOWHEIGHT/2 - 12);
        for (int i = 0; i < strlen(buffer); i ++){
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,buffer[i]);
        }
    }
    
    
    sprintf(string,"-Traffic Jam-");
    buffer = string;
    glRasterPos2f(15, 10);
    for (int i = 0; i < strlen(buffer); i ++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,buffer[i]);
    }

    
    //GL END
    glutSwapBuffers();
}



void Reshape(int w, int h){
    float aspectRatio = (float)w/h;
    VIEW_WIDTH = aspectRatio * VIEW_HEIGHT;
    
    glViewport( 0, 0, w, h);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D(0,WINDOWWIDTH, 0, WINDOWHEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void OnKeyPress(unsigned char key , int x, int y){
    //std::cout << "Key: " << key << "\n";
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
            
        case 32:
            PRESS_SPACE = true;
            break;
            
        case 27:
            PRESS_ESCAPE = true;
            break;
            
    }
}

void OnKeyUp(unsigned char key, int x, int y){
    //std::cout << "Released Key: " << key << "\n";
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
            
        case 32:
            PRESS_SPACE = false;
            break;
    }
}


void OnMouseClick(int button,int state,int x, int y){
    if (button == GLUT_LEFT_BUTTON){
        if (state == GLUT_DOWN){
            if (PRESS_MOUSE){
                HELD_MOUSE = true;
                PRESS_MOUSE = false;
            } else {
                PRESS_MOUSE = true;
            }
            
            LEFT_MOUSE_DOWN = true;

            
        }else if (state == GLUT_UP){
            LEFT_MOUSE_DOWN = false;
            HELD_MOUSE = false;
            PRESS_MOUSE = false;

        }
    }
    
    if(button == GLUT_RIGHT_BUTTON){
        if (state == GLUT_DOWN){
            RIGHT_MOUSE_DOWN = true;

            
        }else if (GLUT_UP){
            RIGHT_MOUSE_DOWN = false;
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
    if (gameSpeed <= 200) {
        gameSpeed = 200;
    }
    //cr = 200;
    cx = WINDOWWIDTH/2;
    //check if radius is too small
    
    if (cradius < -gameSpeed){
        cradius = -gameSpeed;
    }

    cr =  cradius + (200 * combo + 2 );
    
    if (cr > 12000){
        cr = 12000;
    }
    
//    if (cr <= cradius){
//        cr = 0;
//        cradius = 0;
//    }
    
    //cout << " Cr "  << cr << " Cradius " << cradius << endl;
    
    if (cr < 100){
        cr = 100;
    }
    

    
    
//    if (cradius < 1){
//        cradius = 1;
//    }
    
    char title[1024];
    sprintf(title, "Max Combo - %d" , maxCombo);
    
    glutSetWindowTitle(title);

    
    switch (currentColor) {
        case white:
            player->R = 1.0f;
            player->G = 1.0f;
            player->B = 1.0f;
            
            timerR = 1.0f;
            timerG = 1.0f;
            timerB = 1.0f;
            
            break;
            
        case red:
            player->R = 1.0f;
            player->G = 0.0f;
            player->B = 0.0f;
            
            timerR = 1.0f;
            timerG = 0.0f;
            timerB = 0.0f;

            break;
            
        case green:
            player->R = 0.0f;
            player->G = 1.0f;
            player->B = 0.0f;
            
            timerR = 0.0f;
            timerG = 1.0f;
            timerB = 0.0f;

            break;
            
        case blue:
            player->R = 0.0f;
            player->G = 0.0f;
            player->B = 1.0f;
            
            timerR = 0.0f;
            timerG = 0.0f;
            timerB = 1.0f;

            break;
            
        case purple:
            //R+B
            player->R = 1.0f;
            player->G = 0.0f;
            player->B = 1.0f;
            
            timerR = 1.0f;
            timerG = 0.0f;
            timerB = 1.0f;
            
            break;
            
            
        default:
            break;
    }
    
    cradius-= delta * gameSpeed * .15;
    angle -= cspeed * gameSpeed * .8 * delta;
    fmod(angle, 360.0f);
   // cout << angle << endl;
    
    if (angle < -4.7){
        angle = 1.6;
        
        
        //highBeep.setPitch(2);
        highBeep.setVolume(50);
        highBeep.play();
        
        selectColor = 0 + (int) (difficulty * (rand() / (RAND_MAX + 1.0)));
           
            while (selectColor == currentColor){
                 selectColor = 0 + (int) (difficulty * (rand() / (RAND_MAX + 1.0)));
            }
        currentColor = (colors)selectColor;
        
        gameSpeed-= gameSpeed * .25;
        
        cradius -= 10;
        //cradius = 0;


    }
    
    
//    if (angle <= -3 && angle >= -4 ){
//        if (beep.getStatus() == 0){
//            beep.play();
//        }
//    }
    
    if (angle < -0.5){
        beep.setPitch(abs(angle));
        beep.setVolume(abs(angle)*10);
        
        if (beep.getStatus() == 0){
            beep.play();
        }
    }
    
    
    timer-=delta;
    lineTimer -=delta;
    
    fullTime = (1 - (gameSpeed *.001));
    
    if (fullTime < .12){
        fullTime = .12;
    }
    
    
    if (gameSpeed > 400 * difficulty){
        gameSpeed = 400 * difficulty;
    }
    
    if (timer <= 0){
        if (resetTimer){
            timer = fullTime;
            resetTimer= false;
        }
        if (timer<=0){
            resetTimer = true;
        }
    }
    

    if (resetTimer){
        
        testSquare = 0;
        testSquare = new Square();
        testSquare->sizex = 20;
        testSquare->sizey = 40;
        testSquare->y = WINDOWHEIGHT + (testSquare->sizey) + difficulty * 10;
        testSquare->x = centerLane - testSquare->sizex/2;
        testSquare->sections = 2;
        testSquare->unique = true;
        testSquare->R = 0.5;
        testSquare->G = 0.5;
        testSquare->B = 0.5;
        
        
        objects.push_back(testSquare);
        testSquare = 0;
        
        
        int side = 0 + (int) (4.0 * (rand() / (RAND_MAX + 1.0)));
        int type = 0 + (int) (difficulty * (rand() / (RAND_MAX + 1.0)));
        
        int chain = 0 + (int) ((7.0 - difficulty) * (rand() / (RAND_MAX + 1.0)));
        
        //cout << "side " << side << endl;
        testSquare = new Square();
        testSquare->sections = 2;
        testSquare->sizex = 50;
        testSquare->sizey = 100;
        
        if (side == 0){
        testSquare->x = centerLane + 85 - testSquare->sizex/2 ;
        } else if (side ==1) {
            testSquare->x = centerLane - 85 - testSquare->sizex/2 ;
        }
        
        switch (type){
                
            case white:
                testSquare->unique= true;
                testSquare->R = 1;
                testSquare->G = 1;
                testSquare->B = 1;
                testSquare->currentColor = white;
                
                break;
            
            case red:
                testSquare->unique= true;
                testSquare->R = 1.0;
                testSquare->currentColor = red;
                break;
                
            case green:
                testSquare->unique= true;
                testSquare->G = 1.0;
                testSquare->currentColor = green;
                break;
                
            case blue:
                testSquare->unique= true;
                testSquare->B = 1.0;
                testSquare->currentColor = blue;
                break;
                
            case purple:
                testSquare->unique= true;
                testSquare->B = 1.0;
                testSquare->R = 1.0;
                testSquare->currentColor = purple;
                break;
                
            default:
                break;
        }
        
        //override and place NEEDED chain block in
        if (chain == 1){
            cout << "Saved your soul" << endl;
            testSquare->unique= true;

            switch (currentColor) {
                case white:
                    testSquare->R = 1;
                    testSquare->G = 1;
                    testSquare->B = 1;
                    testSquare->currentColor = white;
                    break;
                    
                case red:
                    testSquare->R = 1;
                    testSquare->G = 0;
                    testSquare->B = 0;
                    testSquare->currentColor = red;
                    break;
                    
                case green:
                    testSquare->R = 0;
                    testSquare->G = 1;
                    testSquare->B = 0;
                    testSquare->currentColor = green;
                    break;
                    
                case blue:
                    testSquare->R = 0;
                    testSquare->G = 0;
                    testSquare->B = 1;
                    testSquare->currentColor = blue;
                    break;
                    
                case purple:
                    testSquare->R = 1;
                    testSquare->G = 0;
                    testSquare->B = 1;
                    testSquare->currentColor = purple;
                    break;
                    
                default:
                    break;
            }
            
        }
        
        testSquare->y = WINDOWHEIGHT + testSquare->sizey ;
        testSquare->vy = -10;
        testSquare->active = true;
        
        numberOfShapes++;
        testSquare->number = numberOfShapes;
        
        if (side == 0 || side == 1){
            objects.push_back(testSquare);
        }
        testSquare = 0;
        
    }
    
    if (PRESS_SPACE){
        if (!movingRight){
            movingRight= true;
            screechRight.play();
        }
        movingLeft = false;
        gameFlipped = true;
    } else {
        if (!movingLeft){
            screechLeft.play();
            movingLeft = true;
        }
        gameFlipped = false;
        movingRight = false;
    }
    
    
    
    //switch sides when pressed
    if (gameFlipped){
        player->x = centerLane - 85 - player->sizex/2;
    } else {
        player->x = centerLane  + 85 - player->sizex/2;
    }
    
    
    for (auto i = objects.begin() ; i != objects.end() ;){
        (*i)->Update(delta);
        
        if ((*i) != player){
            (*i)->vy = - (gameSpeed*1);
        }
        
        if ((*i)->y < 0 + (*i)->sizey + 5){
            i = objects.erase(i);
            boop.play();
            gameSpeed+=10;
           
        } else {
            i++;
        }
    
    }
    
    
    if (PRESS_UP){
        gameSpeed++;
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
        delta = 0;
    }
    
    if(RIGHT_MOUSE_DOWN){
        gameFlipped = true;
    }
    
    if (PRESS_PLUS) {
        combo++;
    }
    
    if (PRESS_MINUS){

    }
}


void CheckCollisions(){
    for (auto i = objects.begin() ; i !=objects.end(); i++){
        Shape* test = *i;
        if (test != player) {
            player->Collide((*test));
            
            //if player collides with object of same color then score goes up
            if (player->overlapping && test->currentColor == currentColor){
                objects.erase(i);
                angle -= cspeed * gameSpeed * .1 * delta;
                gameSpeed += 15 * difficulty;
                //cr+=gameSpeed / 5;
                combo++;
                level+= 1 * combo;
                powerTune.setVolume(50);
                powerTune.play();

            }
            
            //if player collides with object of same color then score goes down
            if (player->overlapping && test->currentColor != currentColor){
                level--;
                gameSpeed-=gameSpeed * .2;
                
                cradius = 0;
                cr = 1;

                
                if (level< 1){
                    level = 1;
                }
                objects.erase(i);
                combo = 0;
                powerDown.play();
                
    
            }
        }
    }
}
















