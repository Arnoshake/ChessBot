#include <iostream>
#include<stdio.h>
#include <string>  // included for chess notation
#include <bitset>  //included for decimal to binary conversion
#include <sstream> //included for move print
#include <cstdint>
#include <vector>
#include <cstdlib> //std::exit
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
    if(square < 0 || square >= 64) throw std::out_of_range("Invalid square"); 
    (bitboard |= (1ULL << square));
}
// void pop_bit(uint64_t& bitboard, int square){
//     get_bit(bitboard, square) ? (bitboard ^= (1ULL << square)) : 0; //not my code... provided in Rook magic stuff
// }
void pop_bit(uint64_t& bitboard, int square){ //AI GEMINI... ternary leads to it acting as a bit toggle
    if(square < 0 || square >= 64) throw std::out_of_range("Invalid square");
    if (get_bit(bitboard, square)) {
        bitboard ^= (1ULL << square);
    }
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
        if (index & (1 << count))                                                                                           //MAY NEED TO BE 1ULL
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
        int occupancy_variations = 1 << bit_count;                                                          //may need to be 1ULL
        
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
    if(square < 0 || square >= 64) throw std::out_of_range("Invalid square");
    return (bitboard & (1ULL << square) ) != 0 ;
}

const uint64_t notAFile =   0xFEFEFEFEFEFEFEFEULL;
const uint64_t notABFile =  0xFCFCFCFCFCFCFCFCULL;
const uint64_t notHFile =   0x7F7F7F7F7F7F7F7FULL;
const uint64_t notGHFile =  0x3F3F3F3F3F3F3F3FULL;

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
inline Color operator!(Color c) { //GPT'd... allows me to do !white = black
    return static_cast<Color>(-static_cast<int>(c));
}

enum Piece {
    none , pawn, bishop, knight, rook, queen, king
    
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

class MoveInformation{
    public:
        std::string chessNotation;
        bool isCheck, isCheckMate, isCapture, isAmbiguous, isPromotion, isEnpassant;
        bool isKingCastle, isQueenCastle;
        char toRank, toFile;
        Piece pieceType, promotionPiece, capturedPiece;             

        Square toSquare;
        Square fromSquare;

        char fromFile, fromRank; //used for ambiguity
        bool uniqueFile, uniqueRank;
        int turn;
        Color playerColor;

       
        


        MoveInformation() { //chatGPT fixed                                 //
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
            fromFile = '\0';
            uniqueFile = false;
            fromRank = '\0';
            uniqueRank = false;
            pieceType = none;
            promotionPiece = none;
            capturedPiece = none;
            toSquare = NO_SQUARE;
            fromSquare = NO_SQUARE;
            turn = 0;
            playerColor = white; 
        }

        MoveInformation(Piece pieceType1, bool isCapture1, Piece capturedPiece1, bool isAmbiguous1, bool isEnpassant1, bool isPromotion1, Piece promotionPiece1){}
//         MoveInformation(Color color, Piece piece, int from, int to,Piece capturedPieceParam,bool capture = true) {
//     playerColor = color;
//     pieceType = piece;
//     fromSquare = from;
//     toSquare = to;
//     isCapture = capture;
//     isCheck = false;
//     isCheckMate = false;
//     isPromotion = false;
//     isEnpassant = false;
//     isKingCastle = false;
//     isQueenCastle = false;
//     promotionPiece = capturedPieceParam;
//     capturedPiece = none;
// }


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

        void printMoveInfo() {
    std::cout << "=== Move Information ===\n";
    std::cout << "Turn: " << turn << " | Player: " << (playerColor == white ? "White" : "Black") << "\n";
    std::cout << "Piece Moved: " << getPieceLetter(pieceType) << "\n";
    std::cout << "From: " << getSquareName(fromSquare) << " (" << fromSquare << ")\n";
    std::cout << "To: " << getSquareName(toSquare) << " (" << toSquare << ")\n";
    std::cout <<"Flags ( [capture][promotion][enPassant][check][checkmate][ambig]):" << isCapture << isPromotion << isEnpassant << isCheck << isCheckMate <<isAmbiguous << "\n";
    if (isCapture) {
        std::cout << "Captured: " << getPieceLetter(capturedPiece) << "\n";
    }

    if (isPromotion) {
        std::cout << "Promotion to: " << getPieceLetter(promotionPiece) << "\n";
    }

    if (isEnpassant) std::cout << "Special: En Passant\n";
    if (isKingCastle) std::cout << "Special: Kingside Castle\n";
    if (isQueenCastle) std::cout << "Special: Queenside Castle\n";
    if (isCheck) std::cout << "Check: Yes\n";
    if (isCheckMate) std::cout << "Checkmate: Yes\n";
    if (isAmbiguous) std::cout << "Note: Move was ambiguous | " << fromFile <<  ", " <<fromRank<<".\n";

    std::cout << "Algebraic Notation: " << chessNotation << "\n";
    std::cout << "========================\n";
}
    };

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
    bool whiteCastlePrivelege; //tracks over all ability to castle
    bool whiteKingCastleRights; //potential to castle king side
    bool whiteQueenCastleRights; // potential ''
    bool whiteCanKingCastle; //if can castle this turn
    bool whiteCanQueenCastle; // ''
    bool whiteHasCastled;
    
    bool blackCastlePrivelege;
    bool blackKingCastleRights;
    bool blackQueenCastleRights;
    bool blackCanKingCastle;
    bool blackCanQueenCastle;
    bool blackHasCastled;
    
    Square whiteKingPosition;
    Square blackKingPosition;
    
public:
    bool operator==(const Board& other) const {
        return occupiedSquares == other.occupiedSquares &&
            whitePawns == other.whitePawns &&
            blackPawns == other.blackPawns &&
            whiteKnights == other.whiteKnights &&
            blackKnights == other.blackKnights &&
            whiteBishops == other.whiteBishops &&
            blackBishops == other.blackBishops &&
            whiteRooks == other.whiteRooks &&
            blackRooks == other.blackRooks &&
            whiteQueens == other.whiteQueens &&
            blackQueens == other.blackQueens &&
            whiteKings == other.whiteKings &&
            blackKings == other.blackKings &&
            emptySquares == other.emptySquares &&
            enPassantTargetSquare == other.enPassantTargetSquare &&
                whiteCastlePrivelege == other.whiteCastlePrivelege &&
                blackCastlePrivelege == other.blackCastlePrivelege &&
                whiteKingCastleRights == other.whiteKingCastleRights &&
                whiteQueenCastleRights == other.whiteQueenCastleRights &&
                blackKingCastleRights == other.blackKingCastleRights &&
                blackQueenCastleRights == other.blackQueenCastleRights;


            //    halfmove_clock == other.halfmove_clock &&
            //    fullmove_number == other.fullmove_number;
    }
    void printCastlingStats(){
        std::cout << "=== Castling Status ===" << std::endl;
        std::cout << "White Castle Privilege:     " << (whiteCastlePrivelege ? "true" : "false") << std::endl;
        std::cout << "White King Castle Rights:   " << (whiteKingCastleRights ? "true" : "false") << std::endl;
        std::cout << "White Queen Castle Rights:  " << (whiteQueenCastleRights ? "true" : "false") << std::endl;
        std::cout << "White Can King Castle Now:  " << (whiteCanKingCastle ? "true" : "false") << std::endl;
        std::cout << "White Can Queen Castle Now: " << (whiteCanQueenCastle ? "true" : "false") << std::endl;
        std::cout << "White Has Castled:          " << (whiteHasCastled ? "true" : "false") << std::endl;

        std::cout << "Black Castle Privilege:     " << (blackCastlePrivelege ? "true" : "false") << std::endl;
        std::cout << "Black King Castle Rights:   " << (blackKingCastleRights ? "true" : "false") << std::endl;
        std::cout << "Black Queen Castle Rights:  " << (blackQueenCastleRights ? "true" : "false") << std::endl;
        std::cout << "Black Can King Castle Now:  " << (blackCanKingCastle ? "true" : "false") << std::endl;
        std::cout << "Black Can Queen Castle Now: " << (blackCanQueenCastle ? "true" : "false") << std::endl;
        std::cout << "Black Has Castled:          " << (blackHasCastled ? "true" : "false") << std::endl;

        std::cout << "==========================" << std::endl;
    }
   
    int enPassantTargetSquare; //probably should be private but shouldnt be modified as it will be passed in board creation
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

        friendlyPieces = 0ULL;
        enemyPieces = 0ULL;

