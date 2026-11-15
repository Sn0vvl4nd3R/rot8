#include <random>

#include "cube.hpp"
#include "move.hpp"

void Cube::Scramble(int num_moves) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_int_distribution<uint8_t> dist(0, 17);

  uint8_t prev_face = 255;

  for (int i = 0; i < num_moves; ++i) {
    uint8_t next_move;

    do {
      next_move = dist(gen);
    } while ((next_move / 3) == prev_face);

    ApplyMove(static_cast<Move>(next_move));
    prev_face = next_move / 3;
  }
}
