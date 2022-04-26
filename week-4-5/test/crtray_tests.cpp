#include "gtest/gtest.h"
#include "../include/crtray.h"

TEST(CRTRayTests, Ray_passes_at_the_center_of_the_scene)
{
    Ray ray = getRayAtPixel(5, 5, 11, 11);
    Ray expectedRay = Ray{CRTVector(0, 0, 0), CRTVector(0, 0, -1)};
    ASSERT_EQ(ray.origin, expectedRay.origin);
    ASSERT_EQ(ray.direction, expectedRay.direction);
}