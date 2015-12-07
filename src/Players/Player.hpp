/*!
 * \file Player.hpp
 * \brief Player description
 * \author Anaël Petit
 */

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <QtPlugin>
#include <QFrame>
#include <QString>
#include <QLabel>

#include "../Model/Coordinates.hpp"

/*!
 * \brief see Game.hpp
 */
class Game;

/*!
 * \class Player
 * \brief class describing a Game Player (either Artificial Intelligence or Human Player)
 */
class Player : public QObject{
public:
	/*!
	 * \brief informs the Player that a new Game is about to start
	 * \param game : pointer to the Game that is about to start
	 */
	virtual void start(Game * game) = 0;

	/*!
	 * \brief virtual method allowing a Player to choose a Coordinate to play
	 * \param game : pointer to the Game to play
	 * \return Coordinates of the choosen Square
	 */
	virtual Coordinates play(Game * game, vector<Coordinates> limit_choices = vector<Coordinates>()) = 0;

	/*!
	 * \brief informs the Player that a Game has ended
	 * \param game : pointer to the Game taht just ended
	 */
	virtual void end(Game * game) = 0;

	/*!
	 * \brief virtual function returning the name set for the Player
	 * \return Player name
	 */
    virtual QString getName() = 0;

	/*!
	 * \brief gets the Team id of the Player
	 * \return Player Team id
	 */
	virtual unsigned int getTeam() = 0;

    /*!
     * \brief gets the Type of Player (Human, IA...)
     * \return the current class name
     */
    virtual QString getType() = 0;

    /*!
    * \brief gets a clean Player instance
    * \return Player instance
    */
    virtual Player * createInstance() = 0;

    /*!
     * \brief gets the Parameter widgets associated a Label for current Player
     * \return a list of pointers to displayable widgets
     */
    virtual QList<QPair<QLabel *, QWidget *> > getParameterWidgets() = 0;

	/*!
	 * \brief virtual destructor for Player
	 */
    virtual ~Player(){}

public slots:

    /*!
     * \brief clickedMove slot, called when a move is clicked in the game
     * \param move
     */
    virtual void clickedMove(Coordinates move) = 0;
};

Q_DECLARE_INTERFACE(Player, "Generic-Games.Player")

#include "../Games/Game.hpp"

#endif
