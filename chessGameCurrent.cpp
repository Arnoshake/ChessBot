#include <iostream>
#include<stdio.h>
#include <string>  // included for chess notation
#include <bitset>  //included for decimal to binary conversion
#include <sstream> //included for move print
#include <cstdint>
enum Square {
    a1 = 0,  b1,  c1,  d1,  e1,  f1,  g1,  h1,
    a2 = 8,  b2,  c2,  d2,  e2,  f2,  g2,  h2,
    a3 = 16, b3,  c3,  d3,  e3,  f3,  g3,  h3,
    a4 = 24, b4,  c4,  d4,  e4,  f4,  g4,  h4,
    a5 = 32, b5,  c5,  d5,  e5,  f5,  g5,  h5,
    a6 = 40, b6,  c6,  d6,  e6,  f6,  g6,  h6,
    a7 = 48, b7,  c7,  d7,  e7,  f7,  g7,  h7,
    a8 = 56, b8,  c8,  d8,  e8,  f8,  g8,  h8
};

class Board
{
private:
    uint64_t borderMask = 0xFF818181818181FFULL;
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
    // handleInput
    std::string playerMove;

    // properNotation
    std::string originSq;
    std::string action;
    std::string destinationSq;
    std::string moveAttribute;

    // 1= white, -1 = black
    int gameTurn;

public:
    Board();

    void initializeBoard()
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

        whitePieces = whitePawns | whiteBishops | whiteRooks | whiteKnights | whiteKings | whiteQueens;
        blackPieces = blackPawns | blackBishops | blackRooks | blackKnights | blackKings | blackQueens;
        occupiedSquares = whitePieces | blackPieces;
        emptySquares = ~occupiedSquares;
    };

    uint64_t getRelevantMoveMask(int fromSquare, char pieceType, bool isWhiteTurn ){
        // Completed on 4/7/25
        // returns the relevant map for a given square.
        init_sliders_attacks(1); //for bishops
        init_sliders_attacks(0); //for rooks


        pieceType = tolower(pieceType);
        uint64_t possibleMoves = 0x0000000000000000ULL;

        uint64_t occupiedSquares = getOccupiedSquares();
        uint64_t friendlyOccupiedSquares;
        uint64_t enemyOccupiedSquares;
        if (isWhiteTurn){
            friendlyOccupiedSquares = getWhitePieces();
            enemyOccupiedSquares = getBlackPieces();

        }
        else{
            friendlyOccupiedSquares = getBlackPieces();
            enemyOccupiedSquares = getWhitePieces();
        }

        switch(pieceType){
            //NEED TO DO PAWN AND KING BECAUSE THEIR MOVES DEPEND ON OCCUPIED
            case 'p': //pawn
                possibleMoves = getPawnMask(fromSquare, isWhiteTurn);
                possibleMoves &= ~occupiedSquares; //only keeps possible moves not blocked by existing
                
                uint64_t possibleCaptures = getPawnCaptures(fromSquare, isWhiteTurn);
                possibleCaptures &= enemyOccupiedSquares;

                possibleMoves |= possibleCaptures;
                break;
                
            case 'k': //king
                possibleMoves = getKingMask(fromSquare);
                possibleMoves &= ~friendlyOccupiedSquares;
                break;
            case 'n': //knight
                possibleMoves = getKnightMask(fromSquare);
                possibleMoves &= ~friendlyOccupiedSquares;
                break;
            case 'r': //rook
                possibleMoves = get_rook_attacks(fromSquare, occupiedSquares );
                possibleMoves &= ~friendlyOccupiedSquares; // removes friendly pieces from possible, blocking captures
                break;
            case 'b': //bishop
                possibleMoves = get_bishop_attacks(fromSquare, occupiedSquares );
                possibleMoves &= ~friendlyOccupiedSquares; // removes friendly pieces from possible, blocking captures
                break;
            case 'q': //queen
                possibleMoves = ( getRookMask(fromSquare) | getBishopMask(fromSquare) );  //all possible directional moves
                possibleMoves &= ~friendlyOccupiedSquares;
                break;
            
        }
        return possibleMoves;
    }
    
    uint64_t getPossibleMovesMask(int fromSquare, char pieceType, bool isWhiteTurn){
        init_sliders_attacks(1); //for bishops
        init_sliders_attacks(0); //for rooks

        uint64_t pieceMask = getRelevantMoveMask(fromSquare, pieceType, isWhiteTurn);
        uint64_t friendlyOccupied;
        if (isWhiteTurn) friendlyOccupied = getWhitePieces();
        else{
            friendlyOccupied = getBlackPieces();
        }
        uint64_t possibleMoves = ( pieceMask & (~friendlyOccupied) ); //removes the ability to capture friendly pieces
    }
    //1 billion helper and bool methods
    bool isWhiteTurn(){
        return (gameTurn == 1);
    }
    bool isWhitePiece(int square)
    {
        return ((whitePieces) & (1ULL << square)) != 0;
    }
    bool isBlackPiece(int square)
    {
        return ((blackPieces) & (1ULL << square)) != 0;
    };
    bool isWhitePawn(int square)
    {
        return ((whitePawns) & (1ULL << square)) != 0;
    };
    bool isBlackPawn(int square)
    {
        return ((blackPawns) & (1ULL << square)) != 0;
    }; // Check if a square contains a black pawn
    bool isWhiteKnight(int square)
    {
        return ((whiteKnights) & (1ULL << square)) != 0;
    }; // Check if a square contains a white knight
    bool isBlackKnight(int square)
    {
        return ((blackKnights) & (1ULL << square)) != 0;
    }; // Check if a square contains a black knight
    bool isWhiteBishop(int square)
    {
        return ((whiteBishops) & (1ULL << square)) != 0;
    }; // Check if a square contains a white bishop
    bool isBlackBishop(int square)
    {
        return ((blackBishops) & (1ULL << square)) != 0;
    }; // Check if a square contains a black bishop
    bool isWhiteRook(int square)
    {
        return ((whiteRooks) & (1ULL << square)) != 0;
    }; // Check if a square contains a white rook
    bool isBlackRook(int square)
    {
        return ((blackRooks) & (1ULL << square)) != 0;
    }; // Check if a square contains a black rook
    bool isWhiteQueen(int square)
    {
        return ((whiteQueens) & (1ULL << square)) != 0;
    }; // Check if a square contains a white queen
    bool isBlackQueen(int square)
    {
        return ((blackQueens) & (1ULL << square)) != 0;
    }; // Check if a square contains a black queen
    bool isWhiteKing(int square)
    {
        return ((whiteKings) & (1ULL << square)) != 0;
    }; // Check if a square contains a white king
    bool isBlackKing(int square)
    {
        return ((blackKings) & (1ULL << square)) != 0;
    };
    bool isOccupied(int square)
    {
        return ((occupiedSquares) & (1ULL << square)) != 0;
        //!= 0 returns it as a boolean instead of numerical value, more accurate
    };
    uint64_t getOccupiedSquares()
    {
        return occupiedSquares;
    };
    uint64_t getWhitePieces()
    {
        return whitePieces;
    };
    uint64_t getBlackPieces()
    {
        return blackPieces;
    };
    uint64_t getWhitePawn()
    {
        return whitePawns;
    };
    uint64_t getBlackPawn()
    {
        return blackPawns;
    };
    uint64_t getWhiteKnight()
    {
        return whiteKnights;
    };
    uint64_t getBlackKnight()
    {
        return blackKnights;
    };
    uint64_t getWhiteBishop()
    {
        return whiteBishops;
    };
    uint64_t getBlackBishop()
    {
        return blackBishops;
    };
    uint64_t getWhiteRook()
    {
        return whiteRooks;
    };
    uint64_t getBlackRook()
    {
        return blackRooks;
    };
    uint64_t getWhiteQueen()
    {
        return whiteQueens;
    };
    uint64_t getBlackQueen()
    {
        return blackQueens;
    };
    uint64_t getWhiteKing()
    {
        return whiteKings;
    };
    uint64_t getBlackKing()
    {
        return blackKings;
    };
};
class Game
{
private:
    bool isWhiteTurn;
    bool isGameOver;
    Board board;

