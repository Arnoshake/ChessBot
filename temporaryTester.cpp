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
        // game.getBoard().removePiece(black,king,e8);
        // game.getBoard().addPiece(black,king,f5);
        // game.getBoard().removePiece(white,pawn,f2);
        // game.getBoard().removePiece(white,bishop,f1);
        // game.getBoard().removePiece(white,knight,g1);

        
        game.getGameTurnCount()++;
        game.getBoard().updateFriendlyEnemy(white);
        moveList = game.generateLegalMoves(game.getBoard(),white); //for checking if its end of game...
    // game.identifyCheckMateMoves(moveList,game.getBoard( ) );
        game.getBoard().displayBoardPolished();
        //game.getBoard().printCastlingStats(); 
        game.takeGameHalfTurn(white);

        // REPLACE WITH FUNCTION
        if (moveList.empty() && !game.isCheckMate(black,game.getBoard())) break; //no legal moves but not mate
        if (game.isCheckMate(black,game.getBoard())){
            game.winner = 1;
            break;
        }
    
        
        // game.identifyCheckMateMoves(moveList,game.getBoard( ) );
        //printBitBoard(game.getBoard().getPawnMask(d4,black));
        std::cout<<"\n<------HALF TURN ------>\n";
        game.getBoard().updateFriendlyEnemy(black);
        moveList = game.generateLegalMoves(game.getBoard(),black);

        game.getBoard().displayBoardPolished();
        //game.getBoard().printCastlingStats();
        game.takeGameHalfTurn(black);
   
        if (game.getBoard().getKing(white) == 0ULL|| game.getBoard().getKing(black) == 0ULL ){
            std::cout <<"\n\n\nNO KING DETECTED: THIS IS IMPOSSIBLE\n\n\n";
            return -1;
        }
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