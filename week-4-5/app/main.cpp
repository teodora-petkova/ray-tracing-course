#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>

#include "../include/crtpoint.h"
#include "../include/color.h"
#include "../include/crtray.h"
#include "../include/crtvector.h"
#include "../include/crttriangle.h"

Ray getRayAtPixel(int x, int y, int width, int height) {
    
	// Assumptions:
	// the camera is at (0, 0, 0) of Oxyz and is looking to -Z
	// the image plane is at distance 1 in front of the camera i.e. the center of the image plane is in (0, 0, -1)

    // a raster center point
    CRTPoint rasterCenterPoint = CRTPoint{x + 0.5f, y + 0.5f};

    // convert raster coordinates to NDC (Normalized Device Coordinates) space [0.0, 1.0]
    CRTPoint ndcPoint = CRTPoint { rasterCenterPoint.x / width, rasterCenterPoint.y / height};
	
	// convert NDC coordinates [0.0, 1.0] to Screen space [-1.0, 1.0]
	float ndcX = (2.0 * ndcPoint.x) - 1.0;
	float ndcY = 1.0 - (2.0 * ndcPoint.y);
	CRTPoint screenPoint = {ndcX, ndcY};

	float aspectRatio = float(width) / float(height);
	CRTPoint p = CRTPoint {screenPoint.x * aspectRatio, screenPoint.y};

	// create a ray
	CRTVector origin = CRTVector{0, 0, 0};
	CRTVector direction = CRTVector{p.x, p.y, -1.0};
	Ray ray = Ray{origin, direction.normalize()};
	return ray;
}

Color getColorAtRayIntersection(Ray ray, std::vector<CRTTriangle> triangles,
    Color backgroundColor)
{
    float minDistance = FLT_MAX;

	Color color = backgroundColor;
	for(CRTTriangle triangle : triangles)
	{
		float rayProjection = ray.direction.dot(triangle.getNormal());
		float rpDist = triangle.A().dot(triangle.getNormal());

		// if the ray is not parallel to the triangle's plane
	    // and the ray is towards the triangle's plane
	    if(rayProjection != 0 && rpDist < 0)
	    {
			float rayLength = 1; // ray.direction.length(); 
			float t = rpDist * (rayLength / rayProjection);

			CRTVector intersectionPoint = ray.origin + ray.direction * t;
    
	    	if(triangle.IsPointInside(intersectionPoint))
	    	{
                if(t <= minDistance)
                {
                    color = triangle.getColor();
					minDistance = t;
				}
	    	}
	    }
	}
	return color;
}

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

std::vector<CRTTriangle> getPyramid()
{
	int y_max = 2;
	int y_min = -1;

	int x_min = -2;
	int x_max = 2;

	CRTVector A = CRTVector(0, y_max, -3);
	CRTVector B = CRTVector(x_min, y_min, -4);
	CRTVector C = CRTVector(x_max, y_min, -4);
	CRTVector D = CRTVector(x_max, y_min, -2);
	CRTVector E = CRTVector(x_min, y_min, -2);

	std::vector<CRTTriangle> triangles = {
		// to see the inner part of the pyramid
	  	//CRTTriangle(E, D, A, Color{156, 39, 6}),
		CRTTriangle(C, D, A, Color{212, 91, 18}),
		CRTTriangle(B, C, A, Color{96, 60, 20}),
		CRTTriangle(E, B, A, Color{95, 84, 38}),
		CRTTriangle(E, D, C, Color{156, 39, 6}),
		CRTTriangle(E, C, B, Color{212, 91, 18})};

	return triangles;
}

std::vector<CRTTriangle> getDiamond()
{
	float x = 1;
	float y = 2;

	CRTVector v1 = CRTVector(0, -y, -3);
	CRTVector v2 = CRTVector(x, 0, -3);
	CRTVector v3 = CRTVector(0, 0, -4);
	CRTVector v4 = CRTVector(-x, 0, -3);
	CRTVector v5 = CRTVector(0, y, -3);
	CRTVector v6 = CRTVector(0, 0, -2);

	return {
		// back triangles with normals outwards
		// to be seen if we look from behind the diamond
		CRTTriangle(v3, v2, v1, Color{0, 100, 0}),
		CRTTriangle(v4, v3, v1, Color{0, 140, 80}),
		CRTTriangle(v5, v2, v3, Color{40, 180, 120}),
		CRTTriangle(v4, v5, v3, Color{80, 220, 160}),
		// front triangles 
		CRTTriangle(v1, v2, v6, Color{120, 10, 160}),
		CRTTriangle(v1, v6, v4, Color{140, 10, 180}),
		CRTTriangle(v6, v2, v5, Color{160, 10, 200}),
		CRTTriangle(v6, v5, v4, Color{180, 20, 220})
		};
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

	generatePPMWithTriangles("4.pyramid.ppm", imageWidth, imageHeight,
		maxColorComponent,  getPyramid(), backgroundColor);

	generatePPMWithTriangles("5.diamond.ppm", imageWidth, imageHeight,
		maxColorComponent, getDiamond(), backgroundColor);
}