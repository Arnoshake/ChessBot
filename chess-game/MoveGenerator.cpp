#include "MoveGenerator.h"
class MoveGenerator{

    private:
    /*
        low level mask generation, used by legalMoveGen functions
        these are called by public methods... private as no user would need direct access
    */
        uint64_t getKnightMask(int square){
            /*
                15  17
            6         10
                x
            10         6
                17  15
            
        up is SL, down is SR
            */
            uint64_t knightMoves = 0ULL;
            uint64_t bitboard = 0ULL;
            
        uint64_t startingSquare = 1ULL << square;
        bitboard |= startingSquare;
        //step 1
        if ( (bitboard >> 17) & (notHFile) ) knightMoves |= (bitboard >> 17);
        if ( (bitboard >> 15) & (notAFile) ) knightMoves |= (bitboard >> 15);
        if ( (bitboard >> 10) & (notGHFile) ) knightMoves |= (bitboard >> 10);
        if ( (bitboard >> 6)  & (notABFile) ) knightMoves |= (bitboard >> 6);
        
        if ( (bitboard << 17) & (notAFile) ) knightMoves |= (bitboard << 17);
        if ( (bitboard << 15) & (notHFile) ) knightMoves |= (bitboard << 15);
        if ( (bitboard << 10) & (notABFile) ) knightMoves |= (bitboard << 10);
        if ( (bitboard << 6)  & (notGHFile) ) knightMoves |= (bitboard << 6);
            
            return knightMoves;
        }
        uint64_t getPawnMask(Board boardOfInterest,int square, Color side){
            int rank = square / 8;
            int file = square % 8;
            //1 = white, 0 = black
            uint64_t possibleMoves = 0ULL;
            uint64_t possibleCaptures = 0ULL;
            uint64_t pawnBitBoard = 1ULL << square;
            uint64_t friendlyPieces;
            uint64_t enemyPieces;
            if (side == white){ // pawns move up.. << & en Passant rank = 4
                friendlyPieces = boardOfInterest.getPieces(white);
                enemyPieces = boardOfInterest.getPieces(black);
                uint64_t singlePush = ( pawnBitBoard << 8) & boardOfInterest.getEmptySquares(); 
                uint64_t doublePush = 0ULL;
                if (rank == 1 && singlePush) doublePush = (singlePush << 8) & boardOfInterest.getEmptySquares();

                
                // if (rank == 2 & singlePush){
                //     doublePush = (singlePush << 8) & emptySquares;
                // }
                possibleMoves = singlePush | doublePush ;
                pawnBitBoard = 1ULL << square;
                possibleCaptures |= ( (pawnBitBoard & notHFile) << 9 ) & (enemyPieces | (1ULL << boardOfInterest.enPassantTargetSquare) );
                possibleCaptures |= ( (pawnBitBoard & notAFile )<< 7) & (enemyPieces | (1ULL << boardOfInterest.enPassantTargetSquare) );
                possibleMoves |= possibleCaptures;
            }
            else{ //identical logic, different variable values and shifting operator
                friendlyPieces = boardOfInterest.getPieces(black);
                enemyPieces = boardOfInterest.getPieces(white);
        
                uint64_t singlePush = ( pawnBitBoard >> 8) & boardOfInterest.getEmptySquares() ; 
                uint64_t doublePush = 0ULL;
                if (rank == 6 && singlePush) doublePush = (singlePush >> 8) & boardOfInterest.getEmptySquares();
                possibleMoves = singlePush | doublePush ;
                                                                                                //DONT FORGET TO REMOVE
                possibleCaptures |= ( (pawnBitBoard & notAFile) >> 9 ) & (enemyPieces | (1ULL << boardOfInterest.enPassantTargetSquare) );
                possibleCaptures |= ( (pawnBitBoard & notHFile )>> 7) & (enemyPieces | (1ULL << boardOfInterest.enPassantTargetSquare) );
                possibleMoves |= possibleCaptures;
            }
            
            possibleMoves &= ~(friendlyPieces);
            return possibleMoves;
        
        
        }
        uint64_t getKingMask(int square){
            //prevents leftside overflow
            uint64_t notAFile =   0xFEFEFEFEFEFEFEFEULL;
            //prevents rightside overflow
            uint64_t notHFile =   0x7F7F7F7F7F7F7F7FULL;
        
        
            int rank = square / 8;
            int file = square % 8;
            uint64_t kingAttack = 0ULL;
            
            if ( rank > 0) kingAttack |= 1ULL << (square - 8); // down
            if ( rank < 7) kingAttack |= 1ULL << (square + 8); // up
            if ( file > 0) kingAttack |= 1ULL << (square - 1); // left
            if ( file < 7) kingAttack |= 1ULL << (square + 1); // right
        
            if ( rank < 7 && file < 7) kingAttack |= 1ULL << (square + 9); //top right
            if ( rank < 7 && file > 0) kingAttack |= 1ULL << (square + 7); // top left
            if ( rank > 0 && file > 0) kingAttack |= 1ULL << (square - 9); // bot left
            if ( rank > 0 && file < 7) kingAttack |= 1ULL << (square - 7); // bot left
            
        
            return kingAttack;
        }
    
