#include <iostream>
#include <core/engine.h>
#include<thread>
#include <ctime>
#include <graphic/graphic.h>
#include <core/log.h>
#include <editor/editor.h>
#include <core/file.h>
#include "core/config.h"
#include <core/component.h>


class EngineCustom: public emp::Engine
{
};

float timer = 0.0f;
constexpr int WIDTH = 1000000;

class Test
{
public:
    std::string name;
    emp::Mode mode;
    emp::ConfigEngine* config;
    bool success = false;
    bool fail = false;

	Test()
	{
		
	}

    virtual void Check(EngineCustom& engine)
    {
	    
    };

	void Success()
	{
        success = true;
	}

	void Fail()
	{
        fail = true;
	}
};

class TestConsole :  public Test
{
public:

	TestConsole()
	{
        name = "Engine Mode Console";
        mode = emp::Mode::Empty;
        config = new emp::ConfigEngine("Mushroom Engine", mode);
	}

    void Check(EngineCustom& engine) override
    {
        if (engine.is_running)
        {
            Success();
        }
		else
        {
            Fail();
        }
    }
};

class TestLauncher : public Test
{
public:

    TestLauncher()
    {
        name = "Engine Mode Launcher";
        mode = emp::Mode::Launcher;
        config = new emp::ConfigEngine("Mushroom Engine", mode);
    }

    void Check(EngineCustom& engine) override
    {
        if (engine.is_running)
        {
            Success();
        }
        else
        {
            Fail();
        }
    }
};

class TestLogger : public Test
{
public:
    TestLogger()
	{
        name = "TEST logger";
        mode = emp::Mode::Editor;
        config = new emp::ConfigEngine("Mushroom Engine", mode);
	}
	
    void Check(EngineCustom& engine) override
    {
        if (engine.GetLogManager()->GetLogs().size() > 1000)
        {
            if (engine.GetFPS() < 120) {
                Success();
            }
            else
            {
                Fail();
            }
        }else
        {
            emp::LOG::Info("Test log");
        }
       
    }
};


class TestEditor : public Test
{
public:
    TestEditor()
    {
        name = "TEST Engine mode Editor";
        mode = emp::Mode::Editor;
        config = new emp::ConfigEngine("Mushroom Engine", mode);
    }

    void Check(EngineCustom& engine) override
    {
        if (engine.is_running)
        {
            Success();
        }
        else
        {
            Fail();
        }
    }
};



int max_value(int arr[], int size) {
    int max = arr[0];

    for (int j = 0; j < size; ++j) {
        if (arr[j] > max) {
            max = arr[j];
        }
    }
    return max;
}

int main()
{
    list<Test*> tests;
    tests.push_back(new TestConsole());
    tests.push_back(new TestLauncher());
    tests.push_back(new TestEditor());
    tests.push_back(new TestLogger());
	
	std::cout << "Launching of the test manager\n";
    std::cout << "Number of test: "<< tests.size() <<"\n\n";
    std::cout << "\033[34m====================================================================================\033[39m\n";
  
    bool global_success = true;

    int i = 0;
    for (Test* test : tests)
    {
        std::cout << "\033[34m[ TEST ]\033[39m Test "<< std::to_string(i) <<": " << test->name << "\n";
    	
        clock_t start, end;
        int max;
        int* arr = new int[WIDTH];
        std::srand(std::time(nullptr));
        for (size_t i = 0; i < WIDTH; i++) {
            arr[i] = std::rand();
        }

        EngineCustom engine = EngineCustom();
        engine.Init(test->config);
        engine.Start();
        while (engine.is_running && !test->success && !test->fail)
        {
            start = clock();
            end = clock();
            float dt = float(start - end);

            engine.Update();

            test->Check(engine);

            start = clock();
            max = max_value(arr, WIDTH);
            end = clock();
        }
        engine.Stop();
        while (engine.is_running)
        {
	        
        }
    	
        if (test->success)
        {
            std::cout << "\n\033[34m[ TEST ]\033[39m Test " << std::to_string(i) << ": \033[32m[SUCCESS]\033[39m\n";
        }
        else {
            std::cout << "\n\033[34m[ TEST ]\033[39m Test " << std::to_string(i) << ": \033[31m[FAIL]\033[39m\n";
            global_success = false;
        }
        i++;
        std::cout << "\033[34m====================================================================================\033[39m\n";
    }
	
    std::cout << "\nResult of the tests\n";
    i = 0;
    for (Test* test : tests)
    {
        if (test->success)
        {
            std::cout << "\033[34m[ TEST ]\033[39m Test " << std::to_string(i) << ": \033[32m[SUCCESS]\033[39m\n";
        }
        else {
            std::cout << "\033[34m[ TEST ]\033[39m Test " << std::to_string(i) << ": \033[31m[FAIL]\033[39m\n";
        }
        i++;
    }
	
    if (global_success)
    {
        std::cout << "\033[34m[ TEST ]\033[39m Global Test: \033[32m[SUCCESS]\033[39m\n";
    }
    else {
        std::cout << "\033[34m[ TEST ]\033[39m Global Test: \033[31m[FAIL]\033[39m\n";
    }

	

    exit(EXIT_SUCCESS);
}

