#include <queue>
#include <vector>
#include <cstdint>
#include <algorithm>

#include "korf.hpp"
#include "../core/cube.hpp"
#include "../core/move.hpp"

Korf::Korf(void) {
  {
    ori_table.fill(255);
    Cube start;
    std::queue<Cube> q;
    ori_table[start.GetCornerOriIndex()] = 0;
    q.push(start);

    while (!q.empty()) {
      Cube curr = q.front();
      q.pop();
      
      uint8_t dist = ori_table[curr.GetCornerOriIndex()];

      for (uint8_t m = 0; m < 18; ++m) {
        Move mv = static_cast<Move>(m);
        Cube next = curr;
        next.ApplyMove(mv);
        uint16_t idx = next.GetCornerOriIndex();

        if (ori_table[idx] == 255) {
          ori_table[idx] = dist + 1;
          q.push(next);
        }
      }
    }
  }

  {
    perm_table.fill(255);
    Cube start;
    std::queue<Cube> q;
    perm_table[start.GetCornerPermIndex()] = 0;
    q.push(start);

    while (!q.empty()) {
      Cube curr = q.front();
      q.pop();
      
      uint8_t dist = perm_table[curr.GetCornerPermIndex()];

      for (uint8_t m = 0; m < 18; ++m) {
        Move mv = static_cast<Move>(m);
        Cube next = curr;
        next.ApplyMove(mv);
        uint32_t idx = next.GetCornerPermIndex();

        if (perm_table[idx] == 255) {
          perm_table[idx] = dist + 1;
          q.push(next);
        }
      }
    }
  }
}

bool Korf::Search(const Cube& cube, int depth, int max_depth, uint8_t prev_face, std::vector<Move>& path) {
  if (cube.IsSolved()) return true;
  if (depth >= max_depth) return false;

  uint8_t h_ori = ori_table[cube.GetCornerOriIndex()];
  uint8_t h_perm = perm_table[cube.GetCornerPermIndex()];
  uint8_t heuristic = std::max(h_ori, h_perm);
  if (depth + heuristic > max_depth) return false;

  for (uint8_t m = 0; m < 18; ++m) {
    Move mv = static_cast<Move>(m);
    uint8_t face = m / 3;
    if (face == prev_face || (face / 2 == prev_face / 2 && face < prev_face)) continue;

    Cube next_cube = cube;

    next_cube.ApplyMove(mv);
    path.push_back(mv);
    if (Search(next_cube, depth + 1, max_depth, face, path)) {
      return true;
    }
    path.pop_back();
  }

  return false;
}

std::vector<Move> Korf::Solve(const Cube& initial_state) {
  std::vector<Move> path;

  for (int max_depth = 0; max_depth <= 20; ++max_depth) {
    if (Search(initial_state, 0, max_depth, 255, path)) break;
  }

  return path;
}