        //POSSIBLE CAPTURES/MOVES
    public:
    MoveParser mp;
    /*
        piece and overall legal move generation
    */

        // BITBOARDS!!!!
        uint64_t possiblePawnMovesBitBoard(Board boardOfInterest, Color colorOfInterest){     // pawn moves cant capture...
        
            uint64_t pawnBitBoard = boardOfInterest.getPawns(colorOfInterest);
        
        uint64_t friendlyPieces = boardOfInterest.getPieces(colorOfInterest);
        uint64_t enemyPieces = boardOfInterest.getPieces(!colorOfInterest);
        
            uint64_t possibleMoves = 0ULL;
            uint64_t possibleCaptures = 0ULL;
        
            if (colorOfInterest == white){ // pawns move up.. << & en Passant rank = 4
                uint64_t singlePush = ( pawnBitBoard << 8) & boardOfInterest.getEmptySquares() ; 
                uint64_t doublePush = ( ( ( (0xFF00 & pawnBitBoard) << 8) & boardOfInterest.getEmptySquares() ) << 8 ) & boardOfInterest.getEmptySquares(); //checks for unmoved pawns, single pushes, pushes again
            
                possibleMoves = singlePush | doublePush ;
                
                possibleCaptures |= ( (pawnBitBoard & notHFile) << 9 ) & enemyPieces;
                possibleCaptures |= ( (pawnBitBoard & notAFile )<< 7) & enemyPieces;
                possibleMoves |= possibleCaptures;
                possibleMoves &= ~(friendlyPieces); // may be redundant as possibleCaptures is anded with enemyPieces
            }
            else{ //identical logic, different variable values and shifting operator
                uint64_t singlePush = ( pawnBitBoard >> 8) & boardOfInterest.getEmptySquares() ; 
                uint64_t doublePush = ( ( ( (0x00FF000000000000ULL & pawnBitBoard) >> 8) & boardOfInterest.getEmptySquares() ) >> 8 ) & boardOfInterest.getEmptySquares(); //checks for unmoved pawns, single pushes, pushes again
            
                possibleMoves = singlePush | doublePush ;
                
                possibleCaptures |= ( (pawnBitBoard & notHFile) >> 9 ) & enemyPieces;
                possibleCaptures |= ( (pawnBitBoard & notAFile )>> 7) & enemyPieces;
                possibleMoves |= possibleCaptures;
                possibleMoves &= ~(friendlyPieces);
            }
            return possibleMoves;
        }
        uint64_t possibleKnightMovesBitBoard(Board boardOfInterest, Color colorOfInterest){
            /*
                15  17
            6         10
                x
            10         6
                17  15
            
            */
            //seperate board for knight moves and condition checking. If 1 was used, cascading shifting would happen... bad!

            uint64_t knightBitBoard = boardOfInterest.getKnights(colorOfInterest);
            uint64_t friendlyPieces = boardOfInterest.getPieces(colorOfInterest);
            uint64_t enemyPieces = boardOfInterest.getPieces(!colorOfInterest);
        
            uint64_t bitboard = knightBitBoard;
            uint64_t knightMoves = 0ULL;
        
            //will not change right now because it aint broke... but!
            //I dont need both knightMoves and bitboard. I created these to prevent cascading shifting but by assigning results to an outside/secondary board (knightMoves), cascading is avoided
            knightMoves |= (bitboard & notHFile) << 17;
            knightMoves |= (bitboard & notAFile) << 15;
            knightMoves |= (bitboard & notGHFile) << 10;
            knightMoves |= (bitboard & notABFile) << 6;
            knightMoves |= (bitboard & notAFile) >> 17;
            knightMoves |= (bitboard & notHFile) >> 15; 
            knightMoves |= (bitboard & notABFile) >> 10; 
            knightMoves |= (bitboard & notGHFile) >> 6; 
            knightMoves &= ~friendlyPieces;
            return knightMoves;
        }
        uint64_t possibleBishopMovesBitBoard(Board boardOfInterest, Color colorOfInterest){
        
            uint64_t bishopBitboard = boardOfInterest.getBishops(colorOfInterest);
            uint64_t friendlyPieces = boardOfInterest.getPieces(colorOfInterest);
            uint64_t enemyPieces = boardOfInterest.getPieces(!colorOfInterest);
            

            uint64_t bishopMoves = 0ULL;
            for (int square = 0; square < 64; square++){ // for every bishop, calculate the attacks
                if (isSet(bishopBitboard,square)){
                    bishopMoves |= get_bishop_attacks(square,boardOfInterest.getOccupiedSquares() );
                }
            }
            bishopMoves &= ~friendlyPieces;
            return bishopMoves;
        }
        uint64_t possibleRookMovesBitBoard(Board boardOfInterest, Color colorOfInterest){
        
            uint64_t rookBitBoard = boardOfInterest.getRooks(colorOfInterest);
            uint64_t friendlyPieces = boardOfInterest.getPieces(colorOfInterest);
            uint64_t enemyPieces = boardOfInterest.getPieces(!colorOfInterest);
        
            uint64_t rookMoves = 0ULL;
            for (int square = 0; square < 64; square++){ //only if the bit is set, do you calculate rook moves
                if (isSet(rookBitBoard,square)){
                    rookMoves |= get_rook_attacks(square,boardOfInterest.getOccupiedSquares() );
                }
            }
            rookMoves &= ~ friendlyPieces;
            return rookMoves;
        }
        uint64_t possibleQueenMovesBitBoard(Board boardOfInterest, Color colorOfInterest){
            
            uint64_t queenBitBoard = boardOfInterest.getQueens(colorOfInterest);
            uint64_t friendlyPieces = boardOfInterest.getPieces(colorOfInterest);
            uint64_t enemyPieces = boardOfInterest.getPieces(!colorOfInterest);
        
            uint64_t queenMoves = 0ULL;
            for (int square = 0; square < 64; square++){ 
                if (isSet(queenBitBoard,square)){
                    queenMoves |= get_rook_attacks(square,boardOfInterest.getOccupiedSquares() );
                    queenMoves |= get_bishop_attacks(square,boardOfInterest.getOccupiedSquares() );
                }
            }
            queenMoves &= ~ friendlyPieces;

            return queenMoves;
        }
        uint64_t possibleKingMovesBitBoard(Board boardOfInterest,Color colorOfInterest){

            uint64_t kingBitBoard = boardOfInterest.getKing(colorOfInterest);
            uint64_t friendlyPieces = boardOfInterest.getPieces(colorOfInterest);
            uint64_t enemyPieces = boardOfInterest.getPieces(!colorOfInterest);
        
            uint64_t kingAttack = 0ULL;
            /*
            789
            1K1
            987
            */
            //top
            kingAttack |= (notAFile & kingBitBoard) >> 9;
            kingAttack |= kingBitBoard >> 8;
            kingAttack |= (notAFile & kingBitBoard) >> 7; 
            kingAttack |= (notHFile & kingBitBoard) >> 1;
            //bottom
            kingAttack |= (notHFile & kingBitBoard) << 7;
            kingAttack |= kingBitBoard << 8;
            kingAttack |= (notHFile & kingBitBoard) << 9;
            kingAttack |= (notAFile & kingBitBoard) << 1;
        
            kingAttack &= ~friendlyPieces;
            return (kingAttack);
        }
            
