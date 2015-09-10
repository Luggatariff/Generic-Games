/*!
 * \file Chess.hpp
 * \brief Chess description
 * \author Anaël Petit
 */

#ifndef CHESS_HPP
#define CHESS_HPP

#define CHESS_WIDTH 	8		/*!<Chess board width*/
#define CHESS_HEIGHT	8		/*!<Chess board height*/
#define CHESS_MAX_SCORE 1000	/*!<Chess Maximal score (victory)*/

#include "Game.hpp"
#include "Board.hpp"
#include "map"
#include "sstream"

/*!
 * \brief Attributes than can be found in Chess Board Squares
 */
enum Chess_Attributes{
	CHESS_EMPTY,
	CHESS_BLACK_PAWN,
	CHESS_WHITE_PAWN,
	CHESS_BLACK_KNIGHT,
	CHESS_WHITE_KNIGHT,
	CHESS_BLACK_BISHOP,
	CHESS_WHITE_BISHOP,
	CHESS_BLACK_ROOK,
	CHESS_WHITE_ROOK,
	CHESS_BLACK_KING,
	CHESS_WHITE_KING,
	CHESS_BLACK_QUEEN,
	CHESS_WHITE_QUEEN
};

/*!
 * \class Chess
 * \brief class representing a Chess Game
 */
class Chess: public Game{
	Board<Chess_Attributes> * t_board;		/*!<Chess Game Board*/
	vector<Player *> t_players;				/*!<Chess Players */
	Player * t_next_player;					/*!<next Player*/
	vector<Coordinates> t_last_moves;		/*!<last moves played*/

	vector<Coordinates> t_playable_moves;		/*!<playable moves*/
	map<Player *, unsigned int> t_points;		/*!<Player points*/
	map<Player *, Coordinates> t_king_position;	/*!<Player king position*/
	map<Player *, bool> t_king_checked;			/*!<indicates if king is checked*/

	map<Player *, bool> t_castling_kingside_possible;		/*!<Indicates if kingside castling is still possible*/
	map<Player *, bool> t_castling_kingside_clear;			/*!<Indicates if path is clear for kingside castling*/
	map<Player *, bool> t_castling_queenside_possible;		/*!<Indicates if queenside castling is still possible*/
	map<Player *, bool> t_castling_queenside_clear;			/*!<Indicates if path is clear for queenside castling*/
	map<Player *, bool> t_castling_done;					/*!<Indicates if a castling have been done*/

	bool t_display_playable_coordinates;	/*!<Indicates if playable coordinates must be displayed under game board*/
	bool t_highlight_playable_squares;		/*!<Indicates if playable squares must be highlighted on game board*/
	bool t_display_board;					/*!<Indicates if board is displayed*/
	bool t_display_pgn;						/*!<Indicates if Portable Game Notation is displayed*/

	stringstream t_pgn;	/*!<Contains an updated Portable Game Notation for current Game*/

	/*!
	 * \brief updates Portable Game Notation
	 * \param square_src : Coordinates of source square for last move
	 * \param square_dst : Coordinates of destination square for last move
	 * \param piece_src : moved piece
	 * \param piece_prom : piece which a pawn is promoted to (CHESS_EMPTY when no promotion occured)
	 * \param is_capture : true if a capture occured
	 * \param is_check : true if the move lead to a check on the king
	 */
	inline void update_pgn(Coordinates square_src, Coordinates square_dst, Chess_Attributes piece_src, Chess_Attributes piece_prom, bool is_capture, bool is_check, bool is_kingside_castling, bool is_queenside_castling);
	/*!
	 * \brief computes the corresponding letter for given Chess Piece
	 * \param piece: chess piece
	 * \return corresponding letter in a char
	 */
	inline const char * get_pgn_piece(Chess_Attributes piece);

	/*!
	 * \brief sets a square with unknown but certain content to empty
	 * \param chess_square : square to set to empty
	 */
	void set_to_empty(Square<Chess_Attributes> * chess_square);

	/*!
	 * \brief gets the content of a non empty Chess square
	 * \param chess_square : non empty square
	 */
	Chess_Attributes get_piece(Square<Chess_Attributes> * chess_square);

	/*!
	 * \brief gives the turn to the other player
	 */
	void change_player();

	/*!
	 * \brief checks if a position of the board is under attack
	 * \param position : position Coordinates
	 * \return true if the position is under attack
	 */
	bool position_is_under_attack(Coordinates position);

	/*
	 * \brief adds a playable move for the given player
	 * \param player : pointer to the specified Player
	 * \param move_start : starting square for the specified move
	 * \param move_end : ending square for the specified move
	 */
	inline void add_playable_move(Player * player, Coordinates move_start, Coordinates move_end, Chess_Attributes promotion, bool verify_checked_king);
	/*!
	 * \brief updates t_playable_moves attribute depending on the board structure
	 */
	void update_playable_moves(bool verify_checked_king);

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
	/*!
	 * \brief checks if a square is white
	 * \param square : position to check
	 * \return true if the square is white
	 */
	inline bool check_white_square(Coordinates square);
	/*!
	 * \brief gets the player corresponding to a given piece
	 * \param chess_piece : piece name
	 * \return pointer on corresponding Player
	 */
	inline Player * get_piece_player(Chess_Attributes chess_piece);
	/*!
	 * \brief gets a piece value
	 * \param chess_piece : piece name
	 * \return given piece value
	 */
	inline unsigned int get_piece_value(Chess_Attributes chess_piece);
	/*!
	 * \brief sets a given piece in a given square
	 * \param set_square : square Coordinates
	 * \param set_piece : piece to set
	 */
	inline void set_piece(Coordinates set_square, Chess_Attributes set_piece);
	/*!
	 * \brief remove a given piece from a given square
	 * \param remove_square : square Coordinates
	 * \param remove_piece : piece to remove
	 */
	inline void remove_piece(Square<Chess_Attributes> * remove_square, Chess_Attributes remove_piece);
public:
	/*!
	 * \brief Chess constructor
	 * \param player_one : First Player
	 * \param player_two : Second Player
	 */
	Chess(Player * player_one, Player * player_two, bool display_board=true, bool highlight_playable_squares = false, bool display_playable_coordinates=false, bool display_pgn=true);
	~Chess();
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
