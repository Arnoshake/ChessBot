#include <iostream>
#include<stdio.h>
#include <string>  // included for chess notation
#include <bitset>  //included for decimal to binary conversion
#include <sstream> //included for move print
#include <cstdint>
#include "chessGameCurrent.cpp"
#include <fstream> //included to write to output file
//included because I think in terms of squares... not indexes! Commented out because included temporarily in chessGameCurrent
/*enum Square {
    a1 = 0,  b1,  c1,  d1,  e1,  f1,  g1,  h1,
    a2 = 8,  b2,  c2,  d2,  e2,  f2,  g2,  h2,
    a3 = 16, b3,  c3,  d3,  e3,  f3,  g3,  h3,
    a4 = 24, b4,  c4,  d4,  e4,  f4,  g4,  h4,
    a5 = 32, b5,  c5,  d5,  e5,  f5,  g5,  h5,
    a6 = 40, b6,  c6,  d6,  e6,  f6,  g6,  h6,
    a7 = 48, b7,  c7,  d7,  e7,  f7,  g7,  h7,
    a8 = 56, b8,  c8,  d8,  e8,  f8,  g8,  h8
};*/
enum PieceType {
    P, N, B, R, Q, K, // 0-5 White
    p, n, b, r, q, k, // 6-11 Black
    e //12 empty
};


//MY CODE
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

//MOVEMENT MASKS (w/o pieces) -- CODE MAY BE OBSOLETE NOW
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
uint64_t getQueenMask(int square){
    return ( (getBishopMask(square) | (getRookMask(square)) ));
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

//CURRENTLY WOKRING ON!
uint64_t generatePawnMoves1(uint64_t pawnBitBoard, uint64_t occupied, int color, uint64_t enemyPieces ){ //white = 
    
    uint64_t notAFile =   0xFEFEFEFEFEFEFEFEULL;
    uint64_t notABFile =  0xFCFCFCFCFCFCFCFCULL;
    //prevents rightside overflow
    uint64_t notHFile =   0x7F7F7F7F7F7F7F7FULL;
    uint64_t notGHFile =  0x3F3F3F3F3F3F3F3FULL;

    uint64_t emptySquares =  ~occupied;
    uint64_t possibleMoves = 0;
    
    uint64_t possibleCaptures = 0ULL;
    if (color == 1){ // pawns move up.. << & en Passant rank = 4
        std::cout<<"cheese" << std::endl;
        uint64_t singlePush = ( pawnBitBoard << 8) & emptySquares ; 
        uint64_t doublePush = ( ( ( (0xFF00 & pawnBitBoard) << 8) & emptySquares ) << 8 ) & emptySquares; //checks for unmoved pawns, single pushes, pushes again
    
        possibleMoves = singlePush | doublePush ;
        
        possibleCaptures |= ( (pawnBitBoard & notHFile) << 9 ) & enemyPieces;
        possibleCaptures |= ( (pawnBitBoard & notAFile )<< 7) & enemyPieces;
        possibleMoves |= possibleCaptures;
        return possibleCaptures | possibleMoves;
    }
    else{ //identical logic, different variable values and shifting operator
        return 0ULL;
    }
    return 1ULL;
}




int main(){
    //iniialize magic stuff working for rooks, bishops
    init_sliders_attacks(1); //for bishops
    init_sliders_attacks(0); //for rooks

    uint64_t blackPieces = 0ULL;
    uint64_t whitePieces = 0ULL;
   
    uint64_t occupied = blackPieces | whitePieces;
    
    printBitBoard(generatePawnMoves1(whitePieces,occupied,1,blackPieces) );
    







    /*
    //CODE FOR CREATING LOOKUP TABLE FOR KNIGHTS, KINGS
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



//ARCHIVED KNIGHT CODE

/* This code is naive, first approach at creating bitboard-scale move generation. The power of bitboard is that parallel actions can exist (apply to all at same time)
uint64_t generateKnightMovesBitBoard(uint64_t knightBitBoard, uint64_t occupiedSquares){
    uint64_t knightMovesBoard = 0ULL;
    for (int sq = 0; sq < 64 ; sq ++){
        if (isSet(knightBitBoard,sq)){
            knightMovesBoard |= getKnightMask(sq);
        }
    }
    return knightMovesBoard;
}

  //This code is bad because it checks the mapping AFTER shifting... you want to prevent bad moves BEFORe they happen!
   //if ( (bitboard >> 17) & (notHFile) ) knightMoves |= (bitboard >> 17);
   //if ( (bitboard >> 15) & (notAFile) ) knightMoves |= (bitboard >> 15);
   //if ( (bitboard >> 10) & (notGHFile) ) knightMoves |= (bitboard >> 10);
   //if ( (bitboard >> 6)  & (notABFile) ) knightMoves |= (bitboard >> 6);
   
   //if ( (bitboard << 17) & (notAFile) ) knightMoves |= (bitboard << 17);
   //if ( (bitboard << 15) & (notHFile) ) knightMoves |= (bitboard << 15);
   //if ( (bitboard << 10) & (notABFile) ) knightMoves |= (bitboard << 10);
   //if ( (bitboard << 6)  & (notGHFile) ) knightMoves |= (bitboard << 6);
*/



// ARCHIVED ROOK MOVE GEN, NO BLOCKS, SQUARE BASED


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




    //OLD CODE, WAS WRITTEN BEFORE UNDERSTANDING THE STRENGTHS OF BITBOARD

    /*
// I DONT KNOW IF THIS METHOD IS EVEN NEEDED... BOARDS WILL BE PASSED AND IT WILL BE EXPLICIT WHEN DONE... NOT A DYNAMIC CALL
uint64_t getRelevantMoveMask(int fromSquare, char pieceType, Board state ){
    // Completed on 4/7/25
    // returns the relevant map for a given square.
    init_sliders_attacks(1); //for bishops
    init_sliders_attacks(0); //for rooks

    pieceType = tolower(pieceType);
    uint64_t possibleMoves = 0x0000000000000000ULL;

    uint64_t occupiedSquares = state.getOccupiedSquares();
    uint64_t friendlyOccupiedSquares;
    uint64_t enemyOccupiedSquares;
    if (state.isWhiteTurn()){
        friendlyOccupiedSquares = state.getWhitePieces();
        enemyOccupiedSquares = state.getBlackPieces();

    }
    else{
        friendlyOccupiedSquares = state.getBlackPieces();
        enemyOccupiedSquares = state.getWhitePieces();
    }

    switch(pieceType){
        //NEED TO DO PAWN AND KING BECAUSE THEIR MOVES DEPEND ON OCCUPIED
        case 'p': //pawn
            possibleMoves = getPawnMask(fromSquare, state.isWhiteTurn());
            possibleMoves &= ~occupiedSquares; //only keeps possible moves not blocked by existing
            
            uint64_t possibleCaptures = getPawnCaptures(fromSquare, state.isWhiteTurn());
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
}
*/
