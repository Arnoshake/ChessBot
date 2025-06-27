#include "MoveGenerator.h"
#include <vector>
class MoveGenerator
{ // uses move parser (uses moveInformation)

public:

    //STUFF FOR MAGIC BOARDS FOR ROOK, BISHOP, QUEEN
    uint64_t rook_masks[64];
    uint64_t rook_attacks[64][4096];
    const uint64_t rook_magics[64] = {
            0x8a80104000800020ULL,
            0x140002000100040ULL,
            0x2801880a0017001ULL,
            0x100081001000420ULL,
            0x200020010080420ULL,
            0x3001c0002010008ULL,
            0x8480008002000100ULL,
            0x2080088004402900ULL,
            0x800098204000ULL,
            0x2024401000200040ULL,
            0x100802000801000ULL,
            0x120800800801000ULL,
            0x208808088000400ULL,
            0x2802200800400ULL,
            0x2200800100020080ULL,
            0x801000060821100ULL,
            0x80044006422000ULL,
            0x100808020004000ULL,
            0x12108a0010204200ULL,
            0x140848010000802ULL,
            0x481828014002800ULL,
            0x8094004002004100ULL,
            0x4010040010010802ULL,
            0x20008806104ULL,
            0x100400080208000ULL,
            0x2040002120081000ULL,
            0x21200680100081ULL,
            0x20100080080080ULL,
            0x2000a00200410ULL,
            0x20080800400ULL,
            0x80088400100102ULL,
            0x80004600042881ULL,
            0x4040008040800020ULL,
            0x440003000200801ULL,
            0x4200011004500ULL,
            0x188020010100100ULL,
            0x14800401802800ULL,
            0x2080040080800200ULL,
            0x124080204001001ULL,
            0x200046502000484ULL,
            0x480400080088020ULL,
            0x1000422010034000ULL,
            0x30200100110040ULL,
            0x100021010009ULL,
            0x2002080100110004ULL,
            0x202008004008002ULL,
            0x20020004010100ULL,
            0x2048440040820001ULL,
            0x101002200408200ULL,
            0x40802000401080ULL,
            0x4008142004410100ULL,
            0x2060820c0120200ULL,
            0x1001004080100ULL,
            0x20c020080040080ULL,
            0x2935610830022400ULL,
            0x44440041009200ULL,
            0x280001040802101ULL,
            0x2100190040002085ULL,
            0x80c0084100102001ULL,
            0x4024081001000421ULL,
            0x20030a0244872ULL,
            0x12001008414402ULL,
            0x2006104900a0804ULL,
            0x1004081002402ULL,
        }; 
    uint64_t bishop_masks[64];
    uint64_t bishop_attacks[64][512];
    const uint64_t bishop_magics[64] = {
            0x40040844404084ULL,
            0x2004208a004208ULL,
            0x10190041080202ULL,
            0x108060845042010ULL,
            0x581104180800210ULL,
            0x2112080446200010ULL,
            0x1080820820060210ULL,
            0x3c0808410220200ULL,
            0x4050404440404ULL,
            0x21001420088ULL,
            0x24d0080801082102ULL,
            0x1020a0a020400ULL,
            0x40308200402ULL,
            0x4011002100800ULL,
            0x401484104104005ULL,
            0x801010402020200ULL,
            0x400210c3880100ULL,
            0x404022024108200ULL,
            0x810018200204102ULL,
            0x4002801a02003ULL,
            0x85040820080400ULL,
            0x810102c808880400ULL,
            0xe900410884800ULL,
            0x8002020480840102ULL,
            0x220200865090201ULL,
            0x2010100a02021202ULL,
            0x152048408022401ULL,
            0x20080002081110ULL,
            0x4001001021004000ULL,
            0x800040400a011002ULL,
            0xe4004081011002ULL,
            0x1c004001012080ULL,
            0x8004200962a00220ULL,
            0x8422100208500202ULL,
            0x2000402200300c08ULL,
            0x8646020080080080ULL,
            0x80020a0200100808ULL,
            0x2010004880111000ULL,
            0x623000a080011400ULL,
            0x42008c0340209202ULL,
            0x209188240001000ULL,
            0x400408a884001800ULL,
            0x110400a6080400ULL,
            0x1840060a44020800ULL,
            0x90080104000041ULL,
            0x201011000808101ULL,
            0x1a2208080504f080ULL,
            0x8012020600211212ULL,
            0x500861011240000ULL,
            0x180806108200800ULL,
            0x4000020e01040044ULL,
            0x300000261044000aULL,
            0x802241102020002ULL,
            0x20906061210001ULL,
            0x5a84841004010310ULL,
            0x4010801011c04ULL,
            0xa010109502200ULL,
            0x4a02012000ULL,
            0x500201010098b028ULL,
            0x8040002811040900ULL,
            0x28000010020204ULL,
            0x6000020202d0240ULL,
            0x8918844842082200ULL,
            0x4010011029020020ULL,
        };
    // rook rellevant occupancy bits
    int rook_rellevant_bits[64] = {
        12, 11, 11, 11, 11, 11, 11, 12,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        12, 11, 11, 11, 11, 11, 11, 12
    };
    // bishop rellevant occupancy bits

