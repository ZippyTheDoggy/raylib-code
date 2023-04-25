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

const float DEFAULT_LINE_MARGIN = 0.1f;

class Entity {
    private:
        Vector3 _position;
        Vector3 _size;
        Model _model;
        Mesh _mesh;
        BoundingBox _bounds;
        void Calculate() {
            _model = LoadModelFromMesh(_mesh);
            _bounds = GetModelBoundingBox(_model);
            _bounds.min += _position; // offset bounding position for correct hitreg
            _bounds.max += _position; // offset bounding position for correct hitreg
        }
    public:
         Entity(Vector3 position, Vector3 size, Mesh mesh) : 
            _position(position), _size(size), _mesh(mesh) {
                Calculate();
        };
        void DrawBounding(Color color) {
            DrawBoundingBox(_bounds, color);
        };
        void DrawLines(Color lc, float margin = DEFAULT_LINE_MARGIN) {
            DrawModelWires(_model, _position, 1.0f, lc);
        }
        void Draw(Color color) {
            DrawModel(_model, _position, 1.0f, color);
        };
        Vector3 getPosition() { return _position; }
        Vector3 getSize()     { return _size;     }
        void setPosition(Vector3 pos) {
            _position = pos;
            Calculate();
        };
        void setSize(Vector3 size) {
            _size = size;
            Calculate();
        }

        // TODO: add complex raycasting slices/2 bounding boxes, allow virtual overrides
        bool RayHit(Ray r) {
            return GetRayCollisionBox(r, _bounds).hit;
        }
        template <typename ThisType> 
        static void RenderMany(std::vector<ThisType> entities, Color color) {
            for(ThisType entity : entities) {
                entity.Draw(color);
            }
        };
        template <typename ThisType>
        static void RenderManyLines(std::vector<ThisType> entities, Color color, float margin = DEFAULT_LINE_MARGIN) {
            for(ThisType entity : entities) {
                entity.DrawLines(color, margin);
            }
        };
        template <typename ThisType>
        static void RenderManyBounds(std::vector<ThisType> entities, Color color) {
            for(ThisType entity : entities) {
                entity.DrawBounding(color);
            }
        };
};

class Cube : public Entity {
    public:
        Cube(Vector3 position, Vector3 size) : Entity(position, size, GenMeshCube(
            size.x, size.y, size.z
        )) {
            // 
        }
};
class Cylinder : public Entity {
    public:
        Cylinder(Vector3 position, float radius, float height, int slices) : 
            Entity(position, Vector3 { radius * 2, height, radius * 2}, GenMeshCylinder(
                radius, height, slices
        )) {
            // 
        }
};

// Gets the distance between two vectors.  Returns as a float.  
float V3Distance(Vector3 a, Vector3 b) {
    return std::sqrt(std::pow(a.x - b.x, 2.0f) + std::pow(a.y - b.y, 2.0f) + std::pow(a.z - b.z, 2.0f));
};

// Draws ray from starting position to ending position of length `len` in ray's direction.
void VisualizeRay(Ray r, float len) {
    // ! https://answers.unity.com/questions/887344/how-do-i-move-a-vector3-a-distance-to-a-direction.html
    Vector3 newSpot = r.position + (Vector3Normalize(r.direction) * Vector3 { len, len, len });
    DrawLine3D(r.position, newSpot, RED);
};

class OrbitalCamera {
    public:
        Camera cam = {0};
    private:
        Vector3 position;
        Vector3 target;
        Vector3 up = Vector3 {0.0f, 1.0f, 0.0f};
        int projection = CAMERA_PERSPECTIVE;
        int cam_mode = CAMERA_ORBITAL;
    public:
        OrbitalCamera(Vector3 pos, Vector3 targ, float fov = 45.0f, Vector3 upVec = Vector3 {0.0f, 1.0f, 0.0f}) {
            position = pos;
            targ = target;
            up = upVec;
            cam = {0};
            cam.position = pos;
            cam.target = targ;
            cam.up = upVec;
            cam.fovy = fov;
            cam.projection = projection;
        }
        void Update() {
            UpdateCamera(&cam, cam_mode);
        }
        Camera camera() {
            return cam;
        }
};

int main(int argc, char** args) {

    const int WIDTH = 800;
    const int HEIGHT = 640; 

    Window window("This is the title", 800, 640);

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);

    OrbitalCamera cam({-10.0f, 15.0f, 10.0f}, {0.0f, 0.5f, 0.0f});

    Cube c(Vector3 { 0.0f, 0.0f, 0.0f }, Vector3 {1.0f, 3.0f, 1.0f });
    Cylinder c2(Vector3 {0.0f, 0.0f, 5.0f}, 2.0f, 5.0f, 10);

    window.onUpdate([&](float deltaTime) {
        cam.Update();
    });
    window.onDraw([&]() {
        BeginDrawing();
        BeginMode3D(cam.cam);
        ClearBackground(RAYWHITE);

        Ray mouseRay = GetMouseRay(GetMousePosition(), cam.cam);

        Color cColor = BLUE;
        Color c2Color = BLUE;

        if(c.RayHit(mouseRay)) {
            cColor = RED;
        }
        if(c2.RayHit(mouseRay)) {
            c2Color = RED;
        }

        c.Draw(cColor);
        c2.Draw(c2Color);

        c.DrawBounding(ORANGE);
        c2.DrawBounding(ORANGE);

        EndMode3D();
        EndDrawing();
    });
    window.onDestroy([&]() {
        std::cout << "Program ended!" << std::endl;
    });

    window.loop();

    return 0;
}