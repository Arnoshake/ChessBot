This will be replaced upon completion of the project. As of right now, it will exist as an outline of work remaining.

This week (4/8-4/13): 
- possible moves generation for pieces and bitboards
- Legal Moves generation
- takeTurn

Next week (4/14-4/21):
- Research on bot programming methods and requirements (more specifics, have a general outline currently)
- evaluation
- Board storing & best move
- mini-max & AB pruning


4/8/25:
Worked on masking methods

Need to:
- figure out how data will be passed to methods --> down the line... can address this once logic already working at a change in passing is a performance detail, not a logic detail
- how will pieceType be figured out for each call --> same note as before
- pawns
- look up tables for movements (done for rooks, bishops, implied for queens (replace w a real lookup table?) ) -- need for knights, pawns, kings
  
4/9/25
- completed knight move generator
    - upscaled it to apply to bitboard instead of square
- Now have completed move generation for rooks, bishops, queens, Knights, and King

- got working pawn movement for push, double push, and captures
    - Need to work on en passant and promotions
