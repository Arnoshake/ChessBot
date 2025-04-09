#include <iostream>
#include<stdio.h>
#include <string>  // included for chess notation
#include <bitset>  //included for decimal to binary conversion
#include <sstream> //included for move print
#include <cstdint>
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
#define set_bit(bitboard, square) (bitboard |= (1ULL << square))
#define pop_bit(bitboard, square) (get_bit(bitboard, square) ? (bitboard ^= (1ULL << square)) : 0)
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
class Board
{
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
    // handleInput
    std::string playerMove;

    // properNotation
    std::string originSq;
    std::string action;
    std::string destinationSq;
    std::string moveAttribute;

    // 1= white, -1 = black
    int gameTurn;

public:
    Board();

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
    };

/*
    uint64_t generateRookMoves(uint64_t rookBitboard, uint64_t occupiedSquares){ 
    uint64_t rookMovesBoard = 0ULL;
    for (int sq = 0; sq < 64 ; sq ++){
        if (isSet(rookBitboard,sq)){
            rookMovesBoard |= get_rook_attacks(sq,occupiedSquares);
        }
    }
    return rookMovesBoard;
}
    uint64_t generateBishopMoves(uint64_t bishopBitboard, uint64_t occupiedSquares){
    uint64_t bishopMovesBoard = 0ULL;
    for (int sq = 0; sq < 64 ; sq ++){
        if (isSet(bishopBitboard,sq)){
            bishopMovesBoard |= get_bishop_attacks(sq,occupiedSquares);
        }
    }
    return bishopMovesBoard;
}
    uint64_t generateQueenMoves(uint64_t queenBitboard, uint64_t occupiedSquares){
    uint64_t queenMovesBoard = 0ULL;
    queenMovesBoard = generateBishopMoves(queenBitboard, occupiedSquares) | generateRookMoves(queenBitboard, occupiedSquares);
    
    return queenMovesBoard;
}
    uint64_t generateKnightMoves(uint64_t knightBitBoard){
    /*
        15  17
     6         10
           x
     10         6
        17  15
    */
    
    //seperate board for knight moves and condition checking. If 1 was used, cascading shifting would happen... bad!
   /*
    uint64_t bitboard = knightBitBoard;
    uint64_t knightMoves = 0ULL;

    //prevents leftside overflow
    uint64_t notAFile =   0xFEFEFEFEFEFEFEFEULL;
    uint64_t notABFile =  0xFCFCFCFCFCFCFCFCULL;
    //prevents rightside overflow
    uint64_t notHFile =   0x7F7F7F7F7F7F7F7FULL;
    uint64_t notGHFile =  0x3F3F3F3F3F3F3F3FULL;

    //bitboard is a representation of a number, so if a value is higher or lower than the range of the representation (64 bits), then it will automatically not be shown --> border issues do not matter
    

    
    //checks the mapping before
    
    knightMoves |= (bitboard & notAFile) >> 17;
    knightMoves |= (bitboard & notHFile) >> 15;
    knightMoves |= (bitboard & notABFile) >> 10; 
    knightMoves |= (bitboard & notGHFile) >> 6;
    
    knightMoves |= (bitboard & notHFile) << 17;
    knightMoves |= (bitboard & notAFile) << 15;
    knightMoves |= (bitboard & notGHFile) << 10;
    knightMoves |= (bitboard & notABFile) << 6;
   

    return knightMoves;
}
    */
