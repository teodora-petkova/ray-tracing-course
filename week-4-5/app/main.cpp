#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>

#include "../include/crtpoint.h"
#include "../include/color.h"
#include "../include/crtray.h"
#include "../include/crtvector.h"
#include "../include/crttriangle.h"

void generatePPMWithTriangles(std::string filename, 
	int imageWidth, int imageHeight, int maxColorComponent,
	std::vector<CRTTriangle> triangles, Color backgroundColor)
{
	std::ofstream ppmFileStream(filename, std::ios::out | std::ios::binary | std::ios::trunc);
	if (ppmFileStream) 
	{
		ppmFileStream << "P3\n";
		ppmFileStream << imageWidth << " " << imageHeight << "\n";
		ppmFileStream << maxColorComponent << "\n";

		for (int rowIdx = 0; rowIdx < imageHeight; ++rowIdx)
		{
			for (int colIdx = 0; colIdx < imageWidth; ++colIdx)
			{
				Ray ray = getRayAtPixel(colIdx, rowIdx, imageWidth, imageHeight);
				Color c = getColorAtRayIntersection(ray, triangles, backgroundColor);

				ppmFileStream << c.r << " " << c.g << " " << c.b << "\t";
			}
			ppmFileStream << "\n";
		}
		ppmFileStream.close();
	}
}

int main(void)
{
	const int imageWidth = 1920;
	const int imageHeight = 1080;

	const int maxColorComponent = 255;

	Color backgroundColor = Color{243, 188, 46};
	
	CRTTriangle triangle1 = CRTTriangle(
		CRTVector(-1.75, -1.75, -3),
		CRTVector(1.75, -1.75, -3),
		CRTVector(0, 1.75, -3),
		Color{156, 39, 6}
	);

	generatePPMWithTriangles("1.basic_triangle.ppm", imageWidth, imageHeight,
		maxColorComponent,  std::vector<CRTTriangle>{triangle1}, backgroundColor);


	CRTTriangle triangle2 = CRTTriangle(
		CRTVector(-3.75, -2.75, -4),
		CRTVector(4.75, -2.75, -4),
		CRTVector(-3, 3.75, -4),
		Color{212, 91, 18}
	);

	generatePPMWithTriangles("2.another_triangle.ppm", imageWidth, imageHeight,
		maxColorComponent,  std::vector<CRTTriangle>{triangle2}, backgroundColor);

	CRTTriangle triangle3 = CRTTriangle(
		CRTVector(-0.75, -0.75, -2),
		CRTVector(0.75, -0.75, -2),
		CRTVector(0, 0.75, -2),
		Color{96, 60, 20}
	);

	generatePPMWithTriangles("3.triangles.ppm", imageWidth, imageHeight,
		maxColorComponent,  std::vector<CRTTriangle>{triangle1, triangle2, triangle3}, backgroundColor);

	CRTVector A = CRTVector(0, 1, -3);
	CRTVector B = CRTVector(-1, -1, -4);
	CRTVector C = CRTVector(1, -1, -4);
	CRTVector D = CRTVector(1, -1, -2);
	CRTVector E = CRTVector(-1, -1, -2);

	std::vector<CRTTriangle> triangles = {
	  	//CRTTriangle(E, D, A, Color{156, 39, 6}),
		CRTTriangle(C, D, A, Color{212, 91, 18}),
		CRTTriangle(B, C, A, Color{96, 60, 20}),
		CRTTriangle(E, B, A, Color{95, 84, 38}),
		CRTTriangle(E, D, C, Color{156, 39, 6}),
		CRTTriangle(E, C, B, Color{212, 91, 18})};

	generatePPMWithTriangles("4.pyramid.ppm", imageWidth, imageHeight,
		maxColorComponent,  triangles, backgroundColor);

	std::cout << "end" << std::endl;
}