    const uint64_t getRookMask(int square){

        int targetRank = square / 8; //rows
        int targetFile = square % 8; //columns

        uint64_t rookAttacks = 0ULL;

        /*
            For loop syntax explanation:
                the rank and file are initialized to the first nonSelf tile for each cardinal direction (pieces cannot move to themselves)
                the loop will iterate until it reaches the 2nd to most outward square. This is important for masking with magic bitboards later
                the condition will update so that it then progresses further in the relevant cardinal direction 
        */
        for (int rank = targetRank + 1; rank <= 6; rank++) rookAttacks |= (1ULL << (8*rank + targetFile) ); //up
        for (int rank = targetRank - 1; rank >= 1 ; rank--) rookAttacks |= (1ULL << (8*rank + targetFile) ); //down
        
        for (int file = targetFile + 1; file <= 6 ; file++) rookAttacks |= (1ULL << (8*targetRank + file) ); //right
        for (int file = targetFile - 1; file >= 1 ; file--) rookAttacks |= (1ULL << (8*targetRank + file) ); //left
        return rookAttacks;





        
            return rookAttacks;
    }
    const uint64_t getBishopMask(int square){
        int targetRank = square / 8; //rows
        int targetFile = square % 8; //columns
        uint64_t bishopAttacks = 0ULL;

        /*
            For loop syntax explanation:
                the rank and file are initialized to the first nonSelf tile for each cardinal direction (pieces cannot move to themselves)
                the loop will iterate until it reaches the 2nd to most outward square. This is important for masking with magic bitboards later
                the condition will update so that it then progresses further in the relevant cardinal direction 
        */
        for (int rank = targetRank + 1, file = targetFile + 1; rank <= 6 && file <= 6; rank++, file++) bishopAttacks |= (1ULL << (8*rank + file) ); // NE
        for (int rank = targetRank + 1, file = targetFile + -1; rank <= 6 && file >= 1; rank++, file--) bishopAttacks |= (1ULL << (8*rank + file) ); // NW
        
        for (int rank = targetRank - 1, file = targetFile + 1; rank >= 1 && file <= 6; rank--, file++) bishopAttacks |= (1ULL << (8*rank + file) ); // SE
        for (int rank = targetRank - 1, file = targetFile - 1; rank >= 1 && file >= 1; rank--, file--) bishopAttacks |= (1ULL << (8*rank + file) ); // SW
        return bishopAttacks;
    }
    int bishop_rellevant_bits[64] = {
        6, 5, 5, 5, 5, 5, 5, 6,
        5, 5, 5, 5, 5, 5, 5, 5,
        5, 5, 7, 7, 7, 7, 5, 5,
        5, 5, 7, 9, 9, 7, 5, 5,
        5, 5, 7, 9, 9, 7, 5, 5,
        5, 5, 7, 7, 7, 7, 5, 5,
        5, 5, 5, 5, 5, 5, 5, 5,
        6, 5, 5, 5, 5, 5, 5, 6
    };
    const uint64_t get_bishop_attacks(int square, uint64_t occupancy) {
        
        // calculate magic index
        occupancy &= bishop_masks[square];
        occupancy *=  bishop_magics[square];
        occupancy >>= 64 - bishop_rellevant_bits[square];
        
        // return rellevant attacks
        return bishop_attacks[square][occupancy];
        
    }
    const uint64_t get_rook_attacks(int square, uint64_t occupancy) {
        
        // calculate magic index
        occupancy &= rook_masks[square];
        occupancy *=  rook_magics[square];
        occupancy >>= 64 - rook_rellevant_bits[square];
        
        // return rellevant attacks
        return rook_attacks[square][occupancy];
    }
    const uint64_t bishop_attacks_on_the_fly(int square, uint64_t block)