/* COMMENTING OUT FOR TESTING OF OTHER METHODS!
    uint64_t getRelevantMoveMask(int fromSquare, char pieceType, bool isWhiteTurn ){
        // Completed on 4/7/25
        // returns the relevant map for a given square.
        init_sliders_attacks(1); //for bishops
        init_sliders_attacks(0); //for rooks


        pieceType = tolower(pieceType);
        uint64_t possibleMoves = 0x0000000000000000ULL;

        uint64_t occupiedSquares = getOccupiedSquares();
        uint64_t friendlyOccupiedSquares;
        uint64_t enemyOccupiedSquares;
        if (isWhiteTurn){
            friendlyOccupiedSquares = getWhitePieces();
            enemyOccupiedSquares = getBlackPieces();

        }
        else{
            friendlyOccupiedSquares = getBlackPieces();
            enemyOccupiedSquares = getWhitePieces();
        }

        switch(pieceType){
            //NEED TO DO PAWN AND KING BECAUSE THEIR MOVES DEPEND ON OCCUPIED
            case 'p': //pawn
                possibleMoves = getPawnMask(fromSquare, isWhiteTurn);
                possibleMoves &= ~occupiedSquares; //only keeps possible moves not blocked by existing
                
                uint64_t possibleCaptures = getPawnCaptures(fromSquare, isWhiteTurn);
                possibleCaptures &= enemyOccupiedSquares;

                possibleMoves |= possibleCaptures;
                break;
                
            case 'k': //king
                possibleMoves = getKingMask(fromSquare);
                possibleMoves &= ~friendlyOccupiedSquares;
                break;
            case 'n': //knight
                possibleMoves = getKnightMask(fromSquare);
                possibleMoves &= ~friendlyOccupiedSquares;
                break;
            case 'r': //rook
                possibleMoves = get_rook_attacks(fromSquare, occupiedSquares );
                possibleMoves &= ~friendlyOccupiedSquares; // removes friendly pieces from possible, blocking captures
                break;
            case 'b': //bishop
                possibleMoves = get_bishop_attacks(fromSquare, occupiedSquares );
                possibleMoves &= ~friendlyOccupiedSquares; // removes friendly pieces from possible, blocking captures
                break;
            case 'q': //queen
                possibleMoves = ( getRookMask(fromSquare) | getBishopMask(fromSquare) );  //all possible directional moves
                possibleMoves &= ~friendlyOccupiedSquares;
                break;
            
        }
        return possibleMoves;
    }
    
    //Should be passing bitboard in its entirity instead of a square... logic should be vectorized, removes loops
    uint64_t getPossibleMovesMask(int fromSquare, char pieceType, bool isWhiteTurn){
        init_sliders_attacks(1); //for bishops
        init_sliders_attacks(0); //for rooks

        uint64_t pieceMask = getRelevantMoveMask(fromSquare, pieceType, isWhiteTurn);
        uint64_t friendlyOccupied;
        if (isWhiteTurn) friendlyOccupied = getWhitePieces();
        else{
            friendlyOccupied = getBlackPieces();
        }
        uint64_t possibleMoves = ( pieceMask & (~friendlyOccupied) ); //removes the ability to capture friendly pieces
    }
   
   
   */
    //1 billion helper and bool methods
    bool isWhiteTurn(){
        return (gameTurn == 1);
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
    uint64_t getOccupiedSquares()
    {
        return occupiedSquares;
    };
    uint64_t getEmptySquares(){
        return emptySquares;
    }
    uint64_t getPieces(int color){
        if (color) return getWhitePieces();
        else return getBlackPieces();
    }
    uint64_t getWhitePieces()
    {
        return whitePieces;
    };
    uint64_t getBlackPieces()
    {
        return blackPieces;
    };
    uint64_t getWhitePawn()
    {
        return whitePawns;
    };
    uint64_t getBlackPawn()
    {
        return blackPawns;
    };
    uint64_t getWhiteKnight()
    {
        return whiteKnights;
    };
    uint64_t getBlackKnight()
    {
        return blackKnights;
    };
    uint64_t getWhiteBishop()
    {
        return whiteBishops;
    };
    uint64_t getBlackBishop()
    {
        return blackBishops;
    };
    uint64_t getWhiteRook()
    {
        return whiteRooks;
    };
    uint64_t getBlackRook()
    {
        return blackRooks;
    };
    uint64_t getWhiteQueen()
    {
        return whiteQueens;
    };
    uint64_t getBlackQueen()
    {
        return blackQueens;
    };
    uint64_t getWhiteKing()
    {
        return whiteKings;
    };
    uint64_t getBlackKing()
    {
        return blackKings;
    };
};
class Game
{
private:
    bool isWhiteTurn;
    //bool isGameOver;
    Board board;

    struct MoveInformation
    {

