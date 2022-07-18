#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<sstream>
#include<algorithm>

#include <math.h>
#include <stdio.h>
#include <iostream>
#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"


#include <random>
#include <cstdlib>
#include <ctime>
#include <Camera.h>

#include <Windows.h>
#include <MMSystem.h>


#include <glut.h>


using namespace std;

#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)


int WIDTH = 1280;
int HEIGHT = 720;

GLdouble fovy = 45.0;
GLdouble aspectRatio = (GLdouble)WIDTH / (GLdouble)HEIGHT;
GLdouble zNear = 0.1;
GLdouble zFar = 1000;

Model_3DS car;
Model_3DS smallHouse;
Model_3DS FarmHouse;
Model_3DS worksMan;
Model_3DS wolf;
Model_3DS cone;
Model_3DS road;
Model_3DS myRoad;
Model_3DS booster;
Model_3DS Tree;
Model_3DS TreeC;
Model_3DS dragon;
Model_3DS nakhla;
Model_3DS stone;
Model_3DS sandAnimal;
Model_3DS sec_car;

GLuint tex;
GLTexture tex_ground;
GLTexture tex_landscape;

GLuint texsand;
GLTexture tex_sand;


GLfloat worksManRot = 0;
GLfloat boosterAnimat = 0;

Camera camera(0.0224351,  1.80516,  4.02305,  0.0120116,  1.62228,  3.03997, - 0.00193887,  0.983137, - 0.182862);
Camera cameraSand(0.0224351, 1.80516, 4.02305, 0.0120116, 1.62228, 3.03997, -0.00193887, 0.983137, -0.182862);

boolean xDirection = true;

float barriers[6];
const char barriersNames[6] = { 'w', 'o', 'c', 'w', 'o', 'c' };
int barrierPos[6];

float sandBarriers[9];
const char sandBarriersNames[9] = { 'f', 's', 'w', 'o', 'f', 's', 'w', 'o', 'f' };
int sandBarriersPos[9];

GLuint roadLength = 0;
float moveCar = 0;
float LR = 0;

float moveCarSand = 0;
float LRSand = 0;

float secCarSpeed = 0;

float lightX = 0;

int score = 0;

float darkness = 0.5;

bool changeCamera = false;
bool secScene = true;
bool secCarWon = false;

bool playSound = true;

bool boosterTaken = false;

int countColision = 0;
bool gameOver = false;

void InitBarriers() {

    int pos = -10;
    for (int i = 0; i < 6; i++)
    {
        float r3 = -1.15 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.2 - (-1.15))));
        barriers[i] = r3;

        barrierPos[i] = pos;
        pos -= 7;
    }


}

void InitsandBarrier() {
    int pos = -10;
    for (int i = 0; i < 9; i++)
    {
        float r3 = -1.25 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.3 - (-1.25))));
        sandBarriers[i] = r3;

        sandBarriersPos[i] = pos;
        pos -= 10;
    }

}

