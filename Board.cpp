#include "Board.h"
#include "util.h"
#include <iostream>
#include <cstdint> //uint64_t
#include <cstdlib> // for exit()

#include "MoveGenerator.h"
//BIT OPERATIONS
bool Board::isSet(uint64_t bitboard, Square square) const{
    int squareInt = static_cast<int>(square);
    if(squareInt < 0 || squareInt >= 64){
        std::cerr << "isSet was passed an invalid square parameter";
        exit(1);
    }
    return (bitboard & (1ULL << square) ) != 0 ;
}
void Board::set_bit(uint64_t& bitboard, Square square){
    bitboard |= (1ULL << square);
}
void Board::reset_bit(uint64_t& bitboard, Square square){
    bitboard &= ~(1ULL << square);
}
//CONSTRUCTOR
Board::Board()
{
    whitePawns = 0ULL;
    blackPawns = 0ULL;

    whiteRooks = 0ULL;
    blackRooks = 0ULL;

    whiteKnights = 0ULL;
    blackKnights = 0ULL;

    whiteBishops = 0ULL;
    blackBishops = 0ULL;

    whiteKings = 0ULL;
    blackKings = 0ULL;

    whiteQueens = 0ULL;
    blackQueens = 0ULL;

    friendlyPieces = 0ULL;
    enemyPieces = 0ULL;

    whitePieces = whitePawns | whiteBishops | whiteRooks | whiteKnights | whiteKings | whiteQueens;
    blackPieces = blackPawns | blackBishops | blackRooks | blackKnights | blackKings | blackQueens;
    occupiedSquares = whitePieces | blackPieces;
    emptySquares = ~occupiedSquares;

    //enPassantTargetSquare = -1;

    whiteCastlePrivelege = true;
    whiteKingCastleRights = true;
    whiteQueenCastleRights = true;
    whiteHasCastled = false;

    blackCastlePrivelege = true;
    blackKingCastleRights = true;
    blackQueenCastleRights = true;
    blackHasCastled = false;

    whiteCanKingCastle = false;
    whiteCanQueenCastle = false;
    blackCanKingCastle = false;
    blackCanQueenCastle = false;
};
void Board::initializeBoard()
    {                                       // Initialize the chess board with pieces | LSB = 1a-1d, MSB = 8a-8d
        whitePawns = 0x000000000000FF00ULL; // every digit = 4 bits (squares on the board). In hexa, F = 1111, so the 2nd row's entirely set to 1 for pawns
        blackPawns = 0x00FF000000000000ULL; // 00000000 11111111 000000000
        
        whiteRooks = 0x0000000000000081ULL;
        blackRooks = 0x8100000000000000ULL; // 10000001 00000000 000000000 1010 = A

        whiteKnights = 0x0000000000000042ULL; // 01000010
        blackKnights = 0x4200000000000000ULL;

        whiteBishops = 0x0000000000000024ULL; // (0000)(0000) (0010)(0100)
        blackBishops = 0x2400000000000000ULL;

        whiteKings = 0x0000000000000010ULL; // 00010000
        blackKings = 0x1000000000000000ULL; // 10000000

        whiteQueens = 0x0000000000000008ULL;
        blackQueens = 0x0800000000000000ULL;

        updatePiecesOfSide(white);
        updatePiecesOfSide(black);
        updateOccupiedSquares();
        updateEmptySquares();
        
        updateFriendlyEnemy(white);
        enPassantTargetSquare = NO_SQUARE;
    
    };
    