    {
        // attack bitboard
        uint64_t attacks = 0ULL;
        
        // init files & ranks
        int f, r;
        
        // init target files & ranks
        int tr = square / 8;
        int tf = square % 8;
        
        // generate attacks
        for (r = tr + 1, f = tf + 1; r <= 7 && f <= 7; r++, f++)
        {
            attacks |= (1ULL << (r * 8 + f));
            if (block & (1ULL << (r * 8 + f))) break;
        }
        
        for (r = tr + 1, f = tf - 1; r <= 7 && f >= 0; r++, f--)
        {
            attacks |= (1ULL << (r * 8 + f));
            if (block & (1ULL << (r * 8 + f))) break;
        }
        
        for (r = tr - 1, f = tf + 1; r >= 0 && f <= 7; r--, f++)
        {
            attacks |= (1ULL << (r * 8 + f));
            if (block & (1ULL << (r * 8 + f))) break;
        }
        
        for (r = tr - 1, f = tf - 1; r >= 0 && f >= 0; r--, f--)
        {
            attacks |= (1ULL << (r * 8 + f));
            if (block & (1ULL << (r * 8 + f))) break;
        }
        
        // return attack map for bishop on a given square
        return attacks;
    }
    const uint64_t rook_attacks_on_the_fly(int square, uint64_t block)
    {
        // attacks bitboard
        uint64_t attacks = 0ULL;
        
        // init files & ranks
        int f, r;
        
        // init target files & ranks
        int tr = square / 8;
        int tf = square % 8;
        
        // generate attacks
        for (r = tr + 1; r <= 7; r++)
        {
            attacks |= (1ULL << (r * 8 + tf));
            if (block & (1ULL << (r * 8 + tf))) break;
        }
        
        for (r = tr - 1; r >= 0; r--)
        {
            attacks |= (1ULL << (r * 8 + tf));
            if (block & (1ULL << (r * 8 + tf))) break;
        }
        
        for (f = tf + 1; f <= 7; f++)
        {
            attacks |= (1ULL << (tr * 8 + f));
            if (block & (1ULL << (tr * 8 + f))) break;
        }
        
        for (f = tf - 1; f >= 0; f--)
        {
            attacks |= (1ULL << (tr * 8 + f));
            if (block & (1ULL << (tr * 8 + f))) break;
        }
        
        // return attack map for bishop on a given square
        return attacks;
    }
    #define get_bit(bitboard, square) (bitboard & (1ULL << square))
    void set_bit(uint64_t& bitboard, int square){
        if(square < 0 || square >= 64) throw std::out_of_range("Invalid square"); 
        (bitboard |= (1ULL << square));
    }
    // void pop_bit(uint64_t& bitboard, int square){
    //     get_bit(bitboard, square) ? (bitboard ^= (1ULL << square)) : 0; //not my code... provided in Rook magic stuff
    // }
    void pop_bit(uint64_t& bitboard, int square){ //AI GEMINI... ternary leads to it acting as a bit toggle
        if(square < 0 || square >= 64) throw std::out_of_range("Invalid square");
        if (get_bit(bitboard, square)) {
            bitboard ^= (1ULL << square);
        }
    }
    int count_bits(uint64_t bitboard) {
        // bit count
        int count = 0;
        
        // pop bits untill bitboard is empty
        while (bitboard)
        {
            // increment count
            count++;
            
            // consecutively reset least significant 1st bit
            bitboard &= bitboard - 1;
        }
        
        // return bit count
        return count;
    }
    int get_ls1b_index(uint64_t bitboard) {
        // make sure bitboard is not empty
        if (bitboard != 0)
            // convert trailing zeros before LS1B to ones and count them
            return count_bits((bitboard & -bitboard) - 1);
        
        // otherwise
        else
            // return illegal index
            return -1;
    }
    uint64_t set_occupancy(int index, int bits_in_mask, uint64_t attack_mask)
    {
        // occupancy map
        uint64_t occupancy = 0ULL;
        
        // loop over the range of bits within attack mask
        for (int count = 0; count < bits_in_mask; count++)
        {
            // get LS1B index of attacks mask
            int square = get_ls1b_index(attack_mask);
            
            // pop LS1B in attack map
            pop_bit(attack_mask, square);
            
            // make sure occupancy is on board
            if (index & (1 << count))                                                                                           //MAY NEED TO BE 1ULL
                // populate occupancy map
                occupancy |= (1ULL << square);
        }
        
        // return occupancy map
        return occupancy;
    }
    void init_sliders_attacks(int is_bishop)
    {
        // loop over 64 board squares
        for (int square = 0; square < 64; square++)
        {
            // init bishop & rook masks
            bishop_masks[square] = getBishopMask(square);
            rook_masks[square] = getRookMask(square);
            
            // init current mask
            uint64_t mask = is_bishop ? getBishopMask(square) : getRookMask(square);
            
            // count attack mask bits
            int bit_count = count_bits(mask);
            
            // occupancy variations count
            int occupancy_variations = 1 << bit_count;                                                          //may need to be 1ULL
            
            // loop over occupancy variations
            for (int count = 0; count < occupancy_variations; count++)
            {
                // bishop
                if (is_bishop)
                {
                    // init occupancies, magic index & attacks
                    uint64_t occupancy = set_occupancy(count, bit_count, mask);
                    uint64_t magic_index = occupancy * bishop_magics[square] >> (64 - bishop_rellevant_bits[square]);
                    bishop_attacks[square][magic_index] = bishop_attacks_on_the_fly(square, occupancy);                
                }
                
                // rook
                else
                {
                    // init occupancies, magic index & attacks
                    uint64_t occupancy = set_occupancy(count, bit_count, mask);
                    uint64_t magic_index = occupancy * rook_magics[square] >> (64 - rook_rellevant_bits[square]);
                    rook_attacks[square][magic_index] = rook_attacks_on_the_fly(square, occupancy);                
                }
            }
        }
    }










    static constexpr uint64_t notAFile = 0xFEFEFEFEFEFEFEFEULL;
    static constexpr uint64_t notABFile = 0xFCFCFCFCFCFCFCFCULL;
    static constexpr uint64_t notHFile = 0x7F7F7F7F7F7F7F7FULL;
    static constexpr uint64_t notGHFile = 0x3F3F3F3F3F3F3F3FULL;

