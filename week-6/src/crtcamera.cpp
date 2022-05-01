#include "../include/crtcamera.h"

CRTRay CRTCamera::getCameraRay(CRTVector v)
{
    CRTVector origin = this->position;
    CRTVector direction = v * this->rotationMatrix;
    
    CRTRay ray = CRTRay{origin, direction.normalize()};
    return ray;
}

void CRTCamera::translate(const CRTVector& moveDir) 
{
    const CRTVector moveDirInWorldSpace = moveDir * this->rotationMatrix;
    this->position = this->position + moveDirInWorldSpace;
}

void CRTCamera::dolly(int z)
{
    translate(CRTVector(0, 0, z));
}

void CRTCamera::truck(int x) 
{
    translate(CRTVector(x, 0, 0));
}

void CRTCamera::pedestal(int y) 
{
    translate(CRTVector(0, y, 0));
}

void CRTCamera::tilt(const float degrees)
{
    this->rotationMatrix = rotationMatrix * CRTMatrix::RotateX(degrees);
}

void CRTCamera::pan(const float degrees)
{
    this->rotationMatrix = rotationMatrix * CRTMatrix::RotateY(degrees);
}

void CRTCamera::roll(const float degrees)
{
    this->rotationMatrix = rotationMatrix * CRTMatrix::RotateZ(degrees);
}

void CRTCamera::rotate(const CRTMatrix& m)
{
    this->rotationMatrix = rotationMatrix * m;
}