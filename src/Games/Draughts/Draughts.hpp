/*!
 * \file Draughts.hpp
 * \brief Draughts description
 * \author Ana�l Petit
 */

#ifndef DRAUGHTS_HPP
#define DRAUGHTS_HPP

#define DRAUGHTS_WIDTH 	10		/*!<Draughts board width*/
#define DRAUGHTS_HEIGHT	10		/*!<Draughts board height*/
#define DRAUGHTS_MAX_SCORE 1000	/*!<Draughts Maximal score (victory)*/

#include "Game.hpp"
#include "Board.hpp"

/*!
 * \brief Attributes than can be found in Draughts Board Squares
 */
enum Draughts_Attributes{
	DRAUGHTS_UNPLAYABLE,
	DRAUGHTS_EMPTY,
	DRAUGHTS_BLACK_PAWN,
	DRAUGHTS_WHITE_PAWN,
	DRAUGHTS_BLACK_QUEEN,
	DRAUGHTS_WHITE_QUEEN
};

/*!
 * \class Draughts
 * \brief class representing a Draughts Game
 */
class Draughts: public Game{
	Board<Draughts_Attributes> * t_board;	/*!<Draughts Game Board*/
	vector<Player *> t_players;				/*!<Draughts Players */
	Player * t_next_player;					/*!<next Player*/
	vector<Coordinates> t_last_moves;		/*!<last moves played*/

	vector<Coordinates> t_playable_moves;	/*!<playable moves*/
	unsigned int t_white_pawns;				/*!<Number of white pawns*/
	unsigned int t_black_pawns;				/*!<Number of black pawns*/
	unsigned int t_white_queens;			/*!<Number of white queens*/
	unsigned int t_black_queens;			/*!<Number of black queens*/
	unsigned int t_white_total;				/*!<White total number (pawns+queens)*/
	unsigned int t_black_total;				/*!<Black total number (pawns+queens)*/

	bool t_last_move_was_null;				/*!<Indicate if last move was null (non taking and not a pawn)*/
	unsigned int t_count_null_moves;		/*!<counting moves without a taking or a pawn move (25 maximum, then Draw Game)*/
	bool t_count_last_moves_started;		/*!<boolean indicating if last moves count has started*/
	unsigned int t_count_last_moves;		/*!<counter for last moves*/

	vector<pair< pair<Board<Draughts_Attributes>, Player *>, unsigned int > > t_recurring_boards; /*!<Recurring boards*/
	unsigned int t_current_board_occurence;

	bool t_in_a_taking_row;					/*!<Indicates if a Player is in a row of taking moves*/

	/*!
	 * \brief sets a square with unknown but certain content to empty
	 * \param draughts_square : square to set to empty
	 */
	void set_to_empty(Square<Draughts_Attributes> * draughts_square);

	/*!
	 * \brief gets the content of a non empty Draughts square
	 * \param draughts_square : non empty square
	 */
	Draughts_Attributes get_piece(Square<Draughts_Attributes> * draughts_square);

	/*!
	 * \brief gives the turn to the other player
	 */
	void change_player();

	/*!
	 * \brief compute the number of pieces a taking move allows to take
	 * \param taking_move : move to test
	 * \return amount of eaten pieces
	 */
	unsigned int compute_takes(Coordinates taking_move);
	/*!
	 * \brief updates t_playable_moves attribute depending on the board structure
	 * \param change_player : indicates if the player has to change if there is no pawn to take
	 */
	void update_playable_moves(bool use_last_taking_move = false);

	/*!
	 * \brief checks if the given height is in the board
	 * \param height : given height
	 * \return true if the height is in the board
	 */
	inline bool check_height(int height);
	/*!
	 * \brief checks if the given width is in the board
	 * \param width : given width
	 * \return true if the width is in the board
	 */
	inline bool check_width(int width);
	/*!
	 * \brief checks if the given square is in the board
	 * \param square : square Coordinates
	 * \return true if the square is in the board
	 */
	inline bool check_existence(Coordinates square);
public:
	/*!
	 * \brief Draughts constructor
	 * \param player_one : First Player
	 * \param player_two : Second Player
	 */
	Draughts(Player * player_one, Player * player_two);
	~Draughts();
	Game * copy();

	vector<Player *> players();

	bool isEnded();
	bool isWon();
	int whoWon();
	bool isWinner(unsigned int team_id);

	int score(unsigned int team_id);
	int victoryScore();

	void start();
	unsigned int dimension();
	bool isPlayable(Coordinates coordinates);
	vector<Coordinates> playableCoordinates();

	Player * nextPlayer();
	void play(Coordinates coordinates);
	vector<Coordinates> lastMoves();

	void display(std::ostream & out);
	string getName();
};

#endif
