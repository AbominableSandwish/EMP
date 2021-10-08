#include <iostream>
#include <string>
#include "engine/engine.h"
#include "engine/config.h"
#include "GL/glew.h"

int main() {


	
    mushgine::ConfigEngine* config = new mushgine::ConfigEngine("Mushroom Engine");
    mushgine::Engine engine = mushgine::Engine();
    engine.Init(config);
    engine.Start();
	
    exit(EXIT_SUCCESS);
}
