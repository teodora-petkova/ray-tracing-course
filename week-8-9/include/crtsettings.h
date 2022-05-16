#pragma once

#include "crtcolor.h"

struct CRTImageSettings {
    int height;
    int width;
};

struct CRTSettings {
    CRTImageSettings imageSettings;
    CRTColor backgroundColor;
    CRTColor globalAlbedo;
};