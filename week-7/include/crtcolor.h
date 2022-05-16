#pragma once

#include "crtvector.h"

class CRTColor : public CRTVector
{
public:
	CRTColor(): CRTVector() {}
	CRTColor(float r, float g, float b): CRTVector(r, g, b) {}
	CRTColor(const CRTColor &c): CRTVector(c) {}
	CRTColor(const CRTVector &v): CRTVector(v) {}

	float R() const { return x; }
	float G() const { return y; }
	float B() const { return z; }
};