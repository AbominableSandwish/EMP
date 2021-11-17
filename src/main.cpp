#include <iostream>
#include <string>
#include "core/engine.h"
#include "engine/config.h"

int main() {
	emp::Mode mode = emp::Mode::Editor;
	
    emp::ConfigEngine* config = new emp::ConfigEngine("Mushroom Engine", mode);
    emp::Engine engine = emp::Engine();
    engine.Init(config);
    engine.Start();
		
    exit(EXIT_SUCCESS);
}
