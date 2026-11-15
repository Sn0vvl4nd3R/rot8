#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <vector>

#include "../core/cube.hpp"

class Solver {
public:
  virtual ~Solver(void) = default;
  virtual std::vector<Move> Solve(const Cube& initial_state) = 0;
};

#endif
