#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

#include "./include/raylib.h"
#include "./include/raymath.h"
#include "./include/rlgl.h"
#include "./Window.h"

// Vector3 math operations for ease of use
#include "../util/V3ops.h"

class Cube {
    private:
        Vector3 position;
        Vector3 size;
        bool _current = false;
        Mesh mesh;
        Model model;
        BoundingBox bounding;
        Color col;
    public:
        Cube(Vector3 pos, Vector3 siz, Color c) {
            position = pos;
            size = siz;
            col = c;
            Calculate();
        }
        void Calculate() {
            mesh = GenMeshCube(size.x, size.y, size.z);
            GenMeshTangents(&mesh);
            model = LoadModelFromMesh(mesh);
            bounding = GetModelBoundingBox(model);
            bounding.min += position;
            bounding.max += position;
            _current = true;
        }
        Vector3 getPosition() { return position; };
        Vector3 getSize() { return size; };
        void setPosition(Vector3 pos) { 
            position = pos; 
            _current = false; 
            Calculate(); 
        };
        void setSize(Vector3 siz) { 
            size = siz; 
            _current = false; 
            Calculate(); 
        };
        void Render(bool lines = false, Color lc = BLACK, float margin = .1f) {
            DrawModelEx(model, position, Vector3 {0.0f, 0.0f, 0.0f}, 0.0f, size, col);
            if(lines) DrawModelWiresEx(model, position, Vector3 { 0.0f, 0.0f, 0.0f }, 0.0f, Vector3 {
                size.x + margin, size.y + margin, size.z + margin
            }, lc);
            DrawBoundingBox(bounding, ORANGE);
        }
        bool RayHit(Ray ray) {
            return GetRayCollisionBox(ray, bounding).hit;
        }
        template <typename C>
        static void RenderCubes(std::vector<C> cubes, bool lines = false, Color lc = BLACK, float margin = .1f) {
            for(int i = 0; i < cubes.size(); i++) {
                cubes[i]->Render(lines, lc, margin);
            }
        }
};

float V3Distance(Vector3 a, Vector3 b) {
    return std::sqrt(std::pow(a.x - b.x, 2.0f) + std::pow(a.y - b.y, 2.0f) + std::pow(a.z - b.z, 2.0f));
}

void VisualizeRay(Ray r, float len) {
    // ! https://answers.unity.com/questions/887344/how-do-i-move-a-vector3-a-distance-to-a-direction.html
    Vector3 newSpot = r.position + (Vector3Normalize(r.direction) * Vector3 { len, len, len });
    DrawLine3D(r.position, newSpot, RED);
}

int main(int argc, char** args) {

    const int WIDTH = 800;
    const int HEIGHT = 640;  

    Window window("This is the title", 800, 640);

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);

    int gridSize = 5;
    int gridHeight = 5;

    Vector3 size = {1.0f, 1.0f, 1.0f};
    Color colors[8] = {RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE, PINK, BROWN};

    std::vector<Cube*> cubes = {};
    for(float y = 0.0f; y < gridHeight; y += 1.0f) {
        for(float x = 0.0f; x < gridSize; x += 1.0f) {
            for(float z = 0.0f; z < gridSize; z += 1.0f) {
                Cube* c = new Cube(
                    Vector3 {x, y, z},
                    Vector3 {size.x, size.y, size.z},
                    colors[GetRandomValue(0, 7)]
                );
                cubes.push_back(c);
            }
        }
    }

    Camera cam = {0};
    cam.position = Vector3 {-10.0f, 15.0f, -10.0f};
    cam.target = Vector3 {((float) gridSize / 2), ((float) gridHeight / 2), ((float) gridSize / 2)};
    cam.up = Vector3 {0.0f, 1.0f, 0.0f};
    cam.fovy = 45.0f;
    cam.projection = CAMERA_PERSPECTIVE;

    int cam_mode = CAMERA_ORBITAL;

    window.onUpdate([&](float deltaTime) {
        UpdateCamera(&cam, cam_mode);

        Cube* closest = nullptr;
        int index = -1;
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            std::cout << "Hey, I was pressed!  Delta: " << deltaTime << std::endl;
            Vector2 mousePos = GetMousePosition();
            Ray ray = GetMouseRay(mousePos, cam);
            for(int i = 0; i < cubes.size(); i++) {
                if(cubes[i]->RayHit(ray)) {
                    // TODO: could memoize closest's distance for optimization 
                    if(closest != nullptr && (V3Distance(cam.position, cubes[i]->getPosition()) < (V3Distance(cam.position, closest->getPosition())))) {
                        closest = cubes[i];
                        index = i;
                    } else {
                        closest = cubes[i];
                        index = i;
                    }
                }
            }
            if(index > -1) {
                delete closest;
                cubes.erase(cubes.begin() + index);
            }
        }

    });
    window.onDraw([&]() {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode3D(cam);

        Cube::RenderCubes(cubes, false, RED, .01f);

        Ray ray = GetMouseRay(GetMousePosition(), cam);
        VisualizeRay(ray, 50);

        EndMode3D();
        EndDrawing();
    });
    window.onDestroy([&]() {
        std::cout << "Program ended!" << std::endl;
    });

    window.loop();

    return 0;
}