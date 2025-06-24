#include "Game.h"
#include "util.h"
#include <cctype> // Required for std::tolower()
#include "MoveInformation.h"
Game::Game(){
        gameBoard.initializeBoard();
        playerTakingTurnColor = white;
        gameCondition = ONGOING;
        turnCount = 0; 

        masterMoveList.clear();
        boardStates.clear(); 
    }
Board& Game::getGameBoard()
{
    return gameBoard;
}
Board Game::getCopyBoard() const
{
    return gameBoard;
}
const Board& Game::getReadOnlyBoard() const
{
    return gameBoard;
}

int Game::getGameTurnCount() const{
        return turnCount;
}
void Game::setGameTurnCount(int newTurnCount){
        turnCount = newTurnCount;
}

Color Game::getColorOfPlayerTakingTurn() const{
        return playerTakingTurnColor;
}
void Game::setColorOfPlayerTakingTurn(Color nextPlayerColor) {
        playerTakingTurnColor = nextPlayerColor;
}


void Game::takeGameHalfTurn(Color turn){                                                        
       
        std::cout << "Turn " << getGameTurnCount();
        std::string gameMoveString;
        if (turn == white) std::cout << "W.";
        else if (turn == black) std::cout << "B.";
        
        // FIGURE OUT
        // std::vector<MoveInformation> possibleLegalMoves = MoveGenerator::generateLegalMoves(getReadOnlyBoard(), turn);
        // identifyCheckMateMoves(possibleLegalMoves,getReadOnlyBoard() );
        // updateNotationForMoveList(possibleLegalMoves);
        MoveInformation matchingMove; //initialized inside loop
        while (true) {
            std::string userInput;
            std::cout << "\nPlease make your move! (standard chess notation): ";
            while (1){
                std::cin >> userInput;
                // createMoveFromString(getBoard(), turn, userInput).printMoveInfo();
                // possibleLegalMoves.at(33).printMoveInfo();
                if (userInput == "quit" || userInput == "q" || userInput == "Q" || userInput == "Quit" ){
                std::cout << "\nQuitting program...\n";
                std::exit(0);
                }
                else if (userInput.length() == 1){
                    //printMoveList(possibleLegalMoves);
                std::cout << "\nNo legal move is a single character. Please enter a valid move.\n";
                }
                else{
                    break;
                }
            }   
            // move is not too small (seg fault) or a special command
            MoveInformation userInputtedMove = MoveParser::createMoveFromString(getReadOnlyBoard(),turn,userInput);
            userInputtedMove.playerColor = turn;
            //userInputtedMove.printMoveInfo();


            try {
                matchingMove = getMatchingMove(getReadOnlyBoard(),possibleLegalMoves, userInputtedMove);
                //matchingMove.printMoveInfo();
                break; // exit the loop since we found a valid move
            }
            catch (const std::runtime_error& e) {
                std::cout << "\nError: " << e.what() << "\n";
                std::cout << "Please enter a valid move.\n";
                printMoveList(possibleLegalMoves);
            }
        
        }

        matchingMove.printMoveInfo();
        
        getGameBoard().makeMove(matchingMove,1);                  //make move function does not discern legality, all illegal moves should be filtered out vefore this
        
        if (matchingMove.pieceType == pawn && abs(static_cast<int>(matchingMove.toSquare) - static_cast<int>(matchingMove.fromSquare) ) == 16) {
        getGameBoard().setEnPassantTargetSquare( Square( ( static_cast<int>(matchingMove.toSquare) + static_cast<int>(matchingMove.fromSquare) ) / 2) );
        //std::cout << "Setting enPassantTargetSquare to: " << matchingMove.toSquare  << " + "<<matchingMove.fromSquare<< " -->" <<getBoard().enPassantTargetSquare << "\n";

    }
        else{
            getGameBoard().setEnPassantTargetSquare(NO_SQUARE);
        }
        getGameBoard().updateCastlingRights(matchingMove,turn);

        
        Board newBoard = getCopyBoard();
        boardStates.push_back(newBoard); //adds the new (post-move) board to the game history
        masterMoveList.push_back(matchingMove); //adds the move to bridge between board states
        setColorOfPlayerTakingTurn( !getColorOfPlayerTakingTurn() ); //inverse itself
    
        getReadOnlyBoard().displayBoardPolished();
    }

bool isCheckMate(Color colorOfKing, Board boardState){
    boardState.updateFriendlyEnemy(colorOfKing);
            // std::cout << "\n[DEBUG] Checking for checkmate for " << (colorOfKing == white ? "White" : "Black") << "\n";
            // std::vector<MoveInformation> legalMovesTest = generateLegalMoves(board, colorOfKing);
            // std::cout << "[DEBUG] Legal moves found: " << legalMovesTest.size() << "\n";

            // for (auto& move : legalMovesTest) {
            //     move.printMoveInfo();
            // }

            // std::cout << "[DEBUG] King is " << (isKingInCheck(boardState,colorOfKing) ? "" : "not ") << "in check.\n";

        //std::cout << "\nDebug: checking for checkmate!\n";
        if (!boardState.isKingInCheck(colorOfKing)) return false; //if the king isnt in check, it cant be mate
        // std::cout <<"\nCheckpoint A\n";
        //std::cout << "\nDebug: checkpoint 2\n";
        std::vector<MoveInformation> legalMoves = MoveGenerator::generateLegalMovesOnBoard(boardState, colorOfKing);
        // dont tag checkmates. quality of moves do no matter, quantity does
        // boardState.displayBoardPolished();
    
        std::cout <<"\nLegal Moves:";
        printMoveList(legalMoves);
         std::cout <<"  -> " << legalMoves.size();
       //  std::cout <<"\nCheckpoint B\n";
       //std::cout << "\nDebug: checkpoint 3: " << legalMoves.empty() << std::endl;
       //printMoveList(legalMoves);
    return (legalMoves.empty());
}
