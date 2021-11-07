#include <iostream>
#include <engine/engine.h>
#include <engine/config.h>
#include<thread>


emp::Engine engine = emp::Engine();

float timer = 0.0f;

void Engine()
{
    emp::ConfigEngine* config = new emp::ConfigEngine("Mushroom Engine");
    engine.Init(config);
    engine.Start();
}

int main()
{
	std::cout << "Beginning of the tests\n";
   
    emp::ConfigEngine* config = new emp::ConfigEngine("Mushroom Engine");
    engine.Init(config);
    engine.Start();
    
    while (timer < 1000.0f) {
        timer += clock();
        std::cout << timer << std::endl;
    }
    engine.Stop();
    std::cout << "Ending of the tests\n";

    exit(EXIT_SUCCESS);
}

