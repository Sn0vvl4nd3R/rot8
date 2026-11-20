#include "renderer.hpp"
#include "rcamera.h"
#include <raylib.h>

Color Palette[] = {
  WHITE,
  YELLOW,
  ORANGE,
  RED,
  GREEN,
  BLUE
};

Renderer::Renderer(int width, int height, const char* title) {
  InitWindow(width, height, title);
  SetTargetFPS(60);

  camera.position = {6, 6, 6};
  camera.target = {0, 0, 0};
  camera.up = {0, 1, 0};
  camera.fovy = 45.0f;
  camera.projection = CAMERA_PERSPECTIVE;
}

Renderer::~Renderer(void) {
  CloseWindow();
}

bool Renderer::ShouldClose(void) const {
  return WindowShouldClose();
}

void Renderer::Update(Cube& cube, Solver& solver) {
  constexpr float sensitivity = 0.005f;

  if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
    const Vector2 delta = GetMouseDelta();

    CameraYaw(&camera, -delta.x * sensitivity, true);
    CameraPitch(&camera, -delta.y * sensitivity, true, true, false);
  } else {
    UpdateCamera(&camera, CAMERA_ORBITAL);
  }
}

void Renderer::Draw(const Cube& cube) {
  std::array<uint8_t, 54> stickers = cube.ToStickers();

  BeginDrawing();
  ClearBackground(DARKGRAY);
  BeginMode3D(camera);
    
  DrawCube({0, 0, 0}, 3, 3, 3, BLACK);

  constexpr float size = 0.9f;
  constexpr float thickness = 0.05f;
  constexpr float offset = 1.51f;

  for (int row = 0; row < 3; ++row) {
    for (int col = 0; col < 3; ++col) {
      float x = col - 1.0f;
      float y = 1.0f - row;

      // FaceF (Front)
      DrawCube({ x, y, offset }, size, size, thickness, Palette[stickers[4 * 9 + row * 3 + col]]);

      // FaceB (Back)
      DrawCube({ -x, y, -offset }, size, size, thickness, Palette[stickers[5 * 9 + row * 3 + col]]);

      // FaceR (Right)
      DrawCube({ offset, y, -x }, thickness, size, size, Palette[stickers[3 * 9 + row * 3 + col]]);

      // FaceL (Left)
      DrawCube({ -offset, y, x }, thickness, size, size, Palette[stickers[2 * 9 + row * 3 + col]]);

      // FaceU (Up)
      DrawCube({ x, offset, -y }, size, thickness, size, Palette[stickers[0 * 9 + row * 3 + col]]);

      // FaceD (Down)
      DrawCube({ x, -offset, y }, size, thickness, size, Palette[stickers[1 * 9 + row * 3 + col]]);
    }
  }

  DrawCubeWires({0, 0, 0}, 3, 3, 3, WHITE);

  EndMode3D();
  EndDrawing();
}
