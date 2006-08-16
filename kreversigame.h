#ifndef KREVERSI_GAME_H
#define KREVERSI_GAME_H

#include <QObject>

#include "commondefs.h"

class KReversiBoard;
class Engine;

/**
 *  KReversiGame incapsulates all of the game logic.
 *  It creates KReversiBoard and manages a chips on it.
 *  Whenever the board state changes it emits corresponding signals.
 *  The idea is also to abstract from any graphic representation of the game process
 *
 *  KReversiGame is supposed to be driven by someone from outside.
 *  I.e. it receives commands and emits events when it's internal state changes
 *  due to this commands dispatching.
 *  The main commands are:
 *  makePlayerMove() and makeComputerMove()
 *  Also, after each turn is made a user of this class should check
 *  that isGameOver() returns false,
 *  and that next player can move 
 *  (by calling isAnyPlayerMovePossible() || isAnyComputerMovePossible()).
 *  If, for example isAnyPlayerMovePossible() returns false, than player needs to skip and 
 *  makeComputerMove() should be called.
 *
 *  See KReversiScene for example of working with KReversiGame
 */
class KReversiGame : public QObject
{
    Q_OBJECT
public:
    KReversiGame();
    ~KReversiGame();
    /**
     *  Sets the strength of game engine (1 to 7)
     */
    void setEngineStrength(uint strength);
    /**
     *  @return strength of the game engine
     */
    uint strength() const;
    /**
     *  @return whether the game is already over
     */
    bool isGameOver() const;
    /**
     *  @return whether any player move is at all possible
     */
    bool isAnyPlayerMovePossible() const;
    /**
     *  @return whether any computer move is at all possible
     */
    bool isAnyComputerMovePossible() const;
    /**
     *  @return the board so the callers can examine its current state
     */
    const KReversiBoard& board() const;
    /**
     *  @return a color of the current player
     */
    ChipColor currentPlayer() const { return m_curPlayer; }
    
    // NOTE: this is just a wrapper around KReversiBoard::playerScore
    // Maybe consider merging KReversiBoard into this class?
    // same applies to chipColorAt
    /**
     *  @return score (number of chips) of the player
     */
    int playerScore( ChipColor player ) const;
    // NOTE: this is just a wrapper around KReversiBoard::playerScore
    ChipColor chipColorAt( int row, int col ) const;
    /**
     *  This will make the player move at row, col.
     *  If that is possible of course
     */
    void makePlayerMove(int row, int col);
    /**
     *  This function will make computer decide where he 
     *  wants to put his chip... and he'll put it there!
     */
    void makeComputerMove();
    /**
     *  Undoes the last player-computer move pair
     */
    void undo();
    /**
     *  Returns a hint to current player
     */
    KReversiMove getHint() const;
    /**
     *  Returns true, if it's computer's turn now
     */
    bool isComputersTurn() const { return m_curPlayer == m_computerColor; }
    /**
     *  @return a list of chips which were changed during last move.
     *  First of them will be the move itself, and the rest - chips which
     *  were turned by that move
     */
    QList<KReversiMove> changedChips() const { return m_changedChips; }
signals:
    void boardChanged();
    void moveFinished();
private:
    enum Direction { Up, Down, Right, Left, UpLeft, UpRight, DownLeft, DownRight };
    /**
     * This function will tell you if the move is possible.
     * That's why it was given such a name ;)
     */
    bool isMovePossible( const KReversiMove& move ) const; 
    /**
     *  Searches for "chunk" in direction dir for move.
     *  As my English-skills are somewhat limited, let me introduce 
     *  new terminology ;).
     *  I'll define a "chunk" of chips for color "C" as follows:
     *  (let "O" be the color of the opponent for "C")
     *  CO[O]C <-- this is a chunk
     *  where [O] is one or more opponent's pieces
     */
    bool hasChunk( Direction dir, const KReversiMove& move) const;
    /**
     *  Performs move, i.e. marks all the chips that player wins with
     *  this move with current player color
     */
    void makeMove( const KReversiMove& move );
    /**
     *  Board itself
     */
    KReversiBoard *m_board;
    /**
     *  Color of the current player
     */
    ChipColor m_curPlayer;
    /**
     *  The color of the human played chips
     */
    ChipColor m_playerColor;
    /**
     *  The color of the computer played chips
     */
    ChipColor m_computerColor;
    /**
     *  Our AI
     */
    Engine *m_engine;
     // Well I'm not brief at all :). That's because I think that my
     // English is not well shaped sometimes, so I try to describe things
     // so that me and others can understand. Even simple things.
     // Espesially when I think that my description sucks :)
    /**
     *  This list holds chips that were changed/added during last move
     *  First of them will be the chip added to the board by the player
     *  during last move. The rest of them - chips that were turned by that
     *  move.
     */
    QList<KReversiMove> m_changedChips;
    /**
     *  This list holds chips changed with last computer move
     *  and is used while undoing
     */
    QList<KReversiMove> m_lastUndoComputer;
    /**
     *  This list holds chips changed with last player move
     *  and is used while undoing
     */
    QList<KReversiMove> m_lastUndoPlayer;
};
#endif
