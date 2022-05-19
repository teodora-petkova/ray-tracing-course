#include "../include/crtrenderer.h"

constexpr float EPSILON = 0.0001f;

inline bool isCloseToZero(float num)
{
	return std::fabs(num) < EPSILON;
}

int clamp(float f){
	int i = int(f);
	return std::min(std::max(0, i), 255);
}

CRTRay getRayAtPixel(int x, int y, int width, int height, const CRTCamera &camera) {
    
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
	CRTVector v = CRTVector{p.x, p.y, -1.0};
	CRTRay ray = camera.getCameraRay(v);
	return ray;
}

CRTIntersectionInfo traceRay(const CRTRay& ray, std::vector<CRTMesh> meshes)
{
	bool isTriangleHit = false;
	float minDistance = FLT_MAX;
	CRTTriangle hitTriangle = CRTTriangle();

	for(CRTMesh& mesh : meshes)
	{
		for(CRTTriangle& triangle : mesh.getTriangles())
		{
			float rayProjection = triangle.getNormal().dot(ray.direction);
			float rpDist = (triangle.A() - ray.origin).dot(triangle.getNormal());

			// if the ray is not parallel to the triangle's plane
	    	if(rayProjection != 0) //&& rpDist < 0) // and the ray is towards the triangle's plane
	    	{
				float rayLength = 1; // ray.direction.length(); 
				float t = rpDist * (rayLength / rayProjection);

				CRTVector intersectionPoint = ray.origin + ray.direction * t;
	
	    		if(	triangle.IsPointInside(intersectionPoint) &&
					t > 0 &&
					t < minDistance)
        	    {
					isTriangleHit = true;
					hitTriangle = triangle;
					minDistance = t;
				}
	    	}
		}
	}

	return CRTIntersectionInfo{isTriangleHit, minDistance, hitTriangle};
}

CRTColor computeColor(const CRTVector &intersectionPoint,
	const CRTIntersectionInfo &intersection, const CRTScene& scene)
{
	float shadowBias = EPSILON;
	CRTColor finalColor = CRTColor(0, 0, 0);
	CRTColor albedo = scene.getSettings().globalAlbedo;

	for(auto& light : scene.getLights())
	{
		CRTVector vectorFromIntersectionToLight = light.getPosition() - intersectionPoint;
		CRTVector lightDir = vectorFromIntersectionToLight.normalize();

		CRTVector overPoint = intersectionPoint + intersection.hitTriangle.getNormal() * shadowBias;
		CRTRay shadowRay = CRTRay{overPoint, lightDir};
		
		auto shadowRayIntersection = traceRay(shadowRay, scene.getGeometryObjects());

		CRTColor lightContribution = CRTColor(0, 0, 0);
		if(!shadowRayIntersection.isTriangleHit)
		{
		
			float cosLaw = std::fmax(0, lightDir.dot(intersection.hitTriangle.getNormal()));

			float sphereRadius = vectorFromIntersectionToLight.length();
			float sphereArea = 4 * M_PI * sphereRadius * sphereRadius;

			lightContribution = CRTColor(albedo * (light.getIntensity() / sphereArea) * cosLaw);
		}

		finalColor = finalColor + lightContribution;
	}
	return finalColor;
}

CRTColor getColorAtRayIntersection(const CRTRay &ray, const CRTScene &scene)
{
	CRTIntersectionInfo intersection = traceRay(ray, scene.getGeometryObjects());

	CRTColor color = scene.getSettings().backgroundColor;
	if(intersection.isTriangleHit)
	{
		CRTVector intersectionPoint = ray.origin + ray.direction * intersection.minDistance;
		color = computeColor(intersectionPoint, intersection, scene);
	}
	return color;
}

void renderPPMImage(std::string filename, const CRTCanvas& canvas)
{
	const int maxColorComponent = 255;

	std::ofstream ppmFileStream(filename, std::ios::out | std::ios::binary | std::ios::trunc);
	if (ppmFileStream) 
	{
		ppmFileStream << "P3\n";
		ppmFileStream << canvas.getWidth() << " " << canvas.getHeight() << "\n";
		ppmFileStream << maxColorComponent << "\n";

		for (int colIdx = 0; colIdx < canvas.getHeight(); ++colIdx)
		{
			for (int rowIdx = 0; rowIdx < canvas.getWidth(); ++rowIdx)
			{
				CRTColor c = canvas.getPixel(rowIdx, colIdx);

				ppmFileStream << clamp(c.R()*255) << " " << clamp(c.G()*255) << " " << clamp(c.B()*255) << "\t";
			}
			ppmFileStream << "\n";
		}
		ppmFileStream.close();
	}
}

void renderPixelChunks(int heightStart, int heightEnd, const CRTScene& scene, CRTCanvas* canvas)
{
	int width = scene.getSettings().imageSettings.width;
	int height = scene.getSettings().imageSettings.height;

	for (int x = 0; x < width; x++)
	{
		for (int y = heightStart; y < heightEnd; y++)
		{
			CRTRay ray = getRayAtPixel(x, y, width, height, scene.getCamera());
			CRTColor color = getColorAtRayIntersection(ray, scene);
			canvas->setPixel(x, y, color);
		}
	}
}

void CRTRenderer::RenderImage(std::string filename, const CRTScene& scene)
{
	int width = scene.getSettings().imageSettings.width;
	int height = scene.getSettings().imageSettings.height;

	CRTCanvas canvas = CRTCanvas(width, height);

	int numThreads = std::thread::hardware_concurrency() - 4;

	thread_pool pool(numThreads);

	pool.parallelize_loop(0, height,
						  [&scene, &canvas](const uint32_t &heightStart, const uint32_t &heightEnd)
						  {
                              renderPixelChunks(heightStart, heightEnd, scene, &canvas);
						  });

	renderPPMImage(filename, canvas);
}