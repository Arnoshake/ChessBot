#pragma once

#include <cstdint>

enum gameEndingCondition{
    whiteWins, blackWins,stalemate,ongoing
};
enum Square {
    a1 = 0,  b1,  c1,  d1,  e1,  f1,  g1,  h1,
    a2 = 8,  b2,  c2,  d2,  e2,  f2,  g2,  h2,
    a3 = 16, b3,  c3,  d3,  e3,  f3,  g3,  h3,
    a4 = 24, b4,  c4,  d4,  e4,  f4,  g4,  h4,
    a5 = 32, b5,  c5,  d5,  e5,  f5,  g5,  h5,
    a6 = 40, b6,  c6,  d6,  e6,  f6,  g6,  h6,
    a7 = 48, b7,  c7,  d7,  e7,  f7,  g7,  h7,
    a8 = 56, b8,  c8,  d8,  e8,  f8,  g8,  h8,
    NO_SQUARE = -1
};
enum Color {
    black = -1, white = 1
};

enum Piece {
    none , pawn, bishop, knight, rook, queen, king
    
};


inline Color operator!(Color c) {
    return static_cast<Color>(-static_cast<int>(c));
}

const uint64_t notAFile =   0xFEFEFEFEFEFEFEFEULL;
const uint64_t notABFile =  0xFCFCFCFCFCFCFCFCULL;
const uint64_t notHFile =   0x7F7F7F7F7F7F7F7FULL;
const uint64_t notGHFile =  0x3F3F3F3F3F3F3F3FULL;