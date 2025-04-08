/*
    First attempt at creating masks for rooks (linear) and bishops (diagonal)

    The issue with this code is that its poorly named as well as not very readable. 
    With hindsight, I think that a while loop is a bad idea as there is a defined range of iteration and for loops are more readable
*/
    uint64_t linearPossibleMoves(int fromSquare)
    {
        
        uint64_t linearPossibleMoves = 0x0000000000000000ULL;
        // need to capture | and -- moves, before interuption
        uint64_t spotOfInterest = 1ULL << fromSquare;
        // step 1 is to map all theoretical moves (before blockage)
        while (!(spotOfInterest & borderMask)) //All moves to the right
        { // while this square is not a border
            linearPossibleMoves = linearPossibleMoves | spotOfInterest;
            spotOfInterest = spotOfInterest << 1;
        }

        spotOfInterest = 1ULL << fromSquare;
        while (!(spotOfInterest & borderMask)) //All moves to the left
        { // while this square is not a border
            linearPossibleMoves = linearPossibleMoves | spotOfInterest;
            spotOfInterest = spotOfInterest >> 1;
        }

        spotOfInterest = 1ULL << fromSquare;
        while (!(spotOfInterest & borderMask)) //All moves upward
        { // while this square is not a border
            linearPossibleMoves = linearPossibleMoves | spotOfInterest;
            spotOfInterest = spotOfInterest << 8;
        }

        spotOfInterest = 1ULL << fromSquare;
        while (!(spotOfInterest & borderMask)) //All moves downward
        { // while this square is not a border
            linearPossibleMoves = linearPossibleMoves | spotOfInterest;
            spotOfInterest = spotOfInterest >> 8;
        }

        return linearPossibleMoves;
    };

    uint64_t diagonalPossibleMoves(int fromSquare)
    {
        uint64_t diagonalPossibleMoves = 0x0000000000000000ULL;
        // need to capture | and -- moves, before interuption
        uint64_t spotOfInterest = 1ULL << fromSquare;
        // step 1 is to map all theoretical moves (before blockage)
        while (!(spotOfInterest & borderMask)) //All moves NE
        { // while this square is not a border
            diagonalPossibleMoves = diagonalPossibleMoves | spotOfInterest;
            spotOfInterest = spotOfInterest << 9;
        }

        spotOfInterest = 1ULL << fromSquare;
        while (!(spotOfInterest & borderMask)) //All moves SE
        { // while this square is not a border
            diagonalPossibleMoves = diagonalPossibleMoves | spotOfInterest;
            spotOfInterest = spotOfInterest >> 7;
        }

        spotOfInterest = 1ULL << fromSquare;
        while (!(spotOfInterest & borderMask)) //All moves SW
        { // while this square is not a border
            diagonalPossibleMoves = diagonalPossibleMoves | spotOfInterest;
            spotOfInterest = spotOfInterest >> 9;
        }

        spotOfInterest = 1ULL << fromSquare;
        while (!(spotOfInterest & borderMask)) //All moves NW
        { // while this square is not a border
            diagonalPossibleMoves = diagonalPossibleMoves | spotOfInterest;
            spotOfInterest = spotOfInterest << 7;
        }

        return diagonalPossibleMoves;
    };
