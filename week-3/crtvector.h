#include <math.h>

class CRTVector {
public:
    CRTVector(float x, float y, float z)
    :x{x}, y{x}, z{z} { }

    float X() { return x; }
    float Y() { return y; }
    float Z() { return z; }

    float length() {
        return sqrtf(x * x + y * y + z * z);
    }

    CRTVector normalize() {
        float len = length();
        return CRTVector(x / len, y / len, z / len);
    }

private:
	float x;
	float y;
	float z;
};
