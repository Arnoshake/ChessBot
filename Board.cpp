#include "Board.h"

#include <iostream>
#include <cstdint> //uint64_t
#include <cstdlib> // for exit()

bool Board::isPiece(Color colorOfInterest, Square square) const
{
    if (colorOfInterest == noColor)
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
    if (colorOfInterest == noColor)
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
    if (colorOfInterest == noColor)
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
    if (colorOfInterest == noColor)
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
    if (colorOfInterest == noColor)
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
    if (colorOfInterest == noColor)
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
    if (colorOfInterest == noColor)
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
    if (colorOfInterest == noColor)
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

uint64_t Board::getOccupiedSquares() const
{
    return occupiedSquares;
}
uint64_t Board::setOccupiedSquares(uint64_t occupiedBitBoard){
    occupiedSquares = occupiedBitBoard;
}

uint64_t Board::getEmptySquares() const{
    return emptySquares;
}
uint64_t Board::setEmptySquares(uint64_t emptySquaresBitBoard){
    emptySquares = emptySquaresBitBoard;
}

uint64_t Board::getPiecesOfSide(Color colorOfInterest) const{

}
uint64_t Board::setPiecesOfSide(uint64_t newBitBoard, Color colorOfInterest){
    {
    if (colorOfInterest == noColor)
    {
        std::cerr << "setPiecesOfSide was called with no color" << std::endl;
        exit(1);
    }
    else
    {
        if (colorOfInterest == white){
            whitePieces = newBitBoard;
        }
        else{ //must be black
            blackPieces = newBitBoard;
        }
    }
}
}

uint64_t Board::getPawns(Color colorOfInterest) const{
    if (colorOfInterest == noColor)
    {
        std::cerr << "getPawns was called with no color" << std::endl;
        exit(1);
    }
    else
    {
        if (colorOfInterest == white){
            return whitePawns;
        }
        else{ //must be black
            return blackPawns;
        }
    }
}
uint64_t Board::setPawns(uint64_t newBitBoard, Color colorOfInterest){
    if (colorOfInterest == noColor)
    {
        std::cerr << "setPawns was called with no color" << std::endl;
        exit(1);
    }
    else
    {
        if (colorOfInterest == white){
            whitePawns = newBitBoard;
        }
        else{ //must be black
            blackPawns = newBitBoard;
        }
    }
}

uint64_t Board::getKnights(Color colorOfInterest) const{
    if (colorOfInterest == noColor)
    {
        std::cerr << "getKnights was called with no color" << std::endl;
        exit(1);
    }
    else
    {
        if (colorOfInterest == white){
            return whiteKnights;
        }
        else{ //must be black
            return blackKnights;
        }
    }
}
uint64_t Board::setKnights(uint64_t newBitBoard, Color colorOfInterest){
    if (colorOfInterest == noColor)
    {
        std::cerr << "setKnights was called with no color" << std::endl;
        exit(1);
    }
    else
    {
        if (colorOfInterest == white){
            whiteKnights = newBitBoard;
        }
        else{ //must be black
            blackKnights = newBitBoard;
        }
    }
}

uint64_t Board::getBishops(Color colorOfInterest) const{
    if (colorOfInterest == noColor)
    {
        std::cerr << "getBishops was called with no color" << std::endl;
        exit(1);
    }
    else
    {
        if (colorOfInterest == white){
            return whiteBishops;
        }
        else{ //must be black
            return blackBishops;
        }
    }
}
uint64_t Board::setBishops(uint64_t newBitBoard, Color colorOfInterest){
    if (colorOfInterest == noColor)
    {
        std::cerr << "setBishops was called with no color" << std::endl;
        exit(1);
    }
    else
    {
        if (colorOfInterest == white){
            whiteBishops = newBitBoard;
        }
        else{ //must be black
            blackBishops = newBitBoard;
        }
    }
}
uint64_t Board::getRooks(Color colorOfInterest) const{
    if (colorOfInterest == noColor)
    {
        std::cerr << "getRooks was called with no color" << std::endl;
        exit(1);
    }
    else
    {
        if (colorOfInterest == white){
            return whiteRooks;
        }
        else{ //must be black
            return blackRooks;
        }
    }
}
uint64_t Board::setRooks(uint64_t newBitBoard, Color colorOfInterest){
    if (colorOfInterest == noColor)
    {
        std::cerr << "setRooks was called with no color" << std::endl;
        exit(1);
    }
    else
    {
        if (colorOfInterest == white){
            whiteRooks = newBitBoard;
        }
        else{ //must be black
            blackRooks = newBitBoard;
        }
    }
}

uint64_t Board::getQueens(Color colorOfInterest) const{
    if (colorOfInterest == noColor)
    {
        std::cerr << "getQueens was called with no color" << std::endl;
        exit(1);
    }
    else
    {
        if (colorOfInterest == white){
            return whiteQueens;
        }
        else{ //must be black
            return blackQueens;
        }
    }
}
uint64_t Board::setQueens(uint64_t newBitBoard, Color colorOfInterest){
    if (colorOfInterest == noColor)
    {
        std::cerr << "setQueens was called with no color" << std::endl;
        exit(1);
    }
    else
    {
        if (colorOfInterest == white){
            whiteQueens = newBitBoard;
        }
        else{ //must be black
            blackQueens = newBitBoard;
        }
    }
}
uint64_t Board::getKing(Color colorOfInterest) const{
    if (colorOfInterest == noColor)
    {
        std::cerr << "getKing was called with no color" << std::endl;
        exit(1);
    }
    else
    {
        if (colorOfInterest == white){
            return whiteKings;
        }
        else{ //must be black
            return blackKings;
        }
    }

}
uint64_t Board::setKing(uint64_t newBitBoard, Color colorOfInterest){
    if (colorOfInterest == noColor)
    {
        std::cerr << "setKing was called with no color" << std::endl;
        exit(1);
    }
    else
    {
        if (colorOfInterest == white){
            whiteKings = newBitBoard;
        }
        else{ //must be black
            blackKings = newBitBoard;
        }
    }
}
