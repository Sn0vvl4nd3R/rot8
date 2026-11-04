#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <vector>

#include "../core/cube.cpp"
#include "../core/move.cpp"

class Solver {
public:
  virtual ~Solver(void) = default;
  virtual std::vector<Move> solve(const Cube &initial_state) = 0;
}

#endif
