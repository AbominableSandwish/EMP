/*
 * Author: Dylan von Arx
 * Time: 02.10.2021
*/
#include <string>
#pragma once

#include <cassert>
#include <memory>
#include <unordered_map>
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
	
	class SystemManager
	{
	public:
        template<typename T>
        std::shared_ptr<T> RegisterSystem()
        {
            const char* typeName = typeid(T).name();

            //assert(systems.find(typeName) == systems.end() && "Registering system more than once.");
        	
            auto system = std::make_shared<T>();
            systems.insert({ typeName, system });
            return system;
        }
		
        template<typename T>
        std::shared_ptr<T> RegisterSystem(Engine& engine, string name)
        {
            const char* typeName = typeid(T).name();

            //assert(systems.find(typeName) == systems.end() && "Registering system more than once.");

            auto system = std::make_shared<T>(engine, name);
            systems.insert({ typeName, system });
            return system;
        }
		
    private:
        // Map from system type string pointer to a system pointer
        std::unordered_map<const char*, std::shared_ptr<System>> systems;
	};
}
