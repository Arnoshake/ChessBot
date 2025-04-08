#include <iostream>
#include <stdio.h>
/*
    This code was an exercise/introduction to:
    - bitboards
    - hexadecimal representation of chess positions
    - bit operations
    - printing the bitboard
*/
int main(){
    uint64_t whitePawns   = 0x000000000000FF00ULL; //every digit = 4 bits (squares on the board). In hexa, F = 1111, so the 2nd row's entirely set to 1 for pawns
    uint64_t blackPawns   = 0x00FF000000000000ULL; // 00000000 11111111 000000000

    uint64_t whiteRooks   = 0x0000000000000081ULL;
    uint64_t blackRooks   = 0x8100000000000000ULL; // 10000001 00000000 000000000 1010 = A

    uint64_t whiteKnights = 0x0000000000000042ULL; // 01000010
    uint64_t blackKnights = 0x4200000000000000ULL;

    uint64_t whiteBishops = 0x0000000000000024ULL; // (0000)(0000) (0010)(0100)
    uint64_t blackBishops = 0x2400000000000000ULL;

    uint64_t whiteKings   = 0x0000000000000010ULL; // 00010000
    uint64_t blackKings   = 0x1000000000000000ULL; // 10000000

    uint64_t whiteQueens  = 0x0000000000000008ULL;
    uint64_t blackQueens  = 0x0800000000000000ULL;

    uint64_t whitePieces = whitePawns | whiteRooks|  whiteKnights | whiteKings | whiteQueens | whiteBishops;
    uint64_t blackPieces = blackPawns | blackRooks | blackKnights | blackKings | blackQueens | blackBishops;
    uint64_t occupiedSquares = whitePieces | blackPieces;
    
    std::cout << "*ABCDEFGH\n";
    //rank = row, file = col
    for (int rank = 7; rank >= 0; rank--) {  // Starts from rank 8 (top) down to rank 1
        for (int file = 0; file < 8; file++) {
        int square = rank * 8 + file;
            if (file == 0) std::cout << (rank +1);
            /*
            chatGPT'd to get "1ULL << square"
                creates a mask that puts a 1 at the bit value of the value of square 
                so that it can then compare. They will overlay
                10000001 & 1000000 = 1
            */
            if (occupiedSquares & (1ULL << square)){ //if its occupied,
                if (whitePieces & (1ULL << square)){ //occupied by white
                    if (whitePawns & (1ULL << square)) std::cout << 'P';
                    if (whiteKnights & (1ULL << square)) std::cout << 'N';
                    if (whiteBishops & (1ULL << square)) std::cout << 'B';
                    if (whiteRooks & (1ULL << square)) std::cout << 'R';
                    if (whiteQueens & (1ULL << square)) std::cout << 'Q';
                    if (whiteKings & (1ULL << square)) std::cout << 'K';
                }
                else{ // occupied by black
                    if (blackPawns & (1ULL << square)) std::cout << 'p';
                    if (blackKnights & (1ULL << square)) std::cout << 'n';
                    if (blackBishops & (1ULL << square)) std::cout << 'b';
                    if (blackRooks & (1ULL << square)) std::cout << 'r';
                    if (blackQueens & (1ULL << square)) std::cout << 'q';
                    if (blackKings & (1ULL << square)) std::cout << 'k';
                }
            }
            else{ //not occupied
                std::cout<<".";
            }
        }
        std::cout << std::endl;
    }
    return 0;
}

