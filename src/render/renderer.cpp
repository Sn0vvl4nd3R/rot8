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

void Renderer::Draw(const Cube& cube) {
  while (!WindowShouldClose()) {
    IsKeyPressed(KEY_SPACE);
    UpdateCamera(&camera, CAMERA_ORBITAL);

    BeginDrawing();
    ClearBackground(DARKGRAY);
    BeginMode3D(camera);
    
    DrawCube({0, 0, 0}, 2, 2, 2, RED);
    // DrawCubeWires({0, 0, 0}, 2, 2, 2, BLACK);
    DrawGrid(10, 1.0f);

    EndMode3D();
    EndDrawing();
  }

  CloseWindow();
}
