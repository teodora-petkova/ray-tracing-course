#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>

#include "../include/crtpoint.h"
#include "../include/color.h"
#include "../include/crtray.h"
#include "../include/crtvector.h"
#include "../include/crttriangle.h"
#include "../include/crtcamera.h"

std::vector<CRTTriangle> getPyramid(bool toRemoveFrontTriangle = false)
{
	float xmin = -1.5;
	float xmax = 1.5;

	float ymin = -1;
	float ymax = 2.5;

	CRTVector A = CRTVector(0, ymax, -3);
	CRTVector B = CRTVector(xmin, ymin, -4);
	CRTVector C = CRTVector(xmax, ymin, -4);
	CRTVector D = CRTVector(xmax, ymin, -2);
	CRTVector E = CRTVector(xmin, ymin, -2);

	std::vector<CRTTriangle> triangles = {
		// back triangles with normals outwards
		// to be seen if we look from behind the diamond
	  	CRTTriangle(E, D, A, Color{156, 39, 6}),
		CRTTriangle(A, D, C, Color{212, 91, 18}),
		CRTTriangle(A, C, B, Color{96, 60, 20}),
		CRTTriangle(A, B, E, Color{95, 84, 38}),
		CRTTriangle(E, D, C, Color{156, 39, 6}),
		CRTTriangle(E, C, B, Color{200, 81, 28})
		};

	if(toRemoveFrontTriangle)
	{
		triangles.erase(triangles.begin());
	}

	return triangles;
}

std::vector<CRTTriangle> getDiamond()
{
	float a = 1;
	float b = 2;
	CRTVector v1 = CRTVector(0, -b, -3);
	CRTVector v2 = CRTVector(a, 0, -3);
	CRTVector v3 = CRTVector(0, 0, -4);
	CRTVector v4 = CRTVector(-a, 0, -3);
	CRTVector v5 = CRTVector(0, b, -3);
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

CRTRay getRayAtPixel(int x, int y, int width, int height, CRTCamera camera) {
    
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
	CRTRay ray = camera.getCameraRay(CRTVector{p.x, p.y, -1.0});
	return ray;
}

Color getColorAtRayIntersection(CRTRay ray, CRTCamera camera,
	std::vector<CRTTriangle> triangles, Color backgroundColor)
{
    float minDistance = FLT_MAX;

	Color color = backgroundColor;
	for(CRTTriangle triangle : triangles)
	{
		float rayProjection = ray.direction.dot(triangle.getNormal());
		float rpDist = (triangle.A() - camera.getPosition()).dot(triangle.getNormal());

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

void generatePPMWithTriangles(std::string filename, std::vector<CRTTriangle> triangles,
	CRTCamera camera, int imageWidth, int imageHeight, int maxColorComponent, Color backgroundColor)
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
				CRTRay ray = getRayAtPixel(colIdx, rowIdx, imageWidth, imageHeight, camera);
				Color c = getColorAtRayIntersection(ray, camera, triangles, backgroundColor);

				ppmFileStream << c.r << " " << c.g << " " << c.b << "\t";
			}
			ppmFileStream << "\n";
		}
		ppmFileStream.close();
	}
}