// methods to determine what piece is at square
bool Board::isPiece(Color colorOfInterest, Square square) const
{
    if (colorOfInterest == NO_COLOR)
    {
        std::cerr << "IsPiece was called on a square with no piece or color" << std::endl;
        exit(1);
    }
    else
    {
        uint64_t pieceBitBoard = getPiecesOfSide(colorOfInterest);
        return ((pieceBitBoard & (1ULL << square)) != 0); // return true if an overlap exists
    }
}
bool Board::DoesSquareHavePiecePieceOfColor(Piece pieceOfInterest, Color colorOfInterest, Square square) const
{ // checks if square has a piece of a specific color
    if (colorOfInterest == NO_COLOR)
    {
        std::cerr << "isPawn was called on a square with no piece or color" << std::endl;
        exit(1);
    }
    else
    {
        uint64_t bitBoardOfInterest = 0ULL;
        switch (pieceOfInterest)
        {
        case pawn:
            bitBoardOfInterest = getPawns(colorOfInterest);
            break;
        case bishop:
            bitBoardOfInterest = getBishops(colorOfInterest);
            break;
        case knight:
            bitBoardOfInterest = getKnights(colorOfInterest);
            break;
        case rook:
            bitBoardOfInterest = getRooks(colorOfInterest);
            break;
        case queen:
            bitBoardOfInterest = getQueens(colorOfInterest);
            break;
        case king:
            bitBoardOfInterest = getKing(colorOfInterest);
            break;
        default:
            return false;
        }
        return ((bitBoardOfInterest & (1ULL << square)) != 0); // return true if an overlap exists
    }
}
bool Board::isPawn(Color colorOfInterest, Square square) const
{
    if (colorOfInterest == NO_COLOR)
    {
        std::cerr << "isPawn was called on a square with no color" << std::endl;
        exit(1);
    }
    else
    {
        uint64_t pawnBitBoard = getPawns(colorOfInterest);
        return ((pawnBitBoard & (1ULL << square)) != 0); // return true if an overlap exists
    }
}
bool Board::isKnight(Color colorOfInterest, Square square) const
{
    if (colorOfInterest == NO_COLOR)
    {
        std::cerr << "isKnight was called on a square with no color" << std::endl;
        exit(1);
    }
    else
    {
        uint64_t knightBitBoard = getKnights(colorOfInterest);
        return ((knightBitBoard & (1ULL << square)) != 0); // return true if an overlap exists
    }
}
bool Board::isBishop(Color colorOfInterest, Square square) const
{
    if (colorOfInterest == NO_COLOR)
    {
        std::cerr << "isBishop was called on a square with no color" << std::endl;
        exit(1);
    }
    else
    {
        uint64_t bishopBitBoard = getBishops(colorOfInterest);
        return ((bishopBitBoard & (1ULL << square)) != 0); // return true if an overlap exists
    }
}
bool Board::isRook(Color colorOfInterest, Square square) const
{
    if (colorOfInterest == NO_COLOR)
    {
        std::cerr << "isRook was called on a square with no color" << std::endl;
        exit(1);
    }
    else
    {
        uint64_t rookBitBoard = getRooks(colorOfInterest);
        return ((rookBitBoard & (1ULL << square)) != 0); // return true if an overlap exists
    }
}
bool Board::isQueen(Color colorOfInterest, Square square) const
{
    if (colorOfInterest == NO_COLOR)
    {
        std::cerr << "isQueen was called on a square with no color" << std::endl;
        exit(1);
    }
    else
    {
        uint64_t queenBitBoard = getQueens(colorOfInterest);
        return ((queenBitBoard & (1ULL << square)) != 0); // return true if an overlap exists
    }
}
bool Board::isKing(Color colorOfInterest, Square square) const
{
    if (colorOfInterest == NO_COLOR)
    {
        std::cerr << "isKing was called on a square with no color" << std::endl;
        exit(1);
    }
    else
    {
        uint64_t kingBitBoard = getKing(colorOfInterest);
        return ((kingBitBoard & (1ULL << square)) != 0); // return true if an overlap exists
    }
}
bool Board::isOccupied(Square square) const
{
    return ((getOccupiedSquares() & (1ULL << square)) != 0);
}

