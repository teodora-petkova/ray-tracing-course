#pragma once

#include "crtcolor.h"

class CRTCanvas
{
public:
	CRTCanvas(int width, int height);
    ~CRTCanvas();

	int getWidth() const { return width; }
	int getHeight() const { return height; }

	CRTColor getPixel(int x, int y) const;
	void setPixel(int x, int y, const CRTColor& color);

private:
	int width;
	int height;

	CRTColor* pixels;
};