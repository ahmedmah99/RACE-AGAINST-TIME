


#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<sstream>
#include "Camera.h"

class Vector3f {
public:
    float x, y, z;

    Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
        x = _x;
        y = _y;
        z = _z;
    }

    Vector3f operator+(Vector3f& v) {
        return Vector3f(x + v.x, y + v.y, z + v.z);
    }

    Vector3f operator-(Vector3f& v) {
        return Vector3f(x - v.x, y - v.y, z - v.z);
    }

    Vector3f operator*(float n) {
        return Vector3f(x * n, y * n, z * n);
    }

    Vector3f operator/(float n) {
        return Vector3f(x / n, y / n, z / n);
    }

    Vector3f unit() {
        return *this / sqrt(x * x + y * y + z * z);
    }

    Vector3f cross(Vector3f v) {
        return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }
};

Vector3f eye, center, up, temp;

Camera::Camera(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ) {
    eye = Vector3f(eyeX, eyeY, eyeZ);
    center = Vector3f(centerX, centerY, centerZ);
    up = Vector3f(upX, upY, upZ);
};

void Camera::moveX(float d) {
    Vector3f right = up.cross(center - eye).unit();
    temp = right * d;
    eye = eye + temp;
    center = center + temp;

}

void Camera::moveY(float d) {

    temp = up.unit() * d;
    eye = eye + temp;
    center = center + temp;
}

void Camera::moveZ(float d) {
    Vector3f view = (center - eye).unit();

    temp = view * d;
    eye = eye + temp;
    center = center + temp;
}

void Camera::rotateX(float a) {
    Vector3f view = (center - eye).unit();
    Vector3f right = up.cross(view).unit();

    temp = up * sin(DEG2RAD(a));
    view = view * cos(DEG2RAD(a)) + temp;
    up = view.cross(right);
    center = eye + view;
}

void Camera::rotateY(float a) {
    Vector3f view = (center - eye).unit();
    Vector3f right = up.cross(view).unit();

    temp = right * sin(DEG2RAD(a));
    view = view * cos(DEG2RAD(a)) + temp;
    right = view.cross(up);
    center = eye + view;
}

void Camera::ThirdView(float d,float LR) {
    
    float LRtemp = LR;
    gluLookAt(eye.x + LR, eye.y, eye.z+d, center.x + LR, center.y, center.z+d, up.x, up.y, up.z);
    LRtemp = 0;

    //std::cout << "eyex  " << eye.x << "eyey  " << eye.y << "eyez  " << eye.z << "centerx  " << center.x << "cenetery  " << center.y << "centerz  " << center.z << "upx  " << up.x << "upy  " << up.y << "upz  " << up.z;
}

void Camera::FirstView(float d, float LR) {
    float LRtemp = LR;
    gluLookAt(-0.0205681 + LR, 0.643809, -0.0327406 + d, -0.0311014 + LR, 0.529954, -1.02618 + d, -0.00120712, 0.993498, -0.113848);
    LRtemp = 0;
}

void Camera::print() {
    std::cout << "eyex  " << eye.x << "eyey  " << eye.y << "eyez  " << eye.z << "centerx  " << center.x << "cenetery  " << center.y << "centerz  " << center.z << "upx  " << up.x << "upy  " << up.y << "upz  " << up.z;
}

void Camera::gameOver() {
    gluLookAt(1.85502, 1.25653, 28.2126, 1.93109, 1.26501, 27.2156, -0.00841819, 0.999934, 0.00786678);
}