// Getter, Setter, Update methods
uint64_t Board::getOccupiedSquares() const
{
    return occupiedSquares;
}
void Board::updateOccupiedSquares()
{
    occupiedSquares = getPiecesOfSide(white) | getPiecesOfSide(black);
}

uint64_t Board::getEmptySquares() const
{
    return emptySquares;
}
void Board::updateEmptySquares()
{
    emptySquares = !getOccupiedSquares();
}

uint64_t Board::getPiecesOfSide(Color colorOfInterest) const
{
    if (colorOfInterest == NO_COLOR)
        {
            std::cerr << "getPiecesOfSide was called with no color" << std::endl;
            exit(1);
        }
        else
        {
            if (colorOfInterest == white)
            {
                return whitePieces;
            }
            else
            { // must be black
                return blackPieces;
            }
        }
}
void Board::updatePiecesOfSide(Color colorOfInterest) //piecesOfSide is a sum of the other, tracked boards
{
        if (colorOfInterest == NO_COLOR)
        {
            std::cerr << "setPiecesOfSide was called with no color" << std::endl;
            exit(1);
        }
        else
        {
            if (colorOfInterest == white)
            {
                whitePieces = whitePawns | whiteBishops | whiteKnights | whiteRooks | whiteQueens | whiteKings;
            }
            else
            { // must be black
                blackPieces = blackPawns | blackBishops | blackKnights | blackRooks | blackQueens | blackKings;
            }
        }
}

uint64_t Board::getFriendlyPieces() const{
    return friendlyPieces;
}
void Board::setFriendlyPieces(Color colorOfInterest){
    if (colorOfInterest ==NO_COLOR){
        std::cerr << "setFriendlyPieces was passed NO_COLOR as a parameter";
        exit(1);
    }
    else{
        friendlyPieces = getPiecesOfSide(colorOfInterest);
    }
}
uint64_t Board::getEnemyPieces() const{
    return enemyPieces;
}
void Board::setEnemyPieces(Color colorOfInterest){
    if (colorOfInterest ==NO_COLOR){
        std::cerr << "setEnemyPieces was passed NO_COLOR as a parameter";
        exit(1);
    }
    else{
        enemyPieces = getPiecesOfSide(!colorOfInterest);
    }

}
void Board::updateFriendlyEnemy(Color color){
    setFriendlyPieces(color);
    setEnemyPieces(color);
}

uint64_t Board::getPawns(Color colorOfInterest) const
{
    if (colorOfInterest == NO_COLOR)
    {
        std::cerr << "getPawns was called with no color" << std::endl;
        exit(1);
    }
    else
    {
        if (colorOfInterest == white)
        {
            return whitePawns;
        }
        else
        { // must be black
            return blackPawns;
        }
    }
}
void Board::setPawns(uint64_t newBitBoard, Color colorOfInterest)
{
    if (colorOfInterest == NO_COLOR)
    {
        std::cerr << "setPawns was called with no color" << std::endl;
        exit(1);
    }
    else
    {
        if (colorOfInterest == white)
        {
            whitePawns = newBitBoard;
        }
        else
        { // must be black
            blackPawns = newBitBoard;
        }
    }
}

uint64_t Board::getKnights(Color colorOfInterest) const
{
    if (colorOfInterest == NO_COLOR)
    {
        std::cerr << "getKnights was called with no color" << std::endl;
        exit(1);
    }
    else
    {
        if (colorOfInterest == white)
        {
            return whiteKnights;
        }
        else
        { // must be black
            return blackKnights;
        }
    }
}
void Board::setKnights(uint64_t newBitBoard, Color colorOfInterest)
{
    if (colorOfInterest == NO_COLOR)
    {
        std::cerr << "setKnights was called with no color" << std::endl;
        exit(1);
    }
    else
    {
        if (colorOfInterest == white)
        {
            whiteKnights = newBitBoard;
        }
        else
        { // must be black
            blackKnights = newBitBoard;
        }
    }
}

