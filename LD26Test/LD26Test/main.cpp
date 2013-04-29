/*
 =================
LD 26 Minimalism 
 By Evan Chapman
 
 =================
 */

#include <iostream>
#include <string>
#include <cmath>
#include <OpenGL/OpenGL.h>
#include <SFML/Audio.hpp>
#include <SFML/Audio/SoundSource.hpp>
#include <SFML/Graphics.hpp>
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
int difficulty = 3;
int gameSpeed;
float speedFactor;

unsigned int combo;
unsigned int maxCombo;
bool comboBroken;

//Konami Code
int kup,kdown,kleft,kright;

bool movingRight;
bool movingLeft = true;

bool pauseGame = false;

int sections = 5;

enum colors {white,red,green,blue,purple};
int selectColor;

bool deflateTimer = false;
float deflateSpeed = 100;
float deflateAmount;

float finalCR;

colors currentColor = white;

float timerR , timerG , timerB;

float fullTime = 1;
bool resetTimer;
float timer;

float bonusFullTime = 1;
bool bonusActive= false;
bool killBonus;
float bonusTimer;

bool resetLevel = true;

float colorChangeTimerFull = .09;
float colorChangeTimer = colorChangeTimerFull;

float bonusR,bonusG,bonusB;

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
sf::Music psychedelic;
sf::Music bummer;
sf::Music wimp;
sf::Music trafficjam;
sf::Music combobreaker;

sf::Music  easy;
sf::Music  normal;
sf::Music  hard;
sf::Music  insanity;
sf::Music  potato;

sf::Music comboSound[25];


bool newCombo = false;
bool brokeCombo= false;

