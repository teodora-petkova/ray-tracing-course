#include "../include/crtray.h"

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