    // for individual squares
    uint64_t getKnightMask(int square)
    {
        /*
            15  17
        6         10
            x
        10         6
            17  15

    up is SL, down is SR
        */
        uint64_t knightMoves = 0ULL;
        uint64_t bitboard = 0ULL;

        uint64_t startingSquare = 1ULL << square;
        bitboard |= startingSquare;
        // step 1
        if ((bitboard >> 17) & (notHFile))
            knightMoves |= (bitboard >> 17);
        if ((bitboard >> 15) & (notAFile))
            knightMoves |= (bitboard >> 15);
        if ((bitboard >> 10) & (notGHFile))
            knightMoves |= (bitboard >> 10);
        if ((bitboard >> 6) & (notABFile))
            knightMoves |= (bitboard >> 6);

        if ((bitboard << 17) & (notAFile))
            knightMoves |= (bitboard << 17);
        if ((bitboard << 15) & (notHFile))
            knightMoves |= (bitboard << 15);
        if ((bitboard << 10) & (notABFile))
            knightMoves |= (bitboard << 10);
        if ((bitboard << 6) & (notGHFile))
            knightMoves |= (bitboard << 6);

        return knightMoves;
    }
    uint64_t getPawnMask(const Board &boardOfInterest, int square, Color side)
    {
        int rank = square / 8;
        int file = square % 8;
        // 1 = white, 0 = black
        uint64_t possibleMoves = 0ULL;
        uint64_t possibleCaptures = 0ULL;
        uint64_t pawnBitBoard = 1ULL << square;

        // copy is used due to the const nature of the boardOfInterest parameter
        Board copyOfBoard = boardOfInterest;
        copyOfBoard.updateFriendlyEnemy(side);
        uint64_t friendlyPieces = copyOfBoard.getFriendlyPieces();
        uint64_t enemyPieces = copyOfBoard.getEnemyPieces();

        if (side == white)
        { // pawns move up.. << & en Passant rank = 4

            uint64_t singlePush = (pawnBitBoard << 8) & boardOfInterest.getEmptySquares();
            uint64_t doublePush = 0ULL;
            if (rank == 1 && singlePush)
                doublePush = (singlePush << 8) & boardOfInterest.getEmptySquares();

            possibleMoves = singlePush | doublePush;
            pawnBitBoard = 1ULL << square;
            possibleCaptures |= ((pawnBitBoard & notHFile) << 9) & (enemyPieces | (1ULL << boardOfInterest.getEnPassantTargetSquare()));
            possibleCaptures |= ((pawnBitBoard & notAFile) << 7) & (enemyPieces | (1ULL << boardOfInterest.getEnPassantTargetSquare()));
            possibleMoves |= possibleCaptures;
        }
        else
        { // identical logic, different variable values and shifting operator

            uint64_t singlePush = (pawnBitBoard >> 8) & boardOfInterest.getEmptySquares();
            uint64_t doublePush = 0ULL;
            if (rank == 6 && singlePush)
                doublePush = (singlePush >> 8) & boardOfInterest.getEmptySquares();
            possibleMoves = singlePush | doublePush;
            // DONT FORGET TO REMOVE
            possibleCaptures |= ((pawnBitBoard & notAFile) >> 9) & (enemyPieces | (1ULL << boardOfInterest.getEnPassantTargetSquare()));
            possibleCaptures |= ((pawnBitBoard & notHFile) >> 7) & (enemyPieces | (1ULL << boardOfInterest.getEnPassantTargetSquare()));
            possibleMoves |= possibleCaptures;
        }

        possibleMoves &= ~(friendlyPieces);
        return possibleMoves;
    }
    uint64_t getKingMask(int square)
    {
        // prevents leftside overflow
        uint64_t notAFile = 0xFEFEFEFEFEFEFEFEULL;
        // prevents rightside overflow
        uint64_t notHFile = 0x7F7F7F7F7F7F7F7FULL;

        int rank = square / 8;
        int file = square % 8;
        uint64_t kingAttack = 0ULL;

        if (rank > 0)
            kingAttack |= 1ULL << (square - 8); // down
        if (rank < 7)
            kingAttack |= 1ULL << (square + 8); // up
        if (file > 0)
            kingAttack |= 1ULL << (square - 1); // left
        if (file < 7)
            kingAttack |= 1ULL << (square + 1); // right

        if (rank < 7 && file < 7)
            kingAttack |= 1ULL << (square + 9); // top right
        if (rank < 7 && file > 0)
            kingAttack |= 1ULL << (square + 7); // top left
        if (rank > 0 && file > 0)
            kingAttack |= 1ULL << (square - 9); // bot left
        if (rank > 0 && file < 7)
            kingAttack |= 1ULL << (square - 7); // bot left

        return kingAttack;
    }

