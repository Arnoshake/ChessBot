#include <iostream>
#include<stdio.h>
#include <string>  // included for chess notation
#include <bitset>  //included for decimal to binary conversion
#include <sstream> //included for move print
#include <cstdint>

#include <fstream> //included to write to output file
//included because I think in terms of squares... not indexes!
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
//MY CODE!!
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





    /*int fromSquare = square;

    This is older code. I did not like how the while loop was too abstract and very long code
        After looking at chess programming youtube, his for loops were superior. decided to switch to this
    uint64_t rightBorder = 0x8080808080808080ULL;
    uint64_t leftBorder =  0x0101010101010101ULL;
    uint64_t topBorder =   0xFF00000000000000ULL;
    uint64_t botBorder =   0x00000000000000FFULL;

        uint64_t rookAttacks = 0x0ULL;
        uint64_t leftPossibleMoves = 0x0ULL;
        uint64_t rightPossibleMoves = 0x0ULL;
        uint64_t upPossibleMoves = 0x0ULL;
        uint64_t downPossibleMoves = 0x0ULL;
        // need to capture | and -- moves, before interuption
       
        // step 1 is to map all theoretical moves (before blockage)
        uint64_t spotOfInterest = 1ULL << fromSquare; // 000010000
        while ((spotOfInterest & rightBorder) == 0) 
        { // while this square is not on the border
            rightPossibleMoves |= spotOfInterest; //combine the existing possible moves with the spot of interest mask
            spotOfInterest = (spotOfInterest << 1); //shift right 
            std::cout <<"RIGHT: " << rightPossibleMoves << std::endl;

           


            if (spotOfInterest & rightBorder){
                rightPossibleMoves |= spotOfInterest;
                break;
            }
        }
            
            
        rookAttacks |= rightPossibleMoves; //add the right possible moves to the master linear possible moves
        //std::cout <<"RIGHT*: " << rightPossibleMoves << std::endl;
        
        spotOfInterest = 1ULL << fromSquare;
        while ((spotOfInterest & leftBorder) == 0)
        { // while this square is not on the border
            leftPossibleMoves |= spotOfInterest; //combine the existing possible moves with the spot of interest mask
            spotOfInterest = (spotOfInterest >> 1); //shift right 
            std::cout <<"LEFT: " << leftPossibleMoves << std::endl;

           


            if (spotOfInterest & leftBorder){
                leftPossibleMoves |= spotOfInterest;
                break;
            }
        }
        //std::cout <<"LEFT*: " << leftPossibleMoves << std::endl;
        rookAttacks |= leftPossibleMoves; //add the right possible moves to the master linear possible moves


        spotOfInterest = 1ULL << fromSquare;
        while ((spotOfInterest & topBorder) == 0)
        { // while this square is not on the border
            upPossibleMoves |= spotOfInterest; //combine the existing possible moves with the spot of interest mask
            spotOfInterest = (spotOfInterest << 8); //shift right 
            //std::cout <<"UP: " << upPossibleMoves << std::endl;

           
            if (spotOfInterest & topBorder){
                upPossibleMoves |= spotOfInterest;
                break;
            }
        }
        //std::cout <<"TOP*: " << upPossibleMoves << std::endl;
        rookAttacks |= upPossibleMoves; //add the right possible moves to the master linear possible moves
            
        spotOfInterest = 1ULL << fromSquare;
        while ((spotOfInterest & botBorder) == 0) 
        { // while this square is not on the border
            downPossibleMoves |= spotOfInterest; //combine the existing possible moves with the spot of interest mask
            spotOfInterest = (spotOfInterest >> 8); //shift right 
           // std::cout <<"DOWN: " << downPossibleMoves << std::endl;

            if (spotOfInterest & botBorder){
                downPossibleMoves |= spotOfInterest;
                break;
            }
        }
            
        //std::cout <<"BOT*: " << downPossibleMoves << std::endl;
        rookAttacks |= downPossibleMoves; //add the right possible moves to the master linear possible moves

        rookAttacks = rookAttacks ^ (1ULL << fromSquare);
    */
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










    /*
    int fromSquare = square;
    uint64_t rightBorder = 0x8080808080808080ULL;
    uint64_t leftBorder =  0x0101010101010101ULL;
    uint64_t topBorder =   0xFF00000000000000ULL;
    uint64_t botBorder =   0x00000000000000FFULL;

        uint64_t bishopAttacks = 0x0ULL;
        uint64_t northEastMoves = 0x0ULL;
        uint64_t southEastMoves = 0x0ULL;
        uint64_t southWestMoves = 0x0ULL;
        uint64_t NorthWestMoves = 0x0ULL;
        // need to capture | and -- moves, before interuption
       
        // step 1 is to map all theoretical moves (before blockage)
        uint64_t spotOfInterest = 1ULL << fromSquare; // 000010000
        while ((spotOfInterest & (rightBorder | topBorder)) == 0) //NORTHEAST 
        { // while this square is not on the border
            northEastMoves |= spotOfInterest; //combine the existing possible moves with the spot of interest mask
            spotOfInterest = (spotOfInterest << 9); //shift up and right (number is larger by 9 digits aka SL)
            //std::cout <<"RIGHT: " << northEastMoves << std::endl;

           


            if (spotOfInterest & (rightBorder | topBorder)){
                northEastMoves |= spotOfInterest;
                break;
            }
        }
            
            
        bishopAttacks |= northEastMoves; //add the right possible moves to the master linear possible moves
        //std::cout <<"RIGHT*: " << rightPossibleMoves << std::endl;
        
        spotOfInterest = 1ULL << fromSquare;
        while ((spotOfInterest & (rightBorder | botBorder)) == 0) //SOUTHEAST
        { // while this square is not on the border
            southEastMoves |= spotOfInterest; //combine the existing possible moves with the spot of interest mask
            spotOfInterest = (spotOfInterest >> 7); //shift right 
            std::cout <<"LEFT: " << southEastMoves << std::endl;

           


            if (spotOfInterest & (rightBorder | botBorder)){
                southEastMoves |= spotOfInterest;
                break;
            }
        }
        //std::cout <<"LEFT*: " << leftPossibleMoves << std::endl;
        bishopAttacks |= southEastMoves; //add the right possible moves to the master linear possible moves


        spotOfInterest = 1ULL << fromSquare;
        while ((spotOfInterest & (leftBorder | botBorder)) == 0) //SOUTHWEST
        { // while this square is not on the border
            southWestMoves |= spotOfInterest; //combine the existing possible moves with the spot of interest mask
            spotOfInterest = (spotOfInterest >> 9); //shift right 
            //std::cout <<"UP: " << upPossibleMoves << std::endl;

           
            if (spotOfInterest & (leftBorder | botBorder)){
                southWestMoves |= spotOfInterest;
                break;
            }
        }
        //std::cout <<"TOP*: " << upPossibleMoves << std::endl;
        bishopAttacks |= southWestMoves; //add the right possible moves to the master linear possible moves
            
        spotOfInterest = 1ULL << fromSquare;
        while ((spotOfInterest & (leftBorder | topBorder)) == 0) 
        { // while this square is not on the border
            NorthWestMoves |= spotOfInterest; //combine the existing possible moves with the spot of interest mask
            spotOfInterest = (spotOfInterest << 7); //shift right 
           // std::cout <<"DOWN: " << downPossibleMoves << std::endl;

            if (spotOfInterest & (leftBorder | topBorder)){
                NorthWestMoves |= spotOfInterest;
                break;
            }
        }
            
        //std::cout <<"BOT*: " << downPossibleMoves << std::endl;
        bishopAttacks |= NorthWestMoves; //add the right possible moves to the master linear possible moves

        bishopAttacks = bishopAttacks ^ (1ULL << fromSquare);

        return bishopAttacks;
        */
}
uint64_t getKnightMask(int square){
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
    /*
        15  17
     6         10
           x
     10         6
        17  15
    
 up is SL, down is SR
    */
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
uint64_t getPawnMask(int square, int side){
    int rank = square / 8;
    int file = square % 8;
    //1 = white, 0 = black
    uint64_t pawnAttack = 0ULL;

    if (side == 1){ //white pawn (moves upward)
        if (rank == 1){ //has not moved yet, able to move two forward
            pawnAttack |= 1ULL << (square + 8);
        
        }
        else{ //pawn can move one
            if (rank < 7) pawnAttack |= 1ULL << (square + 8);
        }
    }
    else if (side == 0){ //black (moves downward)
        //do not need to worry about overflow because pawns never exist on outer ranks... they promote

        if (rank == 6){ //has not moved yet, able to move two forward
            pawnAttack |= 1ULL << (square - 8);
            pawnAttack |= 1ULL << (square - 16);
        }
        else{ //pawn can move one
            if (rank > 0)pawnAttack |= 1ULL << (square - 8);
        }
    }
    return pawnAttack;

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
uint64_t getPawnCaptures(int square, int side){
    int rank = square / 8;
    int file = square % 8;

    uint64_t pawnCaptures = 0ULL;
    if (side == 1){
        if (rank < 7){
            pawnCaptures |= 1ULL << (square + 9);
            pawnCaptures |= 1ULL << (square + 7);
        }
    }
    else{
        if (rank > 0){
            pawnCaptures |= 1ULL << (square - 9);
            pawnCaptures |= 1ULL << (square - 7);
        }
    }
    return pawnCaptures;
}

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

void writeToFileFormat(std::ofstream& outputFile,std::string name, uint64_t pieceArray[64]){
    outputFile << "const uint64_t " << name << "[64] = {\n";

    // Loop through the data to format the array for copy-pasting
    for (size_t i = 0; i < 64; ++i) {
        outputFile << pieceArray[i];

        // Add a comma after each element except the last
        if (i != 63) {
            outputFile << "," << std::endl;
        }

    }

    // Close the array definition
    outputFile << "\n};" << std::endl << std::endl;
}

struct PawnMoves{

};
int main(){
    init_sliders_attacks(1); //for bishops
    init_sliders_attacks(0); //for rooks
    uint64_t blackPieces = 0ULL;
    uint64_t whitePieces = 0ULL;
    set_bit(blackPieces,c3); //black piece
    set_bit(blackPieces,a3); //black piece
    set_bit(whitePieces,b3);
    set_bit(whitePieces,a1);  //friendly white piece
    printBitBoard(get_rook_attacks(b3,(whitePieces|blackPieces)));
    
    /*
    //EXAMPLE CODE FOR HOW TO USE BISHOP, ROOK MAGIC STUFF... IMPLIES QUEEN MOVES
    init_sliders_attacks(1); //for bishops
    init_sliders_attacks(0); //for rooks
    uint64_t occupiedSquares = 0ULL;
    uint64_t possibleMoves = 0ULL;
    uint64_t blackPieces = 0ULL;
    uint64_t whitePieces = 0ULL;
    set_bit(blackPieces,c3); //black piece
    set_bit(blackPieces,a3); //black piece
    set_bit(whitePieces,b3);
    set_bit(whitePieces,a1);  //friendly white piece
    uint64_t enemyOccupiedSquares = blackPieces;
    occupiedSquares = whitePieces | blackPieces;
    possibleMoves = getPawnMask(b2, 1);
    possibleMoves &= ~occupiedSquares; //only keeps possible moves not blocked by existing
    uint64_t possibleCaptures = getPawnCaptures(b2, 1);
    possibleCaptures &= enemyOccupiedSquares;
    possibleMoves |= possibleCaptures;
    printBitBoard(possibleCaptures);
    printBitBoard(possibleMoves);
    */
    /*
    for (int i = 8; i < 64;i++){
        printBitBoard(getPawnMask(i,1));
    }

    std::ofstream outFile("possibleMoveMasks.txt");  // Open (or create) a file named "output.txt"

    if (!outFile) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }
        
    uint64_t whitePawnMoves[64];
    uint64_t blackPawnMoves[64];
    uint64_t knightMoves[64];
    uint64_t rookMoves[64];
    uint64_t bishopMoves[64];
    uint64_t queenMoves[64];
    uint64_t kingMoves[64];
    for (int i = 0; i < 64; i ++){
        if (i > 7) whitePawnMoves[i] = getPawnMask(i,1);
        if (i < 56) blackPawnMoves[i] = getPawnMask(i,0);

        knightMoves[i] = getKnightMask(i);
        rookMoves[i] = getRookMask(i);
        bishopMoves[i] = getBishopMask(i);
        queenMoves[i] = (rookMoves[i] | bishopMoves[i]);
        kingMoves[i] = getKingMask(i);
    }
    writeToFileFormat(outFile,"possibleWhitePawnMoves",whitePawnMoves);
    writeToFileFormat(outFile,"possibleBlackPawnMoves",blackPawnMoves);
    writeToFileFormat(outFile,"possibleKnightMoves",knightMoves);
    writeToFileFormat(outFile,"possibleKingMoves",kingMoves);
    
    
   
    outFile.close(); 
    
        */
    return 0;
    
}



