#pragma once
#include <math.h>

#include <glut.h>
#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)


class Camera {
public:

    Camera(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY , float upZ);


    void moveX(float d);
    void moveY(float d);

    void moveZ(float d);

    void rotateX(float a);

    void rotateY(float a);

    void ThirdView(float d, float LR);

    void FirstView(float d, float LR);

    void gameOver();

    void print();
 

};