    // BITBOARDS!!!!
    uint64_t possiblePawnMovesBitBoard(const Board &boardOfInterest, Color colorOfInterest)
    { // pawn moves cant capture...

        uint64_t pawnBitBoard = boardOfInterest.getPawns(colorOfInterest);

        Board copyOfBoard = boardOfInterest;
        copyOfBoard.updateFriendlyEnemy(colorOfInterest);
        uint64_t friendlyPieces = copyOfBoard.getFriendlyPieces();
        uint64_t enemyPieces = copyOfBoard.getEnemyPieces();

        uint64_t possibleMoves = 0ULL;
        uint64_t possibleCaptures = 0ULL;

        if (colorOfInterest == white)
        { // pawns move up.. << & en Passant rank = 4
            uint64_t singlePush = (pawnBitBoard << 8) & boardOfInterest.getEmptySquares();
            uint64_t doublePush = ((((0xFF00 & pawnBitBoard) << 8) & boardOfInterest.getEmptySquares()) << 8) & boardOfInterest.getEmptySquares(); // checks for unmoved pawns, single pushes, pushes again

            possibleMoves = singlePush | doublePush;

            possibleCaptures |= ((pawnBitBoard & notHFile) << 9) & (enemyPieces | (1ULL << boardOfInterest.getEnPassantTargetSquare()));
            possibleCaptures |= ((pawnBitBoard & notAFile) << 7) & (enemyPieces | (1ULL << boardOfInterest.getEnPassantTargetSquare()));
            possibleMoves |= possibleCaptures;
            possibleMoves &= ~(friendlyPieces); // may be redundant as possibleCaptures is anded with enemyPieces
        }
        else
        { // identical logic, different variable values and shifting operator
            uint64_t singlePush = (pawnBitBoard >> 8) & boardOfInterest.getEmptySquares();
            uint64_t doublePush = ((((0x00FF000000000000ULL & pawnBitBoard) >> 8) & boardOfInterest.getEmptySquares()) >> 8) & boardOfInterest.getEmptySquares(); // checks for unmoved pawns, single pushes, pushes again

            possibleMoves = singlePush | doublePush;

            possibleCaptures |= ((pawnBitBoard & notHFile) >> 9) & (enemyPieces | (1ULL << boardOfInterest.getEnPassantTargetSquare()));;
            possibleCaptures |= ((pawnBitBoard & notAFile) >> 7) & (enemyPieces | (1ULL << boardOfInterest.getEnPassantTargetSquare()));;
            possibleMoves |= possibleCaptures;
            possibleMoves &= ~(friendlyPieces);
        }
        return possibleMoves;
    }
    uint64_t possibleKnightMovesBitBoard(const Board &boardOfInterest, Color colorOfInterest)
    {
        /*
            15  17
        6         10
            x
        10         6
            17  15

        */
        // seperate board for knight moves and condition checking. If 1 was used, cascading shifting would happen... bad!

        uint64_t knightBitBoard = boardOfInterest.getKnights(colorOfInterest);

        Board copyOfBoard = boardOfInterest;
        copyOfBoard.updateFriendlyEnemy(colorOfInterest);
        uint64_t friendlyPieces = copyOfBoard.getFriendlyPieces();
        uint64_t enemyPieces = copyOfBoard.getEnemyPieces();

        uint64_t bitboard = knightBitBoard;
        uint64_t knightMoves = 0ULL;

        // will not change right now because it aint broke... but!
        // I dont need both knightMoves and bitboard. I created these to prevent cascading shifting but by assigning results to an outside/secondary board (knightMoves), cascading is avoided
        knightMoves |= (bitboard & notHFile) << 17;
        knightMoves |= (bitboard & notAFile) << 15;
        knightMoves |= (bitboard & notGHFile) << 10;
        knightMoves |= (bitboard & notABFile) << 6;
        knightMoves |= (bitboard & notAFile) >> 17;
        knightMoves |= (bitboard & notHFile) >> 15;
        knightMoves |= (bitboard & notABFile) >> 10;
        knightMoves |= (bitboard & notGHFile) >> 6;
        knightMoves &= ~friendlyPieces;
        return knightMoves;
    }
    uint64_t possibleBishopMovesBitBoard(const Board &boardOfInterest, Color colorOfInterest)
    {

        uint64_t bishopBitboard = boardOfInterest.getBishops(colorOfInterest);

        Board copyOfBoard = boardOfInterest;
        copyOfBoard.updateFriendlyEnemy(colorOfInterest);
        uint64_t friendlyPieces = copyOfBoard.getFriendlyPieces();
        uint64_t enemyPieces = copyOfBoard.getEnemyPieces();

        uint64_t bishopMoves = 0ULL;
        for (int square = 0; square < 64; square++)
        { // for every bishop, calculate the attacks
            if (boardOfInterest.isSet(bishopBitboard, Square(square)))
            {
                bishopMoves |= get_bishop_attacks(square, boardOfInterest.getOccupiedSquares());
            }
        }
        bishopMoves &= ~friendlyPieces;
        return bishopMoves;
    }
    uint64_t possibleRookMovesBitBoard(const Board &boardOfInterest, Color colorOfInterest)
    {

        uint64_t rookBitBoard = boardOfInterest.getRooks(colorOfInterest);

        Board copyOfBoard = boardOfInterest;
        copyOfBoard.updateFriendlyEnemy(colorOfInterest);
        uint64_t friendlyPieces = copyOfBoard.getFriendlyPieces();
        uint64_t enemyPieces = copyOfBoard.getEnemyPieces();

        uint64_t rookMoves = 0ULL;
        for (int square = 0; square < 64; square++)
        { // only if the bit is set, do you calculate rook moves
            if (boardOfInterest.isSet(rookBitBoard, Square(square)))
            {
                rookMoves |= get_rook_attacks(square, boardOfInterest.getOccupiedSquares());
            }
        }
        rookMoves &= ~friendlyPieces;
        return rookMoves;
    }
    uint64_t possibleQueenMovesBitBoard(const Board &boardOfInterest, Color colorOfInterest)
    {

        uint64_t queenBitBoard = boardOfInterest.getQueens(colorOfInterest);

        Board copyOfBoard = boardOfInterest;
        copyOfBoard.updateFriendlyEnemy(colorOfInterest);
        uint64_t friendlyPieces = copyOfBoard.getFriendlyPieces();
        uint64_t enemyPieces = copyOfBoard.getEnemyPieces();

        uint64_t queenMoves = 0ULL;
        for (int square = 0; square < 64; square++)
        {
            if (boardOfInterest.isSet(queenBitBoard, Square(square)))
            {
                queenMoves |= get_rook_attacks(square, boardOfInterest.getOccupiedSquares());
                queenMoves |= get_bishop_attacks(square, boardOfInterest.getOccupiedSquares());
            }
        }
        queenMoves &= ~friendlyPieces;

        return queenMoves;
    }
    uint64_t possibleKingMovesBitBoard(const Board &boardOfInterest, Color colorOfInterest)
    {

        uint64_t kingBitBoard = boardOfInterest.getKing(colorOfInterest);

        Board copyOfBoard = boardOfInterest;
        copyOfBoard.updateFriendlyEnemy(colorOfInterest);
        uint64_t friendlyPieces = copyOfBoard.getFriendlyPieces();
        uint64_t enemyPieces = copyOfBoard.getEnemyPieces();

        uint64_t kingAttack = 0ULL;
        /*
        789
        1K1
        987
        */
        // top
        kingAttack |= (notAFile & kingBitBoard) >> 9;
        kingAttack |= kingBitBoard >> 8;
        kingAttack |= (notAFile & kingBitBoard) >> 7;
        kingAttack |= (notHFile & kingBitBoard) >> 1;
        // bottom
        kingAttack |= (notHFile & kingBitBoard) << 7;
        kingAttack |= kingBitBoard << 8;
        kingAttack |= (notHFile & kingBitBoard) << 9;
        kingAttack |= (notAFile & kingBitBoard) << 1;

        kingAttack &= ~friendlyPieces;
        return (kingAttack);
    }

