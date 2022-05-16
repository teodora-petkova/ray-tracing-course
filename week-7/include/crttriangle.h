#pragma once

#include "crtcolor.h"
#include "crtvector.h"

class CRTTriangle
{
public:
    CRTTriangle(CRTVector a, CRTVector b, CRTVector c, CRTColor color)
    : a(a), b(b), c(c) 
    {
        // counter-clockwise order of the vertices A->B->C
        this->AB = b - a; // e0
        this->BC = c - b; // e1
        this->CA = a - c; // e2
        this->normal = AB.cross(BC).normalize();

        this->color = color;
    }

    CRTVector A() { return this->a; }
    CRTVector B() { return this->b; }
    CRTVector C() { return this->c; }
    CRTColor getColor() { return this->color; }

    CRTVector getNormal() const { return this->normal; }
    float getArea() const { return (this->AB.cross(this->BC)).length()/2; }

    bool IsPointInside(CRTVector p) const 
    {
        return this->normal.dot(AB.cross((p - a))) > 0 &&
               this->normal.dot(BC.cross((p - b))) > 0 &&
               this->normal.dot(CA.cross((p - c))) > 0;
    }

private:
    CRTVector a;
    CRTVector b;
    CRTVector c;

    CRTColor color;

    CRTVector AB;
    CRTVector BC;
    CRTVector CA;

    CRTVector normal;
};