void InitLightSource()
{
    // Enable Lighting for this OpenGL Program
    glEnable(GL_LIGHTING);

    // Enable Light Source number 0
    // OpengL has 8 light sources
    glEnable(GL_LIGHT0);

    // Define Light source 0 ambient light
    GLfloat ambient[] = { 0.1f, 0.1f, 0.1, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

    // Define Light source 0 diffuse light
    GLfloat diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

    // Define Light source 0 Specular light
    GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

    // Finally, define light source 0 position in World Space
    GLfloat light_position[] = { 0, 3.0f, 0, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

//=======================================================================
// Material Configuration Function
//======================================================================
void InitMaterial()
{
    // Enable Material Tracking
    glEnable(GL_COLOR_MATERIAL);

    // Sich will be assigneet Material Properties whd by glColor
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // Set Material's Specular Color
    // Will be applied to all objects
    GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

    // Set Material's Shine value (0->128)
    GLfloat shininess[] = { 96.0f };
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

void myInit(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    gluPerspective(fovy, aspectRatio, zNear, zFar);
    //*******************************************************************************************//
    // fovy:			Angle between the bottom and top of the projectors, in degrees.			 //
    // aspectRatio:		Ratio of width to height of the clipping plane.							 //
    // zNear and zFar:	Specify the front and back clipping planes distances from camera.		 //
    //*******************************************************************************************//

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

    if (gameOver) {
        camera.gameOver();
    }

    else if (secScene) {
        if (!changeCamera) {
            camera.ThirdView(moveCar, LR);
        }

        else {
            camera.FirstView(moveCar, LR);
        }
    }
    else
    {
        if (!changeCamera) {
            cameraSand.ThirdView(moveCarSand, LRSand);
        }

        else {
            cameraSand.FirstView(moveCarSand, LRSand);
        }

    }
    //*******************************************************************************************//
    // EYE (ex, ey, ez): defines the location of the camera.									 //
    // AT (ax, ay, az):	 denotes the direction where the camera is aiming at.					 //
    // UP (ux, uy, uz):  denotes the upward orientation of the camera.							 //
    //*******************************************************************************************//

    InitLightSource();

    InitMaterial();

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_NORMALIZE);
}

float distance(float movingObjectx, float movingObjectZ, float objectx, float objectz)
{
    return sqrt(pow(movingObjectx - objectx, 2) +
        pow(objectz - movingObjectZ, 2) * 1.0);
}
void firstSceneCollision() {

    for (int i = 0; i < 6; i++) {
        


        float dist = distance(LR, moveCar, barriers[i], barrierPos[i]);
        

        if (dist <= 0.7) {
            barriers[i] = 'N';
            countColision += 1;
            PlaySound(TEXT("sounds/collision.wav"), NULL, SND_ASYNC | SND_FILENAME);
            score += 10;
        }

    }
}
void secSceneCollision() {
    for (int i = 0; i < 9; i++) {



        float dist = distance(LRSand, moveCarSand, sandBarriers[i], sandBarriersPos[i]);


        if (dist <= 0.7) {
            sandBarriers[i] = 'N';
            countColision += 1;
            PlaySound(TEXT("sounds/collision.wav"), NULL, SND_ASYNC | SND_FILENAME);
            score += 10;
        }

    }
}

void boosterisTakenSand() {

    float dist = distance(LRSand, moveCarSand, 0, -55);

    if (dist <= 0.7) {

        boosterTaken = true;
    }

}

void boosterisTaken() {
    float dist = distance(LR, moveCar, 0, -30);

    if (dist <= 0.7) {
        boosterTaken = true;
    }
}
void Keyboard(unsigned char key, int x, int y) {

    boosterisTaken();
    boosterisTakenSand();

    if (boosterTaken &&  playSound) {
        playSound = false;
        PlaySound(TEXT("sounds/sparkle.wav"), NULL, SND_ASYNC | SND_FILENAME);
    }

    if (boosterTaken && playSound) {
        playSound = false;
        PlaySound(TEXT("sounds/sparkle.wav"), NULL, SND_ASYNC | SND_FILENAME);
    }
    //float d = 0.01;
    float d = 0.2f;

    switch (key) {
    case 'w':
        camera.moveY(d);
        break;
    case 's':
        camera.moveY(-d);
        break;
    case 'a':
        camera.moveX(d);
        break;
    case 'd':
        camera.moveX(-d);
        break;
    case 'q':
        camera.moveZ(d);
        break;
    case 'e':
        camera.moveZ(-d);
        break;

    case 'i':
        if (secScene) {
            moveCar -= 0.1;

        }
        else {
            moveCarSand -= 0.25;
        }
        break;
    case 'l':
        if (secScene) {
            if (LR < 1.2) {
                LR += 0.15;
            }
        }
        else
        {
            if (LRSand < 1.40) {
                LRSand += 0.15;
            }
        }
        break;
    case 'j':
        if (secScene) {
            if (LR > -1.15) {
                LR -= 0.15;
           
            }
        }
        else {
            if (LRSand > -1.35) {
                LRSand -= 0.15;
            }
        }
        break;
        
    case '1':
        if (!changeCamera) {
            changeCamera = true;
        }
        else
        {
            changeCamera = false;
        }
        break;
    case GLUT_KEY_ESCAPE:
        exit(EXIT_SUCCESS);
    }

    if (secScene) {
        firstSceneCollision();
    }
    else {
        secSceneCollision();
    }

    glutPostRedisplay();
}



void Timer(int value)
{
    
    if (!gameOver) {
        score += 10;
    }

    glutPostRedisplay();

    // recall the Timer function after 20 seconds (20,000 milliseconds)
    glutTimerFunc(1000, Timer, 0);
}

void Special(int key, int x, int y) {
    float a = 1.0;

    switch (key) {

    case GLUT_KEY_UP:
        camera.rotateX(a);
        break;
    case GLUT_KEY_DOWN:
        camera.rotateX(-a);
        break;
    case GLUT_KEY_LEFT:
        camera.rotateY(a);
        break;
    case GLUT_KEY_RIGHT:
        camera.rotateY(-a);
        break;
    }

    glutPostRedisplay();
}

void myMouse(int button, int state, int x, int y) {

    if (button == GLUT_LEFT_BUTTON) {

        if (boosterTaken) {
            if (secScene) {
                moveCar -= 5;
                boosterTaken = false;
                playSound = true;
            }
            else
            {
                moveCarSand -= 5;
                boosterTaken = false;
            }
        }
    }
}

void Anim()
{
    if (xDirection) {
        worksManRot += 0.2;
    }
    else if (!xDirection){
        worksManRot -= 0.2;
    }

    if (worksManRot >= 30) {
        xDirection = false;
    }
    else if (worksManRot <= -30) {
        xDirection = true;
    }

    if (xDirection) {
        lightX += 0.6;
    }
    else if (!xDirection) {
        lightX -= 0.6;
    }

    if (lightX >= 30) {
        xDirection = false;
    }
    else if (lightX <= -30) {
        xDirection = true;
    }

    boosterAnimat += 0.7;

    if (!secScene) {
        secCarSpeed -= 0.07;
        darkness -= 0.0005;
    }


    glutPostRedisplay();
}




void drawGround()
{
    glPushMatrix();
    glColor3ub(124, 252, 0);
    glScalef(1, 0.01, 1);
    glutSolidCube(2);
    glPopMatrix();
}


//land scape and drawing the road and the ground

void wolfAnimalBarrier() {
    glPushMatrix();
    wolf.Draw();
    glPopMatrix();
}
void workerBarrier() {
    glPushMatrix();
    glScalef(0.4, 0.4, 0.4);
    glRotatef(worksManRot, 0, 0, 1);
    worksMan.Draw();
    glPopMatrix();
}
void drawBooster() {



    glPushMatrix();
    glTranslatef(0, 1, -30);
    glScalef(0.1, 0.1, 0.1);
    glRotatef(-90, 0, 0, 1);
    glRotatef(boosterAnimat, 1, 0, 0);
    booster.Draw();
    glPopMatrix();

}


//--------------- sec scene ------

void sandGround() {
    glDisable(GL_LIGHTING);	// Disable lighting 

    glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

    glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

    glBindTexture(GL_TEXTURE_2D, tex_sand.texture[0]);	// Bind the ground texture

    glPushMatrix();
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);	// Set quad normal direction.
    glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
    glVertex3f(-2, 0, -2);
    glTexCoord2f(1, 0);
    glVertex3f(2, 0, -2);
    glTexCoord2f(1, 1);
    glVertex3f(2, 0, 2);
    glTexCoord2f(0, 1);
    glVertex3f(-2, 0, 2);
    glEnd();
    glPopMatrix();

    glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

    glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}
void roadSand() {
    glPushMatrix();
    glTranslatef(0, 0, -5);

    glPushMatrix();
    glTranslatef(0.0f, 0.02f, 0.0f);
    glScalef(0.3f, 0.07f, 1.0f);
    road.Draw();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.02f, -20.0f);
    glScalef(0.3f, 0.07f, 1.0f);
    road.Draw();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.02f, -40.0f);
    glScalef(0.3f, 0.07f, 1.0f);
    road.Draw();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.02f, -60.0f);
    glScalef(0.3f, 0.07f, 1.0f);
    road.Draw();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.02f, -80.0f);
    glScalef(0.3f, 0.07f, 1.0f);
    road.Draw();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.02f, -90.0f);
    glScalef(0.3f, 0.07f, 1.0f);
    road.Draw();
    glPopMatrix();

    glPopMatrix();
}
void sandScene() {

    int drawGroundouterloop = 5;


    for (int i = 0; i < 27; i++) {

        int drawGroundinnerloop = -30;

        for (int j = 0; j < 20; j++) {


            glPushMatrix();
            glTranslatef(drawGroundinnerloop, 0, drawGroundouterloop);
            sandGround();
            glPopMatrix();

            drawGroundinnerloop += 4;
        }

        drawGroundouterloop -= 4;

    }
}
void drawDesertRoadCourse() {
    roadSand();
    sandScene();
}
void drawNakhla() {

    glPushMatrix();
    glTranslatef(-17, 25, -40);
    glScalef(0.004, 0.004, 0.004);
    glRotatef(-90, 0, 1, 0);
    nakhla.Draw();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(-17, 25, -70);
    glScalef(0.004, 0.004, 0.004);
    glRotatef(-90, 0, 1, 0);
    nakhla.Draw();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(17, 25, -40);
    glScalef(0.004, 0.004, 0.004);
    nakhla.Draw();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(17, 25, -70);
    glScalef(0.004, 0.004, 0.004);
    nakhla.Draw();
    glPopMatrix();
}
void drawSmallHouseSand() {
    glPushMatrix();
    glTranslatef(-10, 0, -20);
    glRotatef(90, 1, 0, 0);
    glRotatef(-90, 0, 0, 1);
    smallHouse.Draw();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(15, 0, -30);
    glRotatef(90, 1, 0, 0);
    smallHouse.Draw();
    glPopMatrix();
}
void carDrawSand() {
    glPushMatrix();
    glTranslatef(LRSand, 0, moveCarSand);
    glScalef(0.5, 0.5, 0.5);
    glRotatef(180, 0, 1, 0);
    car.Draw();
    glPopMatrix();
}
void pineTreeSand() {
    glPushMatrix();
    glTranslatef(8, -0.1, -10);
    TreeC.Draw();
    glPopMatrix();
}
void drawAnimal() {
    glPushMatrix();
    glScalef(0.02, 0.02, 0.02);
    sandAnimal.Draw();
    glPopMatrix();
}
void stoneDraw() {
    glPushMatrix();
    stone.Draw();
    glPopMatrix();
}
void drawSecCar() {
    glPushMatrix();
    glTranslatef(1.5, 0.3, secCarSpeed);
    glScalef(0.5, 0.5, 0.5);
    glRotatef(180, 0, 1, 0);
    sec_car.Draw();
    glPopMatrix();
}
void DrawsandBarriers() {

    glPushMatrix();
    glTranslatef(sandBarriers[0], 0, -10);
    drawAnimal();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(sandBarriers[1], 0.4, -20);
    stoneDraw();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0.6, 0);
    glTranslatef(sandBarriers[2], 0, -30);
    workerBarrier();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(sandBarriers[3], 0, -40);
    wolfAnimalBarrier();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(sandBarriers[4], 0.4, -50);
    drawAnimal();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(sandBarriers[5], 0, -60);
    stoneDraw();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0.6, 0);
    glTranslatef(sandBarriers[6], 0, -70);
    workerBarrier();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(sandBarriers[7], 0, -80);
    wolfAnimalBarrier();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(sandBarriers[8], 0, -90);
    drawAnimal();
    glPopMatrix();

}
void drawBoosterSand() {
    glPushMatrix();
    glTranslatef(0, 1, -55);
    glScalef(0.1, 0.1, 0.1);
    glRotatef(-90, 0, 0, 1);
    glRotatef(boosterAnimat, 1, 0, 0);
    booster.Draw();
    glPopMatrix();
}