void generatePPMWithMovingCameraPosition(int imageWidth, int imageHeight,
	int maxColorComponent, Color backgroundColor)
{
	CRTCamera camera = CRTCamera();
	
	std::vector<CRTTriangle> triangle = { CRTTriangle(
		CRTVector(-1.75, -1.75, -3),
		CRTVector(1.75, -1.75, -3),
		CRTVector(0, 1.75, -3),
		Color{156, 39, 6}
	)};

	// moving left and right
	camera.truck(3.2);
	generatePPMWithTriangles("samples/1.basic_triangle_truck_left.ppm", triangle,
		camera, imageWidth, imageHeight, maxColorComponent, backgroundColor);

	camera.truck(-6.4);
	generatePPMWithTriangles("samples/1.basic_triangle_truck_right.ppm", triangle,
		camera, imageWidth, imageHeight, maxColorComponent, backgroundColor);

	// return at the center
	camera.truck(3.2);

	// moving up and down
	camera.pedestal(-1.1);
	generatePPMWithTriangles("samples/2.basic_triangle_pedestal_up.ppm", triangle,
		camera, imageWidth, imageHeight, maxColorComponent, backgroundColor);

	camera.pedestal(2.2);
	generatePPMWithTriangles("samples/2.basic_triangle_pedestal_down.ppm", triangle,
		camera, imageWidth, imageHeight, maxColorComponent, backgroundColor);

	// return at the center
	camera.pedestal(-1.1);

	// moving in and out
	camera.dolly(1.1);
	generatePPMWithTriangles("samples/3.basic_triangle_dolly_out.ppm", triangle,
		camera, imageWidth, imageHeight, maxColorComponent, backgroundColor);

	camera.dolly(-2.2);
	generatePPMWithTriangles("samples/3.basic_triangle_dolly_in.ppm", triangle,
		camera, imageWidth, imageHeight, maxColorComponent, backgroundColor);

	// return at the center
	//camera.pedestal(1.1);
}

void generatePPMWithRotatingCamera(int imageWidth, int imageHeight,
	int maxColorComponent, Color backgroundColor)
{	
	CRTCamera camera = CRTCamera();
	auto diamond = getDiamond();
	
	// rotate around x
	camera.tilt(25);
	generatePPMWithTriangles("samples/4.diamond_tilt.ppm", diamond,
		camera, imageWidth, imageHeight, maxColorComponent,  backgroundColor);
	camera.tilt(-25);

	// rotate around y
	camera.pan(-30);
	generatePPMWithTriangles("samples/5.diamond_pan.ppm", diamond,
		camera, imageWidth, imageHeight, maxColorComponent, backgroundColor);
	camera.pan(30);

	// rotate around z
	camera.roll(45);
	generatePPMWithTriangles("samples/6.diamond_roll.ppm", diamond,
		camera, imageWidth, imageHeight, maxColorComponent, backgroundColor);
	camera.roll(45);
}

void createFramesForRotatingAroundObject(std::vector<CRTTriangle> triangles, const char* objectName,
	float distanceFromCameraToObjectCenter, 
	int imageWidth, int imageHeight, int maxColorComponent, Color backgroundColor)
{
	CRTCamera camera = CRTCamera();

	float rotationAngle = 10;
	float angle = (rotationAngle / 2) * (M_PI/180.f);
	float dist = 2 * sinf(angle) * distanceFromCameraToObjectCenter;
	float x = cosf(angle) * dist;
	float z = sinf(angle) * dist;

	for(int i=0; i<=36 /*frames*/; i+=1)
	{
		char filepath[50];
		sprintf(filepath, "%s%d.ppm", objectName, i);
		generatePPMWithTriangles(filepath, triangles,
			camera, imageWidth, imageHeight, maxColorComponent, backgroundColor);

		camera.pan(rotationAngle); // rotate around y
		camera.dolly(-z);
		camera.truck(x);
		//camera.translate(CRTVector(x, 0, -z));
	}
}

int main(void)
{
	const int imageWidth = 800; // 1920;
	const int imageHeight = 400; // 1080;

	const int maxColorComponent = 255;

	Color backgroundColor = Color{243, 188, 46};

	generatePPMWithMovingCameraPosition(imageWidth, imageHeight,
										maxColorComponent, backgroundColor);

	generatePPMWithRotatingCamera(imageWidth, imageHeight,
								  maxColorComponent, backgroundColor);

	auto pyramid = getPyramid();
	float r = 3; // distance to the center of the object
	createFramesForRotatingAroundObject(pyramid, "samples/pyramid", r,
		imageWidth, imageHeight, maxColorComponent, backgroundColor);\
	
	auto diamond = getDiamond();
	r = 3; // distance to the center of the object
	createFramesForRotatingAroundObject(diamond, "samples/diamond", r,
		imageWidth, imageHeight, maxColorComponent, backgroundColor);

	std::cout << "end" << std::endl;
}