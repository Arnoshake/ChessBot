#pragma once
#include "types.h"
#include "UsedLibs.h"
class MoveInformation{
    public:
        std::string chessNotation;
        bool isCheck, isCheckMate, isCapture, isAmbiguous, isPromotion, isEnpassant;
        bool isKingCastle, isQueenCastle;
        char toRank, toFile;
        Piece pieceType, promotionPiece, capturedPiece;             

        Square toSquare;
        Square fromSquare;

        char fromFile, fromRank; //used for ambiguity
        bool uniqueFile, uniqueRank;
        int turn;
        Color playerColor;

       
        


        MoveInformation();
        MoveInformation(Piece pieceType1, bool isCapture1, Piece capturedPiece1, bool isAmbiguous1, bool isEnpassant1, bool isPromotion1, Piece promotionPiece1);

        char getPieceLetter(Piece pieceType); //CHATGPT'D
        std::string getSquareName(int square); //CHATGPT'D
        void printMoveInfo();
    };