//---------
void landscape() {
    glDisable(GL_LIGHTING);	// Disable lighting 

    glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

    glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

    glBindTexture(GL_TEXTURE_2D, tex_landscape.texture[0]);	// Bind the ground texture

    glPushMatrix();
    glTranslatef(-60, 30, -120);
    glScalef(15, 5, 1);
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);	// Set quad normal direction.
    glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
    glVertex3f(-10, -10, 0);
    glTexCoord2f(1, 0);
    glVertex3f(10, -10, -0);
    glTexCoord2f(1, 1);
    glVertex3f(10, 10, 0);
    glTexCoord2f(0, 1);
    glVertex3f(-10, 10, 0);
    glEnd();
    glPopMatrix();

    glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

    glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}
void RenderGround()
{
    glDisable(GL_LIGHTING);	// Disable lighting 

    glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

    glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

    glBindTexture(GL_TEXTURE_2D, tex_ground.texture[0]);	// Bind the ground texture

    glPushMatrix();
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);	// Set quad normal direction.
    glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
    glVertex3f(-2, 0, -2);
    glTexCoord2f(1, 0);
    glVertex3f(2, 0, -2);
    glTexCoord2f(1, 1);
    glVertex3f(2, 0, 2);
    glTexCoord2f(0, 1);
    glVertex3f(-2, 0, 2);
    glEnd();
    glPopMatrix();

    glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

    glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}




