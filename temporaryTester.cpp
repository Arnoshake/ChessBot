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
    //updates castling rights at game start
    game.getBoard().updateCastlingRights(game.getBoard(),MoveInformation(),white);
    game.getBoard().updateCastlingRights(game.getBoard(),MoveInformation(),black);
    //game.getBoard().addPiece(black,pawn,d4);
    std::vector<MoveInformation> moveList;

   
                    
    while (1){

        //WHITE TURN
        game.getGameTurnCount()++;
        game.getBoard().updateFriendlyEnemy(white);
        moveList = game.generateLegalMoves(game.getBoard(),white);
        game.getBoard().displayBoardPolished();
        game.takeGameHalfTurn(white);

        game.checkGameCondition(game);
        if (game.gameCondition != ongoing) break;
        std::cout<<"\n    <------HALF TURN ------>\n";

        //BLACK TURN
        game.getBoard().updateFriendlyEnemy(black);
        moveList = game.generateLegalMoves(game.getBoard(),black);
        game.getBoard().displayBoardPolished();
        game.takeGameHalfTurn(black);

        game.checkGameCondition(game);
        if (game.gameCondition != ongoing) break;
         std::cout<<"\n<------<>------>\n";
    }
        

    //only executes once gameCondition != ongoing

    std::cout << "\n\n";
    if (game.gameCondition == whiteWins){
        std::cout<<"WHITE WINS!\n";
    }
    else if (game.gameCondition == blackWins){
        std::cout<<"BLACK WINS!\n";
    }
    else if (game.gameCondition == stalemate){
        std::cout<<"STALEMATE!\n";
    }
    else{
        std::cout<<"THIS SHOULD NOT HAPPEN | GAME CONDITION ERROR!\n";
    }

    
    std::ofstream outfile("GameInformation.txt");  // Open file for writing
    if (!outfile) {  // Check if file opened successfully
        std::cerr << "Error opening file for writing!" << std::endl;
        return 1;
    }
    
    game.boardStates.push_back(game.getBoard()); //adds the checkmated board state
    std::cout << game.masterMoveList.size() << "\n";
    outfile << "MOVES MADE THIS GAME:" << std::endl;  // Write to the file
    for (int i = 0 ;i < game.masterMoveList.size();i++){
        /*
            1. white_move , black_move
            2. white_move , black_move
        */
        if (i %2 == 0){ //white moves
            outfile << std::endl <<  (i/2 + 1)<< ". "<< game.masterMoveList.at(i).chessNotation;
            if (i != game.masterMoveList.size() - 1) outfile << " , ";
            
        }
        else{ //black moves
        outfile << game.masterMoveList.at(i).chessNotation ;
        }

    }

    outfile.close();  // Close the file

    std::cout <<"\nExiting the Program...\n";
    return 0;
}