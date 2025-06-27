#pragma once

#include "MoveInformation.h"
#include "Board.h"
class MoveGenerator{ //uses move parser (uses moveInformation)

public:

    //STUFF FOR MAGIC BOARDS FOR ROOK, BISHOP, QUEEN
    static uint64_t rook_masks[64];
    static uint64_t rook_attacks[64][4096];
    static const uint64_t rook_magics[64];
    static uint64_t bishop_masks[64];
    static uint64_t bishop_attacks[64][512];
    static const uint64_t bishop_magics[64];
    
    static int rook_rellevant_bits[64];
     // bishop rellevant occupancy bits

    static const uint64_t getRookMask(int square);

static const uint64_t getBishopMask(int square);
    static int bishop_rellevant_bits[64];
    static const uint64_t get_bishop_attacks(int square, uint64_t occupancy);
    static const uint64_t get_rook_attacks(int square, uint64_t occupancy);
    static const uint64_t bishop_attacks_on_the_fly(int square, uint64_t block);
    static const uint64_t rook_attacks_on_the_fly(int square, uint64_t block);
    static int get_bit(const uint64_t& bitboard, int square);
    static void set_bit(uint64_t& bitboard, int square);

    // void pop_bit(uint64_t& bitboard, int square){
    //     get_bit(bitboard, square) ? (bitboard ^= (1ULL << square)) : 0; //not my code... provided in Rook magic stuff
    // }
    static void pop_bit(uint64_t& bitboard, int square);//AI GEMINI... ternary leads to it acting as a bit toggle

    static int count_bits(uint64_t bitboard);
    static int get_ls1b_index(uint64_t bitboard);
    static uint64_t set_occupancy(int index, int bits_in_mask, uint64_t attack_mask);
    static void init_sliders_attacks(int is_bishop);

    static constexpr uint64_t notAFile = 0xFEFEFEFEFEFEFEFEULL;
    static constexpr uint64_t notABFile = 0xFCFCFCFCFCFCFCFCULL;
    static constexpr uint64_t notHFile = 0x7F7F7F7F7F7F7F7FULL;
    static constexpr uint64_t notGHFile = 0x3F3F3F3F3F3F3F3FULL;

        static uint64_t getKnightMask(int square);
        static uint64_t getPawnMask(const Board& boardOfInterest,int square, Color side);
        static uint64_t getKingMask(int square);
        
   
        // BITBOARDS!!!!
        static uint64_t possiblePawnMovesBitBoard(const Board& boardOfInterest, Color colorOfInterest); // Board passed by reference (avoids unneeded copy) but ensures no changes
        static uint64_t possibleKnightMovesBitBoard(const Board& boardOfInterest, Color colorOfInterest);
        static uint64_t possibleBishopMovesBitBoard(const Board& boardOfInterest, Color colorOfInterest);
        static uint64_t possibleRookMovesBitBoard(const Board& boardOfInterest, Color colorOfInterest);
        static uint64_t possibleQueenMovesBitBoard(const Board& boardOfInterest, Color colorOfInterest);
        static uint64_t possibleKingMovesBitBoard(const Board& boardOfInterest,Color colorOfInterest);
        static uint64_t attackedByPawns(const Board& boardOfInterest,Color colorOfInterest);
        //MOVE GENERATION FOR USER & AI
        static std::vector<MoveInformation> generatePseudoLegalMovesFromBitboard(const Board& boardOfInterest, const uint64_t& bitBoard, Piece pieceType, Color color);
        static std::vector<MoveInformation> generateLegalMoves(const Board& boardstate, Color color);
        static std::vector<MoveInformation> generateLegalMovesOnBoard(const Board& boardstate, Color color);
        // HELPER/CLEANUP OF PSEUDO LEGAL MOVE GEN
        static void findAmbiguousMoves(std::vector<MoveInformation> &legalMovesList);
        static void pruneMovesThatKeepSelfInCheck(std::vector<MoveInformation> &legalMovesList, const Board& board);
};