void CarDraw() {
    glPushMatrix();
    glTranslatef(LR, 0, moveCar);
    glScalef(0.5, 0.5, 0.5);
    glRotatef(180, 0, 1, 0);
    car.Draw();
    glPopMatrix();
}
void roadDraw() {

    glPushMatrix();
    glTranslatef(0, 0, -5);

    glPushMatrix();
    glTranslatef(0.0f, 0.02f, 0.0f);
    glScalef(0.25f, 0.07f, 1.0f);
    road.Draw();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.02f, -20.0f);
    glScalef(0.25f, 0.07f, 1.0f);
    road.Draw();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.02f, -40.0f);
    glScalef(0.25f, 0.07f, 1.0f);
    road.Draw();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.02f, -60.0f);
    glScalef(0.25f, 0.07f, 1.0f);
    road.Draw();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.02f, -80.0f);
    glScalef(0.25f, 0.07f, 1.0f);
    road.Draw();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.02f, -90.0f);
    glScalef(0.25f, 0.07f, 1.0f);
    road.Draw();
    glPopMatrix();

    glPopMatrix();
}
void drawGround2() {


   
    int drawGroundouterloop = 5;


    for (int i = 0; i < 20; i++) {

        int drawGroundinnerloop = -30;

        for (int j = 0; j < 20; j++) {

            
            glPushMatrix();
            glTranslatef(drawGroundinnerloop, 0, drawGroundouterloop);
            RenderGround();
            glPopMatrix();

            drawGroundinnerloop += 4;
        }

        drawGroundouterloop -= 4;

    }
}
void DrawRoadCourse() {
   
      roadDraw();
      drawGround2();
    
}



