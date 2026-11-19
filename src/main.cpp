#include <iostream>
#include <raylib.h>

#include "core/cube.hpp"
#include "solvers/korf.hpp"
#include "render/renderer.hpp"
#include "solvers/kociemba.hpp"

int main(void) {
  Cube cb;
  cb.FromStickers("BRGWWGWGGYYRGYYBBROOOOOOOOOWRYBRRBGBGWRYGRGBWRYYWBBYWW");
  Kociemba ko;

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
  
  // cb.Scramble(8);
  // cb.Print2D();
  // std::vector<Move> sol = ko.Solve(cb);

  // for (Move m : sol) {
  //   std::cout << MoveToString(m) << ' ';
  //   cb.ApplyMove(m);
  // }
  // std::cout << '\n' << (cb.IsSolved()) << '\n';
  // cb.Print2D();

  Renderer rndr(800, 800, "Hello, World");
  while (!rndr.ShouldClose()) {
    rndr.Draw(cb);
    rndr.Update(cb, ko);
  }

  return 0;
}
