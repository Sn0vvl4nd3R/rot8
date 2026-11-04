#ifndef CUBE_HPP
#define CUBE_HPP

#include <array>
#include <cstdint>
#include <vector>

#include "move.hpp"

enum Corners : uint8_t {
  URF, UFL, ULB, UBR,
  DFR, DLF, DBL, DRB
};

enum Edges : uint8_t {
  UR, UF, UL, UB,
  DR, DF, DL, DB,
  FR, FL, BL, BR
};

class Cube {
public:
  std::array<uint8_t, 8> corner_perm;
  std::array<uint8_t, 8> corner_ori;
  std::array<uint8_t, 12> edge_perm;
  std::array<uint8_t, 12> edge_ori;

  Cube(void);

  void ApplyMove(Move m);
  void Scramble(int num_moves);
  bool IsSolved(void) const;

  std::vector<float> ToOneHot(void) const;

  uint32_t GetCornerPermIndex(void) const;
  uint16_t GetCornerOriIndex(void) const;

  void Print2D(void) const;
};

#endif