void drawSmallHouse() {
    glPushMatrix();
    glTranslatef(-6, 0, -30);
    glRotatef(90, 1, 0, 0);
    glRotatef(-90, 0, 0, 1);
    smallHouse.Draw();
    glPopMatrix();
}

void skyBox() {
    //sky box
    glPushMatrix();

    GLUquadricObj* qobj;
    qobj = gluNewQuadric();
    glTranslated(50, 0, 0);
    glRotated(90, 1, 0, 1);
    glBindTexture(GL_TEXTURE_2D, tex);
    gluQuadricTexture(qobj, true);
    gluQuadricNormals(qobj, GL_SMOOTH);
    gluSphere(qobj, 200, 200, 200);
    gluDeleteQuadric(qobj);



    glPopMatrix();
}
void coneBarrier() {
    glPushMatrix();
    glScalef(0.2, 0.2, 0.2);
    cone.Draw();
    glPopMatrix();
}

void farmHouseBarrier() {
    glPushMatrix();
    glTranslatef(-6, 0, -11);
    glScalef(0.5, 0.5, 0.8);
    glRotatef(180, 0, 1, 0);
    FarmHouse.Draw();
    glPopMatrix();
}


void DrawHouses() {
    farmHouseBarrier();
    drawSmallHouse();

    glPushMatrix();
    glTranslatef(0, 0, -45);
    farmHouseBarrier();
    glPopMatrix();
}

