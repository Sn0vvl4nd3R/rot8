#include <random>
#include <string>
#include <iostream>

#include "cube.hpp"
#include "move.hpp"

enum Face : uint8_t {
  FaceU = 0, FaceD = 1, FaceL = 2,
  FaceR = 3, FaceF = 4, FaceB = 5
};

static constexpr uint8_t kCornerColors[8][3] = {
  {FaceU, FaceR, FaceF},
  {FaceU, FaceF, FaceL},
  {FaceU, FaceL, FaceB},
  {FaceU, FaceB, FaceR},
  {FaceD, FaceF, FaceR},
  {FaceD, FaceL, FaceF},
  {FaceD, FaceB, FaceL},
  {FaceD, FaceR, FaceB}
};

static constexpr uint8_t kCornerFacelets[8][3] = {
  {FaceU * 9 + 8, FaceR * 9 + 0, FaceF * 9 + 2},
  {FaceU * 9 + 6, FaceF * 9 + 0, FaceL * 9 + 2},
  {FaceU * 9 + 0, FaceL * 9 + 0, FaceB * 9 + 2},
  {FaceU * 9 + 2, FaceB * 9 + 0, FaceR * 9 + 2},
  {FaceD * 9 + 2, FaceF * 9 + 8, FaceR * 9 + 6},
  {FaceD * 9 + 0, FaceL * 9 + 8, FaceF * 9 + 6},
  {FaceD * 9 + 6, FaceB * 9 + 8, FaceL * 9 + 6},
  {FaceD * 9 + 8, FaceR * 9 + 8, FaceB * 9 + 6}
};

static constexpr uint8_t kEdgeColors[12][2] = {
  {FaceU, FaceR}, {FaceU, FaceF}, {FaceU, FaceL}, {FaceU, FaceB},
  {FaceD, FaceR}, {FaceD, FaceF}, {FaceD, FaceL}, {FaceD, FaceB},
  {FaceF, FaceR}, {FaceF, FaceL}, {FaceB, FaceL}, {FaceB, FaceR}
};

static constexpr uint8_t kEdgeFacelets[12][2] = {
  {FaceU * 9 + 5, FaceR * 9 + 1},
  {FaceU * 9 + 7, FaceF * 9 + 1},
  {FaceU * 9 + 3, FaceL * 9 + 1},
  {FaceU * 9 + 1, FaceB * 9 + 1},
  {FaceD * 9 + 5, FaceR * 9 + 7},
  {FaceD * 9 + 1, FaceF * 9 + 7},
  {FaceD * 9 + 3, FaceL * 9 + 7},
  {FaceD * 9 + 7, FaceB * 9 + 7},
  {FaceF * 9 + 5, FaceR * 9 + 3},
  {FaceF * 9 + 3, FaceL * 9 + 5},
  {FaceB * 9 + 5, FaceL * 9 + 3},
  {FaceB * 9 + 3, FaceR * 9 + 5}
};

static constexpr const char* kColorBlocks[6] = {
  "\033[47;30m U \033[0m",
  "\033[43;30m D \033[0m",
  "\033[48;5;208;30m L \033[0m",
  "\033[41;37m R \033[0m",
  "\033[42;30m F \033[0m",
  "\033[44;37m B \033[0m"
};

void Cube::Scramble(int num_moves) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_int_distribution<uint8_t> dist(0, 17);

  uint8_t prev_face = 255;

  for (int i = 0; i < num_moves; ++i) {
    uint8_t next_move;

    do {
      next_move = dist(gen);
    } while ((next_move / 3) == prev_face);

    ApplyMove(static_cast<Move>(next_move));
    prev_face = next_move / 3;
  }
}

uint16_t Cube::GetCornerOriIndex(void) const {
  uint16_t idx = 0;

  for (int i = 0; i < 7; ++i) {
    idx = idx * 3 + corner_ori[i];
  }

  return idx;
}

uint32_t Cube::GetCornerPermIndex(void) const {
  uint32_t idx = 0;

  for (int i = 0; i < 7; ++i) {
    uint32_t cnt = 0;

    for (int j = i + 1; j < 8; ++j) {
      if (corner_perm[j] < corner_perm[i]) ++cnt;
    }

    idx = idx * (8 - i) + cnt;
  }

  return idx;
}

uint32_t Cube::GetCornerIndex(void) const {
  return GetCornerPermIndex() * 2187 + GetCornerOriIndex();
}

uint16_t Cube::GetEdgeOriIndex(void) const {
  uint16_t idx = 0;

  for (int i = 0; i < 12; ++i) {
    idx = idx * 2 + edge_ori[i];
  }

  return idx;
}

uint32_t Cube::GetEdgePermIndex(void) const {
  uint32_t idx = 0;

  for (int i = 0; i < 7; ++i) {
    uint32_t cnt = 0;

    for (int j = i + 1; j < 8; ++j) {
      if (edge_perm[j] < edge_perm[i]) ++cnt;
    }

    idx = idx * (8 - i) + cnt;
  }

  return idx;
}

