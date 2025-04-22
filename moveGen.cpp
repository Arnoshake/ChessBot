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
uint64_t getQueenMask(int square){
    return ( (getBishopMask(square) | (getRookMask(square)) ));
}
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

uint64_t generatePawnMoves1(Board state){ //NEED TO DO EN PASSANT AND TOUCH UP ON PASSED PARAMETERS
    //Modularize the code below to happen every time a state is created? (Outsource to board class)
    uint64_t occupied = state.getOccupiedSquares();
    uint64_t emptySquares = ~state.getOccupiedSquares();
    int color = state.getTurn(); //a board should store info on whos move it is, thats very important!

    uint64_t pawnBitBoard;
    uint64_t enemyPieces;
    uint64_t friendlyPieces;
    if (color){
        friendlyPieces = state.getWhitePieces();
        enemyPieces = state.getBlackPieces();
    }
    else{
        friendlyPieces = state.getBlackPieces();
        enemyPieces = state.getWhitePieces();
    }

    uint64_t notAFile =   0xFEFEFEFEFEFEFEFEULL;
    uint64_t notABFile =  0xFCFCFCFCFCFCFCFCULL;
    //prevents rightside overflow
    uint64_t notHFile =   0x7F7F7F7F7F7F7F7FULL;
    uint64_t notGHFile =  0x3F3F3F3F3F3F3F3FULL;

    uint64_t possibleMoves = 0ULL;
    uint64_t possibleCaptures = 0ULL;

    if (color == 1){ // pawns move up.. << & en Passant rank = 4
        std::cout<<"cheese" << std::endl;
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
    return 1ULL;
}

void addPiece(Board state, int color, int piece, int square){
    if (white){
        switch(piece){
            case pawn:
                set_bit(state.getWhitePawn(), square);
                set_bit(state.getWhitePieces(),square);
                break;
            case bishop:
                set_bit(state.getWhiteBishop(), square);
                set_bit(state.getWhitePieces(),square);
                break;
            case knight:
                set_bit(state.getWhiteKnight(), square);
                set_bit(state.getWhitePieces(),square);
                break;
            case rook:
                set_bit(state.getWhiteRook(), square);
                set_bit(state.getWhitePieces(),square);
                break;
            case queen:
                set_bit(state.getWhiteQueen(), square);
                set_bit(state.getWhitePieces(),square);
                break;
            case king:
                set_bit(state.getWhiteKing(), square);
                set_bit(state.getWhitePieces(),square);
                break;
        }

    }
    else{
        switch(piece){
            case pawn:
                set_bit(state.getBlackPawn(), square);
                set_bit(state.getBlackPieces(),square);
                break;
            case bishop:
                set_bit(state.getBlackBishop(), square);
                set_bit(state.getBlackPieces(),square);
                break;
            case knight:
                set_bit(state.getBlackKnight(), square);
                set_bit(state.getBlackPieces(),square);
                break;
            case rook:
                set_bit(state.getBlackRook(), square);
                set_bit(state.getBlackPieces(),square);
                break;
            case queen:
                set_bit(state.getBlackQueen(), square);
                set_bit(state.getBlackPieces(),square);
                break;
            case king:
                set_bit(state.getBlackKing(), square);
                set_bit(state.getBlackPieces(),square);
                break;
        }
    }
}


bool isLegalMove(Board state, Move move){

}
int main(){
    //iniialize magic stuff working for rooks, bishops
    init_sliders_attacks(1); //for bishops
    init_sliders_attacks(0); //for rooks
    Board state;
    state.initializeBoard();
    addPiece(state,1,pawn,d7);
    addPiece(state,1,pawn,h7);
    addPiece(state,1,pawn,e6);
    
    printBitBoard(generatePawnMoves1(state));
    return 0;
    


    
}


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



