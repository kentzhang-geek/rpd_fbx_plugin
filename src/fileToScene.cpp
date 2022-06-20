//
// Created by Admin on 2022/6/18.
//

#include "scene.h"
#include "fbxsdk.h"
#include "fileToScene.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include "ParseNode.h"


std::unique_ptr<SceneT> fileToFBSScene(std::string filename, flatbuffers::FlatBufferBuilder & builder) {
    FbxManager *lSdkManager = FbxManager::Create();

    // Create the IO settings object.
    FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
    lSdkManager->SetIOSettings(ios);

    // Create an importer using the SDK manager.
    FbxImporter *lImporter = FbxImporter::Create(lSdkManager, "");

    // Use the first argument as the filename for the importer.
    if (!lImporter->Initialize(filename.c_str(), -1, lSdkManager->GetIOSettings())) {
        printf("Call to FbxImporter::Initialize() failed.\n");
        printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
        exit(-1);
    }

    // Create a new scene so that it can be populated by the imported file.
    FbxScene* lScene = FbxScene::Create(lSdkManager,"myScene");

    // Import the contents of the file into the scene.
    lImporter->Import(lScene);

    // Default use triangulate
    FbxGeometryConverter converter(lSdkManager);
    bool cret = converter.Triangulate(lScene, true, false);

    // convert to flatbuffers
    auto scene = new SceneT();
    scene->root = ParseNode(lScene->GetRootNode());

    lScene->Destroy();

    // The file is imported, so get rid of the importer.
    lImporter->Destroy();

    return std::unique_ptr<SceneT>(scene);
}
