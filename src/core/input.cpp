#include "core/input.h"
#include <algorithm>
#include <graphic/graphic.h>
#include <iostream>
#include <core/engine.h>
#include <sdl.h>
#include <core/log.h>


namespace emp {
  InputSystem::InputSystem() { instance_input = this; }

  InputSystem::InputSystem(Engine& engine, string name) : System(engine, name) { instance_input = this; }

    void InputSystem::Init()
    {
    }

    void InputSystem::Start()
    {
    }

    void InputSystem::Destroy()
    {
    }

    void InputSystem::Update(float)
    {
        //Event handler
        SDL_Event e;
        //Handle events on queue
        while (SDL_PollEvent(&e) != 0)
        {
            switch (e.type)
            {
            case SDL_QUIT:
                exit(0);
                break;
            case SDL_WINDOWEVENT:
                if (e.window.event == SDL_WINDOWEVENT_RESIZED)
                {

                    int w, h;
                    SDL_GetWindowSize(this->engine->GetGraphicManager()->window, &w, &h);
                    LOG::Info("Resizing window w: " + std::to_string(w) + " h: " + std::to_string(h) + "\n");
                    this->engine->GetGraphicManager()->SetSizeWindow(w, h);
                   

                }
                break;
                //User presses a key
            case SDL_KEYDOWN:
            {
                //Select surfaces based on key press
                switch (e.key.keysym.sym)
                {
                case SDLK_UP:
                    isPress_up = true;
                    break;

                case SDLK_DOWN:
                    isPress_down = true;
                    break;

                case SDLK_LEFT:
                    isPress_left = true;
                    break;

                case SDLK_RIGHT:
                    isPress_right = true;
                    break;

                case SDLK_SPACE:
                    isPress_space = true;
                    break;

                case SDL_SCANCODE_KP_ENTER:
                    isPress_enter = true;
                    break;

                case SDLK_LCTRL:
                    isPress_ctrl_l = true;
                    break;

                case SDLK_ESCAPE:
                    isPress_escape = true;
                    break;
                case SDLK_s:
                    isPress_s = true;
                    if (isPress_ctrl_l) {
                        LOG::Info("Saving Project...");
                        LOG::Info("Entities : " + std::to_string(54));
                        LOG::Info("components : " + std::to_string(147));
                        LOG::Info("Size Data : " + std::to_string(1.42255f) + " Mo");
                        LOG::Info("Project Save : MyFirstProject.data");
                    }
                    break;

                default:
                    break;
                }
            }
            break;

            //User presses a key
            case  SDL_KEYUP:
            {
                //Select surfaces based on key press
                switch (e.key.keysym.sym)
                {
                case SDLK_UP:
                    isPress_up = false;
                    break;

                case SDLK_DOWN:
                    isPress_down = false;
                    break;

                case SDLK_LEFT:
                    isPress_left = false;
                    break;

                case SDLK_RIGHT:
                    isPress_right = false;
                    break;

                case SDLK_SPACE:
                    isPress_space = false;
                    break;

                case SDL_SCANCODE_KP_ENTER:
                    isPress_enter = false;
                    break;

                case SDLK_LCTRL:
                    isPress_ctrl_l = false;
                    break;

                case SDLK_ESCAPE:
                    isPress_escape = false;
                    break;

                case SDLK_s:
                    isPress_s = false;
                    break;

                default:
                    break;
                }
            }
            break;
            }
        }

        //LOG::Error(std::to_string(GetCounterKeyPress()));
    }
    

    InputSystem* InputSystem::GetInstance()
    {
        if (instance_input == nullptr)
        {
            instance_input = new InputSystem();
        }
        return instance_input;
    }

}