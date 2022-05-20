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
    const std::vector<int>& trianglesVertices, int materialIndex)
{
    int trianglesCount = trianglesVertices.size() / 3;

    std::vector<CRTColor> trianglesColors;
    for (int i = 0; i < trianglesCount; i++)
    {
        trianglesColors.push_back(getRandomColor());
    }
    this->materialIndex = materialIndex;

    std::vector<CRTVector> normals(vertices.size());

    // create vertex normals
    // to get the vertex normal vector, simply sum up all the face normals for that vertex
    // after than normalize the calculated vector
    for(int i = 0; i < trianglesVertices.size(); i+=3)
    {
        int vi1 = trianglesVertices[i];
        int vi2 = trianglesVertices[i + 1];
        int vi3 = trianglesVertices[i + 2];
        
        CRTVector v1 = vertices[vi1];
        CRTVector v2 = vertices[vi2];
        CRTVector v3 = vertices[vi3];

        CRTVector normal = (v2 - v1).cross(v3 - v2).normalize();
    
        normals[vi1] += normal;
        normals[vi2] += normal;
        normals[vi3] += normal;
    }

    // create triangles based on vertices with vertex normals
    for(int i = 0; i < trianglesVertices.size(); i+=3)
    {
        int vi1 = trianglesVertices[i];
        int vi2 = trianglesVertices[i + 1];
        int vi3 = trianglesVertices[i + 2];

        auto t = CRTTriangle(
            vertices[vi1],
            vertices[vi2],
            vertices[vi3], 
            normals[vi1].normalize(),
            normals[vi2].normalize(),
            normals[vi3].normalize(),
            trianglesColors[i / 3]);

        this->triangles.push_back(t);
    }
}