    uint64_t attackedByPawns(const Board &boardOfInterest, Color colorOfInterest)
    {

        uint64_t pawnBitBoard = boardOfInterest.getPawns(colorOfInterest);

        Board copyOfBoard = boardOfInterest;
        copyOfBoard.updateFriendlyEnemy(colorOfInterest);
        uint64_t enemyPieces = copyOfBoard.getEnemyPieces();

        uint64_t possibleCaptures = 0ULL;

        if (colorOfInterest == white)
        { // pawns move up.. << & en Passant rank = 4

            possibleCaptures |= ((pawnBitBoard & notHFile) << 9) & enemyPieces;
            possibleCaptures |= ((pawnBitBoard & notAFile) << 7) & enemyPieces;
        }
        else
        { // identical logic, different variable values and shifting operator
            possibleCaptures |= ((pawnBitBoard & notAFile) >> 9) & enemyPieces;
            possibleCaptures |= ((pawnBitBoard & notHFile) >> 7) & enemyPieces;
        }
        return possibleCaptures;
    }

    // MOVE GENERATION FOR USER & AI
        std::vector<MoveInformation> generatePseudoLegalMovesFromBitboard(const Board& boardOfInterest, uint64_t bitBoard, Piece pieceType, Color color){ //color may be passed implicitly by game variable
            /*

            */
        
        if (bitBoard == 0ULL) return {};

        Board copyOfBoard = boardOfInterest;
        copyOfBoard.updateFriendlyEnemy(color);
        std::vector<MoveInformation> moveListForBoard;
        
        uint64_t possibleMask = 0ULL;
        while (bitBoard){
            int fromSquare = __builtin_ctzll(bitBoard);  // finds the LSB that is set to one
            switch(pieceType){
                case pawn:
                    possibleMask= getPawnMask(boardOfInterest, fromSquare,color);
                    break;
                case knight:
                    possibleMask = getKnightMask(fromSquare);
                    possibleMask &= ~boardOfInterest.getFriendlyPieces();
                    break;
                case bishop:
                    possibleMask = get_bishop_attacks(fromSquare,boardOfInterest.getOccupiedSquares());
                    possibleMask &= ~boardOfInterest.getFriendlyPieces();

                    break;
                case rook:
                    possibleMask = get_rook_attacks(fromSquare,boardOfInterest.getOccupiedSquares());
                    possibleMask &= ~boardOfInterest.getFriendlyPieces();

                    break;
                case queen:
                    possibleMask = get_rook_attacks(fromSquare,boardOfInterest.getOccupiedSquares() ) | get_bishop_attacks(fromSquare,boardOfInterest.getOccupiedSquares());
                    possibleMask &= ~boardOfInterest.getFriendlyPieces();
                    break;
                case king:
                    possibleMask = getKingMask(fromSquare);
                    possibleMask &= ~boardOfInterest.getFriendlyPieces();
                    break;
                case NO_PIECE:  //idealy impossible... if a bit is set, a piece must be present
                    break;

            }

            while (possibleMask){ //for each possible destination (set bit in possible mask)
                int destination = __builtin_ctzll(possibleMask); //iterate to the first set bit/piece on the board

                MoveInformation legalMove;
                legalMove.playerColor = color;                                                                              //moves are created assumed white, must reassign black
                //generate the attributes of the move
                legalMove.pieceType = Piece(pieceType);
                legalMove.fromSquare = Square(fromSquare);
                legalMove.toSquare = Square(destination);

                legalMove.toFile = 'a' + (destination % 8) ;
                legalMove.toRank = '1' + (destination / 8 );
                //now grab information from board at the destination

                //captures

                if (destination == boardOfInterest.getEnPassantTargetSquare() && pieceType == pawn && boardOfInterest.getPieceAtSquare(Square(destination)) == NO_PIECE){
                    legalMove.isCapture = true;
                    legalMove.isEnpassant = true;
                    legalMove.capturedPiece = pawn;
                }
                else if (boardOfInterest.getPieceAtSquare(Square(destination)) != NO_PIECE){
                    legalMove.isCapture = true;
                    legalMove.capturedPiece = boardOfInterest.getPieceAtSquare(Square(destination));
                }
                else{
                    legalMove.isCapture = false;
                }

                //castling
                if (color == white) { //castling will never be generated from a bitboard. its outside the normal rules for king movement
                    if ( legalMove.pieceType == king && legalMove.fromSquare == e1 && legalMove.toSquare == g1 ) legalMove.isKingCastle = true;
                    if ( legalMove.pieceType == king && legalMove.fromSquare == e1 && legalMove.toSquare == c1 ) legalMove.isQueenCastle = true;
                }
                if (color == black){

                    if ( legalMove.pieceType == king && legalMove.fromSquare == e8 && legalMove.toSquare == g8 ) legalMove.isKingCastle = true;
                    if ( legalMove.pieceType == king && legalMove.fromSquare == e8 && legalMove.toSquare == c8 ) legalMove.isQueenCastle = true;
                }

                //promotions
                if (color == white) {
                    if ( pieceType == pawn && (destination >= a8 && destination <= h8) ){
                        legalMove.isPromotion = true;
                    }
                }
                else{
                    if ( pieceType == pawn && (destination >= a1 && destination <= h1) ){
                        legalMove.isPromotion = true;
                    }
                }

                //en passant
                if (pieceType == pawn && legalMove.isCapture && legalMove.toSquare == boardOfInterest.getEnPassantTargetSquare()) legalMove.isEnpassant = true;

                /*
                    CHECK FOR IF IT PUTS YOUR OWN KING IN CHECK!!!
                */
               Board copyBoard = boardOfInterest;
                copyBoard.makeMove(legalMove,0);
               if ( !(copyBoard.isKingInCheck(legalMove.playerColor))){ //if the move doesnt put your own king in check
                    //add move to list
                    legalMove.chessNotation = MoveParser::getMoveString(legalMove);
                    if (legalMove.isPromotion){
                        legalMove.promotionPiece = bishop;
                        moveListForBoard.push_back(legalMove);
                        legalMove.promotionPiece = rook;
                        moveListForBoard.push_back(legalMove);
                        legalMove.promotionPiece = queen;
                        moveListForBoard.push_back(legalMove);
                        legalMove.promotionPiece = knight;
                        moveListForBoard.push_back(legalMove);
                    }
                    else{
                    moveListForBoard.push_back(legalMove);
                    }
               }

                possibleMask &= (possibleMask - 1); //clear the bit as it is done with
            }

            bitBoard &= (bitBoard - 1); //clears the LSB.. cool trick!
        }

        return moveListForBoard;

    }
        std::vector<MoveInformation> generateLegalMoves(const Board& boardstate, Color color){

            std::vector<MoveInformation> allLegalMoves;
            std::vector<MoveInformation> movesToAdd;
            movesToAdd = generatePseudoLegalMovesFromBitboard(boardstate, boardstate.getPawns(color),pawn,color);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd = generatePseudoLegalMovesFromBitboard(boardstate, boardstate.getKnights(color),knight,color);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd = generatePseudoLegalMovesFromBitboard(boardstate, boardstate.getBishops(color),bishop,color);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd = generatePseudoLegalMovesFromBitboard(boardstate, boardstate.getRooks(color),rook,color);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd = generatePseudoLegalMovesFromBitboard(boardstate, boardstate.getQueens(color),queen,color);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd = generatePseudoLegalMovesFromBitboard(boardstate, boardstate.getKing(color),king,color);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd.clear();                                                             // is this necessary?

            if (boardstate.canKingCastle(color)){
                // std::cout << "\n DEBUG : CASTLING IS ADDED\n";
                movesToAdd.push_back(MoveParser::createMoveFromString(boardstate, color, "O-O"));
            }
            if (boardstate.canQueenCastle(color)){
                movesToAdd.push_back(MoveParser::createMoveFromString(boardstate, color, "O-O-O"));
            }
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end()); // adding possible castles
            // std::cout << "\nDEBUG : MOVES BEFORE IDENTIFY MATES AND AMBIG MOVES\n";
            //printMoveList(allLegalMoves);

