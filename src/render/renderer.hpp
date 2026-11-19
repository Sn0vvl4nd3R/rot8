#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <queue>
#include <raylib.h>

#include "../core/cube.hpp"
#include "../core/move.hpp"

class Renderer {
private:
  Camera3D camera;
  std::queue<Move> move_queue;
  float move_time;

  void DrawCube3D(const Cube& cube);

public:
  Renderer(int width, int height, const char* title);
  ~Renderer(void);

  bool ShouldClose(void) const;
  // void Update(Cube& cube, ...);
  void Draw(const Cube& cube);
};

#endif
