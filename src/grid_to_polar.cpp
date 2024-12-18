#include <array>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <glm/detail/qualifier.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_uint3_sized.hpp>
#include <iostream>
#include <raylib.h>
#include <rlgl.h>

constexpr size_t length = 32;
constexpr size_t width = 32;
constexpr size_t height = 64;
constexpr float speed = 10.0f;
constexpr float horizontal_space = 0.5;
constexpr float vertical_space = 0.5;

struct Grid3DColor {
    std::array<std::array<std::array<Color, 10>, 10>, 10> grid{};
    [[nodiscard]] static auto getColor(glm::u8vec3 const& /*v*/) -> Color {
        /*v;*/
        return RED;
    };
    void update(float delta) { delta++; }
};

struct Panel {
    std::array<std::array<Color, width>, height> colors{};
    float angle = 0;
    void updatePosition(float delta) { angle += speed * delta; }

    [[nodiscard]] static auto toGridCoord(float r, float theta, float z) -> glm::vec3 {
        /*return {r, theta, z};*/
        return {r * cos(theta), r * sin(theta), z};
    }

    void updateColors(Grid3DColor const& grid) {
        for (size_t row = 0; row < height; row++) {
            for (size_t col = 0; col < width; col++) {
                auto r = static_cast<float>(col) * horizontal_space;
                auto theta = angle;
                auto z = static_cast<float>(row) * vertical_space;
                colors[row][col] = grid.getColor(toGridCoord(r, theta, z));
            }
        }
    }

    void update(Grid3DColor const& grid, float delta) {
        updatePosition(delta);
        updateColors(grid);
    }

    void draw() const {
        for (size_t row = 0; row < 16; row++) {
            for (size_t col = 0; col < 32; col++) {
                auto r = static_cast<float>(col) * horizontal_space;
                auto theta = angle;
                auto z = static_cast<float>(row) * vertical_space;
                auto u8_coord = toGridCoord(r, theta, z);
                auto coord = Vector3{u8_coord[0], u8_coord[2], u8_coord[1]};
                DrawCube(coord, 0.1f, 0.1f, 0.1f, RED);
                /*DrawSphere(Vector3{0, 0, 0}, 0.03f, RED);*/
            }
        }
    }

    /*[[nodiscard]] auto serialize() const -> std::array<Color, width * height>;*/
};

struct Display {
    Grid3DColor const& grid;
    std::array<Panel, 4> panels{};

    explicit Display(const Grid3DColor& grid) : grid(grid) {
        panels[1].angle = PI / 2;
        panels[2].angle = PI;
        panels[3].angle = 3 * PI / 2;
    }

    void update(float delta) {
        for (auto& panel : panels) {
            panel.update(grid, delta);
        }
    }

    void draw() {
        for (auto& panel : panels) {
            panel.draw();
        }
    }
};

constexpr auto cube_position = Vector3{-0.5, -0.5, -0.5};
constexpr float cube_width = 1.0;
constexpr float cube_height = 1.0;
constexpr float cube_length = 1.0;

auto main() -> int {
    std::cout << "start\n";
    InitWindow(800, 400, "volumetric display sim");
    SetTargetFPS(240);
    /**/
    Grid3DColor grid{};
    /*Panel panel{};*/
    Display display{grid};
    /*auto prev_time = GetTime();*/

    Camera camera = {
        .position = {20.0f, 20.0f, 20.0f}, // Camera position
        .target = {0.0f, 0.0f, 0.0f},      // Camera looking at point
        .up = {0.0f, 1.0f, 0.0f},          // Camera up vector (rotation towards target)
        .fovy = 45.0f,                     // Camera field-of-view Y
        .projection = CAMERA_PERSPECTIVE   // Camera projection type
    };

    while (!WindowShouldClose()) {
        BeginDrawing();
        BeginMode3D(camera);
        ClearBackground(BLACK);
        DrawGrid(10, 5.0f);
        /*auto delta = GetTime() - prev_time;*/
        /*if (delta >= 0.01) {*/
        /*std::cout << "delta: " << delta << '\n';*/
        /*grid.update(static_cast<float>(delta));*/
        std::cout << "frame delta: " << GetFrameTime() << '\n';
        display.update(GetFrameTime());
        /*prev_time = GetTime();*/
        /*}*/
        display.draw();
        EndMode3D();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
