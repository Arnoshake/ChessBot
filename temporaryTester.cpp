#include <iostream>
#include<stdio.h>
#include <string>  // included for chess notation
#include <bitset>  //included for decimal to binary conversion
#include <sstream> //included for move print
#include <cstdint>
#include "chessGameCurrent.cpp"
#include <fstream> //included to write to output file
#include <vector>

int main() {
    init_sliders_attacks(0);
    init_sliders_attacks(1);
    Game game;
    //game.getBoard().addPiece(black,pawn,d4);
  
   
                    
    while (1){
        game.getGameTurnCount()++;
        game.getBoard().updateFriendlyEnemy(white);
    std::vector<MoveInformation> moveList = game.generateLegalMoves(game.getBoard(),white); //for checking if its end of game...
    // game.identifyCheckMateMoves(moveList,game.getBoard( ) );
    game.getBoard().displayBoardPolished();
    game.takeGameHalfTurn(white);

    if (moveList.empty() && !game.isCheckMate(black,game.getBoard())) break; //no legal moves but not mate
    if (game.isCheckMate(black,game.getBoard())){
        game.winner = 1;
        break;
    }
    game.getBoard().updateFriendlyEnemy(black);
    moveList = game.generateLegalMoves(game.getBoard(),black);
    game.identifyCheckMateMoves(moveList,game.getBoard( ) );
    //printBitBoard(game.getBoard().getPawnMask(d4,black));
     std::cout<<"\n<------HALF TURN ------>\n";
    game.getBoard().displayBoardPolished();
    game.takeGameHalfTurn(black);
    //printBitBoard(game.getBoard().getPawns(black));
    
    if (moveList.empty() && !game.isCheckMate(white,game.getBoard())) break; //no legal moves but not mate
    if (game.isCheckMate(white,game.getBoard())){                                                                                   //ISSUE RIGHT HERE!!!!
        game.winner = -1;
        break;
    }
    std::cout<<"\n<------<>------>\n";
    }
    std::cout << "\n\n";
    if (game.winner == 1){
        std::cout<<"WHITE WINS!\n";
    }
    if (game.winner == -1){
        std::cout<<"BLACK WINS!\n";
    }
    if (game.winner == 0){
        std::cout<<"STALEMATE!\n";
    }
    
    
    std::cout <<"\nExiting the Program...\n";
    return 0;
}