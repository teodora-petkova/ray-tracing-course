#include "..\include\crtscene.h"

using namespace rapidjson;

CRTVector loadVector(const Value::ConstArray& arr)
{
    assert(arr.Size() == 3);

    CRTVector vec{
        static_cast<float>(arr[0].GetDouble()),
        static_cast<float>(arr[1].GetDouble()),
        static_cast<float>(arr[2].GetDouble())
    };

    return vec;
}

CRTMatrix loadMatrix(const Value::ConstArray& arr)
{
    assert(arr.Size() == 9);

    CRTMatrix matrix{
        static_cast<float>(arr[0].GetDouble()), static_cast<float>(arr[1].GetDouble()), static_cast<float>(arr[2].GetDouble()),
        static_cast<float>(arr[3].GetDouble()), static_cast<float>(arr[4].GetDouble()), static_cast<float>(arr[5].GetDouble()),
        static_cast<float>(arr[6].GetDouble()), static_cast<float>(arr[7].GetDouble()), static_cast<float>(arr[8].GetDouble()),
    };

    return matrix;
}

std::vector<CRTVector> loadVectors(const Value::ConstArray& arr)
{
    assert(arr.Size() % 3 == 0);

    std::vector<CRTVector> vectors;

    for (Value::ConstValueIterator p = arr.Begin(); p != arr.End(); p+=3)
    {
        CRTVector vec{
            static_cast<float>(p[0].GetDouble()),
            static_cast<float>(p[1].GetDouble()),
            static_cast<float>(p[2].GetDouble())
        };
        vectors.push_back(vec);
    }

    return vectors;
}

std::vector<int> loadIndices(const Value::ConstArray& arr)
{
    assert(arr.Size() % 3 == 0);

    std::vector<int> indices;

    for (auto& i : arr)
    {
        indices.push_back(i.GetInt());
    }

    return indices;
}

Document getJsonDocument(const std::string& filename)
{
    std::ifstream ifs(filename);
    assert(ifs.is_open());

    IStreamWrapper isw(ifs);
    Document doc;
    doc.ParseStream(isw);

    if(doc.HasParseError()) 
    {
        std::cout << "Error : " << doc.GetParseError() << "\n";
        std::cout << "Offset : " << doc.GetErrorOffset() << "\n";
        assert(false);
    }

    assert(doc.IsObject());
    return doc;
}

void CRTScene:: parseSceneFile(const std::string &sceneFileName)
{
    Document doc = getJsonDocument(sceneFileName);

    const Value &settingsVal = doc.FindMember(crtSceneSettings)->value;
    
    // image settings
    if(!settingsVal.IsNull() && settingsVal.IsObject())
    {
        const Value &bgColorVal = settingsVal.FindMember(crtSceneBGColor)->value;
        assert(!bgColorVal.IsNull() && bgColorVal.IsArray());
        settings.backgroundColor = CRTColor(loadVector(bgColorVal.GetArray()));

        const Value &imageSettingsVal = settingsVal.FindMember(crtSceneImageSettings)->value;
        if(!imageSettingsVal.IsNull() && imageSettingsVal.IsObject())
        {
            const Value &imageWidthVal = imageSettingsVal.FindMember(crtSceneImageWidth)->value;
            const Value &imageHeightVal = imageSettingsVal.FindMember(crtSceneImageHeight)->value;

            assert(!imageWidthVal.IsNull() && imageWidthVal.IsInt() &&
                !imageHeightVal.IsNull() && imageHeightVal.IsInt());

            settings.imageSettings.width = imageWidthVal.GetInt();
            settings.imageSettings.height = imageHeightVal.GetInt();

            camera.fillImageSettings(settings.imageSettings.width, settings.imageSettings.height);
        }
    
        const Value &albedoVal = settingsVal.FindMember(crtSceneGlobalAlbedo)->value;
        if(!albedoVal.IsNull() && albedoVal.IsArray())
        {
            settings.globalAlbedo = CRTColor(loadVector(albedoVal.GetArray()));
        }
    }

    // camera rotation matrix and position
    const Value &cameraVal = doc.FindMember(crtSceneCamera)->value;
    if(!cameraVal.IsNull() && cameraVal.IsObject())
    {
        const Value &matrixVal = cameraVal.FindMember(crtSceneMatrix)->value;
        assert(!matrixVal.IsNull() && matrixVal.IsArray());
        camera.setRotationMatrix(loadMatrix(matrixVal.GetArray()));

        const Value &positionVal = cameraVal.FindMember(crtScenePosition)->value;
        assert(!positionVal.IsNull() && positionVal.IsArray());
        camera.setPosition(loadVector(positionVal.GetArray()));
    }

    // objects
    const Value &objectsVal = doc.FindMember(crtObjects)->value;
    if(!objectsVal.IsNull() && objectsVal.IsArray())
    {
        for (auto& objectVal : objectsVal.GetArray())
        {
            const Value &verticesVal = objectVal.FindMember(crtVertices)->value;
            assert(!verticesVal.IsNull() && verticesVal.IsArray());
            auto vertices = loadVectors(verticesVal.GetArray());

            const Value &trianglesVal = objectVal.FindMember(crtTriangles)->value;
            assert(!trianglesVal.IsNull() && trianglesVal.IsArray());
            auto trianglesVertices = loadIndices(trianglesVal.GetArray());

            auto geometryObject = CRTMesh(vertices, trianglesVertices);
            geometryObjects.push_back(geometryObject);
        }
    }

    // lights
    const Value &lightsVal = doc.FindMember(crtLights)->value;
    if(!lightsVal.IsNull() && lightsVal.IsArray())
    {
        for (auto& lightVal : lightsVal.GetArray())
        {
            const Value &intensityVal = lightVal.FindMember(crtLightIntensity)->value;
            assert(!intensityVal.IsNull() && intensityVal.IsInt());
            int lightIntensity = intensityVal.GetInt();

            const Value &positionVal = lightVal.FindMember(crtLightPosition)->value;
            assert(!positionVal.IsNull() && positionVal.IsArray());
            CRTVector lightPosition = loadVector(positionVal.GetArray());

            auto light = CRTLight(lightIntensity, lightPosition);
            lights.push_back(light);
        }
    }
}