uint64_t Board::getBishops(Color colorOfInterest) const
{
    if (colorOfInterest == NO_COLOR)
    {
        std::cerr << "getBishops was called with no color" << std::endl;
        exit(1);
    }
    else
    {
        if (colorOfInterest == white)
        {
            return whiteBishops;
        }
        else
        { // must be black
            return blackBishops;
        }
    }
}
void Board::setBishops(uint64_t newBitBoard, Color colorOfInterest)
{
    if (colorOfInterest == NO_COLOR)
    {
        std::cerr << "setBishops was called with no color" << std::endl;
        exit(1);
    }
    else
    {
        if (colorOfInterest == white)
        {
            whiteBishops = newBitBoard;
        }
        else
        { // must be black
            blackBishops = newBitBoard;
        }
    }
}
uint64_t Board::getRooks(Color colorOfInterest) const
{
    if (colorOfInterest == NO_COLOR)
    {
        std::cerr << "getRooks was called with no color" << std::endl;
        exit(1);
    }
    else
    {
        if (colorOfInterest == white)
        {
            return whiteRooks;
        }
        else
        { // must be black
            return blackRooks;
        }
    }
}
void Board::setRooks(uint64_t newBitBoard, Color colorOfInterest)
{
    if (colorOfInterest == NO_COLOR)
    {
        std::cerr << "setRooks was called with no color" << std::endl;
        exit(1);
    }
    else
    {
        if (colorOfInterest == white)
        {
            whiteRooks = newBitBoard;
        }
        else
        { // must be black
            blackRooks = newBitBoard;
        }
    }
}

uint64_t Board::getQueens(Color colorOfInterest) const
{
    if (colorOfInterest == NO_COLOR)
    {
        std::cerr << "getQueens was called with no color" << std::endl;
        exit(1);
    }
    else
    {
        if (colorOfInterest == white)
        {
            return whiteQueens;
        }
        else
        { // must be black
            return blackQueens;
        }
    }
}
void Board::setQueens(uint64_t newBitBoard, Color colorOfInterest)
{
    if (colorOfInterest == NO_COLOR)
    {
        std::cerr << "setQueens was called with no color" << std::endl;
        exit(1);
    }
    else
    {
        if (colorOfInterest == white)
        {
            whiteQueens = newBitBoard;
        }
        else
        { // must be black
            blackQueens = newBitBoard;
        }
    }
}
uint64_t Board::getKing(Color colorOfInterest) const
{
    if (colorOfInterest == NO_COLOR)
    {
        std::cerr << "getKing was called with no color" << std::endl;
        exit(1);
    }
    else
    {
        if (colorOfInterest == white)
        {
            return whiteKings;
        }
        else
        { // must be black
            return blackKings;
        }
    }
}
void Board::setKing(uint64_t newBitBoard, Color colorOfInterest)
{
    if (colorOfInterest == NO_COLOR)
    {
        std::cerr << "setKing was called with no color" << std::endl;
        exit(1);
    }
    else
    {
        if (colorOfInterest == white)
        {
            whiteKings = newBitBoard;
        }
        else
        { // must be black
            blackKings = newBitBoard;
        }
    }
}

Piece Board::getPieceAtSquare(Square square) const{
        if ( (blackPawns | whitePawns ) & (1ULL << square) ) return pawn;
        if ( (blackKnights | whiteKnights ) & (1ULL << square) ) return knight;
        if ( (blackBishops | whiteBishops ) & (1ULL << square) ) return bishop;
        if ( (blackRooks | whiteRooks ) & (1ULL << square) ) return rook;
        if ( (blackQueens | whiteQueens ) & (1ULL << square) ) return queen;
        if ( (blackKings | whiteKings ) & (1ULL << square) ) return king;
        return NO_PIECE;
    }
