#include <iostream>
#include<stdio.h>
#include <string>  // included for chess notation
#include <bitset>  //included for decimal to binary conversion
#include <sstream> //included for move print
#include <cstdint>
#include "chessGameCurrent.cpp"
#include <fstream> //included to write to output file







int main(){
    //iniialize magic stuff working for rooks, bishops
    init_sliders_attacks(1); //for bishops
    init_sliders_attacks(0); //for rooks

    std::cout << "Let's Play Chess!\n";
    Game game;
    game.getBoard().removePiece(white,pawn,a2);
    game.displayBoard();
  
    // Board test;
    // test.addPiece(white,pawn,a2);
    // //test.addPiece(white,pawn,h2);
    // printBitBoard(test.getOccupiedSquares());
    // std::vector<MoveInformation> possibleLegalMoves = game.generateLegalMoves(white);
    // for (int i = 0; i < possibleLegalMoves.size();i++){
    //     game.printMove( possibleLegalMoves.at(i) ) ;
    // }

    return 0;
}




