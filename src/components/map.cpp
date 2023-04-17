//#include <components/map.h>
//#include <core/engine.h>
//#include <core/config.h>
//#include "core/component.h"
//#include <graphic/graphic.h>
//#include <components/transform.h>
//#include <components/light.h>
//#include <components/camera.h>
//#include <math/matrice.h>
//#include <graphic/shader.h>
//#include <core/file.h>
//
//namespace emp {
//    int numX = 512,
//        numY = 512,
//        numOctaves = 1;
//    double persistence = 0.3;
//
// const int heiht_map = 4;
//
//#define maxPrimeIndex 10
//    int primeIndex = 0;
//
//    int primes[maxPrimeIndex][3] = {
//      { 995615039, 600173719, 701464987 },
//      { 831731269, 162318869, 136250887 },
//      { 174329291, 946737083, 245679977 },
//      { 362489573, 795918041, 350777237 },
//      { 457025711, 880830799, 909678923 },
//      { 787070341, 177340217, 593320781 },
//      { 405493717, 291031019, 391950901 },
//      { 458904767, 676625681, 424452397 },
//      { 531736441, 939683957, 810651871 },
//      { 997169939, 842027887, 423882827 }
//    };
//
//    double Noise(int i, int x, int y) {
//        int n = x + y * 57;
//        n = (n << 13) ^ n;
//        int a = primes[i][0], b = primes[i][1], c = primes[i][2];
//        int t = (n * (n * n * a + b) + c) & 0x7fffffff;
//        return 1.0 - (double)(t) / 1073741824.0;
//    }
//
//    double SmoothedNoise(int i, int x, int y) {
//        double corners = (Noise(i, x - 1, y - 1) + Noise(i, x + 1, y - 1) +
//            Noise(i, x - 1, y + 1) + Noise(i, x + 1, y + 1)) / 16,
//            sides = (Noise(i, x - 1, y) + Noise(i, x + 1, y) + Noise(i, x, y - 1) +
//                Noise(i, x, y + 1)) / 8,
//            center = Noise(i, x, y) / 4;
//        return corners + sides + center;
//    }
//
//    double Interpolate(double a, double b, double x) {  // cosine interpolation
//        double ft = x * 3.1415927,
//            f = (1 - cos(ft)) * 0.5;
//        return  a * (1 - f) + b * f;
//    }
//
//    double InterpolatedNoise(int i, double x, double y) {
//        int integer_X = x;
//        double fractional_X = x - integer_X;
//        int integer_Y = y;
//        double fractional_Y = y - integer_Y;
//
//        double v1 = SmoothedNoise(i, integer_X, integer_Y),
//            v2 = SmoothedNoise(i, integer_X + 1, integer_Y),
//            v3 = SmoothedNoise(i, integer_X, integer_Y + 1),
//            v4 = SmoothedNoise(i, integer_X + 1, integer_Y + 1),
//            i1 = Interpolate(v1, v2, fractional_X),
//            i2 = Interpolate(v3, v4, fractional_X);
//        return Interpolate(i1, i2, fractional_Y);
//    }
//
//    double ValueNoise_2D(double x, double y) {
//        double total = 0,
//            frequency = pow(2, numOctaves),
//            amplitude = 1;
//        for (int i = 0; i < numOctaves; ++i) {
//            frequency /= 0.7f;
//            amplitude *= persistence;
//            total += InterpolatedNoise((primeIndex + i) % maxPrimeIndex,
//                x / frequency, y / frequency) * amplitude;
//        }
//        return total / frequency;
//    }
//
//    float velocity = 0.01f;
//    glm::vec3 offset;
//    int seed;
//    Map::Map(int entity, float r, float g, float b)
//    {
//        this->entity = entity;
//        this->color = glm::vec4(r, g, b, 1.0f);
//    }
//    //CUBE
//    void Map::Init()
//    {
//
//    }
//
//    //CUBEMANAGER
//    MapManager::MapManager(Engine& engine, ConfigGraphic& config) : System(engine, "CubeManager")
//    {
//        this->config = &config;
//        m_component = engine.GetComponentManager();
//    }
//
//    void MapManager::Init()
//    {
//        bool warning = false;
//        m_component = engine->GetComponentManager();
//
//        {
//          
//        }
//
//    }
//    
//    void LoadChunck(int x, int y) {
//        seed = std::rand() / 500;
//        int size = heiht_map;
//        int level[heiht_map][heiht_map] = { 0 };
//        int turn = 8;
//        for (int t = 0; t < turn; t++) {
//            for (int i = 0; i < size; i++) {
//                for (int j = 0; j < size; j++) {
//                    int up = seed;
//                    level[i][j] += up;
//                    double noise = ValueNoise_2D(i + x, j + y);
//                    level[i][j] = noise * 10000;
//
//                }
//            }
//        }
//    }
//
//    void MapManager::Start() {
//      
//        seed = std::rand() / 500;
//        int size = heiht_map;
//        int level[heiht_map][heiht_map] = { 0 };
//        int turn = 8;
//        for (int t = 0; t < turn; t++) {
//            for (int i = 0; i < size; i++) {
//                for (int j = 0; j < size; j++) {
//                    int up = seed;
//                    level[i][j] += up;
//                    double noise = ValueNoise_2D(i + x, j + y);
//                    level[i][j] = noise * 10000;
//
//                }
//            }
//        }
//
//        this->array = new  std::vector<Transform>();
//        for (int i = 0; i < size; i++) {
//            for (int j = 0; j < size; j++) {
//
//                this->array->push_back(Transform(i * 300 - (size * 300 / 2), -1100 + level[i][j], j * 300 - (size * 300 / 2), 0, 0, 0, 3.0f, 3.0f, 3.0f));
//            }
//        }
//    }
//
//    int move_x = 0;
//    void MapManager::Refresh() {
//        int size = heiht_map;
//        int level[heiht_map][heiht_map] = { 0 };
//        int turn = 8;
//        for (int t = 0; t < turn; t++) {
//            for (int i = 0; i < size; i++) {
//                for (int j = 0; j < size; j++) {
//                    int up = seed;
//                    level[i][j] += up;
//                    double noise = ValueNoise_2D(i + offset.x, j);
//                    level[i][j] = noise * 10000;
//                }
//            }
//        }
//
//        int i = 0;
//        for (std::vector<Transform>::iterator it = array->begin(); it < array->end(); it++)
//        {
//            int x = i % heiht_map;
//            int y = i / heiht_map;
//            glm::vec3 pos = glm::vec3(x * 300 - (size * 300 / 2), -700 + level[x][y], y * 300 - (size * 300 / 2));
//            it->SetPosition(pos);
//            i++;
//        }
//    }
//
//    void MapManager::Destroy()
//    {
//
//    }
//
//    void MapManager::Update(float dt)
//    {
//        time += dt;
//        offset.x = time * 15;
//    }
//
//    void MapManager::Draw()
//    {
//        Refresh();
//    }
//}