Color Board::getColorAtSquare(Square square) const {                  
    uint64_t mask = 1ULL << square;
    
    if ((whitePawns | whiteKnights | whiteBishops | whiteRooks | whiteQueens | whiteKings) & mask){
        return white;
    }
        

    else if ((blackPawns | blackKnights | blackBishops | blackRooks | blackQueens | blackKings) & mask){
        return black;
    }
    else{
        return NO_COLOR;
    }
        
    //will not return because this function will only be called on squares with pieces
}

Square Board::getEnPassantTargetSquare() const{
    return enPassantTargetSquare;
}
void Board::setEnPassantTargetSquare(Square newEnPassantSquare){
    enPassantTargetSquare = newEnPassantSquare;
}
bool Board::operator==(const Board &other) const
{
    return occupiedSquares == other.occupiedSquares &&
           whitePawns == other.whitePawns &&
           blackPawns == other.blackPawns &&
           whiteKnights == other.whiteKnights &&
           blackKnights == other.blackKnights &&
           whiteBishops == other.whiteBishops &&
           blackBishops == other.blackBishops &&
           whiteRooks == other.whiteRooks &&
           blackRooks == other.blackRooks &&
           whiteQueens == other.whiteQueens &&
           blackQueens == other.blackQueens &&
           whiteKings == other.whiteKings &&
           blackKings == other.blackKings &&
           emptySquares == other.emptySquares;
}

// Manipulating Board

void Board::addPiece(Color color,Piece piece, Square square){
    if (color == NO_COLOR || piece == NO_PIECE || square == NO_SQUARE){
        std::cerr << "addPiece was called with a fault parameter (NO_COLOR, NO_PIECE, or NO_SQUARE)";
        exit(1);
    }
            switch(piece){
                case pawn:{
                    //getPawns is constant and set_bit is by reference so buffer variable required
                    uint64_t pawnBitBoard = getPawns(color);
                    set_bit(pawnBitBoard,square);
                    setPawns(pawnBitBoard,color);
                    break;
                }
                case bishop:{
                    uint64_t bishopBitBoard = getBishops(color);
                    set_bit(bishopBitBoard,square);
                    setBishops(bishopBitBoard,color);
                    break;
                }
                case knight:{
                    uint64_t knightBitBoard = getKnights(color);
                    set_bit(knightBitBoard,square);
                    setKnights(knightBitBoard,color);
                    break;
                }
                case rook:{
                    uint64_t rookBitBoard = getRooks(color);
                    set_bit(rookBitBoard,square);
                    setRooks(rookBitBoard,color);
                    break;
                }
                case queen:{
                    uint64_t queenBitBoard = getQueens(color);
                    set_bit(queenBitBoard,square);
                    setQueens(queenBitBoard,color);
                    break;
                }
                case king:{
                    uint64_t kingBitBoard = getKing(color);
                    set_bit(kingBitBoard,square);
                    setKing(kingBitBoard,color);
                    break;
                }
                case NO_PIECE:
                default:
                    break;
            }
            updatePiecesOfSide(color);
            updateOccupiedSquares();
            updateEmptySquares();
    }
