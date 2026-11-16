#ifndef KOCIEMBA_HPP
#define KOCIEMBA_HPP

#include <vector>

#include "solver.hpp"
#include "../core/move.hpp"

class Kociemba : Solver {
private:
  std::array<uint8_t, 2187> corner_table;
  std::array<uint8_t, 4096> edge_table;
  std::array<uint8_t, 4096> slice_table;

  std::array<uint8_t, 40320> corner_table2;
  std::array<uint8_t, 40320> edge_table2;

  std::array<Move, 10> allowed_moves = {
    Move::U, Move::U2, Move::UPrime,
    Move::D, Move::D2, Move::DPrime,
    Move::L2, Move::R2, Move::F2, Move::B2
  };

  bool SearchPhase1(const Cube& cube, int depth, int max_depth, uint8_t prev_face, std::vector<Move>& path);
  bool SearchPhase2(const Cube& cube, int depth, int max_depth, uint8_t prev_face, std::vector<Move>& path);

public:
  Kociemba(void);

  std::vector<Move> Solve(const Cube& initial_state) override;
};

#endif