            //CLEANS UP PSEUDO GEN
            pruneMovesThatKeepSelfInCheck(allLegalMoves,boardstate);
            findAmbiguousMoves(allLegalMoves); //updates notation of moves that are ambig
            return allLegalMoves;
        }
        std::vector<MoveInformation> generateLegalMovesOnBoard(const Board& boardstate, Color color){
            std::vector<MoveInformation> allLegalMoves;
            std::vector<MoveInformation> movesToAdd;
            movesToAdd = generatePseudoLegalMovesFromBitboard(boardstate, boardstate.getPawns(color),pawn,color);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd = generatePseudoLegalMovesFromBitboard(boardstate, boardstate.getKnights(color),knight,color);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd = generatePseudoLegalMovesFromBitboard(boardstate, boardstate.getBishops(color),bishop,color);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd = generatePseudoLegalMovesFromBitboard(boardstate, boardstate.getRooks(color),rook,color);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd = generatePseudoLegalMovesFromBitboard(boardstate, boardstate.getQueens(color),queen,color);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd = generatePseudoLegalMovesFromBitboard(boardstate, boardstate.getKing(color),king,color);
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end());

            movesToAdd.clear();
            if (boardstate.canKingCastle(color)){
                //std::cout << "\n DEBUG : CASTLING IS ADDED\n";
                movesToAdd.push_back(MoveParser::createMoveFromString(boardstate, color, "O-O"));
            }
            if (boardstate.canQueenCastle(color)){
                movesToAdd.push_back(MoveParser::createMoveFromString(boardstate,color, "O-O-O"));
            }
            allLegalMoves.insert(allLegalMoves.end(), movesToAdd.begin(), movesToAdd.end()); // adding possible castles

            pruneMovesThatKeepSelfInCheck(allLegalMoves,boardstate);
            findAmbiguousMoves(allLegalMoves); //updates notation of moves that are ambig
            return allLegalMoves;

        }

    // HELPER/CLEANUP OF PSEUDO LEGAL MOVE GEN
    void findAmbiguousMoves(std::vector<MoveInformation> &legalMovesList)
    { // edits list of legal moves to show ambiguity
        // std::vector<MoveInformation> copyofLegalMoves = legalMovesList;
        // void because it will edit the list of moves directly
        for (auto &move : legalMovesList)
        {
            std::vector<MoveInformation> conflictingMovesList;
            for (const auto &other : legalMovesList)
            {

                if (&move == &other)
                    continue; // prevents self-comparison, if something is already marked ambiguous, skip it!
                if (move.pieceType == other.pieceType && move.playerColor == other.playerColor && move.toSquare == other.toSquare && move.fromSquare != other.fromSquare)
                {
                    conflictingMovesList.push_back(other);
                }
            }
            if (!conflictingMovesList.empty())
            { // if conflicting moves exist (ambiguity!)
                move.isAmbiguous = true;
                bool fileSufficient = true, rankSufficient = true;

                for (auto &conflictingMove : conflictingMovesList)
                {
                    if ((conflictingMove.fromSquare % 8) == (move.fromSquare % 8))
                        fileSufficient = false;

                    if ((conflictingMove.fromSquare / 8) == (move.fromSquare / 8))
                        rankSufficient = false;
                }

                // only updating one because this process will repeat when the "other" move ibecomes the "move" move
                if (fileSufficient)
                {
                    move.fromFile = 'a' + (move.fromSquare % 8);
                    move.uniqueFile = true;
                }
                if (rankSufficient)
                {
                    move.fromRank = '1' + (move.fromSquare / 8);
                    move.uniqueRank = true;
                }
                if (!rankSufficient && !fileSufficient)
                {
                    move.fromRank = '1' + (move.fromSquare / 8);
                    move.fromFile = 'a' + (move.fromSquare % 8);
                    move.uniqueRank = true;
                    move.uniqueFile = true;
                }
            }
        }
    }
    void pruneMovesThatKeepSelfInCheck(std::vector<MoveInformation> &legalMovesList, const Board &board)
    {
        int index = 0;
        for (MoveInformation move : legalMovesList)
        {
            Board copy = board;
            copy.makeMove(move, 0);
            if (copy.isKingInCheck(move.playerColor))
            {
                legalMovesList.erase(legalMovesList.begin() + index); // removes any moves where your own king is still in check
                continue;
            }
            else
            {
                index++;
            }
        }
    }
};