sf::Image ludumIntro;

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
void SpecialKey(int key, int x ,int y);
void SpecialKeyUp(int key, int x, int y);
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
    cr      =   0;
    cspeed  =   .0025f;
    angle   =   0.0f; // "North"
    
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-WINDOWWIDTH)/2,
                           (glutGet(GLUT_SCREEN_HEIGHT)-WINDOWHEIGHT)/2);
    
    glutInitWindowSize(WINDOWWIDTH, WINDOWHEIGHT);
    glutCreateWindow("LD26 Minimalism");

    
    //Set GLUT Utility Functions

    glutReshapeFunc(Reshape);//Set resize function
    glutKeyboardFunc(OnKeyPress);
    glutSpecialFunc(SpecialKey);
    glutSpecialUpFunc(SpecialKeyUp);
    glutKeyboardUpFunc(OnKeyUp);
    glutMouseFunc(OnMouseClick);
    glutIdleFunc(GameLoop);
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glutDisplayFunc(Display); //Set display function

    if (fullscreen){
        glutFullScreen();
    }
    
    if (!powerTune.openFromFile("powerup.ogg")) {
        cout << "Error loading song..." << endl;
    }


    if (!music.openFromFile("game.ogg")) {
        cout << "Error loading song..." << endl;
    }
    
    music.setLoop(true);
    music.setVolume(45);
    
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
    
    
    if (!easy.openFromFile("easy.ogg")){
        cout << "Error loading sound..." << endl;
    }
    if (!normal.openFromFile("normal.ogg")){
        cout << "Error loading sound..."<< endl;
    }
    if (!hard.openFromFile("hard.ogg")){
        cout << "Error loading sound..."<< endl;
    }
    if (!insanity.openFromFile("insanity.ogg")){
        cout << "Error loading sound..."<< endl;
    }
    if (!potato.openFromFile("potato.ogg")){
        cout << "Error loading sound..."<< endl;
    }
    
    
    if (!psychedelic.openFromFile("psychedelic.ogg")){
        cout << "Error loading sound..."<< endl;
    }
    
    if (!bummer.openFromFile("bummer.ogg")){
        cout << "Error loading sound..."<< endl;
    }
    
    if (!combobreaker.openFromFile("combobreaker.ogg")){
        cout << "Error loading sound..."<< endl;
    }
    
    
    if (!trafficjam.openFromFile("trafficjam.ogg")){
        cout << "Error loading sound..."<< endl;
    }
    
    
    

    
    
    if (!comboSound[1].openFromFile("combo1.ogg")){
        cout << "Error loading combo sound..." << endl;
    }
    if (!comboSound[2].openFromFile("combo2.ogg")){
        cout << "Error loading combo sound..." << endl;
    }
    if (!comboSound[3].openFromFile("combo3.ogg")){
        cout << "Error loading combo sound..." << endl;
    }
    
    if (!comboSound[4].openFromFile("combo4.ogg")){
        cout << "Error loading combo sound..." << endl;
    }
    
    if (!comboSound[5].openFromFile("combo5.ogg")){
        cout << "Error loading combo sound..." << endl;
    }
    
    if (!comboSound[6].openFromFile("combo6.ogg")){
        cout << "Error loading combo sound..." << endl;
    }
    
    if (!comboSound[7].openFromFile("combo7.ogg")){
        cout << "Error loading combo sound..." << endl;
    }
    
    if (!comboSound[8].openFromFile("combo8.ogg")){
        cout << "Error loading combo sound..." << endl;
    }
    
    if (!comboSound[9].openFromFile("combo9.ogg")){
        cout << "Error loading combo sound..." << endl;
    }
    
    if (!comboSound[10].openFromFile("combo10.ogg")){
        cout << "Error loading combo sound..." << endl;
    }
    
    if (!comboSound[11].openFromFile("combo11.ogg")){
        cout << "Error loading combo sound..." << endl;
    }
    
    if (!comboSound[12].openFromFile("combo12.ogg")){
        cout << "Error loading combo sound..." << endl;
    }
    
    if (!comboSound[13].openFromFile("combo13.ogg")){
        cout << "Error loading combo sound..." << endl;
    }
    
    if (!comboSound[14].openFromFile("combo14.ogg")){
        cout << "Error loading combo sound..." << endl;
    }
    
    if (!comboSound[15].openFromFile("combo15.ogg")){
        cout << "Error loading combo sound..." << endl;
    }
    
    if (!comboSound[16].openFromFile("combo16.ogg")){
        cout << "Error loading combo sound..." << endl;
    }
    
    if (!comboSound[17].openFromFile("combo17.ogg")){
        cout << "Error loading combo sound..." << endl;
    }
    
    if (!comboSound[18].openFromFile("combo18.ogg")){
        cout << "Error loading combo sound..." << endl;
    }
    
    if (!comboSound[19].openFromFile("combo19.ogg")){
        cout << "Error loading combo sound..." << endl;
    }
    if (!comboSound[20].openFromFile("combo20.ogg")){
        cout << "Error loading combo sound..." << endl;
    }
    
    
    
    if (!ludumIntro.loadFromFile("ludumintro.png")){
        cout << "Error loading image..." << endl;
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
    player->sizex = 50;
    player->sizey = 100;
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
    
    if(!pauseGame){
        CalculateDrawTime();
    }else{
        delta = 0;
        timeOfLastDraw = 0;
    }
    
    if (PRESS_ESCAPE){
        glutDestroyWindow(1);
        powerDown.stop();
        powerTune.stop();
        boop.stop();
        highBeep.stop();
        screechLeft.stop();
        screechRight.stop();
        beep.stop();
        
        
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


    glutPostRedisplay();
    
    //Calculate time after draw
    if (!pauseGame){
        GetCurrentTime();
    } else{
        delta = 0;
        timeOfLastDraw = 0;
    }
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

    glClear(GL_COLOR_BUFFER_BIT);

    DrawTimer();

    for (auto i = objects.begin() ; i != objects.end() ; i++){
        (*i)->Draw();
    }

    player->Draw();
   
    if (combo > maxCombo){
        maxCombo = combo;
        if (!comboBroken && maxCombo >2 ){
            combobreaker.play();
        }
        comboBroken = true;
    }
    
    //Draw text
    char string[120];
    char * buffer;

    cout << "cr " << cr << endl;
    //if circle encompasses screen change text to black
    if (cr >= 690){
        glColor3f(0.0, 0.0, 0.0);
    } else {
        glColor3f(1.0, 1.0, 1.0);
    }
    
    if (combo > 0){
        sprintf(string,"%d", combo);
        buffer = string;
        int stringLength = 0;
        
        for (int i = 0; i < strlen(buffer); i ++){
            stringLength += glutBitmapWidth(GLUT_BITMAP_HELVETICA_18, buffer[i]);
        }
        
        glRasterPos2f(WINDOWWIDTH/2 - stringLength/2, WINDOWHEIGHT/2 - 7);
        for (int i = 0; i < strlen(buffer); i ++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,buffer[i]);
        }
    }
    
    
    switch (difficulty) {
        case 1:
            sprintf(string,"Potato Mode");
            break;
            
        case 2:
            sprintf(string,"Easy Mode Max Combo %d" , maxCombo);
            break;
            
        case 3:
            sprintf(string,"Normal Mode Max Combo %d" , maxCombo);
            break;
            
        case 4:
            sprintf(string,"Hard Mode Max Combo %d" , maxCombo);
            break;
            
        case 5:
            sprintf(string,"Insanity Mode Max Combo %d" , maxCombo);
            break;
            
        default:
            break;
    }
    
    buffer = string;
    
    int stringLength = 0;
    
    for (int i = 0; i < strlen(buffer); i ++){
        stringLength += glutBitmapWidth(GLUT_BITMAP_HELVETICA_18, buffer[i]);
    }
    
    glRasterPos2f((WINDOWWIDTH/2) - stringLength / 2, WINDOWHEIGHT - 20);
    for (int i = 0; i < strlen(buffer); i ++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,buffer[i]);
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
            delta = 0;
            break;
            
        case 32:
            PRESS_SPACE = true;
            break;
            
        case 27:
            PRESS_ESCAPE = true;
            break;
            
        case 'p':
            combo++;
            break;
            
            
        case '0':
            difficulty = 1;
            resetLevel = true;
            maxCombo = 0;
            gameSpeed = difficulty * 100;
            angle = -4.7;
            if (potato.getStatus() == 0 ){
                potato.play();
            }
            break;
            
        case '1':
            difficulty = 2;
            maxCombo = 0;
            resetLevel = true;
            gameSpeed = difficulty * 100;
            angle = -4.7;
            if (easy.getStatus() == 0 ){
                easy.play();
            }
            break;
            
        case '2':
            difficulty = 3;
            maxCombo = 0;
            resetLevel = true;
            gameSpeed = difficulty * 100;
            angle = -4.7;
            if (normal.getStatus() == 0 ){
                normal.play();
            }
            break;
            
        case '3':
            difficulty = 4;
            maxCombo = 0;
            resetLevel = true;
            gameSpeed = difficulty * 100;
            angle = -4.7;
            if (hard.getStatus() == 0 ){
                hard.play();
            }
            break;
            
        case '4':
            difficulty = 5;
            maxCombo = 0;
            resetLevel = true;
            gameSpeed = difficulty * 100;
            angle = -4.7;
            if (insanity.getStatus() == 0 ){
                insanity.play();
            }
            break;
            
        
    }
}

