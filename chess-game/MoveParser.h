#pragma once
#include "types.h"
#include "MoveInformation.h"
#include "Board.h"
class MoveParser{
    public:
        std::string getMoveString(MoveInformation move);
        MoveInformation createMoveFromString(Board boardWaitingForMove, Color playerColor, std::string chessNotation);
        MoveInformation parseMove(Color playerColor);

};
    