void pineTree() {
    glPushMatrix();
    glTranslatef(4, -0.1, -6);
    TreeC.Draw();
    glPopMatrix();
}
void tree() {
    glPushMatrix();
    glTranslatef(4, 0.2, -2);
    Tree.Draw();
    glPopMatrix();
}

void DrawTrees() {
    float distance = 0;
    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        glTranslatef(0,0,distance);

        tree();
        pineTree();

        glPopMatrix();

        distance -= 20;
    }
}

void Barriers() {

    glPushMatrix();
    glTranslatef(barriers[0], 0, -10);
    coneBarrier();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(barriers[1], 0.4, -17);
    workerBarrier();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(barriers[2], 0, -24);
    wolfAnimalBarrier();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(barriers[3], 0, -31);
    coneBarrier();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(barriers[4], 0.4, -38);
    workerBarrier();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(barriers[5], 0, -45);
    wolfAnimalBarrier();
    glPopMatrix();

}

void FinishLine(){

    glPushMatrix();
    glTranslatef(0, 2, -70);
    dragon.Draw();
    glPopMatrix();
}

void FinishLineSand() {

    glPushMatrix();
    glTranslatef(0, 2, -100);
    dragon.Draw();
    glPopMatrix();
}

void displayText(float x, float y, int r, int g, int b, const char* string) {
    int j = strlen(string);

    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    for (int i = 0; i < j; i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
    }
}

void print(int x, int y,float r, float g, float b, char* string)
{
    int len, i;

    glColor3f(r, g, b);
    //set the position of the text in the window using the x and y coordinates
    glRasterPos2f(x, y);

    //get the length of the string to display
    len = (int)strlen(string);
    
    //loop to display character by character
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
    }
}


void print2(double x, double y, double z, char* string)
{
    int len, i;

    glRasterPos3f(x, y, z);

    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
    }
}

void drawRecord() {

    glDisable(GL_LIGHTING);
    glColor3f(0, 1, 0);
    glPushMatrix();

    char* p0s[20];
    sprintf((char*)p0s, "SCORE %d ", score);
    print2(-0.5+LR, 2, moveCar, (char*)p0s);
    
    glPopMatrix();

    glEnable(GL_LIGHTING);
    glColor3f(1, 1, 1);
}
void drawRecordSand() {

    glDisable(GL_LIGHTING);
    glColor3f(0,0,1);
    glPushMatrix();

    char* p0s[20];
    sprintf((char*)p0s, "SCORE %d ", score);
    print2(-0.5+LRSand, 2, moveCarSand, (char*)p0s);

    glPopMatrix();

    glEnable(GL_LIGHTING);
    glColor3f(1, 1, 1);
}

//
//camera

