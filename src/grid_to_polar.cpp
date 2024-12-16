#include <array>
#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <glm/detail/qualifier.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_uint3_sized.hpp>

constexpr size_t length = 32;
constexpr size_t width = 32;
constexpr size_t height = 64;
constexpr float speed = 0.0;
constexpr float horizontal_space = 1.0;
constexpr float vertical_space = 1.0;

struct Color {
    glm::u8vec3 rgb{};
};

struct Grid3DColor {
    std::array<std::array<std::array<Color, height>, width>, length> grid;
    [[nodiscard]] auto getColor(glm::u8vec3 const& v) const -> Color { return grid[v[0]][v[1]][v[2]]; };
    void update(long long delta) { delta++; }
};

struct Panel {
    std::array<std::array<Color, width>, height> colors;
    float angle;
    void updatePosition(long long delta) { angle += speed * delta; }
    [[nodiscard]] static auto toGrid(float r, float theta, float z) -> glm::u8vec3 {
        return {r * cos(theta), r * sin(theta), z};
    }
    void updateColors(Grid3DColor const& grid) {
        for (size_t row = 0; row < height; row++) {
            for (size_t col = 0; col < width; col++) {
                auto r = col * horizontal_space;
                auto theta = angle;
                auto z = row * vertical_space;
                colors[row][col] = grid.getColor(toGrid(r, theta, z));
            }
        }
    }
    void update(Grid3DColor const& grid, long long delta) {
        updatePosition(delta);
        updateColors(grid);
    }
    [[nodiscard]] auto serialize() const -> std::array<Color, width * height>;
};

struct Display {
    std::array<Panel, 4> display;
};

auto main() -> int {
    Display display;
    Grid3DColor grid;
    bool running = true;
    auto prev_time = std::chrono::steady_clock::now();
    while (running) {
        auto delta = std::chrono::steady_clock::now() - prev_time;
        if (delta >= std::chrono::milliseconds(10)) {
            grid.update(delta.count());
            for (auto panel : display.display) {
                panel.update(grid, delta.count());
                /*panel.serialize(); // data to send to microcontroller*/
            }
            prev_time = std::chrono::steady_clock::now();
        }
    }
}

TEST_CASE("test") { FAIL(); }
