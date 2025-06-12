#include "MoveInformation.h"
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

       
        


        MoveInformation() { //chatGPT fixed                                 //
            isCheck = false;
            isCheckMate = false;
            isCapture = false;
            isAmbiguous = false;
            isPromotion = false;
            isEnpassant = false;
            isKingCastle = false;
            isQueenCastle = false;
            toRank = 0;
            toFile = 0;
            fromFile = '\0';
            uniqueFile = false;
            fromRank = '\0';
            uniqueRank = false;
            pieceType = none;
            promotionPiece = none;
            capturedPiece = none;
            toSquare = NO_SQUARE;
            fromSquare = NO_SQUARE;
            turn = 0;
            playerColor = white; 
        }

        MoveInformation(Piece pieceType1, bool isCapture1, Piece capturedPiece1, bool isAmbiguous1, bool isEnpassant1, bool isPromotion1, Piece promotionPiece1){}
//         MoveInformation(Color color, Piece piece, int from, int to,Piece capturedPieceParam,bool capture = true) {
//     playerColor = color;
//     pieceType = piece;
//     fromSquare = from;
//     toSquare = to;
//     isCapture = capture;
//     isCheck = false;
//     isCheckMate = false;
//     isPromotion = false;
//     isEnpassant = false;
//     isKingCastle = false;
//     isQueenCastle = false;
//     promotionPiece = capturedPieceParam;
//     capturedPiece = none;
// }


        char getPieceLetter(Piece pieceType) { //CHATGPT'D
            switch (pieceType) {
                case pawn: return 'P';
                case knight: return 'N';
                case bishop: return 'B';
                case rook: return 'R';
                case queen: return 'Q';
                case king: return 'K';
                default: return '~';
            }
        }
        std::string getSquareName(int square) {         //CHATGPT'D
            // This is a simple version assuming the square is represented by an integer
            // You can enhance this if you need to map it more clearly
            char file = 'a' + (square % 8); // 'a' to 'h'
            char rank = '1' + (square / 8); // '1' to '8'
            return std::string(1, file) + std::string(1, rank);
        }   

        void printMoveInfo() {
    std::cout << "=== Move Information ===\n";
    std::cout << "Turn: " << turn << " | Player: " << (playerColor == white ? "White" : "Black") << "\n";
    std::cout << "Piece Moved: " << getPieceLetter(pieceType) << "\n";
    std::cout << "From: " << getSquareName(fromSquare) << " (" << fromSquare << ")\n";
    std::cout << "To: " << getSquareName(toSquare) << " (" << toSquare << ")\n";
    std::cout <<"Flags ( [capture][promotion][enPassant][check][checkmate][ambig]):" << isCapture << isPromotion << isEnpassant << isCheck << isCheckMate <<isAmbiguous << "\n";
    if (isCapture) {
        std::cout << "Captured: " << getPieceLetter(capturedPiece) << "\n";
    }

    if (isPromotion) {
        std::cout << "Promotion to: " << getPieceLetter(promotionPiece) << "\n";
    }

    if (isEnpassant) std::cout << "Special: En Passant\n";
    if (isKingCastle) std::cout << "Special: Kingside Castle\n";
    if (isQueenCastle) std::cout << "Special: Queenside Castle\n";
    if (isCheck) std::cout << "Check: Yes\n";
    if (isCheckMate) std::cout << "Checkmate: Yes\n";
    if (isAmbiguous) std::cout << "Note: Move was ambiguous | " << fromFile <<  ", " <<fromRank<<".\n";

    std::cout << "Algebraic Notation: " << chessNotation << "\n";
    std::cout << "========================\n";
}
    };

