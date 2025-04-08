#ifndef BOARD_H
#define BOARD_H
#include <cstdint>
#include <string>

/*
    First attempt at creating the board class with an outline of some necessary methods
*/
class Board
{
public:
    Board();
    void run(Board game);
    void initializeBoard();
    bool properNotation();
    bool CheckLegalMove(int originIndex, int destIndex);
    void displayBoard();
    
    bool isGameOver();

    struct MoveInformation
    {

        std::string chessNotation;
        bool isCheck,isCheckMate, isCapture, isAmbiguous, isPromotion;
        bool isKingCastle, isQueenCastle;
        char toRank, toFile, fromValue;
        char pieceType, promotionPiece;

        int toSquare;
        int fromSquare;
        int turn;

    };
    MoveInformation parseMove();
    void printMove(Board::MoveInformation move);
    bool isLegal(Board::MoveInformation move);
    void makeMove(Board::MoveInformation move);
    // BELOW IS CHAT GENERATED METHODS... content is written by me, function names and param generated
    bool isWhitePiece(int square){
        return ( (whitePieces) & (1ULL << square)) != 0;
    }
    bool isBlackPiece(int square){
        return ( (blackPieces)& (1ULL << square)) != 0;
    };
    bool isWhitePawn(int square){
        return ( (whitePawns) & (1ULL << square)) != 0;
    };
    bool isBlackPawn(int square){
        return ( (blackPawns) & (1ULL << square)) != 0;
    } ;   // Check if a square contains a black pawn
    bool isWhiteKnight(int square){
        return ( (whiteKnights) & (1ULL << square)) != 0;
    } ; // Check if a square contains a white knight
    bool isBlackKnight(int square){
        return ( (blackKnights) & (1ULL << square)) != 0;
    } ; // Check if a square contains a black knight
    bool isWhiteBishop(int square){
        return ( (whiteBishops) & (1ULL << square)) != 0;
    } ; // Check if a square contains a white bishop
    bool isBlackBishop(int square) {
        return ( (blackBishops) & (1ULL << square)) != 0;
    }; // Check if a square contains a black bishop
    bool isWhiteRook(int square) {
        return ( (whiteRooks) & (1ULL << square)) != 0;
    };   // Check if a square contains a white rook
    bool isBlackRook(int square){
        return ( (blackRooks) & (1ULL << square)) != 0;
    } ;   // Check if a square contains a black rook
    bool isWhiteQueen(int square){
        return ( (whiteQueens) & (1ULL << square)) != 0;
    } ;  // Check if a square contains a white queen
    bool isBlackQueen(int square){
        return ( (blackQueens) & (1ULL << square)) != 0;
    } ;  // Check if a square contains a black queen
    bool isWhiteKing(int square){
        return ( (whiteKings) & (1ULL << square)) != 0;
    } ;   // Check if a square contains a white king
    bool isBlackKing(int square){
        return ( (blackKings) & (1ULL << square)) != 0;
    } ;   
    bool isOccupied(int square){
        return ( (occupiedSquares) & (1ULL << square)) != 0;
        //!= 0 returns it as a boolean instead of numerical value, more accurate
    } ; 
    uint64_t getOccupiedSquares(){
        return occupiedSquares;
    };
    uint64_t getWhitePiece(){
        return whitePieces;
    };
    uint64_t getBlackPiece(){
        return blackPieces;
    };
    uint64_t getWhitePawn(){
        return whitePawns;
    };
    uint64_t getBlackPawn(){
        return blackPawns;
    };
    uint64_t getWhiteKnight(){
        return whiteKnights;
    };
    uint64_t getBlackKnight(){
        return blackKnights;
    };
    uint64_t getWhiteBishop(){
        return whiteBishops;
    };
    uint64_t getBlackBishop(){
        return blackBishops;
    };
    uint64_t getWhiteRook(){
        return whiteRooks;
    };
    uint64_t getBlackRook(){
        return blackRooks;
    };
    uint64_t getWhiteQueen(){
        return whiteQueens;
    };
    uint64_t getBlackQueen(){
        return blackQueens;
    };
    uint64_t getWhiteKing(){
        return whiteKings;
    };
    uint64_t getBlackKing(){
        return blackKings;
    };
    
private:
    
};

#endif // CHESSGAME_H