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