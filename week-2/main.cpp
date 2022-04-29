#include <iostream>
#include <fstream>
#include <vector>

struct Point
{
	int x;
	int y;
};

struct Color
{
	int r;
	int g;
	int b;
};

struct Shape
{
	Point center;
	int radius;
	Color color;
};

int random(int min, int max)
{
	// not uniform, but ok for simple test scripts
	return min + rand() % ((max + 1) - min);
}

void generatePPM(const char *fileName, Color *pixels,
				 int imageWidth, int imageHeight, int maxColorComponent)
{
	std::ofstream ppmFileStream(fileName, std::ios::out | std::ios::binary | std::ios::trunc);
	ppmFileStream << "P3\n";
	ppmFileStream << imageWidth << " " << imageHeight << "\n";
	ppmFileStream << maxColorComponent << "\n";

	for (int rowIdx = 0; rowIdx < imageHeight; ++rowIdx)
	{
		for (int colIdx = 0; colIdx < imageWidth; ++colIdx)
		{
			Color c = pixels[colIdx + rowIdx * imageWidth];
			ppmFileStream << c.r << " " << c.g << " " << c.b << "\t";
		}
		ppmFileStream << "\n";
	}

	ppmFileStream.close();
}

Color generateRandomColor()
{
	int c1 = random(0, 255);
	int c2 = random(0, 255);
	int c3 = random(0, 255);
	return Color{c1, c2, c3};
}

Color generateRandomPurpleGreenishColor()
{
	int c = random(0, 255);
	int c2 = random(0, 255);
	return Color{c, c2, c};
}

void generateRectanglesWithRandomColors(int count, Color *pixels, int imageWidth, int imageHeight)
{
	int sizex = int(imageWidth / count);
	int sizey = int(imageHeight / count);

	// generate random colors
	Color *colors = new Color[count * count];
	for (int i = 0; i < count * count; i++)
	{
		colors[i] = generateRandomColor();
	}

	for (int x = 0; x < imageWidth; ++x)
	{
		for (int y = 0; y < imageHeight; ++y)
		{
			Color c = colors[(x / sizex) + (y / sizey) * count];
			if (random(0, 2) == 2)
			{
				// add some noise
				int rand = random(0, 255);
				c = Color{c.r + rand, c.g + rand, c.b + rand};
			}
			pixels[x + y * imageWidth] = c;
		}
	}

	delete[] colors;
}

bool isPointInCircle(const Point &p, const Shape &circle)
{
	return std::pow(p.x - circle.center.x, 2) + std::pow(p.y - circle.center.y, 2) <= std::pow(circle.radius, 2);
}

void generateRandomShapes(Shape* shapes, int count, const Color &backgroundColor,
					Color *pixels, int imageWidth, int imageHeight, bool (*isPointInShape)(const Point &, const Shape &))
{
	int halfy = int(imageHeight / 2);
	int halfx = int(imageWidth / 2);

	for (int x = -halfx; x < halfx; ++x)
	{
		for (int y = -halfy; y < halfy; ++y)
		{
			Color currentColor = backgroundColor;
			for (int i = 0; i < count; i++)
			{
				if (isPointInShape(Point{x, y}, shapes[i]))
				{
					currentColor = shapes[i].color;
				}
			}
			pixels[(x + halfx) + (y + halfy) * imageWidth] = currentColor;
		}
	}
}

double getSquaredDistance(const Point &p1, const Point &p2)
{
	return std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2);
}

void fillWithRandomShapes(Shape* shapes, int count, int imageWidth, int imageHeight)
{
	// a hacky way to generate pseudo random circles

	int halfy = int(imageHeight / 2);
	int halfx = int(imageWidth / 2);

	int epsilon = 5; // allowing a small overlapping
	int maxTrials = 1000;
	int currentTrials = 0;

	int i = 0;
	while (i < count)
	{
		Point p = Point{random(-halfx, halfx), random(-halfy, halfy)};
		int radius = random(2 * epsilon, halfy / 2);
		Color color = generateRandomPurpleGreenishColor();
		Shape newShape = Shape{p, radius, color};

		bool areShapesOverlapped = false;

		for (int j = 0; j < i; j++)
		{
			// avoid full overlap
			float diffBetweenShapeRadiuses = shapes[j].radius + newShape.radius - epsilon;
			if (getSquaredDistance(shapes[j].center, newShape.center) < 
					diffBetweenShapeRadiuses*diffBetweenShapeRadiuses)
			{
				areShapesOverlapped = true;
				break;
			}
		}

		if (!areShapesOverlapped)
		{
			shapes[i] = newShape;
			i++;
		}

		currentTrials++;
		if (currentTrials > maxTrials)
		{
			break;
		}
	}
}

bool isPointInHeart(const Point &p, const Shape &heart)
{
	// scale as the equation is for range [0, 1]
	double scale = heart.radius;
	double xx = (p.x - heart.center.x) / scale;
	double yy = (p.y - heart.center.y) / scale;
	return (-(std::pow((std::pow(xx, 2) + std::pow(yy, 2) - 1), 3) + std::pow(xx, 2) * std::pow(yy, 3)) >= 0);
}

void generateRectangles(const char* filepath, int imageWidth, int imageHeight, int maxColorComponent)
{
	Color *pixels = new Color[imageWidth * imageHeight];

	generateRectanglesWithRandomColors(4, pixels, imageWidth, imageHeight);
	generatePPM(filepath, pixels, imageWidth, imageHeight, maxColorComponent);

	delete[] pixels;
}

void generateCircle(const char* filepath, int imageWidth, int imageHeight, int maxColorComponent,
	Color backgroundColor)
{
	Color *pixels = new Color[imageWidth * imageHeight];

	Shape circle = Shape{Point{0, 0}, 500, Color{124, 145, 143}};
	Shape oneCircleArray[1] = {circle};
	generateRandomShapes(oneCircleArray, 1, backgroundColor, pixels, imageWidth, imageHeight, isPointInCircle);
	generatePPM(filepath, pixels, imageWidth, imageHeight, maxColorComponent);

	delete[] pixels;
}

void generateShapes(const char* filepath, int imageWidth, int imageHeight, int maxColorComponent,
	Color backgroundColor, bool (*isPointInShape)(const Point &, const Shape &))
{
	int count = 20;
	Color *pixels = new Color[imageWidth * imageHeight];
	Shape *shapes = new Shape[count];

	fillWithRandomShapes(shapes, count, imageWidth, imageHeight);
	generateRandomShapes(shapes, count, backgroundColor, pixels, imageWidth, imageHeight, isPointInShape);
	generatePPM(filepath, pixels, imageWidth, imageHeight, maxColorComponent);

	delete[] pixels;
	delete[] shapes;
}

int main(void)
{
	const int imageWidth = 1920;
	const int imageHeight = 1080;
	const int maxColorComponent = 255;

	const Color backgroundColor = Color{159, 181, 163};

	generateRectangles("output/rectangles.ppm", imageWidth, imageHeight, maxColorComponent);

	generateCircle("output/circle.ppm", imageWidth, imageHeight, maxColorComponent, backgroundColor);

	generateShapes("output/circles.ppm", imageWidth, imageHeight, maxColorComponent, backgroundColor, isPointInCircle);

	generateShapes("output/hearts.ppm", imageWidth, imageHeight, maxColorComponent, backgroundColor, isPointInHeart);
}