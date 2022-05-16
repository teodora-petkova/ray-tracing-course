#pragma once

#include <math.h>
#include <ostream>

class CRTVector
{
public:
    CRTVector(float x, float y, float z)
        : x{x}, y{y}, z{z} {}

    CRTVector()
        : x{0}, y{0}, z{0} {}

    CRTVector(const CRTVector& other)
    {
        this->x = other.x;
        this->y = other.y;
        this->z = other.z;
    }

    CRTVector& operator=(const CRTVector& other)
    {
        if (this == &other)
            return *this;

        this->x = other.x;
        this->y = other.y;
        this->z = other.z;

        return *this;
    }

    float X() { return x; }
    float Y() { return y; }
    float Z() { return z; }

    float length() const;

    CRTVector normalize();

    CRTVector operator+(const CRTVector &other) const;
    CRTVector operator-(const CRTVector &other) const;
    CRTVector operator*(const float f) const;
    CRTVector operator/(const float f) const;

    CRTVector cross(const CRTVector &other) const;

    float dot(const CRTVector &other) const;

    float area(const CRTVector &other) const;

    bool operator==(const CRTVector &other) const;

    friend std::ostream &operator<<(std::ostream &os, const CRTVector &v);
protected:
    float x;
    float y;
    float z;
};