        std::string chessNotation;
        bool isCheck, isCheckMate, isCapture, isAmbiguous, isPromotion;
        bool isKingCastle, isQueenCastle;
        char toRank, toFile, fromValue;
        char pieceType, promotionPiece;
        

        int toSquare;
        int fromSquare;
        int turn;
    };
    MoveInformation move;

public:
    Game();
    void switchTurns() {};
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
        char whiteKnight = 'K';
        char whiteBishop = 'B';
        char whiteQueen = 'Q';
        char whiteKing = 'K';

        std::cout << "*ABCDEFGH\n";
        // rank = row, file = col
        for (int rank = 7; rank >= 0; rank--)
        { // Starts from rank 8 (top) down to rank 1
            for (int file = 0; file < 8; file++)
            {
                int square = rank * 8 + file;
                if (file == 0)
                    std::cout << (rank + 1);
                /*
                chatGPT'd to get "1ULL << square"
                    creates a mask that puts a 1 at the bit value of the value of square
                    so that it can then compare. They will overlay
                    10000001 & 1000000 = 1
                */
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
    }

    MoveInformation parseMove()
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
        move.isAmbiguous = false;
        // std::cout << moveStr << std::endl;
        if (moveStr.at(0) == 'B' || moveStr.at(0) == 'N' || moveStr.at(0) == 'R' || moveStr.at(0) == 'K' || moveStr.at(0) == 'Q')
        {
            move.pieceType = moveStr.at(0);
            moveStr = moveStr.substr(1);
        }
        else
        {
            move.pieceType = 'P';
        }

        // std::cout << moveStr << std::endl;

        if (move.isCheck || move.isCheckMate)
        {
            moveStr = moveStr.substr(0, moveStr.length() - 1);
        }
        // std::cout << moveStr << std::endl;
        if (move.isPromotion)
        {
            move.promotionPiece = moveStr.at(moveStr.length() - 1);
            moveStr = moveStr.substr(0, moveStr.length() - 2); // remove the last 2 characters because promotion = '=X' where X is piece
        }
        std::cout << moveStr << std::endl;
        // the last thing in the move string at this point must be the to information
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
        if (moveStr.length() == 1)
        {
            // if anything is left, it must be ambigiuous data
            move.isAmbiguous = true;
            move.fromValue = moveStr.at(0);
            std::cout << move.fromValue << std::endl;
        }
        // std::cout << moveStr << std::endl;

        // square = File * 8 + Rank
        // a = 1
        // rank = numbers, File == files
        int asciiFile = static_cast<int>(move.toFile) - 97; //SHOULD BE 97 FOR 0 INDEX SYSTEM!!!!!

        move.toSquare = (asciiFile - 1) + 8 * (move.toRank - '0' - 1); // casting both rank and file to integers and calculating the square index
        std::cout << "TESTING: " << asciiFile << ", " << (move.toRank - '0') << ", " << move.toSquare << std::endl;
        return move;
    }

    void printMove(MoveInformation move)
    {
        int asciiRank = static_cast<int>(move.toRank) - 98;
        // above for testing

        std::stringstream ss;
        ss << "MOVE INFORMATION" << std::endl;
        ss << "Chess Notation: " << move.chessNotation << std::endl
           << "Piece: " << move.pieceType << std::endl;
        if (move.isAmbiguous)
            ss << "From: " << move.fromValue << std::endl;
        ss << "To: " << move.toFile << move.toRank << "  (" << move.toSquare << ") | " << std::endl;

        ss << std::endl
           << std::endl
           << "MOVE STATUS" << std::endl;
        ss << "isAmbiguous: " << move.isAmbiguous << std::endl
           << "isCapture: " << move.isCapture << std::endl
           << "isCheck: " << move.isCheck << std::endl
           << "isCheckMate: " << move.isCheckMate << std::endl
           << std::endl;
        ss << "isKingCastle: " << move.isKingCastle << std::endl
           << "isQueenCastle: " << move.isQueenCastle << std::endl;
        ss << "isPromotion: " << move.isPromotion << std::endl;
        if (move.isPromotion)
        {
            ss << "Promotion Piece: " << move.promotionPiece << std::endl;
        }
        std::cout << ss.str();
    }

};
