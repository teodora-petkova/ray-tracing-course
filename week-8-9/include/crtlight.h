#pragma once

#include "crtvector.h"

class CRTLight
{
public:
    CRTLight(float intensity, const CRTVector& position)
    {
        this->intensity = intensity;
        this->position = position;
    }

    float getIntensity() const { return intensity; }
    CRTVector getPosition() const { return position; }

private:
    float intensity;
    CRTVector position;
};