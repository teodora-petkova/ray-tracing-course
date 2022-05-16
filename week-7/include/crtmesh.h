#pragma once

#include<vector>

#include "crttriangle.h"

class CRTMesh
{
public:
    CRTMesh(const std::vector<CRTVector> &vertices,
            const std::vector<int> &trianglesVertices);

    std::vector<CRTTriangle> getTriangles();

private:
    std::vector<CRTVector> vertices;
    std::vector<int> trianglesVertices;
    std::vector<CRTColor> trianglesColors;
};