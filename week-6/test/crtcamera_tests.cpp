#include "gtest/gtest.h"
#include "../include/crtray.h"
#include "../include/crtcamera.h"

TEST(CRTCameraTests, InitialSettings)
{
    CRTCamera camera = CRTCamera();
    
    ASSERT_EQ(camera.getPosition(), CRTVector(0, 0, 0));
    ASSERT_EQ(camera.getRotationMatrix(), CRTMatrix::Identity());
}

TEST(CRTCameraTests, TestInitialCameraRay)
{
    CRTCamera camera = CRTCamera();

    CRTRay ray = camera.getCameraRay(CRTVector(0, 0, -1));
    CRTRay expectedRay = CRTRay{CRTVector(0, 0, 0), CRTVector(0, 0, -1)};

    ASSERT_EQ(ray.origin, expectedRay.origin);
    ASSERT_EQ(ray.direction, expectedRay.direction);
}

TEST(CRTCameraTests, TestCameraRayAfterPan30_1)
{
    CRTCamera camera = CRTCamera();
    camera.pan(30);

    CRTRay ray = camera.getCameraRay(CRTVector(0, 0, -1));
    CRTRay expectedRay = CRTRay{CRTVector(0, 0, 0), CRTVector(-0.5, 0, -0.866025388)};
    
    ASSERT_EQ(ray.origin, expectedRay.origin);
    ASSERT_EQ(ray.direction, expectedRay.direction);
}

TEST(CRTCameraTests, TestCameraRayAfterPan30_2)
{
    CRTCamera camera = CRTCamera();
    camera.pan(30);

    CRTRay ray = camera.getCameraRay(CRTVector(3, 0, 2));
    CRTRay expectedRay = CRTRay{CRTVector(0, 0, 0), CRTVector(0.997926772, 0, 0.0643593073)};
    
    ASSERT_EQ(ray.origin, expectedRay.origin);
    ASSERT_EQ(ray.direction, expectedRay.direction);
}