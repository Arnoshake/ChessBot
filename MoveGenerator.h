#include "MoveInformation.cpp"
#include "Board.cpp"
class MoveGenerator{ //uses move parser (uses moveInformation)

  
        static uint64_t getKnightMask(int square);
        static uint64_t getPawnMask(const Board& boardOfInterest,int square, Color side);
        static uint64_t getKingMask(int square);
        
   
        // BITBOARDS!!!!
        static uint64_t possiblePawnMovesBitBoard(const Board& boardOfInterest, Color colorOfInterest); // Board passed by reference (avoids unneeded copy) but ensures no changes
        static uint64_t possibleKnightMovesBitBoard(const Board& boardOfInterest, Color colorOfInterest);
        static uint64_t possibleBishopMovesBitBoard(const Board& boardOfInterest, Color colorOfInterest);
        static uint64_t possibleRookMovesBitBoard(const Board& boardOfInterest, Color colorOfInterest);
        static uint64_t possibleQueenMovesBitBoard(const Board& boardOfInterest, Color colorOfInterest);
        static uint64_t possibleKingMovesBitBoard(const Board& boardOfInterest,Color colorOfInterest);
        static uint64_t attackedByPawns(const Board& boardOfInterest,Color colorOfInterest);
        //MOVE GENERATION FOR USER & AI
        static std::vector<MoveInformation> generatePseudoLegalMovesFromBitboard(const Board& boardOfInterest, const uint64_t& bitBoard, Piece pieceType, Color color);
        static std::vector<MoveInformation> generateLegalMoves(const Board& boardstate, Color color);
        static std::vector<MoveInformation> generateLegalMovesOnBoard(const Board& boardstate, Color color);
        // HELPER/CLEANUP OF PSEUDO LEGAL MOVE GEN
        static void findAmbiguousMoves(std::vector<MoveInformation> &legalMovesList);
        static void pruneMovesThatKeepSelfInCheck(std::vector<MoveInformation> &legalMovesList, const Board& board);
};
