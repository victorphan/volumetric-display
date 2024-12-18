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

/*constexpr size_t length = 32;*/
constexpr size_t width = 32;
constexpr size_t height = 16;

constexpr int fps = 240;
constexpr float speed = 0.5 * PI;
constexpr float horizontal_space = 0.5;
constexpr float vertical_space = 0.5;

struct Grid3DColor {
    /*std::array<std::array<std::array<Color, 10>, 10>, 10> grid{};*/
    [[nodiscard]] static auto getColor(glm::u8vec3 const& v) -> Color { return {v[0], v[1], v[2], 255}; };
    /*void update(float delta) { delta++; }*/
};

struct Panel {
    std::array<std::array<Color, width>, height> colors{};
    float angle = 0;
    void updatePosition(float delta) { angle += speed * delta; }

    [[nodiscard]] static auto toGridCoord(float r, float theta, float z) -> glm::vec3 {
        /*return {r, theta, z};*/
        return {r * cos(theta), r * sin(theta), z};
    }

    static auto toColorSpace(glm::vec3 const& v) -> glm::u8vec3 {
        glm::u8vec3 out{};
        out[0] = static_cast<uint8_t>(v[0]);
        /*out[0] = static_cast<uint8_t>((v[0] + static_cast<float>(width) / 2.0f) / static_cast<float>(width) *
         * 255.0f);*/
        /*out[1] = static_cast<uint8_t>((v[1] + static_cast<float>(height) / 2.0f) / static_cast<float>(height) *
         * 255.0f);*/
        /*out[2] = static_cast<uint8_t>(abs(fmod(v[2], 2 * PI)) / 2 * PI * 255.0f);*/
        return out;
    }

    void updateColors(Grid3DColor const& /*grid*/) {
        for (size_t row = 0; row < height; row++) {
            for (size_t col = 0; col < width; col++) {
                /*auto r = static_cast<float>(col + 1);*/
                /*auto z = static_cast<float>(row);*/
                auto r = static_cast<uint8_t>(static_cast<float>(col) / static_cast<float>(width) * 255.0f);
                auto t = static_cast<uint8_t>(abs(fmod(angle, 2 * PI)) / (2 * PI) * 255.0f);
                auto z = static_cast<uint8_t>(static_cast<float>(row) / static_cast<float>(height) * 255.0f);
                /*std::cout << r << " " << angle << " " << z << '\n';*/
                colors[row][col] = Color{r, t, z, 255};
            }
        }
    }

    void update(Grid3DColor const& grid, float delta) {
        updateColors(grid);
        updatePosition(delta);
    }

    void draw() const {
        for (size_t row = 0; row < height; row++) {
            for (size_t col = 0; col < width; col++) {
                auto r = static_cast<float>(col + 1) * horizontal_space;
                auto theta = angle;
                float z = (static_cast<float>(row)) * vertical_space;
                auto u8_coord = toGridCoord(r, theta, z);
                /*std::cout << u8_coord[0] << " " << u8_coord[1] << " " << u8_coord[2] << '\n';*/
                auto coord = Vector3{u8_coord[0], u8_coord[2], u8_coord[1]};
                DrawCube(coord, 0.2f, 0.2f, 0.2f, colors[row][col]);
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
    SetTargetFPS(fps);
    /**/
    Grid3DColor grid{};
    Display display{grid};
    auto prev_time = GetTime();

    Camera camera = {
        .position = {25.0f, 15.0f, 25.0f}, // Camera position
        .target = {0.0f, 2.0f, 0.0f},      // Camera looking at point
        .up = {0.0f, 1.0f, 0.0f},          // Camera up vector (rotation towards target)
        .fovy = 45.0f,                     // Camera field-of-view Y
        .projection = CAMERA_PERSPECTIVE   // Camera projection type
    };

    DisableCursor();

    while (!WindowShouldClose()) {
        UpdateCamera(&camera, CAMERA_THIRD_PERSON);
        BeginDrawing();
        BeginMode3D(camera);
        ClearBackground(BLACK);
        DrawGrid(9, 4.5f);
        auto delta = GetTime() - prev_time;
        display.update(GetFrameTime());
        if (delta >= 0.001) {
            /*grid.update(static_cast<float>(delta));*/
            prev_time = GetTime();
        }
        display.draw();
        EndMode3D();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
