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

    int turnCount = 0;

    while (1){
        game.displayBoard();
        std::cout << "Turn " << turnCount + 1 << ": It is *";
        if (game.getGameTurn() ) std::cout << "WHITE's* ";
        else std::cout <<"BLACK's*";
        std::cout << "turn!\n Please enter your move (alg chess notation): ";
        
        //MoveInformation turnMove = game.parseMove(game.getGameTurn()); // need to handle improper move entry!
        // while (!game.isLegalMove(turnMove)){         FIXXXXX                                                                   //fix the improper printing order (double printing due to while logic)
        //     std::cout<< "This is NOT a legal move! Please try again!  \n ";
        //     turnMove = game.parseMove(game.getGameTurn());
        // }
        
        return 0;
        //game.makeMove(turnMove);
        // game.moveList.push_back(turnMove); FIXXXX
        game.boardStates.push_back(game.getBoard()); //add the new boardstate to the history and make the move

       // if (game.isGameOver() ) break;

        game.switchTurns();
        turnCount++;
    }
    std::cout << "\nGame over!";
    //ADD DYNAMIC MESSAGING FOR STALEMATES/TIES
    if (game.getGameTurn()) std::cout << " White Wins!\n";
    else if (game.getGameTurn()) std::cout << " Black Wins!\n";

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



