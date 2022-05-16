#include "../include/crtrenderer.h"

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

CRTColor getColorAtRayIntersection(const CRTRay& ray, const CRTCamera &camera,
	std::vector<CRTMesh> meshes, const CRTColor &backgroundColor)
{
    float minDistance = FLT_MAX;

	CRTColor color = backgroundColor;
	for(CRTMesh& mesh : meshes)
	{
		for(CRTTriangle& triangle : mesh.getTriangles())
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
					scene.getSettings().backgroundColor);

				ppmFileStream << int(c.R()*255) << " " << int(c.G()*255) << " " << int(c.B()*255) << "\t";
			}
			ppmFileStream << "\n";
		}
		ppmFileStream.close();
	}
}