#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h> // srand, rand
#include <time.h> // time 

#include "..\include\crtrenderer.h"

std::string sstr(int i)
{
	return static_cast<std::ostringstream &>(
			(std::ostringstream() << std::dec << i))
		.str();
}

int main(int argc, char** argv)
{
	srand(42);

	assert(argc == 2);

	for (int i = 0; i <= 2; i++)
	{
		clock_t begin = clock();

		CRTScene scene = CRTScene();
		scene.parseSceneFile(argv[1] + sstr(i) +".crtscene");
		
		CRTRenderer::RenderImage("scene" + sstr(i) + ".ppm", scene);

		clock_t end = clock();
		
		float elapsed_secs = float(end - begin) / CLOCKS_PER_SEC;
		std::cout << "Elapsed time for rendering 'scene" + sstr(i) +"' in seconds: " << elapsed_secs << " sec\n";
	}
}