void Board::removePiece(Color color, Piece piece, Square square){ //only difference to addPiece is that it resetBit instead of setBit
    if (color == NO_COLOR || piece == NO_PIECE || square == NO_SQUARE){
        std::cerr << "removePiece was called with a fault parameter (NO_COLOR, NO_PIECE, or NO_SQUARE)";
        exit(1);
    }
            switch(piece){
                case pawn:{
                    //getPawns is constant and set_bit is by reference so buffer variable required
                    uint64_t pawnBitBoard = getPawns(color);
                    reset_bit(pawnBitBoard,square);
                    setPawns(pawnBitBoard,color);
                }
                    break;
                case bishop:{
                    uint64_t bishopBitBoard = getBishops(color);
                    reset_bit(bishopBitBoard,square);
                    setBishops(bishopBitBoard,color);
                    break;
                }
                case knight:{
                    uint64_t knightBitBoard = getKnights(color);
                    reset_bit(knightBitBoard,square);
                    setKnights(knightBitBoard,color);
                    break;
                }
                case rook:{
                    uint64_t rookBitBoard = getRooks(color);
                    reset_bit(rookBitBoard,square);
                    setRooks(rookBitBoard,color);
                    break;
                }
                case queen:{
                    uint64_t queenBitBoard = getQueens(color);
                    reset_bit(queenBitBoard,square);
                    setQueens(queenBitBoard,color);
                    break;
                }
                case king:{
                    uint64_t kingBitBoard = getKing(color);
                    reset_bit(kingBitBoard,square);
                    setKing(kingBitBoard,color);
                    break;
                }
                case NO_PIECE:
                default:
                    break;
            }
            updatePiecesOfSide(color);
            updateOccupiedSquares();
            updateEmptySquares();
} 
void Board::movePiece(Square from, Square to){
    
        Board &currentBoardState = *this; //currentBoardState acts as an alias to this. Done for readability/simplicitiy

        Piece pieceBeingMoved = currentBoardState.getPieceAtSquare(from);
        if (pieceBeingMoved == NO_PIECE){
            std::cerr << "movePiece was called on a square without a piece";
            exit(1);
        }
        else{
            Color colorOfPieceBeingMoved = currentBoardState.getColorAtSquare(from);
            if (colorOfPieceBeingMoved == NO_COLOR){
            std::cerr << "movePiece was called on a square without a piece";
            exit(1);
            }
            else{
                currentBoardState.removePiece(colorOfPieceBeingMoved , pieceBeingMoved , from);
                currentBoardState.addPiece(colorOfPieceBeingMoved , pieceBeingMoved , to);
            }
            
        }
        
}

// PRINTING/DISPLAYING BOARD INFORMATION

std::string Board::getPieceSymbol(Piece piece, Color color) const {
    switch (piece) {
        case king:   return color == black ? "♔ " : "♚ ";
        case queen:  return color == black ? "♕ " : "♛ ";
        case rook:   return color == black ? "♖ " : "♜ ";
        case bishop: return color == black ? "♗ " : "♝ ";
        case knight: return color == black ? "♘ " : "♞ ";
        case pawn:   return color == black ? "♙ " : "♟ ";
        default:     return "·";  // or " " or "□"
    }
}

void Board::displayBoardPolished() const{
    std::cout << "  \n   +------------------------+" << std::endl;
    for (int rank = 7; rank >= 0; --rank) {
        std::cout << rank + 1 << " | ";
        for (int file = 0; file < 8; ++file) {
            Square square = Square ( rank * 8 + file );
            if (isSet(occupiedSquares,square)){
                Piece piece = getPieceAtSquare(square);
                Color color = getColorAtSquare(square);
                std::string symbol = getPieceSymbol(piece, color);
                std::cout << symbol << " ";
            }
            else{
                std::cout << ".  ";
            }
            
            
        }
        std::cout << "|" << std::endl;
    }
    std::cout << "  +------------------------+" << std::endl;
    std::cout << "    a  b  c  d  e  f  g  h" << std::endl;
}

bool Board::isKingInCheck(Color colorOfKing) const{    //USES MOVEGEN

        uint64_t kingLocation = getKing(colorOfKing);
        Color opponentColor = !colorOfKing;
        uint64_t opponentMoves = MoveGenerator::attackedByPawns(*this, opponentColor) | MoveGenerator::possibleKnightMovesBitBoard(*this, opponentColor) | MoveGenerator::possibleBishopMovesBitBoard(*this, opponentColor) | 
                                    MoveGenerator::possibleRookMovesBitBoard(*this, opponentColor) | MoveGenerator::possibleQueenMovesBitBoard(*this, opponentColor);
       
        return  ( (kingLocation & opponentMoves) != 0); //if there is overlap (>0) return true
}