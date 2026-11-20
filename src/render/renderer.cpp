#include "renderer.hpp"
#include <raylib.h>

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
  UpdateCamera(&camera, CAMERA_ORBITAL);
}

void Renderer::Draw(const Cube& cube) {
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
      DrawCube({ x, y, offset }, size, size, thickness, RED);

      // FaceB (Back)
      DrawCube({ -x, y, -offset }, size, size, thickness, ORANGE);

      // FaceR (Right)
      DrawCube({ offset, y, -x }, thickness, size, size, BLUE);

      // FaceL (Left)
      DrawCube({ -offset, y, x }, thickness, size, size, GREEN);

      // FaceU (Up)
      DrawCube({ x, offset, -y }, size, thickness, size, WHITE);

      // FaceD (Down)
      DrawCube({ x, -offset, y }, size, thickness, size, YELLOW);
    }
  }

  DrawCubeWires({0, 0, 0}, 3, 3, 3, WHITE);

  EndMode3D();
  EndDrawing();
}
