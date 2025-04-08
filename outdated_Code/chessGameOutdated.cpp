/*
    Holds some pseudocode for early brainstorming for method layout and general required logic

    This also holds dead code for parsing algebraic notation into usable information. 
    The way I went about it was confusing as I relied on indexing of a string that order of characters mattered. 
    I opted to switch to parsing the string with C++ string library methods instead of shifting an index as it was easier to visualize and accomplished the same thing
*/
/*
Board Representation
    DONE: Color and piece bitboards 

    user input
        Manual moves: Chess Notation
            Converting "E5-B3" into a value for each square
                Going to need a decimal <--> binary <--> hex set of functions

    Making a Move (from, to):
        determine who's turn.

        determine the color and piece type for the "from" square
        determine the color and piece type for the "to" square

        determine if the move is legal
            mask(from piece's legal moves, to square)
        Make the move
            clear the space occupied
            occupy the new square
            update the relevant boards



Game Execution

    while the game is not over:
        turn = x
        print the board
        prompt a turn
        check for if the game is over

    print winning message

Functions required:

checkGameState --> is the game ongoing, tied, or won
isCheck --> is the king in checkmate?
Bi/Hex/Dec conversion --> set of functions to accomplish each combo
userInput --> intake chess notation move
convertuserInputToDecimal --> convert the string of the move into usable decimal values
checkLegality --> ensures that a move is legal by all rules
makeMove --> map and updates the relevant bitboards and general boardstate

*/






bool Board::isLegal(Board::MoveInformation move){
    /*
    NEED TO CHECK:
    1. Moving Rules
        *Piece-Type
        *No Jumping (except N)
    2. Destination Square
        *can only move to an empty or enemy square
    3. No move can endanger the king

    Special Conditions:
    1. Checking opp king
    2. castling
    3. Pawn movement
        *En Passant
        *Promotion
        *2 Piece move
    */

    //Castling
   
    //DESTINATION SQUARE LEGALITY                   
    int toSquare = move.toSquare;
    if (move.turn == 1){ //WHITE IS MOVING
        //PIECE
        /*
            ensure that there is a piece described to move there
        */
        //DESTINATION SQUARE                DONE!
        if (isWhitePiece(toSquare) ){
            std::cout << "Illegal Move. Cannot capture your own piece." << std::endl;
            return false;
        }
        if (!isWhiteKnight(move.fromSquare)){
            //if the piece being moved is NOT a knight, the pathway must be not obstructed
            
        }
        if (move.isKingCastle){ //FINISH!!!
            if (!isWhiteKing(4) || !isWhiteRook(7)) return false; //pieces not in position
            if (isOccupied(5) || isOccupied(6)) return false; //pieces interrupting
            //check for if pieces have moved (rook, king)
            //check for if any of the involved/between squares risk check
            
        }
    }
    else{ //BLACK IS MOVING
        if ( isBlackPiece(toSquare) ){
            std::cout << "Illegal Move. Cannot capture your own piece." << std::endl;
            return false;
        }
    }
    
    //write inCheck function that will ensure that
   
}
void Board::makeMove(Board::MoveInformation move){
    
}
/*
Display the board
[User Input]
parseMove()
isLegal()
makeMove()
update turn
repeat
*/



