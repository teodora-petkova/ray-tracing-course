#include "gtest/gtest.h"
#include "../include/crttriangle.h"

TEST(CRTTriangleTests, TestTriangleNormalAndArea1)
{
    CRTTriangle t = CRTTriangle(
        CRTVector(-1.75, -1.75, -3),
        CRTVector(1.75, -1.75, -3),
        CRTVector(0, 1.75, -3), Color{0, 0, 0});

    ASSERT_EQ(t.getNormal(), CRTVector(0, 0, 1));
    ASSERT_EQ(t.getArea(), 6.125);
}

TEST(CRTTriangleTests, TestTriangleNormalAndArea2)
{
    CRTTriangle t = CRTTriangle(
        CRTVector(0, 0, -1),
        CRTVector(1, 0, 1),
        CRTVector(-1, 0, 1), Color{0, 0, 0});

    ASSERT_EQ(t.getNormal(), CRTVector(0, -1, 0));  
    ASSERT_EQ(t.getArea(), 2); 
}

TEST(CRTTriangleTests, TestTriangleNormalAndArea3)
{
    CRTTriangle t = CRTTriangle(
        CRTVector(0.56, 1.11, 1.23),
        CRTVector(0.44, -2.368, -0.54),
        CRTVector(-1.56, 0.15, -1.92), Color{0, 0, 0});

    CRTVector v = t.getNormal();
    ASSERT_EQ(t.getNormal(), CRTVector(0.756420016, 0.275748223, -0.593120277));
    ASSERT_FLOAT_EQ(t.getArea(), 6.11862469);
}