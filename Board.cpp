#include "Board.h"

#include <iostream>
#include <cstdint> //uint64_t
#include <cstdlib> // for exit()
//BIT OPERATIONS
void Board::set_bit(uint64_t& bitboard, int square){
    bitboard |= (1ULL << square);
}
void Board::reset_bit(uint64_t& bitboard, int square){
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

// Getter and Setter methods
uint64_t Board::getOccupiedSquares() const
{
    return occupiedSquares;
}
uint64_t Board::updateOccupiedSquares()
{
    occupiedSquares = getPiecesOfSide(white) | getPiecesOfSide(black);
}

uint64_t Board::getEmptySquares() const
{
    return emptySquares;
}
uint64_t Board::updateEmptySquares()
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
uint64_t Board::updatePiecesOfSide(Color colorOfInterest) //piecesOfSide is a sum of the other, tracked boards
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
uint64_t Board::setPawns(uint64_t newBitBoard, Color colorOfInterest)
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
uint64_t Board::setKnights(uint64_t newBitBoard, Color colorOfInterest)
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
uint64_t Board::setBishops(uint64_t newBitBoard, Color colorOfInterest)
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
uint64_t Board::setRooks(uint64_t newBitBoard, Color colorOfInterest)
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
uint64_t Board::setQueens(uint64_t newBitBoard, Color colorOfInterest)
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
uint64_t Board::setKing(uint64_t newBitBoard, Color colorOfInterest)
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
            switch(piece){
                case pawn:
                    //getPawns is constant and set_bit is by reference so buffer variable required
                    uint64_t pawnBitBoard = getPawns(color);
                    set_bit(pawnBitBoard,square);
                    setPawns(pawnBitBoard,color);
                    break;
                case bishop:
                    uint64_t bishopBitBoard = getBishops(color);
                    set_bit(bishopBitBoard,square);
                    setBishops(bishopBitBoard,color);
                    break;
                case knight:
                    uint64_t knightBitBoard = getKnights(color);
                    set_bit(knightBitBoard,square);
                    setKnights(knightBitBoard,color);
                    break;
                case rook:
                    uint64_t rookBitBoard = getRooks(color);
                    set_bit(rookBitBoard,square);
                    setRooks(rookBitBoard,color);
                    break;
                case queen:
                    uint64_t queenBitBoard = getQueens(color);
                    set_bit(queenBitBoard,square);
                    setQueens(queenBitBoard,color);
                    break;
                case king:
                    uint64_t kingBitBoard = getKing(color);
                    set_bit(kingBitBoard,square);
                    setKing(kingBitBoard,color);
                    break;
                case NO_PIECE:
                default:
                    break;
            }
            updatePiecesOfSide(color);
            updateOccupiedSquares();
            updateEmptySquares();
    }
    