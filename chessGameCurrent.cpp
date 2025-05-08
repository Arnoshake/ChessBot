#include <iostream>
#include<stdio.h>
#include <string>  // included for chess notation
#include <bitset>  //included for decimal to binary conversion
#include <sstream> //included for move print
#include <cstdint>
#include <vector>
/*
BELOW IS NOT MY CODE!

Magic Bitboard implementation was sourced from: "https://www.youtube.com/watch?v=4ohJQ9pCkHI" 3/19/25

These are methods required for getting accurate masks for rooks and bishops (sliding pieces) 
I decided that for right now (4/7/25), magic bitboards are outside my scope of learning and I would rather focus on the elements of decision making from bot POV
*/
uint64_t rook_masks[64];
uint64_t rook_attacks[64][4096];
const uint64_t rook_magics[64] = {
        0x8a80104000800020ULL,
        0x140002000100040ULL,
        0x2801880a0017001ULL,
        0x100081001000420ULL,
        0x200020010080420ULL,
        0x3001c0002010008ULL,
        0x8480008002000100ULL,
        0x2080088004402900ULL,
        0x800098204000ULL,
        0x2024401000200040ULL,
        0x100802000801000ULL,
        0x120800800801000ULL,
        0x208808088000400ULL,
        0x2802200800400ULL,
        0x2200800100020080ULL,
        0x801000060821100ULL,
        0x80044006422000ULL,
        0x100808020004000ULL,
        0x12108a0010204200ULL,
        0x140848010000802ULL,
        0x481828014002800ULL,
        0x8094004002004100ULL,
        0x4010040010010802ULL,
        0x20008806104ULL,
        0x100400080208000ULL,
        0x2040002120081000ULL,
        0x21200680100081ULL,
        0x20100080080080ULL,
        0x2000a00200410ULL,
        0x20080800400ULL,
        0x80088400100102ULL,
        0x80004600042881ULL,
        0x4040008040800020ULL,
        0x440003000200801ULL,
        0x4200011004500ULL,
        0x188020010100100ULL,
        0x14800401802800ULL,
        0x2080040080800200ULL,
        0x124080204001001ULL,
        0x200046502000484ULL,
        0x480400080088020ULL,
        0x1000422010034000ULL,
        0x30200100110040ULL,
        0x100021010009ULL,
        0x2002080100110004ULL,
        0x202008004008002ULL,
        0x20020004010100ULL,
        0x2048440040820001ULL,
        0x101002200408200ULL,
        0x40802000401080ULL,
        0x4008142004410100ULL,
        0x2060820c0120200ULL,
        0x1001004080100ULL,
        0x20c020080040080ULL,
        0x2935610830022400ULL,
        0x44440041009200ULL,
        0x280001040802101ULL,
        0x2100190040002085ULL,
        0x80c0084100102001ULL,
        0x4024081001000421ULL,
        0x20030a0244872ULL,
        0x12001008414402ULL,
        0x2006104900a0804ULL,
        0x1004081002402ULL,
    }; 
uint64_t bishop_masks[64];
uint64_t bishop_attacks[64][512];
const uint64_t bishop_magics[64] = {
        0x40040844404084ULL,
        0x2004208a004208ULL,
        0x10190041080202ULL,
        0x108060845042010ULL,
        0x581104180800210ULL,
        0x2112080446200010ULL,
        0x1080820820060210ULL,
        0x3c0808410220200ULL,
        0x4050404440404ULL,
        0x21001420088ULL,
        0x24d0080801082102ULL,
        0x1020a0a020400ULL,
        0x40308200402ULL,
        0x4011002100800ULL,
        0x401484104104005ULL,
        0x801010402020200ULL,
        0x400210c3880100ULL,
        0x404022024108200ULL,
        0x810018200204102ULL,
        0x4002801a02003ULL,
        0x85040820080400ULL,
        0x810102c808880400ULL,
        0xe900410884800ULL,
        0x8002020480840102ULL,
        0x220200865090201ULL,
        0x2010100a02021202ULL,
        0x152048408022401ULL,
        0x20080002081110ULL,
        0x4001001021004000ULL,
        0x800040400a011002ULL,
        0xe4004081011002ULL,
        0x1c004001012080ULL,
        0x8004200962a00220ULL,
        0x8422100208500202ULL,
        0x2000402200300c08ULL,
        0x8646020080080080ULL,
        0x80020a0200100808ULL,
        0x2010004880111000ULL,
        0x623000a080011400ULL,
        0x42008c0340209202ULL,
        0x209188240001000ULL,
        0x400408a884001800ULL,
        0x110400a6080400ULL,
        0x1840060a44020800ULL,
        0x90080104000041ULL,
        0x201011000808101ULL,
        0x1a2208080504f080ULL,
        0x8012020600211212ULL,
        0x500861011240000ULL,
        0x180806108200800ULL,
        0x4000020e01040044ULL,
        0x300000261044000aULL,
        0x802241102020002ULL,
        0x20906061210001ULL,
        0x5a84841004010310ULL,
        0x4010801011c04ULL,
        0xa010109502200ULL,
        0x4a02012000ULL,
        0x500201010098b028ULL,
        0x8040002811040900ULL,
        0x28000010020204ULL,
        0x6000020202d0240ULL,
        0x8918844842082200ULL,
        0x4010011029020020ULL,
    };
// rook rellevant occupancy bits
int rook_rellevant_bits[64] = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12
};
// bishop rellevant occupancy bits

uint64_t getRookMask(int square){

    int targetRank = square / 8; //rows
    int targetFile = square % 8; //columns

    uint64_t rookAttacks = 0ULL;

    /*
        For loop syntax explanation:
            the rank and file are initialized to the first nonSelf tile for each cardinal direction (pieces cannot move to themselves)
            the loop will iterate until it reaches the 2nd to most outward square. This is important for masking with magic bitboards later
            the condition will update so that it then progresses further in the relevant cardinal direction 
    */
    for (int rank = targetRank + 1; rank <= 6; rank++) rookAttacks |= (1ULL << (8*rank + targetFile) ); //up
    for (int rank = targetRank - 1; rank >= 1 ; rank--) rookAttacks |= (1ULL << (8*rank + targetFile) ); //down
    
    for (int file = targetFile + 1; file <= 6 ; file++) rookAttacks |= (1ULL << (8*targetRank + file) ); //right
    for (int file = targetFile - 1; file >= 1 ; file--) rookAttacks |= (1ULL << (8*targetRank + file) ); //left
    return rookAttacks;





    
        return rookAttacks;
}
uint64_t getBishopMask(int square){
    int targetRank = square / 8; //rows
    int targetFile = square % 8; //columns
    uint64_t bishopAttacks = 0ULL;

    /*
        For loop syntax explanation:
            the rank and file are initialized to the first nonSelf tile for each cardinal direction (pieces cannot move to themselves)
            the loop will iterate until it reaches the 2nd to most outward square. This is important for masking with magic bitboards later
            the condition will update so that it then progresses further in the relevant cardinal direction 
    */
    for (int rank = targetRank + 1, file = targetFile + 1; rank <= 6 && file <= 6; rank++, file++) bishopAttacks |= (1ULL << (8*rank + file) ); // NE
    for (int rank = targetRank + 1, file = targetFile + -1; rank <= 6 && file >= 1; rank++, file--) bishopAttacks |= (1ULL << (8*rank + file) ); // NW
    
    for (int rank = targetRank - 1, file = targetFile + 1; rank >= 1 && file <= 6; rank--, file++) bishopAttacks |= (1ULL << (8*rank + file) ); // SE
    for (int rank = targetRank - 1, file = targetFile - 1; rank >= 1 && file >= 1; rank--, file--) bishopAttacks |= (1ULL << (8*rank + file) ); // SW
    return bishopAttacks;
}
int bishop_rellevant_bits[64] = {
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6
};
uint64_t get_bishop_attacks(int square, uint64_t occupancy) {
	
	// calculate magic index
	occupancy &= bishop_masks[square];
	occupancy *=  bishop_magics[square];
	occupancy >>= 64 - bishop_rellevant_bits[square];
	
	// return rellevant attacks
	return bishop_attacks[square][occupancy];
	
}
uint64_t get_rook_attacks(int square, uint64_t occupancy) {
	
	// calculate magic index
	occupancy &= rook_masks[square];
	occupancy *=  rook_magics[square];
	occupancy >>= 64 - rook_rellevant_bits[square];
	
	// return rellevant attacks
	return rook_attacks[square][occupancy];
}
uint64_t bishop_attacks_on_the_fly(int square, uint64_t block)

