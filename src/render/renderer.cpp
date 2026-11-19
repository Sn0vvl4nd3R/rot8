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
  DrawCubeWires({0, 0, 0}, 3, 3, 3, WHITE);
  DrawGrid(10, 1.0f);

  EndMode3D();
  EndDrawing();
}
