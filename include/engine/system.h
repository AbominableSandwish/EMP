/*
 * Author: Dylan von Arx
 * Time: 02.10.2021
*/
#include <string>

using namespace std;
namespace emp
{
    template<typename T>
    class System
    {
    public:
        virtual void Init() = 0;
        virtual void Update(float) = 0;
        virtual void Destroy() = 0;
    protected:
        virtual ~System() {};
        string name;
    };
}