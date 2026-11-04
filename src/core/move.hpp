#ifndef MOVE_HPP
#define MOVE_HPP

#include <cstdint>
#include <cstddef>

enum class Move : uint8_t {
  U, U2, UPrime,
  D, D2, DPrime,
  L, L2, LPrime,
  R, R2, RPrime,
  F, F2, FPrime,
  B, B2, BPrime
};

constexpr Move InvertMove(Move m) noexcept {
  uint8_t value = static_cast<uint8_t>(m);
  uint8_t base = (value / 3) * 3;
  uint8_t power = value % 3;

  return static_cast<Move>(base + (2 - power));
}

constexpr const char* MoveToString(Move m) {
  constexpr const char* names[] = {
    "U", "U2", "UPrime",
    "D", "D2", "DPrime",
    "L", "L2", "LPrime",
    "R", "R2", "RPrime",
    "F", "F2", "FPrime",
    "B", "B2", "BPrime"
  };

  return names[static_cast<size_t>(m)];
}

#endif
