#include <iostream>
#include <fstream>
#include <filesystem>
//namespace fs = std::filesystem;

struct Point {
	int x;
	int y;
};

struct Color {
	int r;
	int g;
	int b;
};

struct Circle {
	Point center;
	int radius;
	Color color;
};

int random(int min, int max) {
	// not uniform, but ok for simple test scripts
	return min + rand() % ((max + 1) - min);
}

void generatePPM(const char* fileName, Color* pixels,
	int imageWidth, int imageHeight, int maxColorComponent) {
	std::ofstream ppmFileStream(fileName, std::ios::out | std::ios::binary | std::ios::trunc);
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

void generateRectanglesWithRandomColors(int count, Color* pixels, int imageWidth, int imageHeight) {

	int sizex = int(imageWidth / count);
	int sizey = int(imageHeight / count);

	// generate random colors
	Color* colors = new Color[count * count];
	for (int i = 0; i < count * count; i++) {
		int c = random(10, 240);
		Color color = Color{random(10, 240), c, c};
		colors[i] = color;
	}

	for (int i = 0; i < count; ++i) {
		for (int j = 0; j < count; ++j) {
			Color c = colors[i + j * count];

			for (int x = i * sizex; x < (i+1) * sizex; ++x) {
				for (int y = j * sizey; y < (j+1) * sizey; ++y) {
					if(random(0, 2) == 2) {
						// add some noise
						pixels[x + y * imageWidth] = Color{c.r + random(10, 50), c.g + random(10, 50), c.b + random(10, 50)};
					}
					else{
						pixels[x + y * imageWidth] = c;
					}
				}
			}
		}
	}

	delete[] colors;
}

bool isInCircle(const Point& p, const Circle& circle) {
	return std::pow(p.x - circle.center.x, 2) + std::pow(p.y - circle.center.y, 2) <= std::pow(circle.radius, 2);
}

void generateCircles(Circle* circles, int count, const Color& backgroundColor,
	Color* pixels, int imageWidth, int imageHeight) {

	int halfy = int(imageHeight / 2);
	int halfx = int(imageWidth / 2);

	for (int x = -halfx; x < halfx; ++x) {

		for (int y = -halfy; y < halfy; ++y) {

			Color currentColor = backgroundColor;
			for (int i = 0; i < count; i++)
			{
				if (isInCircle(Point{x, y}, circles[i]))
				{
					currentColor = circles[i].color;
				}
			}
			pixels[(x + halfx) + (y + halfy) * imageWidth] = currentColor;
		}
	}
}

double getDistance(const Point& p1, const Point& p2) {
	return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
}

void fillWithRandomCircles(Circle* circles, int count, int imageWidth, int imageHeight) {
	// a hacky way to generate pseudo random circles

	int halfy = int(imageHeight / 2);
	int halfx = int(imageWidth / 2);

	int epsilon = 5; // allowing a small overlapping
	int maxTrials = 1000;
	int currentTrials = 0;

	int i = 0;
	while (i < count) {
		Point p = Point{random(-halfx, halfx), random(-halfy, halfy)};
		int radius = random(2*epsilon, halfy);
		int c = random(10, 240);
		Color color = Color{c, random(10, 240), c};
		Circle newCircle = Circle{p, radius, color};

		bool isOverlapped = false;

		for (int j = 0; j < i; j++) {
			if(getDistance(circles[j].center, newCircle.center) < circles[j].radius + newCircle.radius - epsilon) {
				isOverlapped = true;
				break;
			}
		}

		if(!isOverlapped) {
			circles[i] = newCircle;
			i++;
		}

		currentTrials++;
		if(currentTrials > maxTrials) {
			break;
		}
	}
}

int main(void) {

	srand(time(0));

	/*const char *outputPath = "output";
	if(!fs::exists(outputPath)) {
		fs::create_directories(outputPath);
	}*/

	const int imageWidth = 1920;
	const int imageHeight = 1080;
	const int maxColorComponent = 255;

	Color* pixels = new Color[imageWidth * imageHeight];

	generateRectanglesWithRandomColors(4, pixels, imageWidth, imageHeight);
	generatePPM("output/rectangles.ppm", pixels, imageWidth, imageHeight, maxColorComponent);

	Color backgroundColor = Color{159, 181, 163};

	Circle circle = Circle{Point{0, 0}, 500, Color{124, 145, 143}};
	Circle oneCircleArray[1] = {circle};
	generateCircles(oneCircleArray, 1, backgroundColor, pixels, imageWidth, imageHeight);
	generatePPM("output/circle.ppm", pixels, imageWidth, imageHeight, maxColorComponent);

	int count = 20;
	Circle *circles = new Circle[count];
	fillWithRandomCircles(circles, count, imageWidth, imageHeight);
	generateCircles(circles, count, backgroundColor, pixels, imageWidth, imageHeight);
	generatePPM("output/circles.ppm", pixels, imageWidth, imageHeight, maxColorComponent);
	delete[] circles;

	delete[] pixels;
}