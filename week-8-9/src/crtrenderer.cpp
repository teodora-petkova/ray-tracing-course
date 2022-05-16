#include "../include/crtrenderer.h"

constexpr float EPSILON = 0.0001f;

inline bool isCloseToZero(float num)
{
	return std::fabs(num) < EPSILON;
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
	const CRTTriangle &triangle, const CRTCamera &camera,
	std::vector<CRTMesh> meshes, std::vector<CRTLight> lights,
	CRTColor albedo)
{
	float shadowBias = EPSILON;
	CRTColor finalColor = CRTColor(0, 0, 0);
	
	for(auto& light : lights)
	{
		CRTVector vectorFromIntersectionToLight = light.getPosition() - intersectionPoint;
		CRTVector lightDir = vectorFromIntersectionToLight.normalize();
		
		CRTRay shadowRay = CRTRay{intersectionPoint + triangle.getNormal() * shadowBias, lightDir};
		
		auto intersection = traceRay(shadowRay, meshes);

		CRTColor lightContribution = CRTColor(0, 0, 0);
		if(!intersection.isTriangleHit)
		{
			float cosLaw = std::fmax(0, lightDir.dot(triangle.getNormal()));

			float sphereRadius = vectorFromIntersectionToLight.length();
			float sphereArea = 4 * M_PI * sphereRadius * sphereRadius;

			lightContribution = CRTColor(albedo * cosLaw * light.getIntensity() / sphereArea);
		}

		finalColor = finalColor + lightContribution;
	}
	return finalColor;
}

CRTColor getColorAtRayIntersection(const CRTRay &ray, const CRTCamera &camera,
	std::vector<CRTMesh> meshes, std::vector<CRTLight> lights,
	CRTColor backgroundColor, CRTColor globalAlbedo)
{
	CRTIntersectionInfo intersection = traceRay(ray, meshes);

	CRTColor color = backgroundColor;
	if(intersection.isTriangleHit)
	{
		CRTVector intersectionPoint = ray.origin + ray.direction * intersection.minDistance;
		color = computeColor(intersectionPoint, intersection.hitTriangle,
			camera, meshes, lights, globalAlbedo);
	}
	return color;
}

void CRTRenderer::RenderImage(std::string filename, const CRTScene& scene)
{	
	const int maxColorComponent = 255;

	int imageWidth = scene.getSettings().imageSettings.width;
	int imageHeight = scene.getSettings().imageSettings.height;

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
				CRTRay ray = getRayAtPixel(colIdx, rowIdx, imageWidth, imageHeight, 
					scene.getCamera());
				CRTColor c = getColorAtRayIntersection(ray, 
					scene.getCamera(), scene.getGeometryObjects(), 
					scene.getLights(), scene.getSettings().backgroundColor,
					scene.getSettings().globalAlbedo);

				ppmFileStream << int(c.R()*255) << " " << int(c.G()*255) << " " << int(c.B()*255) << "\t";
			}
			ppmFileStream << "\n";
		}
		ppmFileStream.close();
	}
}