{
    // attack bitboard
    uint64_t attacks = 0ULL;
    
    // init files & ranks
    int f, r;
    
    // init target files & ranks
    int tr = square / 8;
    int tf = square % 8;
    
    // generate attacks
    for (r = tr + 1, f = tf + 1; r <= 7 && f <= 7; r++, f++)
    {
        attacks |= (1ULL << (r * 8 + f));
        if (block & (1ULL << (r * 8 + f))) break;
    }
    
    for (r = tr + 1, f = tf - 1; r <= 7 && f >= 0; r++, f--)
    {
        attacks |= (1ULL << (r * 8 + f));
        if (block & (1ULL << (r * 8 + f))) break;
    }
    
    for (r = tr - 1, f = tf + 1; r >= 0 && f <= 7; r--, f++)
    {
        attacks |= (1ULL << (r * 8 + f));
        if (block & (1ULL << (r * 8 + f))) break;
    }
    
    for (r = tr - 1, f = tf - 1; r >= 0 && f >= 0; r--, f--)
    {
        attacks |= (1ULL << (r * 8 + f));
        if (block & (1ULL << (r * 8 + f))) break;
    }
    
    // return attack map for bishop on a given square
    return attacks;
}
uint64_t rook_attacks_on_the_fly(int square, uint64_t block)
{
    // attacks bitboard
    uint64_t attacks = 0ULL;
    
    // init files & ranks
    int f, r;
    
    // init target files & ranks
    int tr = square / 8;
    int tf = square % 8;
    
    // generate attacks
    for (r = tr + 1; r <= 7; r++)
    {
        attacks |= (1ULL << (r * 8 + tf));
        if (block & (1ULL << (r * 8 + tf))) break;
    }
    
    for (r = tr - 1; r >= 0; r--)
    {
        attacks |= (1ULL << (r * 8 + tf));
        if (block & (1ULL << (r * 8 + tf))) break;
    }
    
    for (f = tf + 1; f <= 7; f++)
    {
        attacks |= (1ULL << (tr * 8 + f));
        if (block & (1ULL << (tr * 8 + f))) break;
    }
    
    for (f = tf - 1; f >= 0; f--)
    {
        attacks |= (1ULL << (tr * 8 + f));
        if (block & (1ULL << (tr * 8 + f))) break;
    }
    
    // return attack map for bishop on a given square
    return attacks;
}
#define get_bit(bitboard, square) (bitboard & (1ULL << square))
void set_bit(uint64_t& bitboard, int square){
     (bitboard |= (1ULL << square));
}
void pop_bit(uint64_t& bitboard, int square){
    get_bit(bitboard, square) ? (bitboard ^= (1ULL << square)) : 0; //not my code... provided in Rook magic stuff
}
int count_bits(uint64_t bitboard) {
    // bit count
    int count = 0;
    
    // pop bits untill bitboard is empty
    while (bitboard)
    {
        // increment count
        count++;
        
        // consecutively reset least significant 1st bit
        bitboard &= bitboard - 1;
    }
    
    // return bit count
    return count;
  }
int get_ls1b_index(uint64_t bitboard) {
    // make sure bitboard is not empty
    if (bitboard != 0)
        // convert trailing zeros before LS1B to ones and count them
        return count_bits((bitboard & -bitboard) - 1);
    
    // otherwise
    else
        // return illegal index
        return -1;
}
uint64_t set_occupancy(int index, int bits_in_mask, uint64_t attack_mask)
{
    // occupancy map
    uint64_t occupancy = 0ULL;
    
    // loop over the range of bits within attack mask
    for (int count = 0; count < bits_in_mask; count++)
    {
        // get LS1B index of attacks mask
        int square = get_ls1b_index(attack_mask);
        
        // pop LS1B in attack map
        pop_bit(attack_mask, square);
        
        // make sure occupancy is on board
        if (index & (1 << count))
            // populate occupancy map
            occupancy |= (1ULL << square);
    }
    
    // return occupancy map
    return occupancy;
}
void init_sliders_attacks(int is_bishop)
{
    // loop over 64 board squares
    for (int square = 0; square < 64; square++)
    {
        // init bishop & rook masks
        bishop_masks[square] = getBishopMask(square);
        rook_masks[square] = getRookMask(square);
        
        // init current mask
        uint64_t mask = is_bishop ? getBishopMask(square) : getRookMask(square);
        
        // count attack mask bits
        int bit_count = count_bits(mask);
        
        // occupancy variations count
        int occupancy_variations = 1 << bit_count;
        
        // loop over occupancy variations
        for (int count = 0; count < occupancy_variations; count++)
        {
            // bishop
            if (is_bishop)
            {
                // init occupancies, magic index & attacks
                uint64_t occupancy = set_occupancy(count, bit_count, mask);
                uint64_t magic_index = occupancy * bishop_magics[square] >> (64 - bishop_rellevant_bits[square]);
                bishop_attacks[square][magic_index] = bishop_attacks_on_the_fly(square, occupancy);                
            }
            
            // rook
            else
            {
                // init occupancies, magic index & attacks
                uint64_t occupancy = set_occupancy(count, bit_count, mask);
                uint64_t magic_index = occupancy * rook_magics[square] >> (64 - rook_rellevant_bits[square]);
                rook_attacks[square][magic_index] = rook_attacks_on_the_fly(square, occupancy);                
            }
        }
    }
}

bool isSet(uint64_t bitboard, int square){
    return (bitboard & (1ULL << square) ) != 0 ;
}


enum Square {
    a1 = 0,  b1,  c1,  d1,  e1,  f1,  g1,  h1,
    a2 = 8,  b2,  c2,  d2,  e2,  f2,  g2,  h2,
    a3 = 16, b3,  c3,  d3,  e3,  f3,  g3,  h3,
    a4 = 24, b4,  c4,  d4,  e4,  f4,  g4,  h4,
    a5 = 32, b5,  c5,  d5,  e5,  f5,  g5,  h5,
    a6 = 40, b6,  c6,  d6,  e6,  f6,  g6,  h6,
    a7 = 48, b7,  c7,  d7,  e7,  f7,  g7,  h7,
    a8 = 56, b8,  c8,  d8,  e8,  f8,  g8,  h8
};
enum Color {
    black = -1, white = 1
};
enum Piece {
    none , pawn, bishop, knight, rook, queen, king
    
};
enum boardRep{  //printing --> if black piece, add 6 to integer representation
    P,B,N,R,Q,K, // white (0-5)
    p,b,n,r,q,k  // black (6-11)
};
std::string kingCastleNotation = "O-O";
std::string queenCastleNotation = "O-O-O";


