#include <iostream>
#include<stdio.h>
#include <string>  // included for chess notation
#include <bitset>  //included for decimal to binary conversion
#include <sstream> //included for move print
#include <cstdint>
#include "chessGameCurrent.cpp"
#include <fstream> //included to write to output file



int main() {
    Board board;
    printBitBoard(board.getWhitePawn());
    board.addPiece(white,pawn,a3);
    //printBitBoard(board.getWhitePawn());
    board.addPiece(black,pawn,b4);
    //board.updateFriendlyEnemy(white);

    std::cout << "Before move:\n";
    printBitBoard(board.getWhitePawn());
    printBitBoard(board.getBlackPawn());
    MoveInformation move(white, pawn, a3, b4,pawn);


    board.makeMove(board, move);  // ← make sure this is pass-by-reference

    std::cout << "After move:\nPawn:\n";
    printBitBoard(board.getWhitePawn());
    printBitBoard(board.getBlackPawn());                                    //THIS NEEDS TO BE FIXED!!!
    board.undoMove(board, move);  // ← make sure this is pass-by-reference
    std::cout << "After undo:\n";
    printBitBoard(board.getWhitePawn());
    printBitBoard(board.getBlackPawn());

    return 0;
}