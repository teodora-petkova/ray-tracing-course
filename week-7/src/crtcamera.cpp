#include "..\include\crtcamera.h"

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

void CRTCamera::dolly(float z)
{
    translate(CRTVector(0, 0, z));
}

void CRTCamera::truck(float x) 
{
    translate(CRTVector(x, 0, 0));
}

void CRTCamera::pedestal(float y) 
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

void CRTCamera::fillImageSettings(int imageWidth, int imageHeight)
{
    //TODO:????
}

void CRTCamera::setRotationMatrix(const CRTMatrix &matrix)
{
    this->rotationMatrix = matrix;
}

void CRTCamera::setPosition(const CRTVector &position)
{
    this->position = position;
}