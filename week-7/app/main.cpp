#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h> // srand, rand
#include <time.h> // time 

#include "..\include\crtrenderer.h"

std::string sstr(int i)
{
	return static_cast<std::ostringstream &>(
			   (std::ostringstream() << std::dec << i)).str();
}

int main(void)
{
	srand(42);

	for (int i = 0; i <= 0; i++)
	{
		clock_t begin = clock();

		CRTScene scene = CRTScene();
		//scene.parseSceneFile("D:\\Repositories\\Git-mine\\ray-tracing-course\\week-7\\scenes\\scene"+ sstr(i) +".crtscene");
		scene.parseSceneFile("D:\\Repositories\\Git-mine\\ray-tracing-course\\week-8-9\\scenes\\week-8\\scene"+ sstr(i) +".crtscene");
				
		CRTRenderer::RenderImage("scene" + sstr(i) + ".ppm", scene);

		clock_t end = clock();
		
		float elapsed_secs = float(end - begin) / CLOCKS_PER_SEC;
		std::cout << "Elapsed time for rendering 'scene" + sstr(i) +"' in seconds: " << elapsed_secs << " sec\n";
	}
	/*
	// for images 400 x 200
	Elapsed time for rendering 'scene0' in seconds: 0.237 sec
	Elapsed time for rendering 'scene1' in seconds: 1.49 sec
	Elapsed time for rendering 'scene2' in seconds: 1.193 sec
	Elapsed time for rendering 'scene3' in seconds: 5.071 sec
	Elapsed time for rendering 'scene4' in seconds: 135.414 sec
	*/
}