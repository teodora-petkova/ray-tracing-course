#include "gtest/gtest.h"
#include "../include/crtvector.h"

TEST(CRTVectorTests, TestCrossProduct1)
{
    CRTVector A = CRTVector(3.5, 0, 0);
	CRTVector B = CRTVector(1.75, 3.5, 0);
	CRTVector C = A.cross(B);
	
	ASSERT_EQ(C, CRTVector(0, 0, 12.25));
}

TEST(CRTVectorTests, TestCrossProduct2)
{
	CRTVector A = CRTVector(3, -3, 1);
	CRTVector B = CRTVector(4, 9, 3);
	CRTVector C = A.cross(B);
	ASSERT_EQ(C, CRTVector(-18, -5, 39));
}

TEST(CRTVectorTests, TestParallelogramArea)
{
	CRTVector A = CRTVector(3, -3, 1);
	CRTVector B = CRTVector(4, 9, 3);
	CRTVector C = A.cross(B);
	float area = C.length();
	ASSERT_FLOAT_EQ(area, 43.2434959);
}

TEST(CRTVectorTests, TestCrossProductOfOppositeVectors)
{
	CRTVector A = CRTVector(3, -3, 1);
	CRTVector B = CRTVector(-12, 12, -4);
	CRTVector C = A.cross(B);
	float area = C.length();
	ASSERT_EQ(C, CRTVector(0, 0, 0));
	ASSERT_EQ(area, 0);
}