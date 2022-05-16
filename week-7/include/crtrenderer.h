#pragma once

#include <iostream>
#include <fstream>

#include "crtpoint.h"
#include "crtcamera.h"
#include "crtscene.h"

namespace CRTRenderer
{
    void RenderImage(std::string filename, const CRTScene& scene);
};