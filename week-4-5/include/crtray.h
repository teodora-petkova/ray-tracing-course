#pragma once

#include <vector>

#include "color.h"
#include "crtpoint.h"
#include "crtvector.h"
#include "crttriangle.h"

struct Ray {
	CRTVector origin;
	CRTVector direction;
};

Ray getRayAtPixel(int x, int y, int width, int height);

Color getColorAtRayIntersection(Ray ray, std::vector<CRTTriangle> triangles,
	Color backgroundColor);