uint32_t Cube::GetSliceIndex(void) const {
  uint32_t idx = 0;

  for (int i = 0; i < 12; ++i) {
    if (edge_perm[i] >= 8) {
      idx = idx * 2 + 1;
    } else {
      idx = idx * 2;
    }
  }

  return idx;
}

bool Cube::FromStickers(const std::string& s) {
  std::array<uint8_t, 54> stickers;
  for (int i = 0; i < 54; ++i) {
    for (uint8_t f = 0; f < 6; ++f) {
      if (s[i] == s[f * 9 + 4]) {
        stickers[i] = f;
        break;
      }
    }
  }

  for (int i = 0; i < 12; ++i) {
    uint8_t c0 = stickers[kEdgeFacelets[i][0]];
    uint8_t c1 = stickers[kEdgeFacelets[i][1]];

    for (int j = 0; j < 12; ++j) {
      if (c0 == kEdgeColors[j][0] && c1 == kEdgeColors[j][1]) {
        edge_perm[i] = j;
        edge_ori[i] = 0;
      }
      if (c0 == kEdgeColors[j][1] && c1 == kEdgeColors[j][0]) {
        edge_perm[i] = j;
        edge_ori[i] = 1;
      }
    }
  }

  for (int i = 0; i < 8; ++i) {
    uint8_t c0 = stickers[kCornerFacelets[i][0]];
    uint8_t c1 = stickers[kCornerFacelets[i][1]];
    uint8_t c2 = stickers[kCornerFacelets[i][2]];

    for (int j = 0; j < 8; ++j) {
      if (c0 == kCornerColors[j][0] && c1 == kCornerColors[j][1] && c2 == kCornerColors[j][2]) {
        corner_perm[i] = j;
        corner_ori[i] = 0;
      }
      if (c0 == kCornerColors[j][2] && c1 == kCornerColors[j][0] && c2 == kCornerColors[j][1]) {
        corner_perm[i] = j;
        corner_ori[i] = 1;
      }
      if (c0 == kCornerColors[j][1] && c1 == kCornerColors[j][2] && c2 == kCornerColors[j][0]) {
        corner_perm[i] = j;
        corner_ori[i] = 2;
      }
    }
  }

  return true;
}

std::array<uint8_t, 54> Cube::ToStickers(void) const {
  std::array<uint8_t, 54> stickers;

  return stickers;
}

void Cube::Print2D(void) const {
  std::array<uint8_t, 54> stickers;

  for (uint8_t f = 0; f < 6; ++f) {
    stickers[f * 9 + 4] = f;
  }

  for (int i = 0; i < 12; ++i) {
    uint8_t j = edge_perm[i];
    if (edge_ori[i] == 0) {
      stickers[kEdgeFacelets[i][0]] = kEdgeColors[j][0];
      stickers[kEdgeFacelets[i][1]] = kEdgeColors[j][1];
    } else {
      stickers[kEdgeFacelets[i][0]] = kEdgeColors[j][1];
      stickers[kEdgeFacelets[i][1]] = kEdgeColors[j][0];
    }
  }

  for (int i = 0; i < 8; ++i) {
    uint8_t j = corner_perm[i];
    if (corner_ori[i] == 0) {
      stickers[kCornerFacelets[i][0]] = kCornerColors[j][0];
      stickers[kCornerFacelets[i][1]] = kCornerColors[j][1];
      stickers[kCornerFacelets[i][2]] = kCornerColors[j][2];
    } else if (corner_ori[i] == 1) {
      stickers[kCornerFacelets[i][0]] = kCornerColors[j][2];
      stickers[kCornerFacelets[i][1]] = kCornerColors[j][0];
      stickers[kCornerFacelets[i][2]] = kCornerColors[j][1];
    } else {
      stickers[kCornerFacelets[i][0]] = kCornerColors[j][1];
      stickers[kCornerFacelets[i][1]] = kCornerColors[j][2];
      stickers[kCornerFacelets[i][2]] = kCornerColors[j][0];
    }
  }

  for (int row = 0; row < 3; ++row) {
    std::cout << "          ";
    for (int col = 0; col < 3; ++col) {
      std::cout << kColorBlocks[stickers[FaceU * 9 + row * 3 + col]];
    }
    std::cout << '\n';
  }
  std::cout << '\n';

  constexpr Face mid_faces[4] = {FaceL, FaceF, FaceR, FaceB};
  for (int row = 0; row < 3; ++row) {
    for (Face f : mid_faces) {
      for (int col = 0; col < 3; ++col) {
        std::cout << kColorBlocks[stickers[f * 9 + row * 3 + col]];
      }
      std::cout << ' ';
    }
    std::cout << '\n';
  }
  std::cout << '\n';

  for (int row = 0; row < 3; ++row) {
    std::cout << "          ";
    for (int col = 0; col < 3; ++col) {
      std::cout << kColorBlocks[stickers[FaceD * 9 + row * 3 + col]];
    }
    std::cout << '\n';
  }
}
