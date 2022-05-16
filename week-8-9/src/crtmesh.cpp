#include "../include/crtmesh.h"

float random(int min=0, int max=1)
{
    // rand between 0 and 1
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = max - min;
    float r = random * diff;
    return min + r;
}

CRTColor getRandomColor()
{
	float c1 = random();
	float c2 = random();
	float c3 = random();
	return CRTColor(c1, c2, c3);
}

CRTMesh::CRTMesh(const std::vector<CRTVector>& vertices,
    const std::vector<int>& trianglesVertices)
{
    this->vertices = vertices;
    this->trianglesVertices = trianglesVertices;
    int trianglesCount = this->trianglesVertices.size() / 3;

    for (int i = 0; i < trianglesCount; i++)
    {
        this->trianglesColors.push_back(getRandomColor());
    }
}

std::vector<CRTTriangle> CRTMesh::getTriangles()
{
    std::vector<CRTTriangle> triangles;
    for(int i = 0; i < trianglesVertices.size(); i+=3)
    {
        auto t = CRTTriangle(
            vertices[trianglesVertices[i]],
            vertices[trianglesVertices[i + 1]],
            vertices[trianglesVertices[i + 2]], 
            trianglesColors[i / 3]);
        triangles.push_back(t);
    }
    return triangles;
}