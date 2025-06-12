#include "MoveParser.h"
class MoveParser{
    public:
        std::string getMoveString(MoveInformation move) //creates a string from existing move characteristics
    {
        std::stringstream ss;
        if (move.isKingCastle) ss << "O-O";
        if (move.isQueenCastle) ss << "O-O-O";
        if (!(move.getPieceLetter(move.pieceType) == 'P') ){
            if ( !move.isKingCastle && !move.isQueenCastle) ss << move.getPieceLetter(move.pieceType);
        }
            
        if (move.isAmbiguous){
            if (!move.uniqueFile && move.uniqueRank) {
                ss << move.fromRank; 
            } 
            else if (move.uniqueFile && !move.uniqueRank) {
                ss << move.fromFile;
            } 
            else { // both are needed
                ss << move.fromFile;
                ss << move.fromRank;
    }
        }
        
        if (move.isCapture) ss << 'x';
        
        ss << move.toFile << move.toRank;
        if (move.isPromotion) ss << "=" << move.getPieceLetter(move.promotionPiece);

        if (move.isCheckMate) ss << "#";  // Checkmate gets priority
        else if (move.isCheck){
            ss << "+";
            //std::cout << "\nCHEESE2\n";
        }
        return ss.str();
    } 
        MoveInformation createMoveFromString(Board boardWaitingForMove, Color playerColor, std::string chessNotation){ //turn string into a move with rel attributes
        //assuming a possible chess notation (ex: NOT 1 character length)
        std::string moveStr = chessNotation; // will be editing this
        MoveInformation move;
        move.chessNotation = moveStr;
        // chatGPT suggested rewriting the creation of the specialty flags in this concise format.
        move.playerColor = playerColor;
        move.isCapture = moveStr.find('x') != std::string::npos;
        move.isPromotion = moveStr.find('=') != std::string::npos;
        move.isCheck = moveStr.find('+') != std::string::npos;
        move.isCheckMate = moveStr.find('#') != std::string::npos;
        move.isKingCastle = (moveStr == "O-O"); 
        move.isQueenCastle = (moveStr == "O-O-O");

        if (move.isKingCastle){
            Board copy = boardWaitingForMove;
            
            move.chessNotation = "O-O";
            move.pieceType= king;
            move.isAmbiguous = false;
            move.isCapture = false;

            if (playerColor == white){
               
                move.fromSquare = e1;
                move.toSquare = g1;
                return move;
            }
            else if (playerColor == black){
                move.toFile = 'g';
                move.toRank = '8';
                move.fromSquare = e8;
                move.toSquare = g8;
                
                return move;
            }
        }
        else if (move.isQueenCastle){
            move.chessNotation = "O-O-O";
             move.pieceType= king;
            if (playerColor == white){
                move.fromSquare = e1;
                move.toSquare = c1;
                return move;
            }
            else if (playerColor == black){
                move.fromSquare = e8;
                move.toSquare = c8;
                return move;
            }
        }
        
        move.isAmbiguous = false;                                       //all moves are considered not ambigious at the moment....
        // std::cout << moveStr << std::endl;
        switch (moveStr.at(0)){ //piece type

            case 'B':
                move.pieceType = bishop;
                moveStr = moveStr.substr(1);
                break;
            case 'N':
                move.pieceType = knight;
                moveStr = moveStr.substr(1);
                break;
            case 'R':
                move.pieceType = rook;
                moveStr = moveStr.substr(1);
                break;
            case 'K':
                move.pieceType = king;
                moveStr = moveStr.substr(1);
                break;
            case 'Q':
                move.pieceType = queen;
                moveStr = moveStr.substr(1);
                break;
            default:
                if ( (moveStr.at(0) >= 'a' && moveStr.at(0) <= 'h') || (moveStr.at(0) >= '1' && moveStr.at(0) <= '8') || (moveStr.at(0) == 'x')){
                    move.pieceType = pawn;
                }
                else{
                    move.pieceType= none;
                }
                break;
        }

        // std::cout << moveStr << std::endl;
        
        if (move.isCheck || move.isCheckMate)
        {
            moveStr = moveStr.substr(0, moveStr.length() - 1);
        }
        // std::cout << moveStr << std::endl;
        if (move.isPromotion)
        {
            switch (moveStr.at(moveStr.length() - 1) ){
                case 'B':
                    move.promotionPiece = bishop;
                    break;
                case 'N':
                    move.promotionPiece = knight;
                    break;
                case 'R':
                    move.promotionPiece = rook;
                    break;
                case 'Q':
                    move.promotionPiece = queen;
                    break;
    
            }
            moveStr = moveStr.substr(0, moveStr.length() - 2); // remove the last 2 characters because promotion = '=X' where X is piece
        }

        //std::cout << moveStr << std::endl;
        // the last thing in the move string at this point must be the "to" information
        move.toRank = moveStr.at(moveStr.length() - 1) ;
        moveStr = moveStr.substr(0, moveStr.length() - 1);

        move.toFile = moveStr.at(moveStr.length() - 1) ;                       
        //std::cout <<"\nFile: " << move.toFile << " Rank: " << move.toRank << "\n";
        moveStr = moveStr.substr(0, moveStr.length() - 1);
        // std::cout << moveStr << std::endl;
        int asciiFile = static_cast<int>(move.toFile) - 96; //SHOULD BE 97 FOR 0 INDEX SYSTEM!!!!!
        move.toSquare = Square( (asciiFile - 1) + 8 * (move.toRank - '1' ) ); // casting both rank and file to integers and calculating the square index
        if (move.isCapture)
        {
            moveStr = moveStr.substr(0, moveStr.length() - 1);
            move.capturedPiece = boardWaitingForMove.getPieceAtSquare(move.toSquare);
            // thing before to square, if capture is 'x'
        }
        // std::cout << moveStr << std::endl;
        //std::cout << "boop";
        if (moveStr.length() == 1) 
        {
            move.isAmbiguous = true;
            if (moveStr.at(0) >= 'a' && moveStr.at(0) <= 'h') move.fromFile = moveStr.at(0);
            if (moveStr.at(0) >= '1' && moveStr.at(0) <= '8') move.fromRank = moveStr.at(0);
            
        }
        if (moveStr.length() == 2){                                                     //DOUBLE AMBIGUITY CASE...UNSURE!
            move.fromFile = moveStr.at(0); 
            move.fromRank = moveStr.at(1);
        }
        // std::cout << moveStr << std::endl;

        // square = File * 8 + Rank
        // a = 1
        // rank = numbers, File == files
        
        
        //std::cout << "TESTING: " << asciiFile << ", " << (move.toRank - '0') << ", " << move.toSquare << std::endl;
        
        return move;


    }
        MoveInformation parseMove(Color playerColor)
    {
        std::string lineArg;
        std::getline(std::cin, lineArg);
        if (lineArg == "q") std::exit(0);
        while (lineArg.length() <2){
            std::cout << "Invalid input. Please try again\n";
            std::getline(std::cin, lineArg);
            if (lineArg == "q") std::exit(0);
        }
        std::string moveStr = lineArg; // will be editing this
        
        MoveInformation move;

        move.chessNotation = lineArg;
        // chatGPT suggested rewriting the creation of the specialty flags in this concise format.
        move.isCapture = moveStr.find('x') != std::string::npos;
        move.isPromotion = moveStr.find('=') != std::string::npos;
        move.isCheck = moveStr.find('+') != std::string::npos;
        move.isCheckMate = moveStr.find('#') != std::string::npos;
        move.isKingCastle = (moveStr == "O-O"); 
        move.isQueenCastle = (moveStr == "O-O-O");

        if (move.isKingCastle){
            if (playerColor == white){
                move.fromSquare = e1;
                move.toSquare = g1;
                return move;
            }
            else if (playerColor == black){
                move.fromSquare = e8;
                move.toSquare = g8;
                return move;
            }
        }
        else if (move.isQueenCastle){
            if (playerColor == white){
                move.fromSquare = e1;
                move.toSquare = c1;
                return move;
            }
            else if (playerColor == black){
                move.fromSquare = e8;
                move.toSquare = c8;
                return move;
            }
        }
        move.playerColor = playerColor;
        move.isAmbiguous = false;
        // std::cout << moveStr << std::endl;
        switch (moveStr.at(0)){ //piece type               

            case 'B':
                move.pieceType = bishop;
                moveStr = moveStr.substr(1);
                break;
            case 'N':
                move.pieceType = knight;
                moveStr = moveStr.substr(1);
                break;
            case 'R':
                move.pieceType = rook;
                moveStr = moveStr.substr(1);
                break;
            case 'K':
                move.pieceType = king;
                moveStr = moveStr.substr(1);
                break;
            case 'Q':
                move.pieceType = queen;
                moveStr = moveStr.substr(1);
                break;
            default:
                if ( (moveStr.at(0) >= 'a' && moveStr.at(0) <= 'h') || (moveStr.at(0) >= '1' && moveStr.at(0) <= '8') ){
                    move.pieceType = pawn;
                }
                else{
                    move.pieceType= none;
                }
                break;
        }

        // std::cout << moveStr << std::endl;

        if (move.isCheck || move.isCheckMate)
        {
            moveStr = moveStr.substr(0, moveStr.length() - 1);
        }
        // std::cout << moveStr << std::endl;
        if (move.isPromotion)
        {
            switch (moveStr.at(moveStr.length() - 1) ){
                case 'B':
                    move.promotionPiece = bishop;
                    break;
                case 'N':
                    move.promotionPiece = knight;
                    break;
                case 'R':
                    move.promotionPiece = rook;
                    break;
                case 'Q':
                    move.promotionPiece = queen;
                    break;
    
            }
            moveStr = moveStr.substr(0, moveStr.length() - 2); // remove the last 2 characters because promotion = '=X' where X is piece
        }

        //std::cout << moveStr << std::endl;
        // the last thing in the move string at this point must be the "to" information
        move.toRank = moveStr.at(moveStr.length() - 1);
        move.toFile = moveStr.at(moveStr.length() - 2);
        moveStr = moveStr.substr(0, moveStr.length() - 2);
        // std::cout << moveStr << std::endl;
        if (move.isCapture)
        {
            moveStr = moveStr.substr(0, moveStr.length() - 1);
            // thing before to square, if capture is 'x'
        }
        // std::cout << moveStr << std::endl;
        //std::cout << "boop";
        if (moveStr.length() == 1) 
        {
            move.isAmbiguous = true;
            if (moveStr.at(0) >= 'a' && moveStr.at(0) <= 'h') move.fromFile = moveStr.at(0);
            if (moveStr.at(0) >= '1' && moveStr.at(0) <= '8') move.fromRank = moveStr.at(0);
            
        }
        if (moveStr.length() == 2){                                                     //DOUBLE AMBIGUITY CASE...UNSURE!
            move.fromFile = moveStr.at(0); 
            move.fromRank = moveStr.at(1);
        }
    
        // std::cout << moveStr << std::endl;

        // square = File * 8 + Rank
        // a = 1
        // rank = numbers, File == files
        
        int asciiFile = static_cast<int>(move.toFile) - 97; //SHOULD BE 97 FOR 0 INDEX SYSTEM!!!!!
        move.toSquare = Square( (asciiFile - 1) + 8 * (move.toRank - '0' - 1) ); // casting both rank and file to integers and calculating the square index
        //std::cout << "TESTING: " << asciiFile << ", " << (move.toRank - '0') << ", " << move.toSquare << std::endl;
        return move;
    }
};
    