    struct MoveInformation
    {

        std::string chessNotation;
        bool isCheck, isCheckMate, isCapture, isAmbiguous, isPromotion;
        bool isKingCastle, isQueenCastle;
        char toRank, toFile, fromValue;
        char pieceType, promotionPiece;
        

        int toSquare;
        int fromSquare;
        int turn;
    };
    MoveInformation move;

public:
    Game();
    void switchTurns() {};
    void displayBoard()
    {
        /*
            Created 1/29/25 from 3:00 - 4:06PM
            chatGPT was used for the use of the 1ULL << square function

            Want to improve with using unicode symbols for pieces
        */
        char blackPawn = 'p';
        char blackRook = 'r';
        char blackKnight = 'n';
        char blackBishop = 'b';
        char blackQueen = 'q';
        char blackKing = 'k';

        char whitePawn = 'P';
        char whiteRook = 'R';
        char whiteKnight = 'K';
        char whiteBishop = 'B';
        char whiteQueen = 'Q';
        char whiteKing = 'K';

        std::cout << "*ABCDEFGH\n";
        // rank = row, file = col
        for (int rank = 7; rank >= 0; rank--)
        { // Starts from rank 8 (top) down to rank 1
            for (int file = 0; file < 8; file++)
            {
                int square = rank * 8 + file;
                if (file == 0)
                    std::cout << (rank + 1);
                /*
                chatGPT'd to get "1ULL << square"
                    creates a mask that puts a 1 at the bit value of the value of square
                    so that it can then compare. They will overlay
                    10000001 & 1000000 = 1
                */
                if (!board.isOccupied(square))
                    std::cout << '.';
                else if (board.isWhitePawn(square))
                    std::cout << whitePawn;
                else if (board.isWhiteBishop(square))
                    std::cout << whiteBishop;
                else if (board.isWhiteRook(square))
                    std::cout << whiteRook;
                else if (board.isWhiteKnight(square))
                    std::cout << whiteKnight;
                else if (board.isWhiteQueen(square))
                    std::cout << whiteQueen;
                else if (board.isWhiteKing(square))
                    std::cout << whiteKing;

                else if (board.isBlackPawn(square))
                    std::cout << blackPawn;
                else if (board.isBlackBishop(square))
                    std::cout << blackBishop;
                else if (board.isBlackRook(square))
                    std::cout << blackRook;
                else if (board.isBlackKnight(square))
                    std::cout << blackKnight;
                else if (board.isBlackQueen(square))
                    std::cout << blackQueen;
                else if (board.isBlackKing(square))
                    std::cout << blackKing;
            }
            std::cout << std::endl;
        }
    }

