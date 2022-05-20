#pragma once

#include <iostream>

#include "crtcolor.h"
#include "crtvector.h"

class CRTTriangle
{
public:
    CRTTriangle()
    : a(CRTVector()), b(CRTVector()), c(CRTVector()),
      normalA(CRTVector()), normalB(CRTVector()), normalC(CRTVector()),
      color(CRTColor())
    {}

    void init()
    {
        // counter-clockwise order of the vertices A->B->C
        this->AB = b - a; // e0
        this->BC = c - b; // e1
        this->CA = a - c; // e2

        this->normal = AB.cross(BC).normalize();
    }

    CRTTriangle(CRTVector a, CRTVector b, CRTVector c, 
        CRTVector normalA, CRTVector normalB, CRTVector normalC, 
        CRTColor color)
        : a(a), b(b), c(c), 
          normalA(normalA), normalB(normalB), normalC(normalC), 
          color(color)
    {
        init();
    }

    CRTTriangle(const CRTTriangle& other)
    {
        this->a = other.a;
        this->b = other.b;
        this->c = other.c;

        this->AB = other.AB;
        this->BC = other.BC;
        this->CA = other.CA;

        this->normal = other.normal;

        this->normalA = other.normalA;
        this->normalB = other.normalB;
        this->normalC = other.normalC;

        this->color = other.color;
    }

   CRTTriangle& operator=(const CRTTriangle& other)
    {
        if (this == &other)
            return *this;

        this->a = other.a;
        this->b = other.b;
        this->c = other.c;
        this->color = other.color;

        init();

        return *this;
    }

    CRTVector A() { return this->a; }
    CRTVector B() { return this->b; }
    CRTVector C() { return this->c; }
    CRTColor getColor() const { return this->color; }

    CRTVector getNormal() const { return this->normal; }
    float getArea() const { return (this->AB.cross(this->BC)).length()/2; }

    bool IsPointInside(CRTVector p) const 
    {
        return this->normal.dot(AB.cross((p - a))) >= 0 &&
               this->normal.dot(BC.cross((p - b))) >= 0 &&
               this->normal.dot(CA.cross((p - c))) >= 0;
    }

    CRTVector getNormalAt(CRTVector p) const 
    {
        CRTVector v = getBarycentricCoordinates(p);
        CRTVector normal = normalA * v.X() + normalB * v.Y() + normalC * v.Z();
        return normal.normalize();
    }

    CRTVector getBarycentricCoordinates(CRTVector p) const
    {
        float triangleArea = getArea() * 2;
        float u = (p - a).cross(c - a).length() / triangleArea;
        float v = (b - a).cross(p - a).length() / triangleArea;
        
        return CRTVector(1 - u - v, u, v);
    }

private:
    CRTVector a;
    CRTVector b;
    CRTVector c;

    CRTVector AB;
    CRTVector BC;
    CRTVector CA;

    CRTVector normal;

    CRTVector normalA;
    CRTVector normalB;
    CRTVector normalC;

    CRTColor color;
};