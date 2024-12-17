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

constexpr size_t length = 32;
constexpr size_t width = 32;
constexpr size_t height = 64;
constexpr float speed = 0.01f;
constexpr float horizontal_space = 1.0;
constexpr float vertical_space = 1.0;

struct Grid3DColor {
    std::array<std::array<std::array<Color, 100>, 100>, 100> grid{};
    [[nodiscard]] auto getColor(glm::u8vec3 const& v) const -> Color { return grid[v[0]][v[1]][v[2]]; };
    void update(long long delta) { delta++; }
};

struct Panel {
    std::array<std::array<Color, width>, height> colors;
    float angle;
    void updatePosition(long long delta) { angle += speed * delta; }

    [[nodiscard]] static auto toGridCoord(float r, float theta, float z) -> glm::u8vec3 {
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

    void update(Grid3DColor const& grid, long long delta) {
        updatePosition(delta);
        updateColors(grid);
    }
    [[nodiscard]] auto serialize() const -> std::array<Color, width * height>;
};

class Display {
    Grid3DColor const& grid;
    std::array<Panel, 4> panels{};

  public:
    explicit Display(const Grid3DColor& grid) : grid(grid) {}
    void update(uint64_t delta) {
        for (auto& panel : panels) {
            panel.update(grid, delta);
        }
    }
};

auto main() -> int {
    std::cout << "before start!\n";
    /*InitWindow(800, 400, "volumetric display sim");*/
    /*SetTargetFPS(60);*/
    std::cout << "start!\n";

    Grid3DColor grid{};
    Display display{grid};
    auto prev_time = std::chrono::steady_clock::now();
    /**/
    /*Camera camera{};*/
    /*camera.position = {.x = 16, .y = 16, .z = 16};*/
    /*camera.target = {.x = 0, .y = 0, .z = 0};*/
    /*camera.up = {.x = 0, .y = 1, .z = 0};*/

    /*while (!WindowShouldClose()) {*/
    /*    BeginDrawing();*/
    /*    ClearBackground(BLACK);*/
    /**/
    /*    auto delta = std::chrono::steady_clock::now() - prev_time;*/
    /*    if (delta >= std::chrono::milliseconds(10)) {*/
    /*        grid.update(delta.count());*/
    /*        display.update(delta.count());*/
    /*        prev_time = std::chrono::steady_clock::now();*/
    /*    }*/
    /**/
    /*    EndDrawing();*/
    /*}*/
    std::cout << "done!\n";

    /*CloseWindow();*/
    return 0;
}
