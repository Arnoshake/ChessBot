#include <iostream>
#include<stdio.h>
#include <string>  // included for chess notation
#include <bitset>  //included for decimal to binary conversion
#include <sstream> //included for move print
#include <cstdint>
#include "chessGameCurrent.cpp"
#include <fstream> //included to write to output file






//g++ -std=c++11 codeTest.cpp
int main(){
    //iniialize magic stuff working for rooks, bishops
    init_sliders_attacks(1); //for bishops
    init_sliders_attacks(0); //for rooks

    std::cout << "Let's Test Chess!\n";
    Game game;
    while (1){
        //check if game is over...

        //white
        game.displayBoard();
        std::vector < MoveInformation > possibleMoves = game.generateLegalMoves(game.getGameTurn());
        for(int i = 0; i <possibleMoves.size();i++){
            std::cout << game.getMoveString(possibleMoves.at(i)) << ", ";
        }
        std::cout <<std::endl;
        game.takeGameHalfTurn(game.getGameTurn());
        game.switchTurns();
        std::cout << "XZYColor: " << game.getGameTurn() << std::endl;
        game.displayBoard();
        possibleMoves = game.generateLegalMoves(game.getGameTurn());
        for(int i = 0; i <possibleMoves.size();i++){
            std::cout << game.getMoveString(possibleMoves.at(i)) << ", ";
        }
        std::cout <<std::endl;
        game.takeGameHalfTurn(game.getGameTurn());
        game.switchTurns();

    }
    
    return 0;
}




