#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <string>
#include <vector>
#include "crtvector.h"

struct Point {
	int x;
	int y;
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

void calculateColorsAtPixelsBasedOnRayDir(Color* pixels, int height, int width) {
	
	for (int pixely = 0; pixely < height; ++pixely) {

		for (int pixelx = 0; pixelx < width; ++pixelx) {
			
			float x = pixelx;
			float y = pixely;

			// 1. find its center, based on the raster coordinates
			x += 0.5;
			y += 0.5;

			// 2. convert raster coordinates to NDC (Normalized Device Coordinates) space [0.0, 1.0]
			x /= width;
			y /= height;

			// 3. convert NDC coordinates to Screen space [-1.0, 1.0]
			x = (2.0 * x) - 1.0;
			y = 1.0 - (2.0 * y);

			// 4. consider the aspect ratio
			x *= width / height;

			// 5. create a ray with normalized ray direction
			CRTVector origin = CRTVector{0, 0, 0};
			CRTVector direction = CRTVector{x, y, -1.0};
			Ray ray = Ray{origin, direction.normalize()};

			// 6. store the color calculated by the ray direction
			int r = fabs(ray.direction.X() * 255);
			int g = fabs(ray.direction.Y() * 255);
			int b = fabs(ray.direction.Z() * 255);

			pixels[pixelx + pixely * width] = Color{r, g, b};
		}
	}
}

int main(void) {

	const int maxColorComponent = 255;

	const std::vector<std::pair<int, int>> imageSizes = {
		{50, 100}, 
		{65, 100},
		{75, 100},
		{1000, 50},
		{100, 65},
		{100, 75}, 
		{50, 50}};

	for (auto imageSize : imageSizes) {

		const int imageWidth = imageSize.first;
		const int imageHeight = imageSize.second;

		Color* pixels = new Color[imageWidth * imageHeight];

		calculateColorsAtPixelsBasedOnRayDir(pixels, imageHeight, imageWidth);
		std::string filename = "output/image" + std::to_string(imageSize.first) + "_" + std::to_string(imageSize.second) + ".ppm";
		generatePPM(filename, pixels, imageWidth, imageHeight, maxColorComponent);

		delete[] pixels;
	}
}