#pragma once
#include <cstdint>
#include "types.h"
#include "UsedLibs.h"

uint64_t rook_masks[64];
uint64_t rook_attacks[64][4096];
const uint64_t rook_magics[64];
uint64_t bishop_masks[64];
uint64_t bishop_attacks[64][512];
const uint64_t bishop_magics[64];
int rook_rellevant_bits[64];

uint64_t getRookMask(int square);
uint64_t getBishopMask(int square);
int bishop_rellevant_bits[64];
uint64_t get_bishop_attacks(int square, uint64_t occupancy);
uint64_t get_rook_attacks(int square, uint64_t occupancy);
uint64_t bishop_attacks_on_the_fly(int square, uint64_t block);
uint64_t rook_attacks_on_the_fly(int square, uint64_t block);
uint64_t get_bit(uint64_t bitboard, int square);
void set_bit(uint64_t& bitboard, int square);
void pop_bit(uint64_t& bitboard, int square);
int count_bits(uint64_t bitboard);
int get_ls1b_index(uint64_t bitboard);
uint64_t set_occupancy(int index, int bits_in_mask, uint64_t attack_mask);
void init_sliders_attacks(int is_bishop);

bool isSet(uint64_t bitboard, int square);