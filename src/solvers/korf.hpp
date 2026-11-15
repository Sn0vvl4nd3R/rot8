#ifndef KORF_HPP
#define KORF_HPP

#include <array>
#include <vector>

#include "solver.hpp"
#include "../core/cube.hpp"
#include "../core/move.hpp"

class Korf : public Solver {
private:
  std::array<uint8_t, 2187> ori_table;
  std::array<uint8_t, 40320> perm_table;

  bool Search(const Cube& cube, int depth, int max_depth, uint8_t prev_face, std::vector<Move>& path);

public:
  Korf(void);

  std::vector<Move> Solve(const Cube& initial_state) override;
};

#endif
