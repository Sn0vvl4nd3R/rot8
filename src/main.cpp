#include <iostream>

#include "core/cube.hpp"
#include "solvers/korf.hpp"

int main(void) {
  Cube cb;
  Korf kf;

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
  
  cb.Scramble(8);
  std::vector<Move> sol = kf.Solve(cb);

  for (Move m : sol) {
    std::cout << MoveToString(m) << ' ';
    cb.ApplyMove(m);
  }
  std::cout << '\n' << (cb.IsSolved()) << '\n';

  return 0;
}
