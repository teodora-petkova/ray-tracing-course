#pragma once

#include "crtvector.h"
#include "crtray.h"
#include "crtmatrix.h"

class CRTCamera {
public:
    CRTCamera()
    :position(CRTVector(0, 0, 0)),
    rotationMatrix(CRTMatrix::Identity())
    {}

    CRTVector getPosition() const { return this->position; }
    CRTMatrix getRotationMatrix() const { return this->rotationMatrix; }

    CRTRay getCameraRay(CRTVector v);

    void translate(const CRTVector &moveDir);

    // truck - moving the camera left and right
    void truck(int x);
    // pedestal - moving the camera up and down
    void pedestal(int y);
    // dolly - moving the camera forwards and backwards / 'in and out'
    void dolly(int z);

    void rotate(const CRTMatrix &matrix);
    
    // pan - rotate around y
    void pan(const float degrees);
    // tilt - rotate around x
    void tilt(const float degrees);
    // roll - rotate around z
    void roll(const float degrees);

private:
        CRTMatrix rotationMatrix;
        CRTVector position;
};