#include "engine/engine.h"
#include <iostream>
#include <ctime>
#include <engine/graphic.h>
#include <editor/editor.h>


namespace emp
{
    constexpr int WIDTH = 1000000;

    int max_index(int arr[], int size) {
        size_t max = 0;

        for (int j = 0; j < size; ++j) {
            if (arr[j] > arr[max]) {
                max = j;
            }
        }
        return arr[max];
    }

    int max_value(int arr[], int size) {
        int max = arr[0];

        for (int j = 0; j < size; ++j) {
            if (arr[j] > max) {
                max = arr[j];
            }
        }
        return max;
    }
	
	Engine::Engine() = default;

    void Engine::Init(ConfigEngine* config)
	{
        std::cout << "[ ... ] Loading configuration\n";
        this->m_Config = config;

        this->m_Graphic = new GraphicManager();
        this->m_Graphic->Init();

        this->m_Editor = new emp::ToolManager();
        this->m_Editor->Init(*this);

        this->is_running = true;
	}

	void Engine::Start()
	{
        clock_t start, end;
        int max;
        int* arr = new int[WIDTH];
        std::srand(std::time(nullptr));
        for (size_t i = 0; i < WIDTH; i++) {
            arr[i] = std::rand();
        }

        while (this->is_running)
        {
            start = clock();
            end = clock();
            float dt = start - end;
        	
            this->m_Graphic->Update(dt);
            this->m_Graphic->Draw();
            this->m_Editor->Draw();

            start = clock();
            max = max_value(arr, WIDTH);
            end = clock();
            printf("max_value: %0.3f sec, max = %d, Timer = %d\n",
                ((float)end - start) / CLOCKS_PER_SEC, max, clock() / 1000);
        }
	}


	void Engine::Destroy()
	{
		
	}

    GLFWwindow* Engine::GetWindow()
    {
        return this->m_Graphic->GetWindow();
    }
}

/*include  datetime import datetime
from Scripts.Color import bcolors
from Scripts.SystemManager import SystemManager


class Engine :

    _now = datetime.now()
    _current_time = _now.strftime("%H:%M:%S")
    _start_time = str(_now.today())

    def __init__(self, config) :
  
    return

    def main(self) :
    if self._run :
        self._Manager.__update__()
        return

        def shutdown(self) :
        print("[ ... ] Shutdown Engine")
        self._run = False
        self._Manager.__shutDown__()

        def GetSystem(self, number) :
        return self._Manager.GetSystem(number)

        #order = input("[ Enter for exit ] : ")
#if order == "":
        # Quit
        #self.__shutDown__()
        #self._engine._run = False
        #return
        */

