#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <cmath>

#include "./include/raylib.h"
#include "./include/raymath.h"
#include "./include/rlgl.h"
#include "./Window.h"

// Vector3 math operations for ease of use
#include "../util/V3ops.h"

int main(int argc, char** args) {

    const int WIDTH = 800;
    const int HEIGHT = 640;  

    Window window("This is the title", 800, 640);
    window.fps(60);

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);

    int fps = window.fps();
    float timeTarget = 5.0f;
    float widthStep = (WIDTH / (timeTarget));
    float heightStep = (HEIGHT / (timeTarget));
    std::function<float(float)> fun = [&](float t) {
        float mod = 5;
        return std::modf(t, &mod);
    };
    std::vector<Vector2> graphLines = {
        Vector2 { 0.0f, HEIGHT }
    };

    float seconds = 0.0f;
    window.onUpdate([&](float deltaTime) {
        // graph stepper
        seconds += deltaTime;
        if(seconds >= timeTarget) return;
        float result = fun(seconds);
        float x = widthStep * seconds;
        float y = HEIGHT - (result * heightStep);
        graphLines.push_back(Vector2 {x, y});
    });

    window.onDraw([&]() {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw Lines
        Vector2 last = {0.0f, HEIGHT};
        for(int i = 0; i < graphLines.size(); i++) {
            Vector2 graphItem = graphLines[i];
            DrawLine(last.x, last.y, graphItem.x, graphItem.y, BLACK);
            last = graphItem;
        }

        EndDrawing();
    });
    window.onDestroy([&]() {
        std::cout << "Program ended!" << std::endl;
    });

    window.loop();

    return 0;
}