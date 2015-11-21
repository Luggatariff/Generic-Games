/*!
 * \file Game.hpp
 * \brief Game description
 * \author Anaël Petit
 */

#ifndef GAME_HPP
#define GAME_HPP

#include <QtPlugin>
#include <QFrame>
#include <QString>
#include "../Players/Player.hpp"

/*!
 * \class Game
 * \brief class representing a Game, its number of Player, Coordinates to play, display and ways to know if it ended
 */
class Game : public QObject{
    Q_OBJECT
public:
	/*!
	 * \brief gets the Players list
	 * \return a list of Players and their number
	 */
    virtual vector<Player *> players() = 0;

    /*!
     * \brief gets the number of Players for a Game
     * \return a number of Players
     */
    virtual unsigned int playersNumber()=0;

	/*!
	 * \brief allows to know if the Game has ended
	 * \return True if the Game has ended, False otherwise
	 */
	virtual bool isEnded() = 0;
	/*!
	 * \brief allows to know if the Game has been won by someone
	 * \return True if the Game has been won, False otherwise
	 */
	virtual bool isWon() = 0;
	/*!
	 * \brief gets the tem_id of the Player who won (if any)
	 * \return the team id of the Team who won (-1 if no one won)
	 */
	virtual int whoWon() = 0;
	/*!
	 * \brief indicates if a given Team won the game
	 * \param team_id : considered team
	 * \return True is the Team won, false otherwise
	 */
	virtual bool isWinner(unsigned int team_id) = 0;

	/*!
	 * \brief indicates a arbitrary score as an information about the state of the Game for the given Team
	 * \param team_id : Team id for which the score is computed
	 * \return a value representing the Player's score
	 */
	virtual int score(unsigned int team_id) = 0;
	/*!
	 * \brief gets the maximal score, corresponding to the victory
	 * \return maximal score value
	 */
	virtual int victoryScore() = 0;

	/*!
	 * \brief starts the Game
	 */
	virtual void start() = 0;
	/*!
	 * \brief gets the number of Coordinates used to play
	 * \return dimension of the used Coordinates
	 */
	virtual unsigned int dimension() = 0;
	/*!
	 * \brief indicates if a Square at the given Coordinates is playable
	 * \return True if the Square at the given Coordinates is playable, False otherwise
	 */
	virtual bool isPlayable(Coordinates coordinates) = 0;
	/*!
	 * \brief gets the list of playable Coordinates
	 * \return list of playable Coordinates
	 */
	virtual vector<Coordinates> playableCoordinates() = 0;

	/*!
	 * \brief gets the next Player who has to play
	 * \return pointer to the next Player
	 */
	virtual Player * nextPlayer() = 0;
	/*!
	 * \brief makes the Player actually play the Square at the given Coordinates
	 * \param coordinates : coordinates of the Square to play
	 */
	virtual void play(Coordinates coordinates) = 0;
	/*!
	 * \brief returns last moves, from the oldest to the newest
	 * \return list of last played Coordinates
	 */
	virtual vector<Coordinates> lastMoves() = 0;

	/*!
     * \brief gets a pointer to the Game display
	 */
    virtual QFrame * display() = 0;

	/*!
	 * \brief deep copy for a Game
	 * \return new pointer with source Game copy
	 */
	virtual Game * copy() = 0;

    /*!
     * \brief gets the Type of Game (Chess, TicTacToe...)
     * \return the current class name
     */
    virtual QString getType() = 0;

    /*!
     * \brief gets the Parameter widget associated to a Label for current Game
     * \return a list of displayable widgets
     */
    virtual QList<QPair<QLabel *, QWidget *> > getParameterWidgets() = 0;

    /*!
     * \brief virtual Game destructor
     */
    virtual ~Game(){}

public slots:
    /*!
     * \brief slot for changing Player
     * \param number : index of the modified player
     * \param player : pointer to new Player
     */
    virtual void changePlayer(unsigned int number, Player * player) = 0;

signals:

    /*!
     * \brief signals a move have been clicked on the board
     * \param move : clicked coordinates
     */
    virtual void clickedMove(vector<Coordinates> move) = 0;
};

Q_DECLARE_INTERFACE(Game, "Generic-Games.Game")

#endif
