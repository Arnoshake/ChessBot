#include "Board.h"
#include "MoveParser.h"
class MoveGenerator{

    private:
    /*
        low level mask generation, used by legalMoveGen functions
        these are called by public methods... private as no user would need direct access
    */
        uint64_t getKnightMask(int square);
        uint64_t getPawnMask(Board boardOfInterest,int square, Color side);
        uint64_t getKingMask(int square);
        //POSSIBLE CAPTURES/MOVES
    public:
    MoveParser mp;
    /*
        piece and overall legal move generation
    */

        // BITBOARDS!!!!
        uint64_t possiblePawnMovesBitBoard(Board boardOfInterest, Color colorOfInterest);
        uint64_t possibleKnightMovesBitBoard(Board boardOfInterest, Color colorOfInterest);
        uint64_t possibleBishopMovesBitBoard(Board boardOfInterest, Color colorOfInterest);
        uint64_t possibleRookMovesBitBoard(Board boardOfInterest, Color colorOfInterest);
        uint64_t possibleQueenMovesBitBoard(Board boardOfInterest, Color colorOfInterest);
        uint64_t possibleKingMovesBitBoard(Board boardOfInterest,Color colorOfInterest);
        uint64_t attackedByPawns(Board boardOfInterest,Color colorOfInterest);
        //MOVE GENERATION FOR USER & AI
        std::vector<MoveInformation> generatePseudoLegalMovesFromBitboard(Board boardOfInterest, uint64_t bitBoard, Piece pieceType, Color color);
        std::vector<MoveInformation> generateLegalMoves(Board boardstate, Color color);
        std::vector<MoveInformation> generateLegalMovesOnBoard(Board boardstate, Color color);
        // HELPER/CLEANUP OF PSEUDO LEGAL MOVE GEN
        void findAmbiguousMoves(std::vector<MoveInformation> &legalMovesList);
        void pruneMovesThatKeepSelfInCheck(std::vector<MoveInformation> &legalMovesList, Board board);
};