        uint64_t attackedByPawns(Board boardOfInterest,Color colorOfInterest){       
        
            uint64_t pawnBitBoard = boardOfInterest.getPawns(colorOfInterest);
            uint64_t enemyPieces = boardOfInterest.getPieces(!colorOfInterest);
        
            uint64_t possibleCaptures = 0ULL;
        
            if (colorOfInterest == white){ // pawns move up.. << & en Passant rank = 4
                
                possibleCaptures |= ( (pawnBitBoard & notHFile) << 9 ) & enemyPieces;
                possibleCaptures |= ( (pawnBitBoard & notAFile )<< 7) & enemyPieces;
            }
            else{ //identical logic, different variable values and shifting operator
                possibleCaptures |= ( (pawnBitBoard & notAFile) >> 9 ) & enemyPieces;
                possibleCaptures |= ( (pawnBitBoard & notHFile )>> 7) & enemyPieces;
            }
            return possibleCaptures;
        }
        
        //MOVE GENERATION FOR USER & AI
        std::vector<MoveInformation> generatePseudoLegalMovesFromBitboard(Board boardOfInterest, uint64_t bitBoard, Piece pieceType, Color color){ //color may be passed implicitly by game variable
            /*
            
            */
        if (bitBoard == 0ULL) return {};
        boardOfInterest.updateFriendlyEnemy(color);
        std::vector<MoveInformation> moveListForBoard; 
        uint64_t possibleMask = 0ULL;
        while (bitBoard){
            int fromSquare = __builtin_ctzll(bitBoard);  // finds the LSB that is set to one
            switch(pieceType){
                case pawn:
                    possibleMask= getPawnMask(boardOfInterest, fromSquare,color);
                    break;
                case knight:
                    possibleMask = getKnightMask(fromSquare);
                    possibleMask &= ~boardOfInterest.getFriendlyPieces();
                    break;
                case bishop:
                    possibleMask = get_bishop_attacks(fromSquare,boardOfInterest.getOccupiedSquares());
                    possibleMask &= ~boardOfInterest.getFriendlyPieces();
        
                    break;
                case rook:
                    possibleMask = get_rook_attacks(fromSquare,boardOfInterest.getOccupiedSquares());
                    possibleMask &= ~boardOfInterest.getFriendlyPieces();
                  
                    break;
                case queen:
                    possibleMask = get_rook_attacks(fromSquare,boardOfInterest.getOccupiedSquares() ) | get_bishop_attacks(fromSquare,boardOfInterest.getOccupiedSquares());
                    possibleMask &= ~boardOfInterest.getFriendlyPieces();
                    break;
                case king:
                    possibleMask = getKingMask(fromSquare);
                    possibleMask &= ~boardOfInterest.getFriendlyPieces();
                    break;
                case none:  //idealy impossible... if a bit is set, a piece must be present
                    break;
                
            }
        
            while (possibleMask){ //for each possible destination (set bit in possible mask)
                int destination = __builtin_ctzll(possibleMask); //iterate to the first set bit/piece on the board

                MoveInformation legalMove;
                legalMove.playerColor = color;                                                                              //moves are created assumed white, must reassign black
                //generate the attributes of the move
                legalMove.pieceType = Piece(pieceType);
                legalMove.fromSquare = Square(fromSquare);
                legalMove.toSquare = Square(destination);

                legalMove.toFile = 'a' + (destination % 8) ;
                legalMove.toRank = '1' + (destination / 8 );
                //now grab information from board at the destination

                //captures
                
            
                if (destination == boardOfInterest.enPassantTargetSquare && pieceType == pawn && boardOfInterest.getPieceAtSquare(destination) == none){
                    legalMove.isCapture = true;
                    legalMove.isEnpassant = true;
                    legalMove.capturedPiece = pawn;
                }
                else if (boardOfInterest.getPieceAtSquare(destination) != none){
                    legalMove.isCapture = true;
                    legalMove.capturedPiece = boardOfInterest.getPieceAtSquare(destination);
                }
                else{
                    legalMove.isCapture = false;
                }
                
                //castling
                if (color == white) { //castling will never be generated from a bitboard. its outside the normal rules for king movement
                    if ( legalMove.pieceType == king && legalMove.fromSquare == e1 && legalMove.toSquare == g1 ) legalMove.isKingCastle = true;
                    if ( legalMove.pieceType == king && legalMove.fromSquare == e1 && legalMove.toSquare == c1 ) legalMove.isQueenCastle = true;
                }
                if (color == black){
                    
                    if ( legalMove.pieceType == king && legalMove.fromSquare == e8 && legalMove.toSquare == g8 ) legalMove.isKingCastle = true;
                    if ( legalMove.pieceType == king && legalMove.fromSquare == e8 && legalMove.toSquare == c8 ) legalMove.isQueenCastle = true;
                }

                //promotions
                if (color == white) { 
                    if ( pieceType == pawn && (destination >= a8 && destination <= h8) ){
                        legalMove.isPromotion = true;
                    }
                }
                else{  
                    if ( pieceType == pawn && (destination >= a1 && destination <= h1) ){
                        legalMove.isPromotion = true;
                    }
                }
                
                //en passant
                if (pieceType == pawn && legalMove.isCapture && legalMove.toSquare == boardOfInterest.enPassantTargetSquare) legalMove.isEnpassant = true;

                /*
                    CHECK FOR IF IT PUTS YOUR OWN KING IN CHECK!!!
                */
               Board copyBoard = boardOfInterest;
                copyBoard.makeMove(legalMove,0);
               if ( !(copyBoard.isKingInCheck(legalMove.playerColor))){ //if the move doesnt put your own king in check
                    //add move to list
                    legalMove.chessNotation = mp.getMoveString(legalMove);
                    if (legalMove.isPromotion){                               
                        legalMove.promotionPiece = bishop;
                        moveListForBoard.push_back(legalMove);
                        legalMove.promotionPiece = rook;
                        moveListForBoard.push_back(legalMove);
                        legalMove.promotionPiece = queen;
                        moveListForBoard.push_back(legalMove);
                        legalMove.promotionPiece = knight;
                        moveListForBoard.push_back(legalMove);
                    }
                    else{
                    moveListForBoard.push_back(legalMove);
                    }
               }
                        
               
                possibleMask &= (possibleMask - 1); //clear the bit as it is done with
            }

            bitBoard &= (bitBoard - 1); //clears the LSB.. cool trick!
        }
        
        return moveListForBoard;

    }
        std::vector<MoveInformation> generateLegalMoves(Board boardstate, Color color){ 
            
            std::vector<MoveInformation> allLegalMoves;
            std::vector<MoveInformation> movesToAdd;
            movesToAdd = generatePseudoLegalMovesFromBitboard(boardstate, boardstate.getPawns(color),pawn,color);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd = generatePseudoLegalMovesFromBitboard(boardstate, boardstate.getKnights(color),knight,color);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd = generatePseudoLegalMovesFromBitboard(boardstate, boardstate.getBishops(color),bishop,color);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd = generatePseudoLegalMovesFromBitboard(boardstate, boardstate.getRooks(color),rook,color);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd = generatePseudoLegalMovesFromBitboard(boardstate, boardstate.getQueens(color),queen,color);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd = generatePseudoLegalMovesFromBitboard(boardstate, boardstate.getKing(color),king,color);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd.clear();                                                             // is this necessary?
            
            if (boardstate.canKingCastle(color)){
                // std::cout << "\n DEBUG : CASTLING IS ADDED\n";
                movesToAdd.push_back(mp.createMoveFromString(boardstate, color, "O-O"));
            }
            if (boardstate.canQueenCastle(color)){
                movesToAdd.push_back(mp.createMoveFromString(boardstate, color, "O-O-O"));
            }
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end()); // adding possible castles
            // std::cout << "\nDEBUG : MOVES BEFORE IDENTIFY MATES AND AMBIG MOVES\n";
            //printMoveList(allLegalMoves);

