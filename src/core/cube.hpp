#ifndef CUBE_HPP
#define CUBE_HPP

#include <array>
#include <string>
#include <cstdint>

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

struct MoveDelta {
  std::array<uint8_t, 8> cp;
  std::array<uint8_t, 8> co;
  std::array<uint8_t, 12> ep;
  std::array<uint8_t, 12> eo;
};

static constexpr std::array<MoveDelta, 6> kBaseMoves = {{
  // U
  {
    { UBR, URF, UFL, ULB, DFR, DLF, DBL, DRB },
    { 0 },
    { UB, UR, UF, UL, DR, DF, DL, DB, FR, FL, BL, BR },
    { 0 }
  },
  // D
  {
    { URF, UFL, ULB, UBR, DLF, DBL, DRB, DFR },
    { 0 },
    { UR, UF, UL, UB, DF, DL, DB, DR, FR, FL, BL, BR },
    { 0 }
  },
  // L
  {
    { URF, ULB, DBL, UBR, DFR, UFL, DLF, DRB },
    { 0, 1, 2, 0, 0, 2, 1, 0 },
    { UR, UF, BL, UB, DR, DF, FL, DB, FR, UL, DL, BR },
    { 0 }
  },
  // R
  {
    { DFR, UFL, ULB, URF, DRB, DLF, DBL, UBR },
    { 2, 0, 0, 1, 1, 0, 0, 2 },
    { FR, UF, UL, UB, BR, DF, DL, DB, DR, FL, BL, UR },
    { 0 }
  },
  // F
  {
    { UFL, DLF, ULB, UBR, URF, DFR, DBL, DRB },
    { 1, 2, 0, 0, 2, 1, 0, 0 },
    { UR, FL, UL, UB, DR, FR, DL, DB, UF, DF, BL, BR },
    { 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0 }
  },
  // B
  {
    { URF, UFL, UBR, DRB, DFR, DLF, ULB, DBL },
    { 0, 0, 1, 2, 0, 0, 2, 1 },
    { UR, UF, UL, BR, DR, DF, DL, BL, FR, FL, UB, DB },
    { 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1 }
  },
}};

class Cube {
private:
  std::array<uint8_t, 8> corner_perm;
  std::array<uint8_t, 8> corner_ori;
  std::array<uint8_t, 12> edge_perm;
  std::array<uint8_t, 12> edge_ori;

public:
  Cube(void) :
    corner_perm{ 0, 1, 2, 3, 4, 5, 6, 7 },
    corner_ori{ 0 },
    edge_perm{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 },
    edge_ori{ 0 }
  {}

  void ApplyMove(Move m) {
    uint8_t face = static_cast<uint8_t>(m) / 3;
    uint8_t turns = (static_cast<uint8_t>(m) % 3) + 1;
    const MoveDelta& delta = kBaseMoves[face];

    for (std::size_t t = 0; t < turns; ++t) {
      std::array<uint8_t, 8> next_cp;
      std::array<uint8_t, 8> next_co;
      std::array<uint8_t, 12> next_ep;
      std::array<uint8_t, 12> next_eo;

      for (std::size_t i = 0; i < 8; ++i) {
        next_cp[i] = corner_perm[delta.cp[i]];
        next_co[i] = (corner_ori[delta.cp[i]] + delta.co[i]) % 3;
      }

      for (std::size_t i = 0; i < 12; ++i) {
        next_ep[i] = edge_perm[delta.ep[i]];
        next_eo[i] = (edge_ori[delta.ep[i]] ^ delta.eo[i]);
      }

      corner_perm = next_cp;
      corner_ori = next_co;
      edge_perm = next_ep;
      edge_ori = next_eo;
    }
  }

  bool IsSolved(void) const {
    return (corner_perm == std::array<uint8_t, 8>{ 0, 1, 2, 3, 4, 5, 6, 7 } &&
            corner_ori  == std::array<uint8_t, 8>{ 0 } &&
            edge_perm   == std::array<uint8_t, 12>{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 } &&
            edge_ori    == std::array<uint8_t, 12>{ 0 });
  }

  void Scramble(int num_moves);
  uint16_t GetCornerOriIndex(void) const;
  uint32_t GetCornerPermIndex(void) const;
  uint32_t GetCornerIndex(void) const;
  uint16_t GetEdgeOriIndex(void) const;
  uint32_t GetEdgePermIndex(void) const;
  uint32_t GetSliceIndex(void) const;
  bool FromStickers(const std::string& s);
  void Print2D(void) const;
};

#endif
