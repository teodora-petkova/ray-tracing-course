#include "../include/crtcanvas.h"

CRTCanvas::CRTCanvas(int width, int height)
	:width(width),
	height(height),
	pixels(nullptr)
{
	this->pixels = new CRTColor[this->width * this->height];
}

CRTCanvas::~CRTCanvas()
{
	if (this->pixels != nullptr)
	{
		delete[] this->pixels;
		this->pixels = nullptr;
	}
}

CRTColor CRTCanvas::getPixel(int x, int y) const
{
	return this->pixels[x + y * this->width];
}

void CRTCanvas::setPixel(int x, int y, CRTColor const& color)
{
	if (x < 0 || x >= this->width ||
		y < 0 || y >= this->height)
		return;
	this->pixels[x + y * this->width] = color;
}