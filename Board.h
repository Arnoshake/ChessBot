#include <cstdint> //uint64_t
#include <string> // getPieceSymbol()
enum Color{
    white, black, NO_COLOR
};
inline Color operator!(Color c) { 
    if (c == white) return black;
    else if (c == black) return white;
    else return NO_COLOR;
    
}

enum Square {
    a1 = 0,  b1,  c1,  d1,  e1,  f1,  g1,  h1,
    a2 = 8,  b2,  c2,  d2,  e2,  f2,  g2,  h2,
    a3 = 16, b3,  c3,  d3,  e3,  f3,  g3,  h3,
    a4 = 24, b4,  c4,  d4,  e4,  f4,  g4,  h4,
    a5 = 32, b5,  c5,  d5,  e5,  f5,  g5,  h5,
    a6 = 40, b6,  c6,  d6,  e6,  f6,  g6,  h6,
    a7 = 48, b7,  c7,  d7,  e7,  f7,  g7,  h7,
    a8 = 56, b8,  c8,  d8,  e8,  f8,  g8,  h8,
    NO_SQUARE = -1
};
enum Piece {
    pawn, bishop, knight, rook, queen, king, NO_PIECE
};
class Board{
    /*
        BOARD IS A DATA CONTAINER
        It holds locations, access methods, and barebones bit operations for moving pieces

        Board deals entirely bitboards
    */
    private:
    // White pieces
    uint64_t whitePawns;
    uint64_t whiteKnights;
    uint64_t whiteBishops;
    uint64_t whiteRooks;
    uint64_t whiteQueens;
    uint64_t whiteKings;

    // Black pieces
    uint64_t blackPawns;
    uint64_t blackKnights;
    uint64_t blackBishops;
    uint64_t blackRooks;
    uint64_t blackQueens;
    uint64_t blackKings;
    // Combined white and black pieces
    uint64_t whitePieces;
    uint64_t blackPieces;
    // Occupied squares (both white and black pieces)
    uint64_t occupiedSquares;
    uint64_t emptySquares;

    uint64_t friendlyPieces;
    uint64_t enemyPieces;

    

    Square enPassantTargetSquare; //variable that game will interact with
    // 1= white, -1 = black
    bool whiteCastlePrivelege; //tracks over all ability to castle
    bool whiteKingCastleRights; //potential to castle king side
    bool whiteQueenCastleRights; // potential ''
    bool whiteCanKingCastle; //if can castle this turn
    bool whiteCanQueenCastle; // ''
    bool whiteHasCastled;
    
    bool blackCastlePrivelege;
    bool blackKingCastleRights;
    bool blackQueenCastleRights;
    bool blackCanKingCastle;
    bool blackCanQueenCastle;
    bool blackHasCastled;


    public:

    

    bool isSet(uint64_t bitboard, Square square) const;
    void set_bit(uint64_t& bitboard, Square square);
    void reset_bit(uint64_t& bitboard, Square square); //different than pop bit, used for rooks/magic
    
    Board();
    void initializeBoard();
    bool DoesSquareHavePiecePieceOfColor(Piece pieceOfInterest, Color colorOfInterest, Square square) const;
    bool isPiece(Color colorOfInterest, Square square) const;
    bool isPawn(Color colorOfInterest,Square square) const;
    bool isKnight(Color colorOfInterest,Square square) const;
    bool isBishop(Color colorOfInterest, Square square) const;
    bool isRook(Color colorOfInterest, Square square) const;
    bool isQueen(Color colorOfInterest, Square square) const;
    bool isKing(Color colorOfInterest, Square square) const;
    bool isOccupied(Square square) const;

    uint64_t getOccupiedSquares() const;
    void updateOccupiedSquares();

    uint64_t getEmptySquares() const;
    void updateEmptySquares();

    uint64_t getPiecesOfSide(Color colorOfInterest) const;
    void updatePiecesOfSide(Color colorOfInterest);

    uint64_t getFriendlyPieces() const;
    void setFriendlyPieces(Color colorOfInterest);
    uint64_t getEnemyPieces() const;
    void setEnemyPieces(Color colorOfInterest);
    void updateFriendlyEnemy(Color color);

    uint64_t getPawns(Color colorOfInterest) const;
    void setPawns(uint64_t newBitBoard, Color colorOfInterest);

    uint64_t getKnights(Color colorOfInterest) const;
    void setKnights(uint64_t newBitBoard, Color colorOfInterest);

    uint64_t getBishops(Color colorOfInterest) const;
    void setBishops(uint64_t newBitBoard, Color colorOfInterest);

    uint64_t getRooks(Color colorOfInterest) const;
    void setRooks(uint64_t newBitBoard, Color colorOfInterest);

    uint64_t getQueens(Color colorOfInterest) const;
    void setQueens(uint64_t newBitBoard, Color colorOfInterest);

    uint64_t getKing(Color colorOfInterest) const;
    void setKing(uint64_t newBitBoard, Color colorOfInterest);

    Piece getPieceAtSquare(Square square) const;
    Color getColorAtSquare(Square square) const;

    Square getEnPassantTargetSquare() const;
    bool operator==(const Board& other) const;
    

    void addPiece(Color color,Piece piece, Square square);
    void removePiece(Color color, Piece piece, Square square);
    void movePiece(Square from, Square to);

    std::string getPieceSymbol(Piece piece, Color color) const;
    void displayBoardPolished() const;
};