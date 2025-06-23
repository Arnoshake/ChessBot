#include "Board.cpp"
#include "MoveInformation.cpp"
class MoveGenerator{ //uses move parser (uses moveInformation)

  
        uint64_t getKnightMask(int square);
        uint64_t getPawnMask(const Board& boardOfInterest,int square, Color side);
        uint64_t getKingMask(int square);
        
   
        // BITBOARDS!!!!
        uint64_t possiblePawnMovesBitBoard(const Board& boardOfInterest, Color colorOfInterest); // Board passed by reference (avoids unneeded copy) but ensures no changes
        uint64_t possibleKnightMovesBitBoard(const Board& boardOfInterest, Color colorOfInterest);
        uint64_t possibleBishopMovesBitBoard(const Board& boardOfInterest, Color colorOfInterest);
        uint64_t possibleRookMovesBitBoard(const Board& boardOfInterest, Color colorOfInterest);
        uint64_t possibleQueenMovesBitBoard(const Board& boardOfInterest, Color colorOfInterest);
        uint64_t possibleKingMovesBitBoard(const Board& boardOfInterest,Color colorOfInterest);
        uint64_t attackedByPawns(const Board& boardOfInterest,Color colorOfInterest);
        //MOVE GENERATION FOR USER & AI
        std::vector<MoveInformation> generatePseudoLegalMovesFromBitboard(const Board& boardOfInterest, const uint64_t& bitBoard, Piece pieceType, Color color);
        std::vector<MoveInformation> generateLegalMoves(const Board& boardstate, Color color);
        std::vector<MoveInformation> generateLegalMovesOnBoard(const Board& boardstate, Color color);
        // HELPER/CLEANUP OF PSEUDO LEGAL MOVE GEN
        void findAmbiguousMoves(std::vector<MoveInformation> &legalMovesList);
        void pruneMovesThatKeepSelfInCheck(std::vector<MoveInformation> &legalMovesList, const Board& board);
};