void Display() {
   

    if (!gameOver) {
    
    myInit();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    

        
   
        if (moveCar <= -70 && secScene) {
            secScene = false;
            countColision = 0;
            moveCar = 0;
        }
        else
        {
            if (moveCarSand <= -95 || secCarSpeed <= -95) {
                if(secCarSpeed <= -95){
                    gameOver = true;
                    secCarWon = true;
                }
                else {
                    gameOver = true;
                }
                
            }
        }


        if (secScene) {

            
            GLfloat lightIntensity[] = { 0.4, 0.4, 0.4, 1.0f };

            GLfloat lightPosition[] = { lightX, 2.0, 0.0f, 0.0f };
            glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
            glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);

            
            if (countColision == 3) {
                gameOver = true;
            }

            
            drawRecord();

            DrawRoadCourse();

            landscape();

            CarDraw();

            if (!boosterTaken) {
                drawBooster();
            }


            DrawTrees();

            DrawHouses();

            Barriers();

            FinishLine();
            glFlush();

        }
        else {

            GLfloat lightIntensity[] = { darkness, darkness, darkness, 1.0f };

            GLfloat lightPosition[] = { lightX, 2.0, 0.0f, 0.0f };
            glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
            glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);

            if (countColision == 2) {
                gameOver = true;
            }

            drawRecordSand();
            drawDesertRoadCourse();
            drawNakhla();
            drawSmallHouseSand();
            carDrawSand();
            DrawsandBarriers();
            if (!boosterTaken) {
                drawBoosterSand();
            }
            FinishLineSand();
            pineTreeSand();
            drawSecCar();



        }
        //finish line

        //skybox
        skyBox();
        
    }
    else {
        myInit();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GLfloat lightIntensity[] = { 0.7, 0.7, 0.7, 1.0f };

        GLfloat lightPosition[] = { 0, 2.0, 0.0f, 0.0f };
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

        glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);
        
        if (secScene) {

            char* p0s[20];
            sprintf((char*)p0s, "GAME OVER, Collided Three times, Your score is %d", score);
            print(-14, 2, 1, 0, 0, (char*)p0s);
        }
        else
        {
            char* p0s[20];
            
        

            if (secCarWon) {
                sprintf((char*)p0s, "GAME OVER, THE SECOND CAR BEAT YOU, Your score is %d", score);
                print(-14, 1, 1, 0, 0, (char*)p0s);
            }
            else if(countColision == 2) {
                sprintf((char*)p0s, "GAME OVER, Collided Two times, Your score is %d", score);
                print(-14, 1, 1, 0, 0, (char*)p0s);
            }
            else {
                sprintf((char*)p0s, "YOU WON , Your score is %d", score);
                print(1, 1, 0, 1, 0, (char*)p0s);
            }
        }
        glutSolidCube(0.2);
    }
    glFlush();
    
   
   
    

    
}

void LoadAssets()
{
    // Loading Model files first scene
    car.Load("models/chev/chevro.3DS");
    road.Load("models/Road/HwRoad.3DS");
    FarmHouse.Load("models/FarmHouse/farmHouse.3DS");
    worksMan.Load("models/worksman/workerman3DS.3DS");
    wolf.Load("models/wolf/Wolf_3ds.3DS");
    cone.Load("models/streetBarrier/cone.3DS");
    booster.Load("models/missile/missile.3DS");
    Tree.Load("models/tree/normalTree/Tree1.3DS");
    TreeC.Load("models/tree/Pine_Tree.3DS");
    smallHouse.Load("models/house/house.3DS");
    dragon.Load("models/dragonPortal/dragon.3DS");
    
    //sec scene models
    sandAnimal.Load("models/frog/Frog.3DS");
    nakhla.Load("models/nakhla/tree.3DS");
    stone.Load("models/stone/rock.3DS");
    sec_car.Load("models/secCar/low911.3DS");
    // Loading texture files
    tex_ground.Load("Textures/grass.bmp");
    tex_landscape.Load("Textures/landscape2.bmp");
    

    tex_sand.Load("Textures/sand.bmp");
    loadBMP(&tex, "Textures/blu-sky-3.bmp", true);
}

int main(int argc, char** argv) {

    
    srand(static_cast <unsigned> (time(0)));
    InitBarriers();
    InitsandBarrier();

    glutInit(&argc, argv);

    glutInitWindowSize(640, 480);
    glutInitWindowPosition(50, 50);

    glutCreateWindow("Lab 5");
    glutDisplayFunc(Display);
    glutKeyboardFunc(Keyboard);
    glutIdleFunc(Anim);
    glutTimerFunc(0, Timer, 0);
    glutSpecialFunc(Special);
    glutMouseFunc(myMouse);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

    
    LoadAssets();
    //std::cout << _objfile[0].position.x;*/
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glShadeModel(GL_SMOOTH);

    glutMainLoop();
}