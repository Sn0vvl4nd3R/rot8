#include <iostream>

#include "core/cube.hpp"
#include "core/move.hpp"

int main(void) {
  Cube cb;

  std::cout << (cb.IsSolved()) << '\n';
  // cb.ApplyMove(Move::F);
  // std::cout << (cb.IsSolved()) << '\n';
  // cb.ApplyMove(InvertMove(Move::F));
  // std::cout << (cb.IsSolved()) << '\n';

  // for (std::size_t i = 0; i < 6; ++i) {
  //   cb.ApplyMove(Move::R);
  //   cb.ApplyMove(Move::U);
  //   cb.ApplyMove(Move::RPrime);
  //   cb.ApplyMove(Move::UPrime);
  //   std::cout << (cb.IsSolved()) << '\n';
  // }
  
  cb.Scramble(5);
  std::cout << (cb.IsSolved()) << '\n';
  return 0;
}