    MoveInformation parseMove()
    {
        std::string lineArg;
        std::getline(std::cin, lineArg);
        std::string moveStr = lineArg; // will be editing this

        MoveInformation move;

        move.chessNotation = lineArg;
        // chatGPT suggested rewriting the creation of the specialty flags in this concise format.
        move.isCapture = moveStr.find('x') != std::string::npos;
        move.isPromotion = moveStr.find('=') != std::string::npos;
        move.isCheck = moveStr.find('+') != std::string::npos;
        move.isCheckMate = moveStr.find('#') != std::string::npos;
        move.isKingCastle = (moveStr == "O-O");
        move.isQueenCastle = (moveStr == "O-O-O");
        move.isAmbiguous = false;
        // std::cout << moveStr << std::endl;
        if (moveStr.at(0) == 'B' || moveStr.at(0) == 'N' || moveStr.at(0) == 'R' || moveStr.at(0) == 'K' || moveStr.at(0) == 'Q')
        {
            move.pieceType = moveStr.at(0);
            moveStr = moveStr.substr(1);
        }
        else
        {
            move.pieceType = 'P';
        }

        // std::cout << moveStr << std::endl;

        if (move.isCheck || move.isCheckMate)
        {
            moveStr = moveStr.substr(0, moveStr.length() - 1);
        }
        // std::cout << moveStr << std::endl;
        if (move.isPromotion)
        {
            move.promotionPiece = moveStr.at(moveStr.length() - 1);
            moveStr = moveStr.substr(0, moveStr.length() - 2); // remove the last 2 characters because promotion = '=X' where X is piece
        }
        std::cout << moveStr << std::endl;
        // the last thing in the move string at this point must be the to information
        move.toRank = moveStr.at(moveStr.length() - 1);
        move.toFile = moveStr.at(moveStr.length() - 2);
        moveStr = moveStr.substr(0, moveStr.length() - 2);
        // std::cout << moveStr << std::endl;
        if (move.isCapture)
        {
            moveStr = moveStr.substr(0, moveStr.length() - 1);
            // thing before to square, if capture is 'x'
        }
        // std::cout << moveStr << std::endl;
        if (moveStr.length() == 1)
        {
            // if anything is left, it must be ambigiuous data
            move.isAmbiguous = true;
            move.fromValue = moveStr.at(0);
            std::cout << move.fromValue << std::endl;
        }
        // std::cout << moveStr << std::endl;

        // square = File * 8 + Rank
        // a = 1
        // rank = numbers, File == files
        int asciiFile = static_cast<int>(move.toFile) - 97; //SHOULD BE 97 FOR 0 INDEX SYSTEM!!!!!

        move.toSquare = (asciiFile - 1) + 8 * (move.toRank - '0' - 1); // casting both rank and file to integers and calculating the square index
        std::cout << "TESTING: " << asciiFile << ", " << (move.toRank - '0') << ", " << move.toSquare << std::endl;
        return move;
    }

    void printMove(MoveInformation move)
    {
        int asciiRank = static_cast<int>(move.toRank) - 98;
        // above for testing

        std::stringstream ss;
        ss << "MOVE INFORMATION" << std::endl;
        ss << "Chess Notation: " << move.chessNotation << std::endl
           << "Piece: " << move.pieceType << std::endl;
        if (move.isAmbiguous)
            ss << "From: " << move.fromValue << std::endl;
        ss << "To: " << move.toFile << move.toRank << "  (" << move.toSquare << ") | " << std::endl;

        ss << std::endl
           << std::endl
           << "MOVE STATUS" << std::endl;
        ss << "isAmbiguous: " << move.isAmbiguous << std::endl
           << "isCapture: " << move.isCapture << std::endl
           << "isCheck: " << move.isCheck << std::endl
           << "isCheckMate: " << move.isCheckMate << std::endl
           << std::endl;
        ss << "isKingCastle: " << move.isKingCastle << std::endl
           << "isQueenCastle: " << move.isQueenCastle << std::endl;
        ss << "isPromotion: " << move.isPromotion << std::endl;
        if (move.isPromotion)
        {
            ss << "Promotion Piece: " << move.promotionPiece << std::endl;
        }
        std::cout << ss.str();
    }

    bool isGameOver(){};
};