void OnKeyUp(unsigned char key, int x, int y){
    //std::cout << "Released Key: " << key << "\n";
    switch (key){
        case 'w':

            break;
            
        case 32:
            PRESS_SPACE = false;
            break;
    }
}


void SpecialKey(int key, int x , int y){
    switch (key){
            //up
        case 101:
            if (!PRESS_UP){
                kup++;
            }
            
            if (kup > 2){
                kup = kdown = kleft = kright = 0;
            }
            PRESS_UP = true;
            
            break;
            
            //down
        case 103:
            if (!PRESS_DOWN){
                kdown++;
            }
            
            if (kdown > 2){
                kup = kdown = kleft = kright = 0;
            }
                PRESS_DOWN = true;
            break;
            
            //left
        case 100:
            if (!PRESS_LEFT){
                kleft++;
            }
            if (kleft > 2){
                kup = kdown = kleft = kright = 0;
            }
            PRESS_LEFT = true;
            break;
            
            //right
        case 102:
            if (!PRESS_RIGHT){
                kright++;
            }
            if (kright > 2){
                kup = kdown = kleft = kright = 0;
            }
            PRESS_RIGHT = true;
            break;
            
        default:
            break;
    }
}


void SpecialKeyUp(int key , int x ,int y ){
    switch (key)   {
            
        case 101:
            PRESS_UP = false;
            break;
            
        case 103:
            PRESS_DOWN = false;
            break;
            
        case 100:
            PRESS_LEFT = false;
            break;
            
        case 102:

            PRESS_RIGHT = false;
            break;
            
        default:
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
    powerDown.setPitch(.2 +gameSpeed * .001);
    powerTune.setPitch(.2 +gameSpeed * .001);
    music.setPitch(.999 + finalCR * .00001);
    
    
    if (combo == 5){
        if (newCombo == false){
        comboSound[1].setVolume(100);
        comboSound[1].play();
        }
        newCombo = true;
    }
    
    if (combo == 10){
        if (newCombo == false){
            comboSound[2].setVolume(100);
            comboSound[2].play();
        }
        newCombo = true;
    }
    
    
    if (combo == 15){
        if (newCombo == false){

            comboSound[3].setVolume(200);
            comboSound[3].play();
        }
        newCombo = true;
    }
    

    if (combo == 20){
        if (newCombo == false){

            comboSound[4].setVolume(200);
            comboSound[4].play();
        }
        newCombo = true;
    }
    if (combo == 26){
        if (newCombo == false){
            comboSound[5].setVolume(200);
            comboSound[5].play();
        }
        newCombo = true;
    }
    if (combo == 30){
        if (newCombo == false){
            comboSound[6].setVolume(200);
            comboSound[6].play();
        }
        newCombo = true;
    }
    if (combo == 35){
        if (newCombo == false){

            comboSound[7].setVolume(200);
            comboSound[7].play();
        }
        newCombo = true;
    }
    if (combo == 40){
        if (newCombo == false){

            comboSound[8].setVolume(200);
            comboSound[8].play();
        }
        newCombo = true;
    }
    if (combo == 45){
        if (newCombo == false){

            comboSound[9].setVolume(200);
            comboSound[9].play();
        }
        newCombo = true;
    }
    if (combo == 50){
        if (newCombo == false){

            comboSound[10].setVolume(200);
            comboSound[10].play();
        }
        newCombo = true;
    }
    if (combo == 55){
        if (newCombo == false){

            comboSound[11].setVolume(200);
            comboSound[11].play();
        }
        newCombo = true;
    }
    if (combo == 60){
        if (newCombo == false){

            comboSound[12].setVolume(200);
            comboSound[12].play();
        }
        newCombo = true;
    }
    if (combo == 65){
        if (newCombo == false){

            comboSound[13].setVolume(200);
            comboSound[13].play();
        }
        newCombo = true;
    }
    if (combo == 70){
        if (newCombo == false){

            comboSound[14].setVolume(200);
            comboSound[14].play();
        }
        newCombo = true;
    }
    if (combo == 75){
        if (newCombo == false){

            comboSound[15].setVolume(200);
            comboSound[15].play();
        }
        newCombo = true;
    }
    if (combo == 80){
        if (newCombo == false){

            comboSound[16].setVolume(200);
            comboSound[16].play();
        }
        newCombo = true;
    }

    if (combo == 85){
        if (newCombo == false){
            
            comboSound[17].setVolume(200);
            comboSound[17].play();
        }
        newCombo = true;
    }
    
    if (combo == 90){
        if (newCombo == false){
            
            comboSound[18].setVolume(200);
            comboSound[18].play();
        }
        newCombo = true;
    }
    
    if (combo == 95){
        if (newCombo == false){
            
            comboSound[19].setVolume(200);
            comboSound[19].play();
        }
        newCombo = true;
    }
    
    if (combo >= 100 && combo % 5 == 0){
        if (newCombo == false){
            
            comboSound[20].setVolume(200);
            comboSound[20].play();
        }
        newCombo = true;
    }
    
    
    if (resetLevel) {
        combo = 0;
        angle = 1.6;
        bonusActive = false;
        kup = 4;
        comboBroken = false;
        highBeep.play();
        speedFactor = .5;
        gameSpeed = 0;
        for (int i = 0 ; i < difficulty ; i++){
            speedFactor+=speedFactor;
        }
        
        gameSpeed =  speedFactor * 50;
        
        selectColor = 0 + (int) (difficulty * (rand() / (RAND_MAX + 1.0)));
        if (difficulty !=1){
            while (selectColor == currentColor){
                selectColor = 0 + (int) (difficulty * (rand() / (RAND_MAX + 1.0)));
            }
        }
        currentColor = (colors)selectColor;
        
        for (auto i = objects.begin() ; i != objects.end();){
            Shape* test = *i;
            if (test != player && test->isObstacle){
               i = objects.erase(i);
            } else {
                i++;
            }
        }
        resetLevel = false;
    }
    
    if (gameSpeed <= 200) {
        gameSpeed = 200;
    }
    //cr = 200;
    cx = WINDOWWIDTH/2;
    //check if radius is too small
    
    if (deflateTimer){
        deflateSpeed = 1000;
    } else {
        deflateSpeed = 100;
    }


    finalCR =  (difficulty * 2) * gameSpeed * .1 + (10 * (combo+1));
    
    if (cr < finalCR) {
        cr+= deflateSpeed * delta;
    }
    
    if (cr > finalCR){
        cr-= deflateSpeed * delta;
    }
    
    if (cr > 12000){
        cr = 12000;
    }
    
    

    
    //Set Window Title To max combo
    char title[1024];
    sprintf(title, "Traffic Jam");
    
    glutSetWindowTitle(title);

    //Set player and timer to correct color 
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

    //Change size and angle of timer per game speed
    cradius-= delta * 100;


    angle -= cspeed * gameSpeed * .8 * delta;
    fmod(angle, 360.0f);

    
    if (angle < -4.7){
        angle = 1.6;
        
        if (bonusTimer < 0 && bonusActive){
            killBonus = true;
        } else {
            killBonus = false;
        }
        

        //kill bonus when timer finishes
        if (bonusTimer <= 0 && killBonus){
            if (bonusActive){
                for (auto i = objects.begin() ; i != objects.end();){
                    Shape* test = *i;
                    if (test != player){
                       i = objects.erase(i);
                    } else {
                        i++;
                    }
                }
                gameSpeed *= .9;
            }
            
            bonusActive = false;
        }
        
        
        //highBeep.setPitch(2);
        highBeep.setVolume(55);
        highBeep.play();
        
        cout << "random" << endl;
        selectColor = 0 + (int) (difficulty * (rand() / (RAND_MAX + 1.0)));
        
        if (difficulty !=1){
            while (selectColor == currentColor){
                 selectColor = 0 + (int) (difficulty * (rand() / (RAND_MAX + 1.0)));
            }
        }
        currentColor = (colors)selectColor;
        
        gameSpeed-= gameSpeed * .15;
        
        cradius -= 10;
        //cradius = 0;
    }
    
    

    
    if (angle < -.5){
        beep.setPitch(abs(angle));
        beep.setVolume(5 + abs(angle)*10);

        if (beep.getStatus() == 0){
            beep.play();
        }
    }
    
    
    timer-=delta;
    bonusTimer-=delta;
    colorChangeTimer-=delta;
    
    fullTime = (1 - (gameSpeed *.0007));
    
    
    if (colorChangeTimer < 0){
        colorChangeTimer = colorChangeTimerFull;
        
        bonusR =  (1 + (int) ((100.0 - difficulty) * (rand() / (RAND_MAX + 1.0)))) * .01;
        bonusG =  (1 + (int) ((100.0 - difficulty) * (rand() / (RAND_MAX + 1.0))))* .01;
        bonusB =  (1 + (int) ((100.0 - difficulty) * (rand() / (RAND_MAX + 1.0))))* .01;
        
    }
    
    
    //Timer speed
    if (fullTime < .15){
        fullTime = .15;
    }
    
    
    
    if (gameSpeed > 100 + 250 * difficulty){
        gameSpeed = 100 + 250 * difficulty;
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
    
    //cout << "bonus active " << bonusActive << endl;
    if (bonusTimer>0){
        bonusActive = true;
    }

    
    if (resetTimer){
        
        //Create center lane square
        testSquare = 0;
        testSquare = new Square();
        testSquare->isObstacle = true;
        testSquare->sizex = 20;
        testSquare->sizey = 40;
        testSquare->y = WINDOWHEIGHT + (testSquare->sizey) + difficulty * 10;
        testSquare->x = centerLane - testSquare->sizex/2;
        testSquare->sections = 1;
        testSquare->unique = true;
        testSquare->R = 0.5;
        testSquare->G = 0.5;
        testSquare->B = 0.5;
        
        
        objects.push_back(testSquare);
        testSquare = 0;
        
        //start creation of random squares
        int side = 0 + (int) (3.0 * (rand() / (RAND_MAX + 1.0)));
        int type = 0 + (int) (difficulty * (rand() / (RAND_MAX + 1.0)));
        
        int chain = 0 + (int) ((9.0 - difficulty) * (rand() / (RAND_MAX + 1.0)));
        
        int bonus =  0 + (int) (difficulty * 10 * (rand() / (RAND_MAX + 1.0)));
        
        if (kup == 2 && kdown == 2 && kright ==2 && kleft ==2){
            bonus = 1 ;
            bonusActive = true;
        };
        
        testSquare = new Square();
        testSquare->sections = 1;
        testSquare->sizex = 50;
        testSquare->sizey = 100;
        testSquare->isObstacle = true;
        
        if (bonus == 1 && !bonusActive){
            testSquare->isBonus = true;
        }
        
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
        if (chain <= 1){
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
    
    //if you press space , switch sides1
    
    if (PRESS_SPACE){
        if (!movingRight){
            movingRight= true;
            screechRight.setVolume(20);
            screechRight.play();
        }
        movingLeft = false;
        gameFlipped = true;
    } else {
        if (!movingLeft){
            screechLeft.setVolume(20);
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
    
    //update all objects
    for (auto i = objects.begin() ; i != objects.end() ;){
        (*i)->Update(delta);
        
        if ((*i)->isBonus){
            (*i)->R = bonusR;
            (*i)->G = bonusG;
            (*i)->B = bonusB;
        }
        
        //set every objects speed to game speed
        if ((*i) != player){
            (*i)->vy = - (gameSpeed*1);
        }
        
        //if object passes bottom, remove it
        if ((*i)->y < 0 + (*i)->sizey + 5){
            i = objects.erase(i);
            boop.setVolume(15);
            boop.play();
            gameSpeed += 5 * difficulty;
           
        } else {
            i++;
        }
    
    }
    


    
    //when bonus active change colors
    if (bonusActive){
        for (auto i = objects.begin() ; i!= objects.end(); i ++){
            Shape* test = *i;
            test->R = bonusR;
            test->G = bonusG;
            test->B = bonusB;
            test->currentColor = currentColor;
        }
        
        timerR = bonusR;
        timerG = bonusG;
        timerB = bonusB;
        
    }
    
}


void CheckCollisions(){
    for (auto i = objects.begin() ; i !=objects.end(); i++){
        Shape* test = *i;
        if (test != player) {
            player->Collide((*test));
            
            if (player->overlapping){
                if (test->isBonus){
                    bonusTimer = bonusFullTime;

                    psychedelic.setVolume(400);
                    psychedelic.play();
                    
                    angle = 1.6;
                    objects.erase(i);
                    angle -= cspeed * gameSpeed * .1 * delta;
                    gameSpeed += 15 * difficulty;
                    //cr+=gameSpeed / 5;
                    combo++;
                    newCombo = false;
                    level+= 1 * combo;
                    powerTune.setVolume(20);
                    powerTune.play();
                    break;
                }
            }
            
            //if player collides with object of same color then score goes up
            if (player->overlapping && test->currentColor == currentColor){
                objects.erase(i);
                angle -= cspeed * gameSpeed * .1 * delta;
                gameSpeed += 15 * difficulty;
                //cr+=gameSpeed / 5;
                combo++;
                newCombo = false;
                level+= 1 * combo;
                powerTune.setVolume(20);
                powerTune.play();

            }
            
            //if player collides with object of same color then score goes down
            if (player->overlapping && test->currentColor != currentColor){
                level--;
                gameSpeed-=gameSpeed * .20;
                
                cradius = 0;
                cr = 1;

                if (level< 1){
                    level = 1;
                }
                objects.erase(i);
                resetLevel = true;
                combo = 0;
                bummer.setVolume(200);
                bummer.play();
                powerDown.setVolume(20);
                powerDown.play();
                
            }
        }
    }
}
















