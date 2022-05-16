#include "..\include\crtmatrix.h"

CRTMatrix operator*(const CRTMatrix &m1, const CRTMatrix &m2)
{   
    CRTVector m11 = CRTVector(m1.matrix[0][0], m1.matrix[0][1], m1.matrix[0][2]);
    CRTVector m12 = CRTVector(m1.matrix[1][0], m1.matrix[1][1], m1.matrix[1][2]);
    CRTVector m13 = CRTVector(m1.matrix[2][0], m1.matrix[2][1], m1.matrix[2][2]);

    CRTVector m21 = CRTVector(m2.matrix[0][0], m2.matrix[1][0], m2.matrix[2][0]);
    CRTVector m22 = CRTVector(m2.matrix[0][1], m2.matrix[1][1], m2.matrix[2][1]);
    CRTVector m23 = CRTVector(m2.matrix[0][2], m2.matrix[1][2], m2.matrix[2][2]);

    return CRTMatrix{
        m11.dot(m21), m11.dot(m22), m11.dot(m23),
        m12.dot(m21), m12.dot(m22), m12.dot(m23),
        m13.dot(m21), m13.dot(m22), m13.dot(m23)};
}

CRTVector operator*(const CRTVector &v, const CRTMatrix &m)
{
    float vx = v.dot(CRTVector(m.matrix[0][0], m.matrix[1][0], m.matrix[2][0]));
    float vy = v.dot(CRTVector(m.matrix[0][1], m.matrix[1][1], m.matrix[2][1]));
    float vz = v.dot(CRTVector(m.matrix[0][2], m.matrix[1][2], m.matrix[2][2]));

    return CRTVector(vx, vy, vz);
}

 bool CRTMatrix::operator==(const CRTMatrix &other) const
 {
     bool result = true;
     for (int i = 0; i < dimension; i++)
     {
         for (int j = 0; j < dimension; j++)
         {
             result &= this->matrix[i][j] == other.matrix[i][j];
         }
     }
     return result;
 }

CRTMatrix CRTMatrix::Identity()
{
    return CRTMatrix{
        1, 0, 0,
        0, 1, 0,
        0, 0, 1};
}

CRTMatrix CRTMatrix::RotateX(float degrees)
{
    const float radiants = degreesToRadians(degrees);

	const CRTMatrix rotateAroundX{
		1.f,  		    0.f,			 0.f,
		0.f,  cos(radiants),  -sin(radiants),
		0.f,  sin(radiants),   cos(radiants)};

    return rotateAroundX;
}

CRTMatrix CRTMatrix::RotateY(float degrees)
{
    const float radiants = degreesToRadians(degrees);

    const CRTMatrix rotateAroundY{
        cosf(radiants),   0.f,   -sinf(radiants),
                   0.f,   1.f,               0.f,
        sinf(radiants),   0.f,   cosf(radiants)};
    return rotateAroundY;
}

CRTMatrix CRTMatrix::RotateZ(float degrees)
{
    float radiants = degreesToRadians(degrees);
	
    const CRTMatrix rotateAroundZ{
		cos(radiants),  -sin(radiants),  0.f,
		sin(radiants),   cos(radiants),  0.f,
		          0.f,	           0.f,  1.f};

    return rotateAroundZ;
}