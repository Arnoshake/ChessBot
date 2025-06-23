#include "Board.cpp"
#include <vector>
enum gameEndingCondition{
    WHITE_WINS, BLACK_WINS, STALEMATE ,ONGOING
};

class MoveInformation{}; //PLACEHOLDER
class Game //uses moveGen, moveParse, Board
{
private:
    Color playerTakingTurnColor;
    Board gameBoard;

    int turnCount; //index for move/board history
    
    gameEndingCondition gameCondition;

    std::vector<MoveInformation> masterMoveList; //holds all moves made (game turn - 1 = index)
    std::vector<Board> boardStates; //holds all board states
public:
    
    //CONSTRUCTOR

    Game();

    //GETTER METHODS
    Board& getGameBoard();
    Board getCopyBoard() const;
    const Board& getReadOnlyBoard() const;

    int getGameTurnCount() const;
    void setGameTurnCount(int newTurnCount);

    Color getColorOfPlayerTakingTurn() const;
    void setColorOfPlayerTakingTurn(Color nextPlayerColor);
    

    //GAMEPLAY LOOP

    void takeGameHalfTurn(Color turn);
    
    
};