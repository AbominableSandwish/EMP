#include <iostream>
#include <string>
#include "engine/engine.h"
#include "engine/config.h"

int main() {


	
    emp::ConfigEngine* config = new emp::ConfigEngine("Mushroom Engine");
    emp::Engine engine = emp::Engine();
    engine.Init(config);
    engine.Start();
		
    exit(EXIT_SUCCESS);
}
