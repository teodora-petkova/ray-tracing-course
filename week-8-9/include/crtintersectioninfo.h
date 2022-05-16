#pragma once

#include "crttriangle.h"

struct CRTIntersectionInfo
{
    bool isTriangleHit;
    float minDistance;
    CRTTriangle hitTriangle;
};