            //CLEANS UP PSEUDO GEN
            pruneMovesThatKeepSelfInCheck(allLegalMoves,boardstate);
            findAmbiguousMoves(allLegalMoves); //updates notation of moves that are ambig
            return allLegalMoves;
        }
        std::vector<MoveInformation> generateLegalMovesOnBoard(Board boardstate, Color color){
            std::vector<MoveInformation> allLegalMoves;
            std::vector<MoveInformation> movesToAdd;
            movesToAdd = generatePseudoLegalMovesFromBitboard(boardstate, boardstate.getPawns(color),pawn,color);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd = generatePseudoLegalMovesFromBitboard(boardstate, boardstate.getKnights(color),knight,color);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd = generatePseudoLegalMovesFromBitboard(boardstate, boardstate.getBishops(color),bishop,color);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd = generatePseudoLegalMovesFromBitboard(boardstate, boardstate.getRooks(color),rook,color);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd = generatePseudoLegalMovesFromBitboard(boardstate, boardstate.getQueens(color),queen,color);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd = generatePseudoLegalMovesFromBitboard(boardstate, boardstate.getKing(color),king,color);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd.clear();
            if (boardstate.canKingCastle(color)){
                //std::cout << "\n DEBUG : CASTLING IS ADDED\n";
                movesToAdd.push_back(mp.createMoveFromString(boardstate, color, "O-O"));
            }
            if (boardstate.canQueenCastle(color)){
                movesToAdd.push_back(mp.createMoveFromString(boardstate,color, "O-O-O"));
            }
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end()); // adding possible castles

            pruneMovesThatKeepSelfInCheck(allLegalMoves,boardstate);
            findAmbiguousMoves(allLegalMoves); //updates notation of moves that are ambig
            return allLegalMoves;

        }
    
        // HELPER/CLEANUP OF PSEUDO LEGAL MOVE GEN
        void findAmbiguousMoves(std::vector<MoveInformation> &legalMovesList){ //edits list of legal moves to show ambiguity
            //std::vector<MoveInformation> copyofLegalMoves = legalMovesList;
            //void because it will edit the list of moves directly
            for (auto& move : legalMovesList) {
                std::vector<MoveInformation> conflictingMovesList;
                for (const auto& other : legalMovesList) {
        
                    if (&move == &other ) continue; //prevents self-comparison, if something is already marked ambiguous, skip it!
                    if (move.pieceType == other.pieceType && move.playerColor == other.playerColor && move.toSquare == other.toSquare && move.fromSquare != other.fromSquare) {
                        conflictingMovesList.push_back(other);
                    }
                }
                if (!conflictingMovesList.empty()){ //if conflicting moves exist (ambiguity!)
                        move.isAmbiguous = true;
                        bool fileSufficient = true, rankSufficient = true;
                        
                        for (auto& conflictingMove : conflictingMovesList){
                            if ( (conflictingMove.fromSquare%8) == (move.fromSquare%8) ) fileSufficient = false;
                            
                            if ( (conflictingMove.fromSquare/8) == (move.fromSquare/8) ) rankSufficient = false;
                        
                        }
                        

                        //only updating one because this process will repeat when the "other" move ibecomes the "move" move
                        if (fileSufficient) {
                            move.fromFile = 'a' + (move.fromSquare % 8); 
                            move.uniqueFile = true;
                            
                        } 
                        if (rankSufficient) {
                            move.fromRank = '1' + (move.fromSquare / 8); 
                            move.uniqueRank = true;
                        } 
                        if (!rankSufficient && !fileSufficient) {
                            move.fromRank = '1' + (move.fromSquare / 8); 
                            move.fromFile = 'a' + (move.fromSquare % 8); 
                            move.uniqueRank = true;
                            move.uniqueFile = true;
                        } 
                    }
            
            }
            
        }
        void pruneMovesThatKeepSelfInCheck(std::vector<MoveInformation> &legalMovesList, Board board){
            int index = 0;
            for (MoveInformation move: legalMovesList){
                Board copy = board;
                copy.makeMove(move,0);
                if (copy.isKingInCheck(move.playerColor)){
                    legalMovesList.erase(legalMovesList.begin() + index); //removes any moves where your own king is still in check
                    continue;
                }
                else{
                index++;
                }
            }

        }
        


};
