#include "Board.cpp"
#include "util.h"
int main(){
    Board testBoard = Board();
    testBoard.addPiece(white,pawn,a1);
    testBoard.displayBoardPolished();
    testBoard.movePiece(a1,a2);
    testBoard.displayBoardPolished();

    return 1;
}