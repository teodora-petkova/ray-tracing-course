#include <iostream>
#include <stdio.h>
#include <stdlib.h> // srand, rand
#include <time.h> // time 

#include "..\include\crtrenderer.h"

void raytrace(const char* sceneFilepath)
{
	for (int i = 0; i <= 5; i++)
	{
		clock_t begin = clock();

		std::string filepath = sceneFilepath + std::to_string(i) + ".crtscene";
		std::cout << "Rendering scene: " << filepath << ".\n";

		CRTScene scene = CRTScene();
		scene.parseSceneFile(filepath);
		
		CRTRenderer::RenderImage("scene" + std::to_string(i) + ".ppm", scene);

		clock_t end = clock();
		
		float elapsed_secs = float(end - begin) / CLOCKS_PER_SEC;
		std::cout << "Elapsed time for rendering 'scene" + std::to_string(i) +"' in seconds: " << elapsed_secs << " sec\n";
	}
}

int main(int argc, char** argv)
{
	srand(42);

	assert(argc == 2);
	
	raytrace(argv[1]);
}