void printBitBoard(uint64_t board){
    for (int rank = 8; rank >= 1; rank--)
    { // Starts from rank 8 (top) down to rank 1
        for (int file = 1; file <= 8; file++)
        {
            int square = (rank-1) * 8 + file-1;
            
            if (file == 0) std::cout << (rank + 1) << " ";
        
            if (( 1ULL << square) & board){
                std::cout<<"1";
            }
            else{
                std::cout<<"0";
            }
        }
            
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

class Board{
private:
    
    // White pieces
    uint64_t whitePawns;
    uint64_t whiteKnights;
    uint64_t whiteBishops;
    uint64_t whiteRooks;
    uint64_t whiteQueens;
    uint64_t whiteKings;

    // Black pieces
    uint64_t blackPawns;
    uint64_t blackKnights;
    uint64_t blackBishops;
    uint64_t blackRooks;
    uint64_t blackQueens;
    uint64_t blackKings;
    // Combined white and black pieces
    uint64_t whitePieces;
    uint64_t blackPieces;
    // Occupied squares (both white and black pieces)
    uint64_t occupiedSquares;
    uint64_t emptySquares;

    uint64_t friendlyPieces;
    uint64_t enemyPieces;
    // 1= white, -1 = black
    Color isWhiteTurn;

public:
    //prevents leftside overflow
    const uint64_t notAFile =   0xFEFEFEFEFEFEFEFEULL;
    const uint64_t notABFile =  0xFCFCFCFCFCFCFCFCULL;
    //prevents rightside overflow
    const uint64_t notHFile =   0x7F7F7F7F7F7F7F7FULL;
    const uint64_t notGHFile =  0x3F3F3F3F3F3F3F3FULL;
    int enPassantTargetSquare;
    Board(){
        whitePawns = 0ULL; 
        blackPawns = 0ULL;
        
        whiteRooks = 0ULL;
        blackRooks = 0ULL; 

        whiteKnights = 0ULL; 
        blackKnights = 0ULL;

        whiteBishops = 0ULL; 
        blackBishops = 0ULL;

        whiteKings = 0ULL; 
        blackKings = 0ULL; 

        whiteQueens = 0ULL;
        blackQueens = 0ULL;

        whitePieces = whitePawns | whiteBishops | whiteRooks | whiteKnights | whiteKings | whiteQueens;
        blackPieces = blackPawns | blackBishops | blackRooks | blackKnights | blackKings | blackQueens;
        occupiedSquares = whitePieces | blackPieces;
        emptySquares = ~occupiedSquares;
        isWhiteTurn = white;
        friendlyPieces = 0ULL;
        enemyPieces = 0ULL;
        enPassantTargetSquare = -1;
    };

    void initializeBoard()
    {                                       // Initialize the chess board with pieces | LSB = 1a-1d, MSB = 8a-8d
        whitePawns = 0x000000000000FF00ULL; // every digit = 4 bits (squares on the board). In hexa, F = 1111, so the 2nd row's entirely set to 1 for pawns
        blackPawns = 0x00FF000000000000ULL; // 00000000 11111111 000000000
        
        whiteRooks = 0x0000000000000081ULL;
        blackRooks = 0x8100000000000000ULL; // 10000001 00000000 000000000 1010 = A

        whiteKnights = 0x0000000000000042ULL; // 01000010
        blackKnights = 0x4200000000000000ULL;

        whiteBishops = 0x0000000000000024ULL; // (0000)(0000) (0010)(0100)
        blackBishops = 0x2400000000000000ULL;

        whiteKings = 0x0000000000000010ULL; // 00010000
        blackKings = 0x1000000000000000ULL; // 10000000

        whiteQueens = 0x0000000000000008ULL;
        blackQueens = 0x0800000000000000ULL;

        whitePieces = whitePawns | whiteBishops | whiteRooks | whiteKnights | whiteKings | whiteQueens;
        blackPieces = blackPawns | blackBishops | blackRooks | blackKnights | blackKings | blackQueens;
        occupiedSquares = whitePieces | blackPieces;
        emptySquares = ~occupiedSquares;
        isWhiteTurn = white;
        friendlyPieces = whitePieces;
        enemyPieces = blackPieces;
        enPassantTargetSquare = -1;
    
    };
    uint64_t possiblePawnMovesBitBoard(){ 
      
        uint64_t pawnBitBoard;
        uint64_t enemyPieces;
        uint64_t friendlyPieces;
        if (isWhiteTurn){
            pawnBitBoard = getWhitePawn();
            friendlyPieces = getWhitePieces();
            enemyPieces = getBlackPieces();
        }
        else{
            pawnBitBoard = getBlackPawn();
            friendlyPieces = getBlackPieces();
            enemyPieces = getWhitePieces();
        }
    
       
    
        uint64_t possibleMoves = 0ULL;
        uint64_t possibleCaptures = 0ULL;
    
        if (isWhiteTurn == 1){ // pawns move up.. << & en Passant rank = 4
            uint64_t singlePush = ( pawnBitBoard << 8) & emptySquares ; 
            uint64_t doublePush = ( ( ( (0xFF00 & pawnBitBoard) << 8) & emptySquares ) << 8 ) & emptySquares; //checks for unmoved pawns, single pushes, pushes again
        
            possibleMoves = singlePush | doublePush ;
            
            possibleCaptures |= ( (pawnBitBoard & notHFile) << 9 ) & enemyPieces;
            possibleCaptures |= ( (pawnBitBoard & notAFile )<< 7) & enemyPieces;
            possibleMoves |= possibleCaptures;
            possibleMoves &= ~(friendlyPieces);
            return possibleMoves;
        }
        else{ //identical logic, different variable values and shifting operator
            std::cout<<"cheese cheese" << std::endl;
            uint64_t singlePush = ( pawnBitBoard >> 8) & emptySquares ; 
            uint64_t doublePush = ( ( ( (0x00FF000000000000ULL & pawnBitBoard) >> 8) & emptySquares ) >> 8 ) & emptySquares; //checks for unmoved pawns, single pushes, pushes again
        
            possibleMoves = singlePush | doublePush ;
            
            possibleCaptures |= ( (pawnBitBoard & notHFile) >> 9 ) & enemyPieces;
            possibleCaptures |= ( (pawnBitBoard & notAFile )>> 7) & enemyPieces;
            possibleMoves |= possibleCaptures;
            possibleMoves &= ~(friendlyPieces);
            return possibleCaptures | possibleMoves;
        }
    }
    uint64_t possibleKnightMovesBitBoard(){
        /*
            15  17
         6         10
               x
         10         6
            17  15
        
        */
        //seperate board for knight moves and condition checking. If 1 was used, cascading shifting would happen... bad!
        uint64_t knightBitBoard;
        uint64_t friendlyPieces;
        uint64_t enemyPieces;
        if (isWhiteTurn){
            knightBitBoard = getWhiteKnight();
            friendlyPieces = getWhitePieces();
            enemyPieces = getBlackPieces();
        }
        else{
            knightBitBoard = getBlackKnight();
            friendlyPieces = getBlackPieces();
            enemyPieces = getWhitePieces();
        }
        uint64_t bitboard = knightBitBoard;
        uint64_t knightMoves = 0ULL;
    
        knightMoves |= (bitboard & notHFile) << 17;
        knightMoves |= (bitboard & notAFile) << 15;
        knightMoves |= (bitboard & notGHFile) << 10;
        knightMoves |= (bitboard & notABFile) << 6;
        knightMoves |= (bitboard & notAFile) >> 17;
        knightMoves |= (bitboard & notHFile) >> 15; 
        knightMoves |= (bitboard & notABFile) >> 10; 
        knightMoves |= (bitboard & notGHFile) >> 6; 
        knightMoves &= ~friendlyPieces;
        return knightMoves;
    }
    uint64_t possibleBishopMovesBitBoard(){
        uint64_t bishopBitboard = 0ULL;
        uint64_t friendlyPieces;
        uint64_t enemyPieces;
        if (isWhiteTurn){
            bishopBitboard = getWhiteBishop();
            friendlyPieces = getWhitePieces();
            enemyPieces = getBlackPieces();
        }
        else{
            bishopBitboard = getBlackBishop();
            friendlyPieces = getBlackPieces();
            enemyPieces = getWhitePieces();
        }
        uint64_t bishopMoves = 0ULL;
        for (int square = 0; square < 64; square++){ //only if the bit is set, do you calculate rook moves
            if (isSet(bishopBitboard,square)){
                bishopMoves |= get_bishop_attacks(square,getOccupiedSquares() );
            }
        }
        bishopMoves &= ~friendlyPieces;
        return bishopMoves;
    }
    uint64_t possibleRookMovesBitBoard(){
        
        uint64_t friendlyPieces;
        uint64_t enemyPieces;
        uint64_t rookBitBoard = 0ULL;
        if (isWhiteTurn){
            rookBitBoard = getWhiteRook();
            friendlyPieces = getWhitePieces();
            enemyPieces = getBlackPieces();
        }
        else{
            rookBitBoard = getBlackRook();
            friendlyPieces = getBlackPieces();
            enemyPieces = getWhitePieces();
        }

        uint64_t rookMoves = 0ULL;
        for (int square = 0; square < 64; square++){ //only if the bit is set, do you calculate rook moves
            if (isSet(rookBitBoard,square)){
                rookMoves |= get_rook_attacks(square,getOccupiedSquares() );
            }
        }
        rookMoves &= ~ friendlyPieces;
        return rookMoves;
    }
    uint64_t possibleQueenMovesBitBoard(){
        uint64_t queenBitBoard = 0ULL;
        uint64_t enemyPieces;
        uint64_t friendlyPieces;
        if (isWhiteTurn){
            queenBitBoard = getWhiteQueen();
            friendlyPieces = getWhitePieces();
            enemyPieces = getBlackPieces();
        }
        else{
            queenBitBoard = getBlackQueen();
            friendlyPieces = getBlackPieces();
            enemyPieces = getWhitePieces();
        }
        queenBitBoard |= possibleRookMovesBitBoard() | possibleBishopMovesBitBoard();
        
        return queenBitBoard;
    }
    uint64_t possibleKingMovesBitBoard(){
        
        uint64_t kingBitBoard;
        uint64_t enemyPieces;
        uint64_t friendlyPieces;
        if (isWhiteTurn){
            kingBitBoard = getWhiteKing();
            friendlyPieces = getWhitePieces();
            enemyPieces = getBlackPieces();
        }
        else{
            kingBitBoard = getBlackKing();
            friendlyPieces = getBlackPieces();
            enemyPieces = getWhitePieces();
        }

        

        uint64_t kingAttack = 0ULL;

    
        //top
        kingAttack |= (notAFile & kingBitBoard) >> 9;
        kingAttack |= kingBitBoard >> 8;
        kingAttack |= (notAFile & kingBitBoard) >> 7; 
        kingAttack |= (notHFile & kingBitBoard) >> 1;
        //bottom
        kingAttack |= (notHFile & kingBitBoard) << 7;
        kingAttack |= kingBitBoard << 8;
        kingAttack |= (notHFile & kingBitBoard) << 9;
        kingAttack |= (notAFile & kingBitBoard) << 1;
    
        kingAttack &= ~friendlyPieces;
        return (kingAttack);
    }
    //use the magic bitboard get_rook_attack and get_bishop_attack methods
    uint64_t getKnightMask(int square){
        /*
            15  17
         6         10
               x
         10         6
            17  15
        
     up is SL, down is SR
        */
        //prevents leftside overflow
        uint64_t notAFile =   0xFEFEFEFEFEFEFEFEULL;
        uint64_t notABFile =  0xFCFCFCFCFCFCFCFCULL;
    
        //prevents rightside overflow
        uint64_t notHFile =   0x7F7F7F7F7F7F7F7FULL;
        uint64_t notGHFile =  0x3F3F3F3F3F3F3F3FULL;
        //size of number handles up and down overflow (greater than or less than the upper/lower bound)
        //seperate board for knight moves and condition checking. If 1 was used, cascading shifting would happen... bad!
        uint64_t knightMoves = 0ULL;
        uint64_t bitboard = 0ULL;
        
       uint64_t startingSquare = 1 << square;
       bitboard |= startingSquare;
       //step 1
       if ( (bitboard >> 17) & (notHFile) ) knightMoves |= (bitboard >> 17);
       if ( (bitboard >> 15) & (notAFile) ) knightMoves |= (bitboard >> 15);
       if ( (bitboard >> 10) & (notGHFile) ) knightMoves |= (bitboard >> 10);
       if ( (bitboard >> 6)  & (notABFile) ) knightMoves |= (bitboard >> 6);
       
       if ( (bitboard << 17) & (notAFile) ) knightMoves |= (bitboard << 17);
       if ( (bitboard << 15) & (notHFile) ) knightMoves |= (bitboard << 15);
       if ( (bitboard << 10) & (notABFile) ) knightMoves |= (bitboard << 10);
       if ( (bitboard << 6)  & (notGHFile) ) knightMoves |= (bitboard << 6);
        
        return knightMoves;
    }
    uint64_t getPawnMask(int square, Color side){
        int rank = square / 8;
        int file = square % 8;
        //1 = white, 0 = black
        uint64_t possibleMoves = 0ULL;
        uint64_t possibleCaptures = 0ULL;
        uint64_t pawnBitBoard = 1ULL << square;
        uint64_t friendlyPieces;
        uint64_t enemyPieces;
        if (side == white){ // pawns move up.. << & en Passant rank = 4
            friendlyPieces = getWhitePieces();
            enemyPieces = getBlackPieces();
            uint64_t singlePush = ( pawnBitBoard << 8) & emptySquares ; 
            uint64_t doublePush = 0ULL;
            if (rank == 1 && singlePush) doublePush = (singlePush << 8) & emptySquares;

             
            // if (rank == 2 & singlePush){
            //     doublePush = (singlePush << 8) & emptySquares;
            // }
            possibleMoves = singlePush | doublePush ;
            pawnBitBoard = 1ULL << square;
            possibleCaptures |= ( (pawnBitBoard & notHFile) << 9 ) & enemyPieces;
            possibleCaptures |= ( (pawnBitBoard & notAFile )<< 7) & enemyPieces;
            possibleMoves |= possibleCaptures;
        }
        else{ //identical logic, different variable values and shifting operator
            friendlyPieces = getBlackPieces();
            enemyPieces = getWhitePieces();
    
            uint64_t singlePush = ( pawnBitBoard >> 8) & emptySquares ; 
            uint64_t doublePush = 0ULL;
            if (rank == 6 && singlePush) doublePush = (singlePush >> 8) & emptySquares;
            possibleMoves = singlePush | doublePush ;
            
            possibleCaptures |= ( (pawnBitBoard & notHFile) >> 9 ) & enemyPieces;
            possibleCaptures |= ( (pawnBitBoard & notAFile )>> 7) & enemyPieces;
            possibleMoves |= possibleCaptures;
        }
        
        possibleMoves &= ~(friendlyPieces);
        return possibleMoves;
      
    
    }
    uint64_t getKingMask(int square){
        //prevents leftside overflow
        uint64_t notAFile =   0xFEFEFEFEFEFEFEFEULL;
        //prevents rightside overflow
        uint64_t notHFile =   0x7F7F7F7F7F7F7F7FULL;
     
    
        int rank = square / 8;
        int file = square % 8;
        uint64_t kingAttack = 0ULL;
        
        if ( rank > 0) kingAttack |= 1ULL << (square - 8); // down
        if ( rank < 7) kingAttack |= 1ULL << (square + 8); // up
        if ( file > 0) kingAttack |= 1ULL << (square - 1); // left
        if ( file < 7) kingAttack |= 1ULL << (square + 1); // right
    
        if ( rank < 7 && file < 7) kingAttack |= 1ULL << (square + 9); //top right
        if ( rank < 7 && file > 0) kingAttack |= 1ULL << (square + 7); // top left
        if ( rank > 0 && file > 0) kingAttack |= 1ULL << (square - 9); // bot left
        if ( rank > 0 && file < 7) kingAttack |= 1ULL << (square - 7); // bot left
        
    
        return kingAttack;
    }
    
    void addPiece(Color color, int piece, int square){
        if (color){
            switch(piece){
                case pawn:
                    set_bit(getWhitePawn(), square);
                    set_bit(getWhitePieces(),square);
                    break;
                case bishop:
                    set_bit(getWhiteBishop(), square);
                    set_bit(getWhitePieces(),square);
                    break;
                case knight:
                    set_bit(getWhiteKnight(), square);
                    set_bit(getWhitePieces(),square);
                    break;
                case rook:
                    set_bit(getWhiteRook(), square);
                    set_bit(getWhitePieces(),square);
                    break;
                case queen:
                    set_bit(getWhiteQueen(), square);
                    set_bit(getWhitePieces(),square);
                    break;
                case king:
                    set_bit(getWhiteKing(), square);
                    set_bit(getWhitePieces(),square);
                    break;
            }
        }
        else{
            switch(piece){
                case pawn:
                    set_bit(getBlackPawn(), square);
                    set_bit(getBlackPieces(),square);
                    break;
                case bishop:
                    set_bit(getBlackBishop(), square);
                    set_bit(getBlackPieces(),square);
                    break;
                case knight:
                    set_bit(getBlackKnight(), square);
                    set_bit(getBlackPieces(),square);
                    break;
                case rook:
                    set_bit(getBlackRook(), square);
                    set_bit(getBlackPieces(),square);
                    break;
                case queen:
                    set_bit(getBlackQueen(), square);
                    set_bit(getBlackPieces(),square);
                    break;
                case king:
                    set_bit(getBlackKing(), square);
                    set_bit(getBlackPieces(),square);
                    break;
            }
        }
        set_bit(getOccupiedSquares(),square);
        getEmptySquares() = ~getOccupiedSquares();
    }
    void removePiece(Color color, int piece, int square){
        if (color){
            switch(piece){
                case pawn:
                        pop_bit(getWhitePawn(), square);
                        pop_bit(getWhitePieces(),square);
                    break;
                case bishop:
                        pop_bit(getWhiteBishop(), square);
                        pop_bit(getWhitePieces(),square);
                    break;
                case knight:
                    pop_bit(getWhiteKnight(), square);
                    pop_bit(getWhitePieces(),square);
                    break;
                case rook:
                    pop_bit(getWhiteRook(), square);
                    pop_bit(getWhitePieces(),square);
                    break;
                case queen:
                    pop_bit(getWhiteQueen(), square);
                    pop_bit(getWhitePieces(),square);
                    break;
                case king:
                    pop_bit(getWhiteKing(), square);
                    pop_bit(getWhitePieces(),square);
                    break;
            }
        }
        else{
            switch(piece){
                case pawn:
                    pop_bit(getBlackPawn(), square);
                    pop_bit(getBlackPieces(),square);
                    break;
                case bishop:
                    pop_bit(getBlackBishop(), square);
                    pop_bit(getBlackPieces(),square);
                    break;
                case knight:
                    pop_bit(getBlackKnight(), square);
                    pop_bit(getBlackPieces(),square);
                    break;
                case rook:
                    pop_bit(getBlackRook(), square);
                    pop_bit(getBlackPieces(),square);
                    break;
                case queen:
                    pop_bit(getBlackQueen(), square);
                    pop_bit(getBlackPieces(),square);
                    break;
                case king:
                    pop_bit(getBlackKing(), square);
                    pop_bit(getBlackPieces(),square);
                    break;
            }
        }
        pop_bit(getOccupiedSquares(),square);
        getEmptySquares() = ~getOccupiedSquares();
    }
    void updateFriendlyEnemy(Color color){
        if (white){
            getFriendlyPieces() = getWhitePieces();
            getEnemyPieces() = getBlackPieces();
        }
        else{
            getFriendlyPieces() = getBlackPieces();
            getEnemyPieces() = getWhitePieces();
        }
    }
    Piece getPieceAtSquare(int square){
        if ( (blackPawns | whitePawns ) & (1ULL << square) ) return pawn;
        if ( (blackKnights | whiteKnights ) & (1ULL << square) ) return knight;
        if ( (blackBishops | whiteBishops ) & (1ULL << square) ) return bishop;
        if ( (blackRooks | whiteRooks ) & (1ULL << square) ) return rook;
        if ( (blackQueens | whiteQueens ) & (1ULL << square) ) return queen;
        if ( (blackKings | whiteKings ) & (1ULL << square) ) return king;

        return none;
    }
    //1 billion helper and bool methods
    int getSideForTurn(){
        if (isWhiteTurn == 1) return 1;
        return 0;
    }
    bool isWhitePiece(int square)
    {
        return ((whitePieces) & (1ULL << square)) != 0;
    }
    bool isBlackPiece(int square)
    {
        return ((blackPieces) & (1ULL << square)) != 0;
    };
    bool isWhitePawn(int square)
    {
        return ((whitePawns) & (1ULL << square)) != 0;
    };
    bool isBlackPawn(int square)
    {
        return ((blackPawns) & (1ULL << square)) != 0;
    }; // Check if a square contains a black pawn
    bool isWhiteKnight(int square)
    {
        return ((whiteKnights) & (1ULL << square)) != 0;
    }; // Check if a square contains a white knight
    bool isBlackKnight(int square)
    {
        return ((blackKnights) & (1ULL << square)) != 0;
    }; // Check if a square contains a black knight
    bool isWhiteBishop(int square)
    {
        return ((whiteBishops) & (1ULL << square)) != 0;
    }; // Check if a square contains a white bishop
    bool isBlackBishop(int square)
    {
        return ((blackBishops) & (1ULL << square)) != 0;
    }; // Check if a square contains a black bishop
    bool isWhiteRook(int square)
    {
        return ((whiteRooks) & (1ULL << square)) != 0;
    }; // Check if a square contains a white rook
    bool isBlackRook(int square)
    {
        return ((blackRooks) & (1ULL << square)) != 0;
    }; // Check if a square contains a black rook
    bool isWhiteQueen(int square)
    {
        return ((whiteQueens) & (1ULL << square)) != 0;
    }; // Check if a square contains a white queen
    bool isBlackQueen(int square)
    {
        return ((blackQueens) & (1ULL << square)) != 0;
    }; // Check if a square contains a black queen
    bool isWhiteKing(int square)
    {
        return ((whiteKings) & (1ULL << square)) != 0;
    }; // Check if a square contains a white king
    bool isBlackKing(int square)
    {
        return ((blackKings) & (1ULL << square)) != 0;
    };
    bool isOccupied(int square)
    {
        return ((occupiedSquares) & (1ULL << square)) != 0;
        //!= 0 returns it as a boolean instead of numerical value, more accurate
    };
    uint64_t& getOccupiedSquares()
    {
        return occupiedSquares;
    };
    uint64_t& getEmptySquares(){
        return emptySquares;
    }
    uint64_t& getPieces(int color){
        if (color) return getWhitePieces();
        else return getBlackPieces();
    }
    uint64_t& getWhitePieces()
    {
        return whitePieces;
    };
    uint64_t& getBlackPieces()
    {
        return blackPieces;
    };
    uint64_t& getWhitePawn()
    {
        return whitePawns;
    };
    uint64_t& getBlackPawn()
    {
        return blackPawns;
    };
    uint64_t& getWhiteKnight()
    {
        return whiteKnights;
    };
    uint64_t& getBlackKnight()
    {
        return blackKnights;
    };
    uint64_t& getWhiteBishop()
    {
        return whiteBishops;
    };
    uint64_t& getBlackBishop()
    {
        return blackBishops;
    };
    uint64_t& getWhiteRook()
    {
        return whiteRooks;
    };
    uint64_t& getBlackRook()
    {
        return blackRooks;
    };
    uint64_t& getWhiteQueen()
    {
        return whiteQueens;
    };
    uint64_t& getBlackQueen()
    {
        return blackQueens;
    };
    uint64_t& getWhiteKing()
    {
        return whiteKings;
    };
    uint64_t& getBlackKing()
    {
        return blackKings;
    };
    uint64_t& getFriendlyPieces(){
        return friendlyPieces;
    }
    uint64_t& getEnemyPieces(){
        return enemyPieces;
    }
};

class MoveInformation{
    public:
        std::string chessNotation;
        bool isCheck, isCheckMate, isCapture, isAmbiguous, isPromotion, isEnpassant;
        bool isKingCastle, isQueenCastle;
        char toRank, toFile, fromValue;
        Piece pieceType, promotionPiece, capturedPiece;                 //need to convert character of piece to the enumerated integer version

        int toSquare;
        int fromSquare;

        int turn;
        Color playerColor;
        MoveInformation() { //chatGPT fixed
            isCheck = false;
            isCheckMate = false;
            isCapture = false;
            isAmbiguous = false;
            isPromotion = false;
            isEnpassant = false;
            isKingCastle = false;
            isQueenCastle = false;
            toRank = 0;
            toFile = 0;
            fromValue = 0;
            pieceType = none;
            promotionPiece = none;
            capturedPiece = none;
            toSquare = -1;
            fromSquare = -1;
            turn = 0;
            playerColor = white; 
        }
        char getPieceLetter(Piece pieceType) { //CHATGPT'D
            switch (pieceType) {
                case pawn: return 'P';
                case knight: return 'N';
                case bishop: return 'B';
                case rook: return 'R';
                case queen: return 'Q';
                case king: return 'K';
                default: return '~';
            }
        }
        std::string getSquareName(int square) {         //CHATGPT'D
            // This is a simple version assuming the square is represented by an integer
            // You can enhance this if you need to map it more clearly
            char file = 'a' + (square % 8); // 'a' to 'h'
            char rank = '1' + (square / 8); // '1' to '8'
            return std::string(1, file) + std::string(1, rank);
        }   
};


class Game
{
private:
    Color isWhiteTurn;
    //bool isGameOver;
    Board board;
    int turnCount;


public:
    Game(){
        board.initializeBoard();
        boardStates.push_back(board);
        isWhiteTurn = white;
        whiteCastlePrivelege = true;
        blackCastlePrivelege = true;
        turnCount = 0;
    }
    bool whiteCastlePrivelege;
    bool blackCastlePrivelege;
    Board& getBoard(){
        return board;
    }
    int getGameTurnCount(){
        return turnCount;
    }
    std::vector<MoveInformation> moveList; //holds all moves made (game turn - 1 = index)
    std::vector<Board> boardStates; //holds all board states
    Color& getGameTurn(){
        return isWhiteTurn;
    }
    void switchTurns() {
        if (getGameTurn() == white){
            getGameTurn() = black;
            board.updateFriendlyEnemy(white);
            return;
        }
        if (getGameTurn() == black){
            getGameTurn() = white;
            board.updateFriendlyEnemy(black);
            return;
        }
    }


    bool canKingCastle(int color){ //check for if any of the 4 squares of relevance are in check... done later
        if (color == white && (board.getPieceAtSquare(e1) == king) && (board.getPieceAtSquare(h1) == rook) && ((board.getPieceAtSquare(f1) == none)) && (board.getPieceAtSquare(g1) == none) && whiteCastlePrivelege) return true;
        if (color == black && (board.getPieceAtSquare(e8) == king) && (board.getPieceAtSquare(h8) == rook) && ((board.getPieceAtSquare(f8) == none)) && (board.getPieceAtSquare(g8) == none) && blackCastlePrivelege) return true;
        return false;
    }
    bool canQueenCastle(int color){ //check for if any of the 4 squares of relevance are in check... done later 
        if (color == white && (board.getPieceAtSquare(e1) == king) && (board.getPieceAtSquare(a1) == rook) && ((board.getPieceAtSquare(c1) == none)) && (board.getPieceAtSquare(d1) == none) && whiteCastlePrivelege) return true;
        if (color == black && (board.getPieceAtSquare(e8) == king) && (board.getPieceAtSquare(a8) == rook) && ((board.getPieceAtSquare(c8) == none)) && (board.getPieceAtSquare(d8) == none) && blackCastlePrivelege) return true;
        return false;
    }
    void displayBoard()
    {
        /*
            Created 1/29/25 from 3:00 - 4:06PM
            chatGPT was used for the use of the 1ULL << square function

            Want to improve with using unicode symbols for pieces
        */
        char blackPawn = 'p';
        char blackRook = 'r';
        char blackKnight = 'n';
        char blackBishop = 'b';
        char blackQueen = 'q';
        char blackKing = 'k';

        char whitePawn = 'P';
        char whiteRook = 'R';
        char whiteKnight = 'N';
        char whiteBishop = 'B';
        char whiteQueen = 'Q';
        char whiteKing = 'K';

        
        // rank = row, file = col
    for (int rank = 8; rank >= 1; rank--){ // Starts from rank 8 (top) down to rank 1
        for (int file = 1; file <= 8; file++){
            int square = (rank-1) * 8 + file-1;
            if (file == 0) std::cout << (rank + 1) << " ";
            // std::cout<< board.getPieceAtSquare(square);
            // continue;
            if (!board.isOccupied(square))
                    std::cout << '.';
                else if (board.isWhitePawn(square))
                    std::cout << whitePawn;
                else if (board.isWhiteBishop(square))
                    std::cout << whiteBishop;
                else if (board.isWhiteRook(square))
                    std::cout << whiteRook;
                else if (board.isWhiteKnight(square))
                    std::cout << whiteKnight;
                else if (board.isWhiteQueen(square))
                    std::cout << whiteQueen;
                else if (board.isWhiteKing(square))
                    std::cout << whiteKing;

                else if (board.isBlackPawn(square))
                    std::cout << blackPawn;
                else if (board.isBlackBishop(square))
                    std::cout << blackBishop;
                else if (board.isBlackRook(square))
                    std::cout << blackRook;
                else if (board.isBlackKnight(square))
                    std::cout << blackKnight;
                else if (board.isBlackQueen(square))
                    std::cout << blackQueen;
                else if (board.isBlackKing(square))
                    std::cout << blackKing;
        }
            
        std::cout << std::endl;
    }
    std::cout << "*ABCDEFGH\n";
    std::cout << std::endl;
}
    
    std::vector<MoveInformation> generateMovesFromBitboard(uint64_t bitBoard, Piece pieceType, Color color){ //color may be passed implicitly by game variable
        board.updateFriendlyEnemy(color);
        std::vector<MoveInformation> moveListForBoard; 
        uint64_t possibleMask = 0ULL;
        while (bitBoard){
            int fromSquare = __builtin_ctzll(bitBoard);  // finds the LSB that is set to one
            switch(pieceType){
                case pawn:
                    possibleMask= board.getPawnMask(fromSquare,color);
                    break;
                case knight:
                    possibleMask = board.getKnightMask(fromSquare);
                    possibleMask &= ~board.getFriendlyPieces();
                    break;
                case bishop:
                    possibleMask = get_bishop_attacks(fromSquare,board.getOccupiedSquares());
                    possibleMask &= ~board.getFriendlyPieces();
        
                    break;
                case rook:
                    possibleMask = get_rook_attacks(fromSquare,board.getOccupiedSquares());
                    possibleMask &= ~board.getFriendlyPieces();
                  
                    break;
                case queen:
                    possibleMask = get_rook_attacks(fromSquare,board.getOccupiedSquares() ) | get_bishop_attacks(fromSquare,board.getOccupiedSquares());
                    possibleMask &= ~board.getFriendlyPieces();
                    break;
                case king:
                    possibleMask = board.getKingMask(fromSquare);
                    possibleMask &= ~board.getFriendlyPieces();
                    break;
                case none:
                    break;
                
            }
        
            while (possibleMask){ //for each possible destination (set bit in possible mask)
                int destination = __builtin_ctzll(possibleMask); //iterate to the first set bit/piece on the board

                MoveInformation legalMove;
                //generate the attributes of the move
                legalMove.pieceType = pieceType;
                legalMove.fromSquare = fromSquare;
                legalMove.toSquare = destination;

                legalMove.toFile = 'a' + (destination % 8) ;
                legalMove.toRank = '1' + (destination / 8 );
                //now grab information from board at the destination

                //captures
                if (board.getPieceAtSquare(destination) == none){
                    legalMove.isCapture = false;
                }
                else{
                    legalMove.isCapture = true;
                    legalMove.capturedPiece = board.getPieceAtSquare(destination);
                }
                //castling
                if (white) {
                    if ( legalMove.pieceType == king && legalMove.fromSquare == e1 && legalMove.toSquare == h1 ) legalMove.isKingCastle = true;
                    if ( legalMove.pieceType == king && legalMove.fromSquare == e1 && legalMove.toSquare == a1 ) legalMove.isQueenCastle = true;
                }
                else{
                    if ( legalMove.pieceType == king && legalMove.fromSquare == e8 && legalMove.toSquare == h8 ) legalMove.isKingCastle = true;
                    if ( legalMove.pieceType == king && legalMove.fromSquare == e8 && legalMove.toSquare == a8 ) legalMove.isQueenCastle = true;
                }

                //promotions
                if (white) { 
                    if ( pieceType == pawn && (destination >= a8 && destination <= h8) ){
                        legalMove.isPromotion = true;
                    }
                }
                else{  
                    if ( pieceType == pawn && (destination >= a1 && destination <= h1) ){
                        legalMove.isPromotion = true;
                    }
                }
                
                //en passant
                if (pieceType == pawn && legalMove.isCapture && legalMove.toSquare == board.enPassantTargetSquare) legalMove.isEnpassant = true;

                if (legalMove.isPromotion){                                 //add move for each legal move added --> need to create a check for check and checkmate
                    legalMove.pieceType = bishop;
                    moveListForBoard.push_back(legalMove);
                    legalMove.pieceType = rook;
                    moveListForBoard.push_back(legalMove);
                    legalMove.pieceType = queen;
                    moveListForBoard.push_back(legalMove);
                    legalMove.pieceType = knight;
                    moveListForBoard.push_back(legalMove);
                }
                else{
                moveListForBoard.push_back(legalMove);
                }
                
                possibleMask &= (possibleMask - 1); //clear the bit as it is done with
            }

            bitBoard &= (bitBoard - 1); //clears the LSB.. cool trick!
        }
        return moveListForBoard;

    }

    std::vector<MoveInformation> generateLegalMoves(int color){ // --> need to add castles
        std::vector<MoveInformation> allLegalMoves;
        std::vector<MoveInformation> movesToAdd;
        if (color == white){
            movesToAdd = generateMovesFromBitboard(board.getWhitePawn(),pawn,white);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd = generateMovesFromBitboard(board.getWhiteKnight(),knight,white);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd = generateMovesFromBitboard(board.getWhiteBishop(),bishop,white);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd = generateMovesFromBitboard(board.getWhiteRook(),rook,white);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd = generateMovesFromBitboard(board.getWhiteQueen(),queen,white);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd = generateMovesFromBitboard(board.getWhiteKing(),king,white);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd.clear();
            if (canKingCastle(white)){
                movesToAdd.push_back(parseMove(white, "O-O"));
            }
            if (canQueenCastle(white)){
                movesToAdd.push_back(parseMove(white, "O-O-O"));
            }
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end()); // adding possible castles
        }
        else{
            movesToAdd = generateMovesFromBitboard(board.getBlackPawn(),pawn,black);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd = generateMovesFromBitboard(board.getBlackKnight(),knight,black);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd = generateMovesFromBitboard(board.getBlackBishop(),bishop,black);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd = generateMovesFromBitboard(board.getBlackRook(),rook,black);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd = generateMovesFromBitboard(board.getBlackQueen(),queen,black);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd = generateMovesFromBitboard(board.getBlackKing(),king,black);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd.clear();
            if (canKingCastle(black)){
                movesToAdd.push_back(parseMove(black, "O-O"));
            }
            if (canQueenCastle(black)){
                movesToAdd.push_back(parseMove(black, "O-O-O"));
            }
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end()); // adding possible castles
        }
        return allLegalMoves;
    }
    bool isLegalMove(MoveInformation moveOfInterest){
        std::vector<MoveInformation> legalMoves = generateLegalMoves(moveOfInterest.playerColor);
        std::cout << std::endl <<std::endl;
        std::string moveString;
        for (int i = 0; i < legalMoves.size() ; i++){
            moveString = getMoveString(legalMoves.at(i));
            std::cout << moveString << std::endl;
        }
        if (moveIndexInLegalList(legalMoves,moveOfInterest) >= 0){
            return true;
        }
        
        return false;
        
    }
    
    int moveIndexInLegalList(const std::vector<MoveInformation>& moveList, const MoveInformation& targetMove) { //this method is chatGPT'd
        //finds the index of the matching move (based off chess notation). This will let me access the possibleMoveList and obtain the move and play it
        for (int i = 0; i < moveList.size(); i++) {
            // std::cout << getMoveString(targetMove) << " vs " << getMoveString(moveList.at(i)) << std::endl; testing move find
            if(targetMove.chessNotation == getMoveString(moveList.at(i)) ){
                // std::cout << targetMove.fromSquare << " vs " << moveList.at(i).fromSquare; comparing attributes
                
                return i;
            }
        }
        return -1; //no index for move in list
    }
    
    MoveInformation parseMove(Color playerColor, std::string chessNotation)
    {
        std::string lineArg;
        std::getline(std::cin, lineArg);
        std::string moveStr = lineArg; // will be editing this

        MoveInformation move;

        move.chessNotation = lineArg;
        // chatGPT suggested rewriting the creation of the specialty flags in this concise format.
        move.isCapture = moveStr.find('x') != std::string::npos;
        move.isPromotion = moveStr.find('=') != std::string::npos;
        move.isCheck = moveStr.find('+') != std::string::npos;
        move.isCheckMate = moveStr.find('#') != std::string::npos;
        move.isKingCastle = (moveStr == "O-O"); 
        move.isQueenCastle = (moveStr == "O-O-O");

        if (move.isKingCastle){
            if (playerColor == white){
                move.fromSquare = e1;
                move.toSquare = g1;
                return move;
            }
            else if (playerColor == black){
                move.fromSquare = e8;
                move.toSquare = g8;
                return move;
            }
        }
        else if (move.isQueenCastle){
            if (playerColor == white){
                move.fromSquare = e1;
                move.toSquare = c1;
                return move;
            }
            else if (playerColor == black){
                move.fromSquare = e8;
                move.toSquare = c8;
                return move;
            }
        }
        
        move.isAmbiguous = false;
        // std::cout << moveStr << std::endl;
        switch (moveStr.at(0)){ //piece type

            case 'B':
                move.pieceType = bishop;
                moveStr = moveStr.substr(1);
                break;
            case 'N':
                move.pieceType = knight;
                moveStr = moveStr.substr(1);
                break;
            case 'R':
                move.pieceType = rook;
                moveStr = moveStr.substr(1);
                break;
            case 'K':
                move.pieceType = king;
                moveStr = moveStr.substr(1);
                break;
            case 'Q':
                move.pieceType = queen;
                moveStr = moveStr.substr(1);
                break;
            default:
                if ( (moveStr.at(0) >= 'a' && moveStr.at(0) <= 'h') || (moveStr.at(0) >= '1' && moveStr.at(0) <= '8') ){
                    move.pieceType = pawn;
                }
                else{
                    move.pieceType= none;
                }
                break;
        }

        // std::cout << moveStr << std::endl;

        if (move.isCheck || move.isCheckMate)
        {
            moveStr = moveStr.substr(0, moveStr.length() - 1);
        }
        // std::cout << moveStr << std::endl;
        if (move.isPromotion)
        {
            switch (moveStr.at(moveStr.length() - 1) ){
                case 'B':
                    move.promotionPiece = bishop;
                    break;
                case 'N':
                    move.promotionPiece = knight;
                    break;
                case 'R':
                    move.promotionPiece = rook;
                    break;
                case 'Q':
                    move.promotionPiece = queen;
                    break;
    
            }
            moveStr = moveStr.substr(0, moveStr.length() - 2); // remove the last 2 characters because promotion = '=X' where X is piece
        }

        //std::cout << moveStr << std::endl;
        // the last thing in the move string at this point must be the "to" information
        move.toRank = moveStr.at(moveStr.length() - 1);
        move.toFile = moveStr.at(moveStr.length() - 2);
        moveStr = moveStr.substr(0, moveStr.length() - 2);
        // std::cout << moveStr << std::endl;
        if (move.isCapture)
        {
            moveStr = moveStr.substr(0, moveStr.length() - 1);
            // thing before to square, if capture is 'x'
        }
        // std::cout << moveStr << std::endl;
        //std::cout << "boop";
        if (moveStr.length() == 1 && ( (moveStr.at(0) >= 'a' && moveStr.at(0) <= 'h') || (moveStr.at(0) >= '1' && moveStr.at(0) <= '8') )) 
        {
            move.isAmbiguous = true;
            move.fromValue = moveStr.at(0);
        }
    
        // std::cout << moveStr << std::endl;

        // square = File * 8 + Rank
        // a = 1
        // rank = numbers, File == files
        
        int asciiFile = static_cast<int>(move.toFile) - 97; //SHOULD BE 97 FOR 0 INDEX SYSTEM!!!!!
        move.toSquare = (asciiFile - 1) + 8 * (move.toRank - '0' - 1); // casting both rank and file to integers and calculating the square index
        //std::cout << "TESTING: " << asciiFile << ", " << (move.toRank - '0') << ", " << move.toSquare << std::endl;
        return move;
    }
    //bool isGameOver(){}
    void makeMove(MoveInformation move){                    //DONE FOR RIGHT NOW... MAY NEED LATER TESTING ONCE GAME IS IN FULL EFFECT
        //assuming the move passed in is from the generated legal move list, NOT user data... (important for From data)

        //this is ensured to be legal so responsibility of function is to only update the board
        Color enemyColor;
        if (move.playerColor == white){
            enemyColor = black; 
        }
        if (move.playerColor == black){
            enemyColor = white; 
            
        }
    

        //CAPTURES/ENPASSANT
        if (move.isCapture){
            if (move.isEnpassant){
                //place piece at en passant square...

                board.addPiece(move.playerColor,move.pieceType,move.toSquare); //to square IS en passant square
                //remove pawn that was passant'd
                int pawnToBeRemoved;
                //color determines if the piece that jumped is above or behind the pawn
                if (enemyColor == black) pawnToBeRemoved = move.toSquare + 8;
                else pawnToBeRemoved = move.toSquare - 8;

                board.removePiece(move.playerColor,pawn, pawnToBeRemoved);
      
                //remove piece from origin
                board.removePiece(move.playerColor,move.pieceType,move.fromSquare); 
            }
            else{ //traditional captures
              
                board.removePiece(enemyColor,move.capturedPiece,move.toSquare); //remove the captured piece
                //move the player's piece
                board.removePiece(move.playerColor,move.pieceType,move.fromSquare); // "pick it up"
                board.addPiece(move.playerColor,move.pieceType,move.toSquare); // "place it down"
            }
        }
        //CASTLING
        else if (move.isKingCastle){                                    //could be rewritten to be less explicit and more moduralized --> future update!
            if (move.playerColor == white){
                board.removePiece(white,king,e1); // "pick it up"             
                board.addPiece(white,king,g1); // "place it down"

                board.removePiece(white,rook,h1);          
                board.addPiece(white,rook,f1); 
            }
            else{
                board.removePiece(black,king,e8);              
                board.addPiece(black,king,g8); 
                
                board.removePiece(black,rook,h8);          
                board.addPiece(black,rook,f8); 
            }
        }
        else if (move.isQueenCastle){
            if (move.playerColor == white){
                board.removePiece(white,king,e1); // "pick it up"             
                board.addPiece(white,king,c1); // "place it down"

                board.removePiece(white,rook,a1);          
                board.addPiece(white,rook,d1); 
            }
            else{
                board.removePiece(white,king,e8);           
                board.addPiece(white,king,c8);

                board.removePiece(white,rook,a8);          
                board.addPiece(white,rook,d8); 
            }
        }
        //PROMOTIONS
        if (move.isPromotion){
            board.removePiece(move.playerColor,pawn,move.fromSquare); //all promotions are pawn piece type
            board.addPiece(move.playerColor,move.promotionPiece,move.toSquare);
        }
        
        //regular moves
        if (move.playerColor == white){
            getBoard().addPiece(white,move.pieceType,move.toSquare);
            // displayBoard();
            getBoard().removePiece(white,move.pieceType,move.fromSquare);
            // displayBoard();
        }
        else{
            getBoard().addPiece(black,move.pieceType,move.toSquare);
            //displayBoard();
            getBoard().removePiece(black,move.pieceType,move.fromSquare);
            //displayBoard();
        }
    
    }
        
    void takeGameHalfTurn(Color turn){                                                         // NEED TO EXTRACT USER INPUTTING FROM THE PARSEMOVE FUNCTION...

        std::cout << "Turn " << getGameTurnCount() + 1;
        std::string gameMoveString;
        if (turn == white) std::cout << "W. ";
        else std::cout << "B. ";
        
        std::cout << "Please make your move! (standard chess notation): ";
        std::string userInputtedNotation;

        std::vector<MoveInformation> possibleLegalMoves = generateLegalMoves(turn);

        MoveInformation userInput = parseMove(turn,userInputtedNotation);
        int moveIndex = moveIndexInLegalList(possibleLegalMoves,userInput) ;
        while (moveIndex == -1){ //while move is not in legal move list
            std::cout<< "Illegal Move! Please try another move!\n";
            userInput = parseMove(turn,userInputtedNotation );
        }
        
        MoveInformation matchingMove;
        matchingMove = possibleLegalMoves.at(moveIndex);
        makeMove(matchingMove);
    }
    std::string getMoveString(MoveInformation move)
    {
        std::stringstream ss;
        if (!(move.getPieceLetter(move.pieceType) == 'P') ) ss << move.getPieceLetter(move.pieceType);
        
        if (move.isAmbiguous) ss << move.fromValue;
        
        if (move.isCapture) ss << 'x';
        
        ss << move.toFile << move.toRank;
        if (move.isPromotion) ss << "=" << move.getPieceLetter(move.promotionPiece);

        if (move.isCheck) ss << "+";
        if (move.isCheckMate) ss << "#";
        return ss.str();
    }
    
};