        whitePieces = whitePawns | whiteBishops | whiteRooks | whiteKnights | whiteKings | whiteQueens;
        blackPieces = blackPawns | blackBishops | blackRooks | blackKnights | blackKings | blackQueens;
        occupiedSquares = whitePieces | blackPieces;
        emptySquares = ~occupiedSquares;
        
        enPassantTargetSquare = -1;

        whiteCastlePrivelege = true;
        whiteKingCastleRights = true;
        whiteQueenCastleRights = true;
        whiteHasCastled = false;

        blackCastlePrivelege = true;
        blackKingCastleRights = true;
        blackQueenCastleRights = true;
        blackHasCastled = false;

        whiteCanKingCastle = false;
        whiteCanQueenCastle = false;
        blackCanKingCastle = false;
        blackCanQueenCastle = false;

        
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
        friendlyPieces = whitePieces;
        enemyPieces = blackPieces;
        enPassantTargetSquare = -1;

        whiteKingPosition = e1;
        blackKingPosition = e8;
    
    };
    void clearBoard(){
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
        friendlyPieces = whitePieces;
        enemyPieces = blackPieces;
        enPassantTargetSquare = -1;

        whiteKingPosition = NO_SQUARE;
        blackKingPosition = NO_SQUARE;
        
    }
    
    //MANIPULATING BOARD
    void addPiece(Color color,Piece piece, Square square){
            switch(piece){
                case pawn:
                    set_bit(getPawns(color), square);
                    set_bit(getPieces(color),square);
                    break;
                case bishop:
                    set_bit(getBishops(color), square);
                    set_bit(getPieces(color),square);
                    break;
                case knight:
                    set_bit(getKnights(color), square);
                    set_bit(getPieces(color),square);
                    break;
                case rook:
                    set_bit(getRooks(color), square);
                    set_bit(getPieces(color),square);
                    break;
                case queen:
                    set_bit(getQueens(color), square);
                    set_bit(getPieces(color),square);
                    break;
                case king:
                    set_bit(getKing(color), square);
                    set_bit(getPieces(color),square);
                    if (color == white) whiteKingPosition = square;
                    else blackKingPosition = square;
                    break;
                case none:
                default:
                    break;
            }
        set_bit(getOccupiedSquares(),square);
        getEmptySquares() = ~getOccupiedSquares();
    }
    void removePiece(Color color, Piece piece, Square square){
        switch(piece){
            case pawn:
                pop_bit(getPawns(color), square);
                pop_bit(getPieces(color),square);
                break;
            case bishop:
                pop_bit(getBishops(color), square);
                pop_bit(getPieces(color),square);
                break;
            case knight:
                pop_bit(getKnights(color), square);
                pop_bit(getPieces(color),square);
                break;
            case rook:
                pop_bit(getRooks(color), square);
                pop_bit(getPieces(color),square);
                break;
            case queen:
                pop_bit(getQueens(color), square);
                pop_bit(getPieces(color),square);
                break;
            case king:
                pop_bit(getKing(color), square);
                pop_bit(getPieces(color),square);
                if (color == white) whiteKingPosition = NO_SQUARE;
                else blackKingPosition = NO_SQUARE;
                break;
            case none:
            default:
                break;
            }
            
        
        pop_bit(getOccupiedSquares(),square);
        getEmptySquares() = ~getOccupiedSquares();
    }
    void setPieceAtSquare(Square targetSquare, Piece pieceType, Color colorOfPieceBeingMoved){
        uint64_t mask = 1ULL << targetSquare;
        switch(pieceType){
            case pawn:
                getPawns(colorOfPieceBeingMoved) |= mask;
                break;
            case bishop:
                getBishops(colorOfPieceBeingMoved) |= mask;
                break;
            case knight:
                getKnights(colorOfPieceBeingMoved) |= mask;
                break;
            case rook:
                getRooks(colorOfPieceBeingMoved) |= mask;
                break;
            case queen:
                getQueens(colorOfPieceBeingMoved) |= mask;
                break;
            case king:
                getKing(colorOfPieceBeingMoved) |= mask;
                if (colorOfPieceBeingMoved == white) whiteKingPosition = targetSquare;
                else blackKingPosition = targetSquare;
                break;
            default:
                std::cerr << "Error: Cannot set piece 'none' at square " << targetSquare << "\n";
                break;
        }
        occupiedSquares |= mask;
        getPieces(colorOfPieceBeingMoved) |= mask;
    }
    void clearSquare(Square targetSquare){
        Board &currentBoardState = *this; //currentBoardState acts as an alias to this. Done for readability/simplicitiy
        uint64_t mask = ~(1ULL << targetSquare);
        
        whitePawns   &= mask;
        whiteKnights &= mask;
        whiteBishops &= mask;
        whiteRooks   &= mask;
        whiteQueens  &= mask;
        whiteKings   &= mask;

        blackPawns   &= mask;
        blackKnights &= mask;
        blackBishops &= mask;
        blackRooks   &= mask;
        blackQueens  &= mask;
        blackKings   &= mask;

        if (targetSquare == whiteKingPosition) whiteKingPosition = NO_SQUARE; 
        if (targetSquare == blackKingPosition) blackKingPosition = NO_SQUARE;
    }
    void movePiece(Square from, Square to){ //must be called on a square that HAS a piece... will fail otherwise
        Board &currentBoardState = *this; //currentBoardState acts as an alias to this. Done for readability/simplicitiy

        Piece pieceBeingMoved = currentBoardState.getPieceAtSquare(from);
        Color colorOfPieceBeingMoved = currentBoardState.getColorAtSquare(from);
        currentBoardState.removePiece(colorOfPieceBeingMoved , pieceBeingMoved , from);
        currentBoardState.addPiece(colorOfPieceBeingMoved , pieceBeingMoved , to);

        if (pieceBeingMoved == king){
            if (colorOfPieceBeingMoved == white) whiteKingPosition = to;
            else blackKingPosition = to;
        }
    }

    void makeMove(MoveInformation move, int filterTest){  
        //if (filterTest) std::cout << "[DEBUG] Removing " << move.getPieceLetter(move.capturedPiece) << " from " << move.toSquare << std::endl;
        //assuming the move passed in is from the generated legal move list, NOT user data... (important for From data)
        
        //this is ensured to be legal so responsibility of function is to only update the board
        updateFriendlyEnemy(move.playerColor);
        Color enemyColor = !move.playerColor;
        //if (filterTest) std::cout<< "\n\n[Capture][EnPassant] = " << move.isCapture << move.isEnpassant << std::endl;
        
    
        //CAPTURES/ENPASSANT
        if (move.isCapture){
           
            // if (filterTest) std::cout<< "\n\n[Capture][EnPassant] = " << move.isCapture << move.isEnpassant << std::endl;
            if (move.isEnpassant){
                movePiece(move.fromSquare,move.toSquare);
                Square pawnToBeRemoved;
                //color determines if the piece that jumped is above or behind the pawn
                if (enemyColor == black) pawnToBeRemoved = Square(int(move.toSquare) - 8);
                else pawnToBeRemoved = Square(int(move.toSquare) + 8);
                
                removePiece(enemyColor,pawn, pawnToBeRemoved);
      
                return; //enPassant cannot lead to castling or promotion
            }
            else{ //traditional captures
                removePiece(enemyColor,move.capturedPiece,move.toSquare); //remove the captured piece
                //move the player's piece
                movePiece(move.fromSquare , move.toSquare); //move the capturing piece
            }
        }
       
        //CASTLING
        else if (move.isKingCastle){                                    //could be rewritten to be less explicit and more moduralized --> future update!
            if (move.playerColor == white){
                movePiece(e1,g1); //move the king
                movePiece(h1,f1); //move the rook
            }
            else{
                movePiece(e8,g8); //move the king
                movePiece(h8,f8); //move the rook

            }
        }
        else if (move.isQueenCastle){
            if (move.playerColor == white){
                movePiece(e1,c1); //move the king
                movePiece(a1,d1); //move the rook
            }
            else{
                movePiece(e8,c8); //move the king
                movePiece(a8,d8); //move the rook
            }
        }
        //REGULAR MOVES (Passive/NonCapture)
        else if (move.isCapture == 0) movePiece(move.fromSquare , move.toSquare);

        //PROMOTIONS
        if (move.isPromotion){
            // any move that is a promotion must have triggered one of the previous bool statements
            // so the only change to make is to replace the pawn from the TO square with whatever it promotes to

            removePiece(move.playerColor,pawn,move.toSquare); //all promotions are pawn piece type
            addPiece(move.playerColor,move.promotionPiece,move.toSquare);
        }
        
    } 
    
    

    bool isSquareInCheck(Square squareOfInterest, Color colorOfSquare){
       
        Color opponentColor = !colorOfSquare;
        uint64_t opponentMoves = moveGen.attackedByPawns(*this, opponentColor) | moveGen.possibleKnightMovesBitBoard(*this, opponentColor) | moveGen.possibleBishopMovesBitBoard(*this, opponentColor) | 
                                moveGen.possibleRookMovesBitBoard(*this, opponentColor) | moveGen.possibleQueenMovesBitBoard(*this, opponentColor) | moveGen.possibleKingMovesBitBoard(*this,opponentColor);
        //opponentMoves includes King because this asks if a square is in check, different than if king is in check
        //std::cout <<"\nCheckpoint D\n";
        return  ( ((1ULL << squareOfInterest) & opponentMoves) != 0); 
    }
    bool isKingInCheck(Color colorOfKing){
        MoveGenerator moveGen;
        uint64_t kingLocation = getKing(colorOfKing);
        Color opponentColor = !colorOfKing;
        uint64_t opponentMoves = moveGen.attackedByPawns(*this, opponentColor) | moveGen.possibleKnightMovesBitBoard(*this, opponentColor) | moveGen.possibleBishopMovesBitBoard(*this, opponentColor) | 
                                    moveGen.possibleRookMovesBitBoard(*this, opponentColor) | moveGen.possibleQueenMovesBitBoard(*this, opponentColor);
       
        return  ( (kingLocation & opponentMoves) != 0); //if there is overlap (>0) return true
    }
    // CASTLING
    bool getCastlePrivelege(Color side){
        if (side == white){
            return whiteCastlePrivelege;
        }
        return blackCastlePrivelege;
    }
    bool canKingCastle(Color color){ //check for if any of the 4 squares of relevance are in check... done later
        
        if (color == white){
            bool kingPos = (getPieceAtSquare(e1) == king && !isSquareInCheck(e1,white));
            bool rookPos = (getPieceAtSquare(h1) == rook && !isSquareInCheck(h1,white));
            bool emptySquare1 = ((getPieceAtSquare(f1) == none) && !isSquareInCheck(f1,white)) ;
            bool emptySquare2 = (getPieceAtSquare(g1) == none && !isSquareInCheck(g1,white)) ;
            bool castlePriv = whiteCastlePrivelege;
            
            if (kingPos && rookPos && emptySquare1 && emptySquare2 && castlePriv) return true;
        }
            
        if (color == black){
            bool kingPos = (getPieceAtSquare(e8) == king && !isSquareInCheck(e8,black));
            bool rookPos = (getPieceAtSquare(h8) == rook && !isSquareInCheck(h8,black));
            bool emptySquare1 = ((getPieceAtSquare(f8) == none) && !isSquareInCheck(f8,black)) ;
            bool emptySquare2 = (getPieceAtSquare(g8) == none && !isSquareInCheck(g8,black)) ;
            bool castlePriv = blackCastlePrivelege;
        
            if (kingPos && rookPos && emptySquare1 && emptySquare2 && castlePriv) return true;
        } 
        return false;
    }
    bool canQueenCastle(Color color){ //ensures piece in right location + not in check
        if (color == white && (getPieceAtSquare(e1) == king && !isSquareInCheck(e1,white)) 
                            && (getPieceAtSquare(a1) == rook && !isSquareInCheck(a1,white)) 
                            && ((getPieceAtSquare(b1) == none) && !isSquareInCheck(b1,white)) 
                            && (getPieceAtSquare(c1) == none && !isSquareInCheck(c1,white)) && whiteCastlePrivelege) return true;
        if (color == black && (getPieceAtSquare(e8) == king && !isSquareInCheck(e8,black))
                             && (getPieceAtSquare(a8) == rook && !isSquareInCheck(a8,black)) 
                             && ((getPieceAtSquare(b8) == none) && !isSquareInCheck(b8,black)) 
                             && (getPieceAtSquare(c8) == none && !isSquareInCheck(c8,black)) 
                             && blackCastlePrivelege) return true;
        return false;
    } 
    void updateCastlingRights(MoveInformation moveThatWasMade, Color color){ // call this function on the gameboard AFTER the move has been made
        /*
            Castling can happen if none of the rel pieces are in check and king or rook  has not moved and you havent already castled 

            Once a castling right is lost, it can never be gained... f(x) only needs called if castling rights are still held true
        */
       
       if (color == white){
            //handles if the move made moved needed pieces
            if (moveThatWasMade.isKingCastle || moveThatWasMade.isQueenCastle){
                whiteHasCastled = true;
                whiteCanQueenCastle = false;
                whiteCanKingCastle = false;
                whiteCastlePrivelege = false;
                whiteKingCastleRights = false;
                whiteQueenCastleRights = false;
                return;
            }
            if ((moveThatWasMade.pieceType == rook || moveThatWasMade.pieceType == king )){
                    // a move is made that involves one of the pieces that is not a castle
                    if (moveThatWasMade.pieceType == rook ){ //whichever rook moved voids that side's castling                //ADD CONDITION THAT It ORIGINATES (pawn promotion can fuck this)
                        if (moveThatWasMade.fromSquare % 8 == 7) whiteKingCastleRights = false;
                        if (moveThatWasMade.fromSquare % 8 == 0) whiteQueenCastleRights = false;
                    }
                    if (moveThatWasMade.pieceType == king){ //any king movement voids castling on the new board state
                        whiteHasCastled = false;
                        whiteCanQueenCastle = false;
                        whiteCanKingCastle = false;
                        whiteCastlePrivelege = false;
                        whiteKingCastleRights = false;
                        whiteQueenCastleRights = false;
                    }
            }
            //handles the post move boardstate. Are pieces in location and has castling been maintained previously?
            if (canKingCastle(white)){
                whiteCanKingCastle = true;
            }
            if (canQueenCastle(white)){
                whiteCanQueenCastle = true;
            }

            //postMoveBoardState.whiteQueenCastleRights = canQueenCastle(postMoveBoardState,white);
        }
        else{//black
            if (moveThatWasMade.isKingCastle || moveThatWasMade.isQueenCastle){
                blackHasCastled = true;
                blackCanQueenCastle = false;
                blackCanKingCastle = false;
                blackCastlePrivelege = false;
                blackKingCastleRights = false;
                blackQueenCastleRights = false;
                return;
            }
            //handles if the move made moved needed pieces
            if ((moveThatWasMade.pieceType == rook || moveThatWasMade.pieceType == king )){
                    // a move is made that involves one of the pieces that is not a castle
                    if (moveThatWasMade.pieceType == rook ){ //whichever rook moved voids that side's castling
                        if (moveThatWasMade.fromSquare % 8 == 7) blackKingCastleRights = false;
                        if (moveThatWasMade.fromSquare % 8 == 0) blackQueenCastleRights = false;

                    }
                    if (moveThatWasMade.pieceType == king){ //any king movement voids castling on the new board state
                        blackHasCastled = false;
                        blackCanQueenCastle = false;
                        blackCanKingCastle = false;
                        blackCastlePrivelege = false;
                        blackKingCastleRights = false;
                        blackQueenCastleRights = false;
                    }
            }
            //handles the post move boardstate. Are pieces in location and has castling been maintained previously?
            if (canKingCastle(black)){
                blackCanKingCastle = true;
            }
            if (canQueenCastle(black)){
                blackCanQueenCastle = true;
            }
           
        }
         //if both king and queen rights are lost, then overall rights are lost
        if (!blackKingCastleRights && !blackQueenCastleRights) blackCastlePrivelege = false;
        if (!whiteKingCastleRights && !whiteQueenCastleRights) whiteCastlePrivelege = false;
    }
    

    void updateFriendlyEnemy(Color color){
        if (color == white){
            getFriendlyPieces() = getPieces(white);
            getEnemyPieces() = getPieces(black);
        }
        else{
            getFriendlyPieces() = getPieces(black);
            getEnemyPieces() = getPieces(white);
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
    uint64_t& getPieces(Color color){
        if (color == white) return whitePieces;
        return blackPieces;
    }
    uint64_t& getPawns(Color color){
        if (color == white) return whitePawns;
        return blackPawns;
    }
    uint64_t& getKnights(Color color){
        if (color == white) return whiteKnights;
        return blackKnights;
    
    }
    uint64_t& getBishops(Color color){
        if (color == white) return whiteBishops;
        return blackBishops;
    }
    uint64_t& getRooks(Color color){
        if (color == white) return whiteRooks;
        return blackRooks;
    }
    uint64_t& getQueens(Color color){
        if (color == white) return whiteQueens;
        return blackQueens;
    }
    uint64_t& getKing(Color color){
        if (color == white) return whiteKings;
        return blackKings;
    }
   
    uint64_t& getFriendlyPieces(){
        return friendlyPieces;
    }
    uint64_t& getEnemyPieces(){
        return enemyPieces;
    }

//TESTING CODE
std::string getPieceSymbol(Piece piece, Color color) {
    switch (piece) {
        case king:   return color == black ? "♔ " : "♚ ";
        case queen:  return color == black ? "♕ " : "♛ ";
        case rook:   return color == black ? "♖ " : "♜ ";
        case bishop: return color == black ? "♗ " : "♝ ";
        case knight: return color == black ? "♘ " : "♞ ";
        case pawn:   return color == black ? "♙ " : "♟ ";
        default:     return "·";  // or " " or "□"
    }
}

Color getColorAtSquare(int square) const {                  // POTENTIAL BUG LATER ON IF CALLED ON AN EMPTY SQUARE
    uint64_t mask = 1ULL << square;
    
    if ((whitePawns | whiteKnights | whiteBishops | whiteRooks | whiteQueens | whiteKings) & mask)
        return white;

    if ((blackPawns | blackKnights | blackBishops | blackRooks | blackQueens | blackKings) & mask)
        return black;
    //will not return because this function will only be called on squares with pieces
}
void displayBoardPolished() {
    std::cout << "  \n   +------------------------+" << std::endl;
    for (int rank = 7; rank >= 0; --rank) {
        std::cout << rank + 1 << " | ";
        for (int file = 0; file < 8; ++file) {
            int square = rank * 8 + file;
            if (isSet(occupiedSquares,square)){
                Piece piece = getPieceAtSquare(square);
                Color color = getColorAtSquare(square);
                std::string symbol = getPieceSymbol(piece, color);
                std::cout << symbol << " ";
            }
            else{
                std::cout << ".  ";
            }
            
            
        }
        std::cout << "|" << std::endl;
    }
    std::cout << "  +------------------------+" << std::endl;
    std::cout << "    a  b  c  d  e  f  g  h" << std::endl;
}

};

class MoveParser{
    public:
        std::string getMoveString(MoveInformation move) //creates a string from existing move characteristics
    {
        std::stringstream ss;
        if (move.isKingCastle) ss << "O-O";
        if (move.isQueenCastle) ss << "O-O-O";
        if (!(move.getPieceLetter(move.pieceType) == 'P') ){
            if ( !move.isKingCastle && !move.isQueenCastle) ss << move.getPieceLetter(move.pieceType);
        }
            
        if (move.isAmbiguous){
            if (!move.uniqueFile && move.uniqueRank) {
                ss << move.fromRank; 
            } 
            else if (move.uniqueFile && !move.uniqueRank) {
                ss << move.fromFile;
            } 
            else { // both are needed
                ss << move.fromFile;
                ss << move.fromRank;
    }
        }
        
        if (move.isCapture) ss << 'x';
        
        ss << move.toFile << move.toRank;
        if (move.isPromotion) ss << "=" << move.getPieceLetter(move.promotionPiece);

        if (move.isCheckMate) ss << "#";  // Checkmate gets priority
        else if (move.isCheck){
            ss << "+";
            //std::cout << "\nCHEESE2\n";
        }
        return ss.str();
    } 
        MoveInformation createMoveFromString(Board boardWaitingForMove, Color playerColor, std::string chessNotation){ //turn string into a move with rel attributes
        //assuming a possible chess notation (ex: NOT 1 character length)
        std::string moveStr = chessNotation; // will be editing this
        MoveInformation move;
        move.chessNotation = moveStr;
        // chatGPT suggested rewriting the creation of the specialty flags in this concise format.
        move.playerColor = playerColor;
        move.isCapture = moveStr.find('x') != std::string::npos;
        move.isPromotion = moveStr.find('=') != std::string::npos;
        move.isCheck = moveStr.find('+') != std::string::npos;
        move.isCheckMate = moveStr.find('#') != std::string::npos;
        move.isKingCastle = (moveStr == "O-O"); 
        move.isQueenCastle = (moveStr == "O-O-O");

        if (move.isKingCastle){
            Board copy = boardWaitingForMove;
            
            move.chessNotation = "O-O";
            move.pieceType= king;
            move.isAmbiguous = false;
            move.isCapture = false;

            if (playerColor == white){
               
                move.fromSquare = e1;
                move.toSquare = g1;
                return move;
            }
            else if (playerColor == black){
                move.toFile = 'g';
                move.toRank = '8';
                move.fromSquare = e8;
                move.toSquare = g8;
                
                return move;
            }
        }
        else if (move.isQueenCastle){
            move.chessNotation = "O-O-O";
             move.pieceType= king;
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
        
        move.isAmbiguous = false;                                       //all moves are considered not ambigious at the moment....
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
                if ( (moveStr.at(0) >= 'a' && moveStr.at(0) <= 'h') || (moveStr.at(0) >= '1' && moveStr.at(0) <= '8') || (moveStr.at(0) == 'x')){
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
        move.toRank = moveStr.at(moveStr.length() - 1) ;
        moveStr = moveStr.substr(0, moveStr.length() - 1);

        move.toFile = moveStr.at(moveStr.length() - 1) ;                       
        //std::cout <<"\nFile: " << move.toFile << " Rank: " << move.toRank << "\n";
        moveStr = moveStr.substr(0, moveStr.length() - 1);
        // std::cout << moveStr << std::endl;
        int asciiFile = static_cast<int>(move.toFile) - 96; //SHOULD BE 97 FOR 0 INDEX SYSTEM!!!!!
        move.toSquare = Square( (asciiFile - 1) + 8 * (move.toRank - '1' ) ); // casting both rank and file to integers and calculating the square index
        if (move.isCapture)
        {
            moveStr = moveStr.substr(0, moveStr.length() - 1);
            move.capturedPiece = boardWaitingForMove.getPieceAtSquare(move.toSquare);
            // thing before to square, if capture is 'x'
        }
        // std::cout << moveStr << std::endl;
        //std::cout << "boop";
        if (moveStr.length() == 1) 
        {
            move.isAmbiguous = true;
            if (moveStr.at(0) >= 'a' && moveStr.at(0) <= 'h') move.fromFile = moveStr.at(0);
            if (moveStr.at(0) >= '1' && moveStr.at(0) <= '8') move.fromRank = moveStr.at(0);
            
        }
        if (moveStr.length() == 2){                                                     //DOUBLE AMBIGUITY CASE...UNSURE!
            move.fromFile = moveStr.at(0); 
            move.fromRank = moveStr.at(1);
        }
        // std::cout << moveStr << std::endl;

        // square = File * 8 + Rank
        // a = 1
        // rank = numbers, File == files
        
        
        //std::cout << "TESTING: " << asciiFile << ", " << (move.toRank - '0') << ", " << move.toSquare << std::endl;
        
        return move;


    }
        MoveInformation parseMove(Color playerColor)
    {
        std::string lineArg;
        std::getline(std::cin, lineArg);
        if (lineArg == "q") std::exit(0);
        while (lineArg.length() <2){
            std::cout << "Invalid input. Please try again\n";
            std::getline(std::cin, lineArg);
            if (lineArg == "q") std::exit(0);
        }
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
        move.playerColor = playerColor;
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
        if (moveStr.length() == 1) 
        {
            move.isAmbiguous = true;
            if (moveStr.at(0) >= 'a' && moveStr.at(0) <= 'h') move.fromFile = moveStr.at(0);
            if (moveStr.at(0) >= '1' && moveStr.at(0) <= '8') move.fromRank = moveStr.at(0);
            
        }
        if (moveStr.length() == 2){                                                     //DOUBLE AMBIGUITY CASE...UNSURE!
            move.fromFile = moveStr.at(0); 
            move.fromRank = moveStr.at(1);
        }
    
        // std::cout << moveStr << std::endl;

        // square = File * 8 + Rank
        // a = 1
        // rank = numbers, File == files
        
        int asciiFile = static_cast<int>(move.toFile) - 97; //SHOULD BE 97 FOR 0 INDEX SYSTEM!!!!!
        move.toSquare = Square( (asciiFile - 1) + 8 * (move.toRank - '0' - 1) ); // casting both rank and file to integers and calculating the square index
        //std::cout << "TESTING: " << asciiFile << ", " << (move.toRank - '0') << ", " << move.toSquare << std::endl;
        return move;
    }
};
    
class MoveGenerator{

    private:
    /*
        low level mask generation, used by legalMoveGen functions
        these are called by public methods... private as no user would need direct access
    */
        uint64_t getKnightMask(int square){
            /*
                15  17
            6         10
                x
            10         6
                17  15
            
        up is SL, down is SR
            */
            uint64_t knightMoves = 0ULL;
            uint64_t bitboard = 0ULL;
            
        uint64_t startingSquare = 1ULL << square;
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
        uint64_t getPawnMask(Board boardOfInterest,int square, Color side){
            int rank = square / 8;
            int file = square % 8;
            //1 = white, 0 = black
            uint64_t possibleMoves = 0ULL;
            uint64_t possibleCaptures = 0ULL;
            uint64_t pawnBitBoard = 1ULL << square;
            uint64_t friendlyPieces;
            uint64_t enemyPieces;
            if (side == white){ // pawns move up.. << & en Passant rank = 4
                friendlyPieces = boardOfInterest.getPieces(white);
                enemyPieces = boardOfInterest.getPieces(black);
                uint64_t singlePush = ( pawnBitBoard << 8) & boardOfInterest.getEmptySquares(); 
                uint64_t doublePush = 0ULL;
                if (rank == 1 && singlePush) doublePush = (singlePush << 8) & boardOfInterest.getEmptySquares();

                
                // if (rank == 2 & singlePush){
                //     doublePush = (singlePush << 8) & emptySquares;
                // }
                possibleMoves = singlePush | doublePush ;
                pawnBitBoard = 1ULL << square;
                possibleCaptures |= ( (pawnBitBoard & notHFile) << 9 ) & (enemyPieces | (1ULL << boardOfInterest.enPassantTargetSquare) );
                possibleCaptures |= ( (pawnBitBoard & notAFile )<< 7) & (enemyPieces | (1ULL << boardOfInterest.enPassantTargetSquare) );
                possibleMoves |= possibleCaptures;
            }
            else{ //identical logic, different variable values and shifting operator
                friendlyPieces = boardOfInterest.getPieces(black);
                enemyPieces = boardOfInterest.getPieces(white);
        
                uint64_t singlePush = ( pawnBitBoard >> 8) & boardOfInterest.getEmptySquares() ; 
                uint64_t doublePush = 0ULL;
                if (rank == 6 && singlePush) doublePush = (singlePush >> 8) & boardOfInterest.getEmptySquares();
                possibleMoves = singlePush | doublePush ;
                                                                                                //DONT FORGET TO REMOVE
                possibleCaptures |= ( (pawnBitBoard & notAFile) >> 9 ) & (enemyPieces | (1ULL << boardOfInterest.enPassantTargetSquare) );
                possibleCaptures |= ( (pawnBitBoard & notHFile )>> 7) & (enemyPieces | (1ULL << boardOfInterest.enPassantTargetSquare) );
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
    
        //POSSIBLE CAPTURES/MOVES
    public:
    MoveParser mp;
    /*
        piece and overall legal move generation
    */

        // BITBOARDS!!!!
        uint64_t possiblePawnMovesBitBoard(Board boardOfInterest, Color colorOfInterest){     // pawn moves cant capture...
        
            uint64_t pawnBitBoard = boardOfInterest.getPawns(colorOfInterest);
        
        uint64_t friendlyPieces = boardOfInterest.getPieces(colorOfInterest);
        uint64_t enemyPieces = boardOfInterest.getPieces(!colorOfInterest);
        
            uint64_t possibleMoves = 0ULL;
            uint64_t possibleCaptures = 0ULL;
        
            if (colorOfInterest == white){ // pawns move up.. << & en Passant rank = 4
                uint64_t singlePush = ( pawnBitBoard << 8) & boardOfInterest.getEmptySquares() ; 
                uint64_t doublePush = ( ( ( (0xFF00 & pawnBitBoard) << 8) & boardOfInterest.getEmptySquares() ) << 8 ) & boardOfInterest.getEmptySquares(); //checks for unmoved pawns, single pushes, pushes again
            
                possibleMoves = singlePush | doublePush ;
                
                possibleCaptures |= ( (pawnBitBoard & notHFile) << 9 ) & enemyPieces;
                possibleCaptures |= ( (pawnBitBoard & notAFile )<< 7) & enemyPieces;
                possibleMoves |= possibleCaptures;
                possibleMoves &= ~(friendlyPieces); // may be redundant as possibleCaptures is anded with enemyPieces
            }
            else{ //identical logic, different variable values and shifting operator
                uint64_t singlePush = ( pawnBitBoard >> 8) & boardOfInterest.getEmptySquares() ; 
                uint64_t doublePush = ( ( ( (0x00FF000000000000ULL & pawnBitBoard) >> 8) & boardOfInterest.getEmptySquares() ) >> 8 ) & boardOfInterest.getEmptySquares(); //checks for unmoved pawns, single pushes, pushes again
            
                possibleMoves = singlePush | doublePush ;
                
                possibleCaptures |= ( (pawnBitBoard & notHFile) >> 9 ) & enemyPieces;
                possibleCaptures |= ( (pawnBitBoard & notAFile )>> 7) & enemyPieces;
                possibleMoves |= possibleCaptures;
                possibleMoves &= ~(friendlyPieces);
            }
            return possibleMoves;
        }
        uint64_t possibleKnightMovesBitBoard(Board boardOfInterest, Color colorOfInterest){
            /*
                15  17
            6         10
                x
            10         6
                17  15
            
            */
            //seperate board for knight moves and condition checking. If 1 was used, cascading shifting would happen... bad!

            uint64_t knightBitBoard = boardOfInterest.getKnights(colorOfInterest);
            uint64_t friendlyPieces = boardOfInterest.getPieces(colorOfInterest);
            uint64_t enemyPieces = boardOfInterest.getPieces(!colorOfInterest);
        
            uint64_t bitboard = knightBitBoard;
            uint64_t knightMoves = 0ULL;
        
            //will not change right now because it aint broke... but!
            //I dont need both knightMoves and bitboard. I created these to prevent cascading shifting but by assigning results to an outside/secondary board (knightMoves), cascading is avoided
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
        uint64_t possibleBishopMovesBitBoard(Board boardOfInterest, Color colorOfInterest){
        
            uint64_t bishopBitboard = boardOfInterest.getBishops(colorOfInterest);
            uint64_t friendlyPieces = boardOfInterest.getPieces(colorOfInterest);
            uint64_t enemyPieces = boardOfInterest.getPieces(!colorOfInterest);
            

            uint64_t bishopMoves = 0ULL;
            for (int square = 0; square < 64; square++){ // for every bishop, calculate the attacks
                if (isSet(bishopBitboard,square)){
                    bishopMoves |= get_bishop_attacks(square,boardOfInterest.getOccupiedSquares() );
                }
            }
            bishopMoves &= ~friendlyPieces;
            return bishopMoves;
        }
        uint64_t possibleRookMovesBitBoard(Board boardOfInterest, Color colorOfInterest){
        
            uint64_t rookBitBoard = boardOfInterest.getRooks(colorOfInterest);
            uint64_t friendlyPieces = boardOfInterest.getPieces(colorOfInterest);
            uint64_t enemyPieces = boardOfInterest.getPieces(!colorOfInterest);
        
            uint64_t rookMoves = 0ULL;
            for (int square = 0; square < 64; square++){ //only if the bit is set, do you calculate rook moves
                if (isSet(rookBitBoard,square)){
                    rookMoves |= get_rook_attacks(square,boardOfInterest.getOccupiedSquares() );
                }
            }
            rookMoves &= ~ friendlyPieces;
            return rookMoves;
        }
        uint64_t possibleQueenMovesBitBoard(Board boardOfInterest, Color colorOfInterest){
            
            uint64_t queenBitBoard = boardOfInterest.getQueens(colorOfInterest);
            uint64_t friendlyPieces = boardOfInterest.getPieces(colorOfInterest);
            uint64_t enemyPieces = boardOfInterest.getPieces(!colorOfInterest);
        
            uint64_t queenMoves = 0ULL;
            for (int square = 0; square < 64; square++){ 
                if (isSet(queenBitBoard,square)){
                    queenMoves |= get_rook_attacks(square,boardOfInterest.getOccupiedSquares() );
                    queenMoves |= get_bishop_attacks(square,boardOfInterest.getOccupiedSquares() );
                }
            }
            queenMoves &= ~ friendlyPieces;

            return queenMoves;
        }
        uint64_t possibleKingMovesBitBoard(Board boardOfInterest,Color colorOfInterest){

            uint64_t kingBitBoard = boardOfInterest.getKing(colorOfInterest);
            uint64_t friendlyPieces = boardOfInterest.getPieces(colorOfInterest);
            uint64_t enemyPieces = boardOfInterest.getPieces(!colorOfInterest);
        
            uint64_t kingAttack = 0ULL;
            /*
            789
            1K1
            987
            */
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
            
        uint64_t attackedByPawns(Board boardOfInterest,Color colorOfInterest){       
        
            uint64_t pawnBitBoard = boardOfInterest.getPawns(colorOfInterest);
            uint64_t enemyPieces = boardOfInterest.getPieces(!colorOfInterest);
        
            uint64_t possibleCaptures = 0ULL;
        
            if (colorOfInterest == white){ // pawns move up.. << & en Passant rank = 4
                
                possibleCaptures |= ( (pawnBitBoard & notHFile) << 9 ) & enemyPieces;
                possibleCaptures |= ( (pawnBitBoard & notAFile )<< 7) & enemyPieces;
            }
            else{ //identical logic, different variable values and shifting operator
                possibleCaptures |= ( (pawnBitBoard & notAFile) >> 9 ) & enemyPieces;
                possibleCaptures |= ( (pawnBitBoard & notHFile )>> 7) & enemyPieces;
            }
            return possibleCaptures;
        }
        
        //MOVE GENERATION FOR USER & AI
        std::vector<MoveInformation> generatePseudoLegalMovesFromBitboard(Board boardOfInterest, uint64_t bitBoard, Piece pieceType, Color color){ //color may be passed implicitly by game variable
            /*
            
            */
        if (bitBoard == 0ULL) return {};
        boardOfInterest.updateFriendlyEnemy(color);
        std::vector<MoveInformation> moveListForBoard; 
        uint64_t possibleMask = 0ULL;
        while (bitBoard){
            int fromSquare = __builtin_ctzll(bitBoard);  // finds the LSB that is set to one
            switch(pieceType){
                case pawn:
                    possibleMask= getPawnMask(boardOfInterest, fromSquare,color);
                    break;
                case knight:
                    possibleMask = getKnightMask(fromSquare);
                    possibleMask &= ~boardOfInterest.getFriendlyPieces();
                    break;
                case bishop:
                    possibleMask = get_bishop_attacks(fromSquare,boardOfInterest.getOccupiedSquares());
                    possibleMask &= ~boardOfInterest.getFriendlyPieces();
        
                    break;
                case rook:
                    possibleMask = get_rook_attacks(fromSquare,boardOfInterest.getOccupiedSquares());
                    possibleMask &= ~boardOfInterest.getFriendlyPieces();
                  
                    break;
                case queen:
                    possibleMask = get_rook_attacks(fromSquare,boardOfInterest.getOccupiedSquares() ) | get_bishop_attacks(fromSquare,boardOfInterest.getOccupiedSquares());
                    possibleMask &= ~boardOfInterest.getFriendlyPieces();
                    break;
                case king:
                    possibleMask = getKingMask(fromSquare);
                    possibleMask &= ~boardOfInterest.getFriendlyPieces();
                    break;
                case none:  //idealy impossible... if a bit is set, a piece must be present
                    break;
                
            }
        
            while (possibleMask){ //for each possible destination (set bit in possible mask)
                int destination = __builtin_ctzll(possibleMask); //iterate to the first set bit/piece on the board

                MoveInformation legalMove;
                legalMove.playerColor = color;                                                                              //moves are created assumed white, must reassign black
                //generate the attributes of the move
                legalMove.pieceType = Piece(pieceType);
                legalMove.fromSquare = Square(fromSquare);
                legalMove.toSquare = Square(destination);

                legalMove.toFile = 'a' + (destination % 8) ;
                legalMove.toRank = '1' + (destination / 8 );
                //now grab information from board at the destination

                //captures
                
            
                if (destination == boardOfInterest.enPassantTargetSquare && pieceType == pawn && boardOfInterest.getPieceAtSquare(destination) == none){
                    legalMove.isCapture = true;
                    legalMove.isEnpassant = true;
                    legalMove.capturedPiece = pawn;
                }
                else if (boardOfInterest.getPieceAtSquare(destination) != none){
                    legalMove.isCapture = true;
                    legalMove.capturedPiece = boardOfInterest.getPieceAtSquare(destination);
                }
                else{
                    legalMove.isCapture = false;
                }
                
                //castling
                if (color == white) { //castling will never be generated from a bitboard. its outside the normal rules for king movement
                    if ( legalMove.pieceType == king && legalMove.fromSquare == e1 && legalMove.toSquare == g1 ) legalMove.isKingCastle = true;
                    if ( legalMove.pieceType == king && legalMove.fromSquare == e1 && legalMove.toSquare == c1 ) legalMove.isQueenCastle = true;
                }
                if (color == black){
                    
                    if ( legalMove.pieceType == king && legalMove.fromSquare == e8 && legalMove.toSquare == g8 ) legalMove.isKingCastle = true;
                    if ( legalMove.pieceType == king && legalMove.fromSquare == e8 && legalMove.toSquare == c8 ) legalMove.isQueenCastle = true;
                }

                //promotions
                if (color == white) { 
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
                if (pieceType == pawn && legalMove.isCapture && legalMove.toSquare == boardOfInterest.enPassantTargetSquare) legalMove.isEnpassant = true;

                /*
                    CHECK FOR IF IT PUTS YOUR OWN KING IN CHECK!!!
                */
               Board copyBoard = boardOfInterest;
                copyBoard.makeMove(legalMove,0);
               if ( !(copyBoard.isKingInCheck(legalMove.playerColor))){ //if the move doesnt put your own king in check
                    //add move to list
                    legalMove.chessNotation = mp.getMoveString(legalMove);
                    if (legalMove.isPromotion){                               
                        legalMove.promotionPiece = bishop;
                        moveListForBoard.push_back(legalMove);
                        legalMove.promotionPiece = rook;
                        moveListForBoard.push_back(legalMove);
                        legalMove.promotionPiece = queen;
                        moveListForBoard.push_back(legalMove);
                        legalMove.promotionPiece = knight;
                        moveListForBoard.push_back(legalMove);
                    }
                    else{
                    moveListForBoard.push_back(legalMove);
                    }
               }
                        
               
                possibleMask &= (possibleMask - 1); //clear the bit as it is done with
            }

            bitBoard &= (bitBoard - 1); //clears the LSB.. cool trick!
        }
        
        return moveListForBoard;

    }
        std::vector<MoveInformation> generateLegalMoves(Board boardstate, Color color){ 
            
            std::vector<MoveInformation> allLegalMoves;
            std::vector<MoveInformation> movesToAdd;
            movesToAdd = generatePseudoLegalMovesFromBitboard(boardstate, boardstate.getPawns(color),pawn,color);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd = generatePseudoLegalMovesFromBitboard(boardstate, boardstate.getKnights(color),knight,color);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd = generatePseudoLegalMovesFromBitboard(boardstate, boardstate.getBishops(color),bishop,color);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd = generatePseudoLegalMovesFromBitboard(boardstate, boardstate.getRooks(color),rook,color);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd = generatePseudoLegalMovesFromBitboard(boardstate, boardstate.getQueens(color),queen,color);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd = generatePseudoLegalMovesFromBitboard(boardstate, boardstate.getKing(color),king,color);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd.clear();                                                             // is this necessary?
            
            if (boardstate.canKingCastle(color)){
                // std::cout << "\n DEBUG : CASTLING IS ADDED\n";
                movesToAdd.push_back(mp.createMoveFromString(boardstate, color, "O-O"));
            }
            if (boardstate.canQueenCastle(color)){
                movesToAdd.push_back(mp.createMoveFromString(boardstate, color, "O-O-O"));
            }
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end()); // adding possible castles
            // std::cout << "\nDEBUG : MOVES BEFORE IDENTIFY MATES AND AMBIG MOVES\n";
            //printMoveList(allLegalMoves);

            //CLEANS UP PSEUDO GEN
            pruneMovesThatKeepSelfInCheck(allLegalMoves,boardstate);
            findAmbiguousMoves(allLegalMoves); //updates notation of moves that are ambig
            return allLegalMoves;
        }
        std::vector<MoveInformation> generateLegalMovesOnBoard(Board boardstate, Color color){
            std::vector<MoveInformation> allLegalMoves;
            std::vector<MoveInformation> movesToAdd;
            movesToAdd = generatePseudoLegalMovesFromBitboard(boardstate, boardstate.getPawns(color),pawn,color);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd = generatePseudoLegalMovesFromBitboard(boardstate, boardstate.getKnights(color),knight,color);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd = generatePseudoLegalMovesFromBitboard(boardstate, boardstate.getBishops(color),bishop,color);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd = generatePseudoLegalMovesFromBitboard(boardstate, boardstate.getRooks(color),rook,color);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd = generatePseudoLegalMovesFromBitboard(boardstate, boardstate.getQueens(color),queen,color);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd = generatePseudoLegalMovesFromBitboard(boardstate, boardstate.getKing(color),king,color);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd.clear();
            if (boardstate.canKingCastle(color)){
                //std::cout << "\n DEBUG : CASTLING IS ADDED\n";
                movesToAdd.push_back(mp.createMoveFromString(boardstate, color, "O-O"));
            }
            if (boardstate.canQueenCastle(color)){
                movesToAdd.push_back(mp.createMoveFromString(boardstate,color, "O-O-O"));
            }
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end()); // adding possible castles

            pruneMovesThatKeepSelfInCheck(allLegalMoves,boardstate);
            findAmbiguousMoves(allLegalMoves); //updates notation of moves that are ambig
            return allLegalMoves;

        }
    
        // HELPER/CLEANUP OF PSEUDO LEGAL MOVE GEN
        void findAmbiguousMoves(std::vector<MoveInformation> &legalMovesList){ //edits list of legal moves to show ambiguity
            //std::vector<MoveInformation> copyofLegalMoves = legalMovesList;
            //void because it will edit the list of moves directly
            for (auto& move : legalMovesList) {
                std::vector<MoveInformation> conflictingMovesList;
                for (const auto& other : legalMovesList) {
        
                    if (&move == &other ) continue; //prevents self-comparison, if something is already marked ambiguous, skip it!
                    if (move.pieceType == other.pieceType && move.playerColor == other.playerColor && move.toSquare == other.toSquare && move.fromSquare != other.fromSquare) {
                        conflictingMovesList.push_back(other);
                    }
                }
                if (!conflictingMovesList.empty()){ //if conflicting moves exist (ambiguity!)
                        move.isAmbiguous = true;
                        bool fileSufficient = true, rankSufficient = true;
                        
                        for (auto& conflictingMove : conflictingMovesList){
                            if ( (conflictingMove.fromSquare%8) == (move.fromSquare%8) ) fileSufficient = false;
                            
                            if ( (conflictingMove.fromSquare/8) == (move.fromSquare/8) ) rankSufficient = false;
                        
                        }
                        

                        //only updating one because this process will repeat when the "other" move ibecomes the "move" move
                        if (fileSufficient) {
                            move.fromFile = 'a' + (move.fromSquare % 8); 
                            move.uniqueFile = true;
                            
                        } 
                        if (rankSufficient) {
                            move.fromRank = '1' + (move.fromSquare / 8); 
                            move.uniqueRank = true;
                        } 
                        if (!rankSufficient && !fileSufficient) {
                            move.fromRank = '1' + (move.fromSquare / 8); 
                            move.fromFile = 'a' + (move.fromSquare % 8); 
                            move.uniqueRank = true;
                            move.uniqueFile = true;
                        } 
                    }
            
            }
            
        }
        void pruneMovesThatKeepSelfInCheck(std::vector<MoveInformation> &legalMovesList, Board board){
            int index = 0;
            for (MoveInformation move: legalMovesList){
                Board copy = board;
                copy.makeMove(move,0);
                if (copy.isKingInCheck(move.playerColor)){
                    legalMovesList.erase(legalMovesList.begin() + index); //removes any moves where your own king is still in check
                    continue;
                }
                else{
                index++;
                }
            }

        }
        


};

class Game
{
private:
    Color playerTakingTurnColor;
    //bool isGameOver;
    Board board;
    int turnCount; //index for move/board history
    
    
public:
    gameEndingCondition gameCondition;

    std::vector<MoveInformation> masterMoveList; //holds all moves made (game turn - 1 = index)
    std::vector<Board> boardStates; //holds all board states
  
    MoveGenerator mg;
    MoveParser mp;
    
    Game(){
        board.initializeBoard();
        boardStates.push_back(board);
        playerTakingTurnColor = white;
        gameCondition = ongoing;
        turnCount = 0;
    }
    //GETTER METHODS
    Board& getBoard(){
        return board;
    }
    int& getGameTurnCount(){
        return turnCount;
    }
    Color& getColorOfPlayerTakingTurn(){
        return playerTakingTurnColor;
    }
    //GAME/PLAYER CONDITIONS
    //MOVE GENERATION
    
    //HELPER METHODS TO CLEAN UP PSEUDO MOVE GEN
   
    
    void updateNotationForMoveList(std::vector<MoveInformation> &legalMovesList){
            for (auto& moves: legalMovesList){ //update the strings of all moves after bad moves removed or info updated
                moves.chessNotation = mp.getMoveString(moves);
            }
        }
    void printMoveList(std::vector<MoveInformation> moveList){
    for (int i = 0; i < moveList.size();i++){
        std::cout << mp.getMoveString(moveList.at(i)) << ", ";
        
    }
    
}


    //MAKING MOVE
    MoveInformation getMatchingMove(Board boardState,const std::vector<MoveInformation>& moveList, const MoveInformation& targetMove){ //compares user move to list of legal and combines
        //printMoveList(moveList);
        std::vector<MoveInformation> candidates;
        for (MoveInformation move : moveList){

            //if attributes do not match, cant be the same move
            if(targetMove.pieceType != move.pieceType) continue;
            if (targetMove.toSquare != move.toSquare)continue;
            if ( (targetMove.isCapture != move.isCapture)|| (targetMove.capturedPiece != move.capturedPiece)) continue;
            if ( (targetMove.isPromotion != move.isPromotion) || (targetMove.promotionPiece != move.promotionPiece) ) continue;
            if (targetMove.isEnpassant != move.isEnpassant) continue;
            //if (targetMove.isCheck != move.isCheck) continue;
            //if (targetMove.isCheckMate != move.isCheckMate) continue;
            //dont check ambiguity flag... check the more appliciable,related markers that imply ambiguity
            if (targetMove.uniqueFile && targetMove.fromFile != (move.fromSquare % 8) ) continue;
            if (targetMove.uniqueRank && targetMove.fromRank != (move.fromSquare / 8) ) continue;
            //from square is not considered because it is not set/determined by inputtedMove or chess notation... requires context of board
            //if (targetMove.isAmbiguous && ( (targetMove.uniqueFile != move.uniqueFile) || (targetMove.uniqueRank != move.uniqueRank)  )) continue;
            if (targetMove.chessNotation != move.chessNotation) continue;                         //LEGAL MOVE GEN DOES NOT STORE CHESS NOTATION... it does now!!!
            candidates.push_back(move);
        }
        if (candidates.empty()) throw std::runtime_error("\nNo matching legal move found: possibly invalid move input.\n");
        if (candidates.size() > 1){
            
            // for (MoveInformation move : candidates) move.printMoveInfo();
            // std::cout << std::endl;
            throw std::runtime_error("\nAmbiguous move input: multiple matching legal moves found.\n");
        }
        return candidates.at(0);
    }
    void makeManualGameHalfTurn (Color turn, std::string chessNotation){ // identical to takeGameHalfTurn but move is predetermined
        MoveInformation moveBeingMade;
        moveBeingMade = mp.createMoveFromString(getBoard(),turn,chessNotation);
        
        MoveInformation matchingMove = getMatchingMove(getBoard(), mg.generateLegalMoves(getBoard(), turn) , moveBeingMade);
        getBoard().makeMove(matchingMove,1);                  //make move function does not discern legality, all illegal moves should be filtered out vefore this
        
        if (matchingMove.pieceType == pawn && abs(matchingMove.toSquare - matchingMove.fromSquare) == 16) {
        getBoard().enPassantTargetSquare = ((matchingMove.toSquare + matchingMove.fromSquare) / 2) ;
        //std::cout << "Setting enPassantTargetSquare to: " << matchingMove.toSquare  << " + "<<matchingMove.fromSquare<< " -->" <<getBoard().enPassantTargetSquare << "\n";

    }
        else{
            getBoard().enPassantTargetSquare = -1;
        }
        getBoard().updateCastlingRights(matchingMove,turn);

        Board newBoard = getBoard();
        boardStates.push_back(newBoard); //adds the new (post-move) board to the game history
        masterMoveList.push_back(matchingMove); //adds the move to bridge between board states
        getColorOfPlayerTakingTurn() = !getColorOfPlayerTakingTurn(); //inverse itself
        // printBitBoard(getBoard().getPawns(white));
        // printBitBoard(getBoard().getPawns(black));
        getBoard().displayBoardPolished();
    }
    void takeGameHalfTurn(Color turn){                                                        
       
        
        //printBitBoard(getBoard().getPawns(black));
        
        std::cout << "Turn " << getGameTurnCount();
        std::string gameMoveString;
        if (turn == white) std::cout << "W.";
        else std::cout << "B.";
        
        std::vector<MoveInformation> possibleLegalMoves = mg.generateLegalMoves(getBoard(), turn);
        identifyCheckMateMoves(possibleLegalMoves,getBoard() );
        updateNotationForMoveList(possibleLegalMoves);
        MoveInformation matchingMove; //initialized inside loop
        while (true) {
            std::string userInput;
            std::cout << "\nPlease make your move! (standard chess notation): ";
            while (1){
                std::cin >> userInput;
                // createMoveFromString(getBoard(), turn, userInput).printMoveInfo();
                // possibleLegalMoves.at(33).printMoveInfo();
                if (userInput == "quit" || userInput == "Quit" || userInput == "q" || userInput == "Q"){
                std::cout << "\nQuitting program...\n";
                std::exit(0);
                }
                else if (userInput.length() == 1){
                    //printMoveList(possibleLegalMoves);
                std::cout << "\nPlease enter a valid move.\n";
                }
                else{
                    break;
                }
            }   
            // move is not too small
            MoveInformation userInputtedMove = mp.createMoveFromString(getBoard(),turn,userInput);
            userInputtedMove.playerColor = turn;
            //userInputtedMove.printMoveInfo();


            try {
                matchingMove = getMatchingMove(getBoard(),possibleLegalMoves, userInputtedMove);
                //matchingMove.printMoveInfo();
                break; // exit the loop since we found a valid move
            }
            catch (const std::runtime_error& e) {
                std::cout << "\nError: " << e.what() << "\n";
                std::cout << "Please enter a valid move.\n";
                printMoveList(possibleLegalMoves);
            }
        
        }

        matchingMove.printMoveInfo();
        
        getBoard().makeMove(matchingMove,1);                  //make move function does not discern legality, all illegal moves should be filtered out vefore this
        
        if (matchingMove.pieceType == pawn && abs(matchingMove.toSquare - matchingMove.fromSquare) == 16) {
        getBoard().enPassantTargetSquare = ((matchingMove.toSquare + matchingMove.fromSquare) / 2) ;
        //std::cout << "Setting enPassantTargetSquare to: " << matchingMove.toSquare  << " + "<<matchingMove.fromSquare<< " -->" <<getBoard().enPassantTargetSquare << "\n";

    }
        else{
            getBoard().enPassantTargetSquare = -1;
        }
        getBoard().updateCastlingRights(matchingMove,turn);

        Board newBoard = getBoard();
        boardStates.push_back(newBoard); //adds the new (post-move) board to the game history
        masterMoveList.push_back(matchingMove); //adds the move to bridge between board states
        getColorOfPlayerTakingTurn() = !getColorOfPlayerTakingTurn(); //inverse itself
        // printBitBoard(getBoard().getPawns(white));
        // printBitBoard(getBoard().getPawns(black));
        getBoard().displayBoardPolished();
    }
    
    void identifyCheckMateMoves(std::vector<MoveInformation> &pseudoLegalMoves, Board boardstate){
        //the pseudoLegalMoves vector contains all moves that are possible that do not put king in check

        for (int i = 0; i < pseudoLegalMoves.size();i++){
            Board copyBoard = boardstate;
            Color opponentColor = !(pseudoLegalMoves.at(i).playerColor);
            copyBoard.makeMove(pseudoLegalMoves.at(i),0);
            if (isCheckMate(opponentColor,copyBoard)){                                                      //should not be calle
                //copyBoard.displayBoardPolished();
                //std::cout<<"\nDebug: A MOVE IS CHECKMATE!\n";
                pseudoLegalMoves.at(i).isCheckMate = true;
                if (pseudoLegalMoves.at(i).isCheckMate == true){
                    pseudoLegalMoves.at(i).printMoveInfo();
                    copyBoard.displayBoardPolished();
                }
                // std::cout << "\nTesting move: \n";
                // pseudoLegalMoves.at(i).printMoveInfo(); // Your helper
                // std::cout << "\nBoard after move:\n";
                // copyBoard.displayBoardPolished();
                // std::cout << "\nOpponent legal moves:\n";
                // printMoveList(generateLegalMovesOnBoardNoCheckTags(copyBoard, opponentColor));

            }
            else if (boardstate.isKingInCheck(opponentColor) ) pseudoLegalMoves.at(i).isCheck = true;//not checkmate but could be checkmate
        }
    }
        
    //GAME ENDING CONDITIONS

    bool isCheckMate(Color colorOfKing, Board boardState){
        boardState.updateFriendlyEnemy(colorOfKing);
            // std::cout << "\n[DEBUG] Checking for checkmate for " << (colorOfKing == white ? "White" : "Black") << "\n";
            // std::vector<MoveInformation> legalMovesTest = generateLegalMoves(board, colorOfKing);
            // std::cout << "[DEBUG] Legal moves found: " << legalMovesTest.size() << "\n";

            // for (auto& move : legalMovesTest) {
            //     move.printMoveInfo();
            // }

            // std::cout << "[DEBUG] King is " << (isKingInCheck(boardState,colorOfKing) ? "" : "not ") << "in check.\n";

        //std::cout << "\nDebug: checking for checkmate!\n";
        if (!boardState.isKingInCheck(colorOfKing)) return false; //if the king isnt in check, it cant be mate
        // std::cout <<"\nCheckpoint A\n";
        //std::cout << "\nDebug: checkpoint 2\n";
        std::vector<MoveInformation> legalMoves = mg.generateLegalMovesOnBoard(boardState, colorOfKing);
        // dont tag checkmates. quality of moves do no matter, quantity does
        // boardState.displayBoardPolished();
    
        std::cout <<"\nLegal Moves:";
        printMoveList(legalMoves);
         std::cout <<"  -> " << legalMoves.size();
       //  std::cout <<"\nCheckpoint B\n";
       //std::cout << "\nDebug: checkpoint 3: " << legalMoves.empty() << std::endl;
       //printMoveList(legalMoves);
    return (legalMoves.empty());
}

    bool isStaleMate(Board boardState, Color colorTakingTurn){     
        std::vector<MoveInformation> possibleMoves; 
        if (colorTakingTurn == white){
            possibleMoves = mg.generateLegalMoves(boardState, white);
        }
        else{
            possibleMoves = mg.generateLegalMoves(boardState,black);
        }
        int fiftyMoveRuleFlag = 1;
        size_t sizeOfNewMoveList = std::min(size_t(100),masterMoveList.size());
        std::vector<MoveInformation> pastFiftyMoves (masterMoveList.end() - sizeOfNewMoveList, masterMoveList.end()); //creates a subvector of the moves made so far in the game, size is either 100 half moves or how many moves have been made in the game
        if (pastFiftyMoves.size() < 100) fiftyMoveRuleFlag = 0;
        else{ //size == 100
            for (int i = 0; i < pastFiftyMoves.size(); i++){
                if (pastFiftyMoves.at(i).pieceType == pawn || pastFiftyMoves.at(i).isCapture == true) fiftyMoveRuleFlag = 0;
                //fifty move rule requires no pawn moves or no captures occur for 50 (100 half) moves
            }
        }
        if (fiftyMoveRuleFlag == 1) std::cout <<"\nFIFTY MOVE RULE! STALEMATE!\n";
        return  ( (possibleMoves.empty() && !isCheckMate(colorTakingTurn,boardState)) || fiftyMoveRuleFlag == 1 );
        //if its not mate but player cant move | isCheckMate call takes care of ensuring consideration of check
    }

    void checkGameCondition(Game &gameOfInterest){ // will check for the play taking turn
        Color colorTakingTurn = gameOfInterest.getColorOfPlayerTakingTurn();
        Board currentBoard = gameOfInterest.getBoard();

        if (isCheckMate(colorTakingTurn,currentBoard) ){
            if (colorTakingTurn == white) gameOfInterest.gameCondition = blackWins; //it is checkmate for white so black wins
            else gameOfInterest.gameCondition = whiteWins; //it is checkmate for black so white wins
            return;
            
        }
        else if (isStaleMate(currentBoard,colorTakingTurn)) gameOfInterest.gameCondition = stalemate; //color does not matter for stalemate
    }

};
