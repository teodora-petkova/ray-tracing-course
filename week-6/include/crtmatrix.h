#pragma once

#include <assert.h>

#define _USE_MATH_DEFINES
#include <math.h>
#define degreesToRadians(degrees) degrees * (M_PI/180.f)

#include "crtvector.h"

class CRTMatrix
{
public:
    CRTMatrix(const std::initializer_list<float> &coefficients)
    {
        assert(coefficients.size() == dimension * dimension);

        int i = 0;
        int j = 0;
        for(auto coeff : coefficients)
        {
            if(j % dimension == 0 && j > 0)
            {
                j = 0;
                i++;
            }
            matrix[i][j] = coeff;

            j++;
        }
    }
    
    bool operator==(const CRTMatrix &other) const;

    static CRTMatrix Identity();

    static CRTMatrix RotateX(float degrees);
    static CRTMatrix RotateY(float degrees);
    static CRTMatrix RotateZ(float degrees);

    friend CRTMatrix operator*(const CRTMatrix &m1, const CRTMatrix &m2);
    friend CRTVector operator*(const CRTVector &v1, const CRTMatrix &v2);

private:
    static const int dimension = 3;
    float matrix[dimension][dimension];
};