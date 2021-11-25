/*
 * Author: Dylan von Arx
 * Time: 02.10.2021
*/
#include <string>
#pragma once

#include <core/singleton.h>

namespace emp
{
    class Engine;
    using namespace std;
    class System
    {
    public:
        System(){}
        System(Engine&, string);
        virtual void Init() = 0;
        virtual void Update(float) = 0;
        virtual void Destroy() = 0;
    	virtual ~System() = default;
    protected:
        string name;
        Engine* engine;
    	
    };
}