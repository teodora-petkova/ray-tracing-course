#pragma once

#include <fstream>
#include <iostream>

#include "rapidjson\document.h"
#include "rapidjson\writer.h"
#include "rapidjson\stringbuffer.h"
#include "rapidjson\istreamwrapper.h"

#include "crtsettings.h"
#include "crtmesh.h"
#include "crtcamera.h"

class CRTScene 
{
public:
    CRTScene() {}//const std::string &sceneFileName);

    const std::vector<CRTMesh> &getGeometryObjects() const { return geometryObjects; }
    const CRTCamera &getCamera() const { return camera; }
    const CRTSettings &getSettings() const { return settings; }

    /// Read the content of the given file and store the data into scene members
    /// @param[in] sceneFileName = the scene file to read
    void parseSceneFile(const std::string &sceneFileName);

private:
    std::vector<CRTMesh> geometryObjects;
    CRTCamera camera;
    CRTSettings settings;

    const char *crtSceneSettings = "settings";
    const char *crtSceneBGColor = "background_color";
    const char *crtSceneImageSettings = "image_settings";
    const char *crtSceneImageWidth = "width";
    const char *crtSceneImageHeight = "height";

    const char *crtSceneCamera = "camera";
    const char *crtSceneMatrix = "matrix";
    const char *crtScenePosition = "position";

    const char *crtObjects = "objects";
    const char *crtVertices = "vertices";
    const char *crtTriangles = "triangles";
};