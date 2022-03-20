#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <string>
#include <vector>
#include "crtvector.h"

struct Point {
	float x;
	float y;
};

struct Color {
	int r;
	int g;
	int b;
};

struct Ray {
	CRTVector origin;
	CRTVector direction;
};

int clamp(float v){
	return std::clamp(int(v), 0, 255);
}

float lerp(float a, float b, float t) {
    return (1 - t) * a + t * b;
}

void generatePPM(std::string filename, Color* pixels,
	int imageWidth, int imageHeight, int maxColorComponent) {
	std::ofstream ppmFileStream(filename, std::ios::out | std::ios::binary | std::ios::trunc);
	ppmFileStream << "P3\n";
	ppmFileStream << imageWidth << " " << imageHeight << "\n";
	ppmFileStream << maxColorComponent << "\n";

	for (int rowIdx = 0; rowIdx < imageHeight; ++rowIdx) {

		for (int colIdx = 0; colIdx < imageWidth; ++colIdx) {

			Color c = pixels[colIdx + rowIdx * imageWidth];
			ppmFileStream << c.r << " " << c.g << " " << c.b << "\t";

		}
		ppmFileStream << "\n";
	}

	ppmFileStream.close();
}

void calculateColorsAtPixelsBasedOnRayDir(Color* pixels, int width, int height, Color (*calculateColorAtPixel)(int, int, int, int)) {
	
	for (int y = 0; y < height; ++y) {

		for (int x = 0; x < width; ++x) {

			Color c = calculateColorAtPixel(x, y, width, height);
		
			pixels[x + y * width] = c;
		}
	}
}

Point getRasterCenterPoint(int x, int y, int width, int height) {
	return Point{x + 0.5f, y + 0.5f};
}

Point getNDCPoint(int x, int y, int width, int height) {
	// convert raster coordinates to NDC (Normalized Device Coordinates) space [0.0, 1.0]
	Point p = getRasterCenterPoint(x, y, width, height);
	return Point { p.x / width, p.y / height};
}

Point getScreenPoint(int x, int y, int width, int height) {
	Point p = getNDCPoint(x, y, width, height);
	// convert NDC coordinates [0.0, 1.0] to Screen space [-1.0, 1.0]
	float ndcX = (2.0 * p.x) - 1.0;
	float ndcY = 1.0 - (2.0 * p.y);
	return Point {ndcX, ndcY};
}

Point getScreenPointWithAspectRatio(int x, int y, int width, int height) {
	Point p = getScreenPoint(x, y, width, height);
	float aspectRatio = float(width) / float(height);
	return Point {p.x * aspectRatio, p.y};
}

Ray getRay(int x, int y, int width, int height) {
	Point p = getScreenPointWithAspectRatio(x, y, width, height);
	// create a ray
	CRTVector origin = CRTVector{0, 0, 0};
	CRTVector direction = CRTVector{p.x, p.y, -1.0};
	Ray ray = Ray{origin, direction};
	return ray;
}

Ray getRayWithNormalizedDir(int x, int y, int width, int height) {
	Ray ray = getRay(x, y, width, height);
	return Ray{ray.origin, ray.direction.normalize()};
}

Color caclulateRasterColor(int x, int y, int width, int height) {
	Point p = getRasterCenterPoint(x, y, width, height);
	return Color{int(p.x) % 256, int(p.y) % 256, 0};
}

Color calculateNDCColor(int x, int y, int width, int height) {
	Point p = getNDCPoint(x, y, width, height);
	return Color{clamp(p.x * 255), clamp(p.y * 255), 0};
}

Color calculateScreenColor(int x, int y, int width, int height) {
	Point p = getScreenPoint(x, y, width, height);
	return Color{clamp(abs(p.x) * 255), clamp(abs(p.y) * 255), 0};
}

Color calculateScreenColorWithAspectRatio(int x, int y, int width, int height) {
	Point p = getScreenPointWithAspectRatio(x, y, width, height);
	return Color{clamp(abs(p.x) * 255), clamp(abs(p.y) * 255), 0};
}

Color calculateColorWithRayDir(int x, int y, int width, int height) {
	Ray ray = getRay(x, y, width, height);
	return Color{clamp(abs(ray.direction.X()) * 255), 
		clamp(abs(ray.direction.Y()) * 255), 
		clamp(abs(ray.direction.Z()) * 255)};
}

Color calculateColorWithNormalizedRayDir(int x, int y, int width, int height) {
	Ray ray = getRayWithNormalizedDir(x, y, width, height);
	return Color{clamp(abs(ray.direction.X()) * 255), 
		clamp(abs(ray.direction.Y()) * 255), 
		clamp(abs(ray.direction.Z()) * 255)};
}

/*
Color calculateColorWithNormalizedRayDir2(int x, int y, int width, int height) {
	Ray ray = getRayWithNormalizedDir(x, y, width, height);
	return Color{int(lerp(0, 0.5, abs(ray.direction.X()))*255), 
		int(lerp(0.5, 1, abs(ray.direction.Y()))*255), 
		int(lerp(0, 1, abs(ray.direction.Z()))*255)};
}*/

int main(void) {

	const int imageWidth = 1920;
	const int imageHeight = 1080;
	const int maxColorComponent = 255;

	Color* pixels = new Color[imageWidth * imageHeight];

	calculateColorsAtPixelsBasedOnRayDir(pixels, imageWidth, imageHeight, caclulateRasterColor);
	generatePPM("output/1.RasterSpace.ppm", pixels, imageWidth, imageHeight, maxColorComponent);

	calculateColorsAtPixelsBasedOnRayDir(pixels, imageWidth, imageHeight, calculateNDCColor);
	generatePPM("output/2.NDCSpace.ppm", pixels, imageWidth, imageHeight, maxColorComponent);

	calculateColorsAtPixelsBasedOnRayDir(pixels, imageWidth, imageHeight, calculateScreenColor);
	generatePPM("output/3.ScreenSpace.ppm", pixels, imageWidth, imageHeight, maxColorComponent);

	calculateColorsAtPixelsBasedOnRayDir(pixels, imageWidth, imageHeight, calculateScreenColorWithAspectRatio);
	generatePPM("output/4.ScreenSpaceWithAspectRatio.ppm", pixels, imageWidth, imageHeight, maxColorComponent);

	calculateColorsAtPixelsBasedOnRayDir(pixels, imageWidth, imageHeight, calculateColorWithRayDir);
	generatePPM("output/5.ScreenSpaceWithRayDir.ppm", pixels, imageWidth, imageHeight, maxColorComponent);

	calculateColorsAtPixelsBasedOnRayDir(pixels, imageWidth, imageHeight, calculateColorWithNormalizedRayDir);
	generatePPM("output/6.ScreenSpaceWithNormalizedRayDir.ppm", pixels, imageWidth, imageHeight, maxColorComponent);

	//calculateColorsAtPixelsBasedOnRayDir(pixels, imageWidth, imageHeight, calculateColorWithNormalizedRayDir2);
	//generatePPM("output/7.ScreenSpaceWithNormalizedRayDir.ppm", pixels, imageWidth, imageHeight, maxColorComponent);

	delete[] pixels;
}