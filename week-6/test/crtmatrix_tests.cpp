#include "gtest/gtest.h"
#include "../include/crtmatrix.h"

TEST(CRTMatrixTests, MatrixByMatrixMultiplication)
{
    CRTMatrix m1 = CRTMatrix{
        12, 7, 3,
        4 , 5, 6,
        7 , 8, 9};

    CRTMatrix m2 = CRTMatrix{
        5, 8, 1,
        6, 7, 3,
        5, 9, 1};

    CRTMatrix result = m1 * m2;
    CRTMatrix expected = CRTMatrix{
        117, 172, 36,
         80, 121, 25,
        128, 193, 40};

    ASSERT_EQ(result, expected);
}

TEST(CRTMatrixTests, VectorByMatrixMultiplication)
{
    CRTVector v = CRTVector(3, 4, 2);

    CRTMatrix m = CRTMatrix{
        13, 9, 7,
         8, 7, 4,
         6, 4, 0};

    CRTVector result = v * m;
    CRTVector expected = CRTVector(83, 63, 37);

    ASSERT_EQ(result, expected);
}