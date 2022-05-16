#pragma once

#include <iostream>
#include <fstream>
#include "math.h"

#include "crtpoint.h"
#include "crtcamera.h"
#include "crtscene.h"
#include "crtintersectioninfo.h"

namespace CRTRenderer
{
    void RenderImage(std::string filename, const CRTScene& scene);
};