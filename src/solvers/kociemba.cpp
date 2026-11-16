#include <queue>
#include <algorithm>

#include "kociemba.hpp"
#include "../core/cube.hpp"
#include "../core/move.hpp"

Kociemba::Kociemba(void) {
  {
    corner_table.fill(255);
    Cube start;
    std::queue<Cube> q;
    corner_table[start.GetCornerOriIndex()] = 0;
    q.push(start);

    while (!q.empty()) {
      Cube curr = q.front();
      q.pop();

      uint8_t dist = corner_table[curr.GetCornerOriIndex()];

      for (uint8_t m = 0; m < 18; ++m) {
        Move mv = static_cast<Move>(m);
        Cube next = curr;
        next.ApplyMove(mv);
        uint16_t idx = next.GetCornerOriIndex();

        if (corner_table[idx] == 255) {
          corner_table[idx] = dist + 1;
          q.push(next);
        }
      }
    }
  }

  {
    edge_table.fill(255);
    Cube start;
    std::queue<Cube> q;
    edge_table[start.GetEdgeOriIndex()] = 0;
    q.push(start);

    while (!q.empty()) {
      Cube curr = q.front();
      q.pop();

      uint8_t dist = edge_table[curr.GetEdgeOriIndex()];

      for (uint8_t m = 0; m < 18; ++m) {
        Move mv = static_cast<Move>(m);
        Cube next = curr;
        next.ApplyMove(mv);
        uint16_t idx = next.GetEdgeOriIndex();

        if (edge_table[idx] == 255) {
          edge_table[idx] = dist + 1;
          q.push(next);
        }
      }
    }
  }

  {
    slice_table.fill(255);
    Cube start;
    std::queue<Cube> q;
    slice_table[start.GetSliceIndex()] = 0;
    q.push(start);

    while (!q.empty()) {
      Cube curr = q.front();
      q.pop();

      uint8_t dist = slice_table[curr.GetSliceIndex()];

      for (uint8_t m = 0; m < 18; ++m) {
        Move mv = static_cast<Move>(m);
        Cube next = curr;
        next.ApplyMove(mv);
        uint16_t idx = next.GetSliceIndex();

        if (slice_table[idx] == 255) {
          slice_table[idx] = dist + 1;
          q.push(next);
        }
      }
    }
  }

  {
    corner_table2.fill(255);
    Cube start;
    std::queue<Cube> q;
    corner_table2[start.GetCornerPermIndex()] = 0;
    q.push(start);

    while (!q.empty()) {
      Cube curr = q.front();
      q.pop();

      uint8_t dist = corner_table2[curr.GetCornerPermIndex()];

      for (uint8_t m = 0; m < 10; ++m) {
        Move mv = allowed_moves[m];
        Cube next = curr;
        next.ApplyMove(mv);
        uint32_t idx = next.GetCornerPermIndex();

        if (corner_table2[idx] == 255) {
          corner_table2[idx] = dist + 1;
          q.push(next);
        }
      }
    }
  }

  {
    edge_table2.fill(255);
    Cube start;
    std::queue<Cube> q;
    edge_table2[start.GetEdgePermIndex()] = 0;
    q.push(start);

    while (!q.empty()) {
      Cube curr = q.front();
      q.pop();

      uint8_t dist = edge_table2[curr.GetEdgePermIndex()];

      for (uint8_t m = 0; m < 10; ++m) {
        Move mv = allowed_moves[m];
        Cube next = curr;
        next.ApplyMove(mv);
        uint32_t idx = next.GetEdgePermIndex();

        if (edge_table2[idx] == 255) {
          edge_table2[idx] = dist + 1;
          q.push(next);
        }
      }
    }
  }
}

bool Kociemba::SearchPhase1(const Cube& cube, int depth, int max_depth, uint8_t prev_face, std::vector<Move>& path) {
  if (cube.GetCornerOriIndex() == 0 &&
      cube.GetEdgeOriIndex() == 0 &&
      cube.GetSliceIndex() == 15) return true;
  if (depth >= max_depth) return false;

  uint8_t h_ori = corner_table[cube.GetCornerOriIndex()];
  uint8_t h_perm = edge_table[cube.GetEdgeOriIndex()];
  uint8_t h_slice = slice_table[cube.GetSliceIndex()];
  uint8_t heuristic = std::max({ h_ori, h_perm, h_slice });
  if (depth + heuristic > max_depth) return false;

  for (uint8_t m = 0; m < 18; ++m) {
    Move mv = static_cast<Move>(m);
    uint8_t face = m / 3;
    if (face == prev_face || (face / 2 == prev_face / 2 && face < prev_face)) continue;

    Cube next_cube = cube;

    next_cube.ApplyMove(mv);
    path.push_back(mv);
    if (SearchPhase1(next_cube, depth + 1, max_depth, face, path)) {
      return true;
    }
    path.pop_back();
  }

  return false;
}

bool Kociemba::SearchPhase2(const Cube& cube, int depth, int max_depth, uint8_t prev_face, std::vector<Move>& path) {
  if (cube.IsSolved()) return true;
  if (depth >= max_depth) return false;

  uint8_t h_ori = corner_table2[cube.GetCornerPermIndex()];
  uint8_t h_perm = edge_table2[cube.GetEdgePermIndex()];
  uint8_t heuristic = std::max(h_ori, h_perm);
  if (depth + heuristic > max_depth) return false;

  for (uint8_t m = 0; m < 10; ++m) {
    Move mv = allowed_moves[m];
    uint8_t face = static_cast<uint8_t>(mv) / 3;
    if (face == prev_face || (face / 2 == prev_face / 2 && face < prev_face)) continue;

    Cube next_cube = cube;

    next_cube.ApplyMove(mv);
    path.push_back(mv);
    if (SearchPhase2(next_cube, depth + 1, max_depth, face, path)) {
      return true;
    }
    path.pop_back();
  }

  return false;
}

std::vector<Move> Kociemba::Solve(const Cube& initial_state) {
  Cube cube_copy = initial_state;

  std::vector<Move> path1;
  for (int max_depth = 0; max_depth <= 12; ++max_depth) {
    if (SearchPhase1(cube_copy, 0, max_depth, 255, path1)) break;
  }

  for (Move m : path1) cube_copy.ApplyMove(m);

  std::vector<Move> path2;
  for (int max_depth = 0; max_depth <= 18; ++max_depth) {
    if (SearchPhase2(cube_copy, 0, max_depth, 255, path2)) break;
  }
  path1.insert(path1.end(), path2.begin(), path2.end());

  return path1;
}
