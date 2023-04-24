#pragma once

#include "./include/raylib.h"
#include <string>
#include <functional>

class Window {
    private:
        std::string _title;
        int _width, _height;

        std::function<void(float deltaTime)> _update;
        std::function<void()> _draw;
        std::function<void()> _destroy;

        bool _init = false;
        bool _loop = false;

        int _fps = 60;
    public:
        Window(std::string t, int w, int h) {
            _title = t;
            _width = w;
            _height = h;
            InitWindow(_width, _height, _title.c_str());
            SetTargetFPS(_fps);
            _init = true;
        }
        void fps(int f) {
            _fps = f;
            SetTargetFPS(f);
        }
        int fps() {
            return _fps;
        }
        void onUpdate(std::function<void(float deltaTime)> fun) {
            _update = fun;
        }
        void onDraw(std::function<void()> fun) {
            _draw = fun;
        }
        void onDestroy(std::function<void()> fun) {
            _destroy = fun;
        }

        void loop() {
            if(!_init) return;
            _loop = true;
            while(!WindowShouldClose()) {
                float deltaTime = GetFrameTime();
                if(_update) _update(deltaTime);
                if(_draw) _draw();
            }
            if(_destroy) _destroy();
            CloseWindow();
        }

};