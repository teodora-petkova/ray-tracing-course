#include "..\include\crtvector.h"

float CRTVector::length() const
{
    return sqrt(x * x + y * y + z * z);
}

CRTVector CRTVector::normalize()
{
    float len = length();
    return (*this) / len;
}

CRTVector CRTVector::operator+(const CRTVector &other) const 
{
    return CRTVector(this->x + other.x,
                     this->y + other.y,
                     this->z + other.z);
}

CRTVector  CRTVector::operator-(const CRTVector &other) const 
{
    return CRTVector(this->x - other.x,
                     this->y - other.y,
                     this->z - other.z);
}

CRTVector CRTVector::operator*(const CRTVector &other) const 
{
    return CRTVector(this->x * other.x,
                     this->y * other.y,
                     this->z * other.z);
}

void CRTVector::operator+=(const CRTVector &other) 
{
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;
}

CRTVector CRTVector::operator/(const float f) const
{
    return CRTVector(this->x / f,
                     this->y / f,
                     this->z / f);
}

CRTVector CRTVector::operator*(const float f) const
{
    return CRTVector(this->x * f,
                     this->y * f,
                     this->z * f);
}

CRTVector CRTVector::cross(const CRTVector &other) const
{
    return CRTVector(this->y * other.z - this->z * other.y,
                     this->z * other.x - this->x * other.z,
                     this->x * other.y - this->y * other.x);
}

float CRTVector::dot(const CRTVector &other) const
{
    return this->x * other.x + this->y * other.y + this->z * other.z;
}

CRTVector CRTVector::reflect(const CRTVector &normal) const
{
    return (*this) - normal * 2 * this->dot(normal);
}

bool CRTVector::operator==(const CRTVector &other) const
{
    return this->x == other.x 
        && this->y == other.y 
        && this->z == other.z;
}

std::ostream& operator<<(std::ostream& os, const CRTVector& v)
{
    os << '(' << v.x << ', ' << v.y << ', ' << v.z << ')';
    return os;
}