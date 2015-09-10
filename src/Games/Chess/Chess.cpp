/*!
 * \file Chess.cpp
 * \brief Chess code
 * \author Anaël Petit
 */

#include <Chess/Chess.hpp>
#include <algorithm>

Chess::Chess(Player * player_one, Player * player_two, bool display_board, bool highlight_playable_squares, bool display_playable_coordinates, bool display_pgn){
	Coordinates board_size(2);
	board_size[0] = CHESS_HEIGHT;
	board_size[1] = CHESS_WIDTH;
	this->t_board = new Board<Chess_Attributes>(board_size);

	this->t_players.push_back(player_one);
	this->t_players.push_back(player_two);
	this->t_next_player = player_one;

	this->t_points.insert(pair<Player *, unsigned int>(player_one, 0));
	this->t_points.insert(pair<Player *, unsigned int>(player_two, 0));

	this->t_king_position.insert(pair<Player *, Coordinates>(player_one, Coordinates(2)));
	this->t_king_position.insert(pair<Player *, Coordinates>(player_two, Coordinates(2)));

	this->t_king_checked.insert(pair<Player *, bool>(player_one, false));
	this->t_king_checked.insert(pair<Player *, bool>(player_two, false));

	this->t_castling_kingside_possible.insert(pair<Player *, bool>(player_one, true));
	this->t_castling_kingside_possible.insert(pair<Player *, bool>(player_two, true));
	this->t_castling_kingside_clear.insert(pair<Player *, bool>(player_one, false));
	this->t_castling_kingside_clear.insert(pair<Player *, bool>(player_two, false));
	this->t_castling_queenside_possible.insert(pair<Player *, bool>(player_one, true));
	this->t_castling_queenside_possible.insert(pair<Player *, bool>(player_two, true));
	this->t_castling_queenside_clear.insert(pair<Player *, bool>(player_one, false));
	this->t_castling_queenside_clear.insert(pair<Player *, bool>(player_two, false));
	this->t_castling_done.insert(pair<Player *, bool>(player_one, false));
	this->t_castling_done.insert(pair<Player *, bool>(player_two, false));

	this->t_display_playable_coordinates=display_playable_coordinates;
	this->t_highlight_playable_squares=highlight_playable_squares;
	this->t_display_board=display_board;
	this->t_display_pgn=display_pgn;

	Coordinates square(2);
	for (square[0]=0; square[0]<CHESS_HEIGHT; square[0]++){
		for (square[1]=0; square[1]<CHESS_WIDTH; square[1]++){
			t_board->getSquare(square)->addAttribute(CHESS_EMPTY);
		}
	}
}

Chess::~Chess(){
	delete t_board;
}

Game * Chess::copy(){
	Chess * result = new Chess(this->t_players[0], this->t_players[1]);
	delete result->t_board;

	result->t_board = this->t_board->copy();
	result->t_next_player = this->t_next_player;
	result->t_playable_moves = this->t_playable_moves;

	result->t_points = map<Player *, unsigned int>(this->t_points);
	result->t_king_position =map<Player *, Coordinates>(this->t_king_position);
	result->t_king_checked=map<Player *, bool>(this->t_king_checked);

	result->t_highlight_playable_squares=this->t_highlight_playable_squares;
	result->t_display_playable_coordinates=this->t_display_playable_coordinates;
	result->t_display_board=this->t_display_board;
	result->t_display_pgn=this->t_display_pgn;

	result->t_castling_kingside_clear=this->t_castling_kingside_possible;
	result->t_castling_kingside_possible=this->t_castling_kingside_possible;
	result->t_castling_queenside_clear=this->t_castling_queenside_clear;
	result->t_castling_queenside_possible=this->t_castling_queenside_possible;
	result->t_castling_done=this->t_castling_done;

	return (Game *)result;
}

vector<Player *> Chess::players(){
	return t_players;
}

bool Chess::check_white_square(Coordinates square){
	return (((square[0]+square[1])%2)!=0);
}

bool Chess::isEnded(){
	if (isWon()) return true;

	bool draw_condition = this->t_playable_moves.empty();
	if (!draw_condition){
		bool flag_white_knight=false;
		bool flag_black_knight=false;

		bool flag_white_white_bishop=false;
		bool flag_white_black_bishop=false;
		bool flag_black_white_bishop=false;
		bool flag_black_black_bishop=false;

		draw_condition=true;
		Coordinates square(2);
		for (square[0]=0; square[0]<CHESS_HEIGHT; square[0]++){
			for (square[1]=0; square[1]<CHESS_WIDTH; square[1]++){
				Square<Chess_Attributes> * board_square = t_board->getSquare(square);
				if (!board_square->isAttribute(CHESS_EMPTY)){
					Chess_Attributes piece=this->get_piece(board_square);
					if (piece == CHESS_WHITE_BISHOP){
						bool white_square=this->check_white_square(square);
						if (white_square){
							flag_white_white_bishop=true;
							if (flag_white_black_bishop || flag_black_black_bishop || flag_white_knight || flag_black_knight){
								draw_condition=false;
								break;
							}
						}
						else{
							flag_white_black_bishop=true;
							if (flag_white_white_bishop || flag_black_white_bishop || flag_white_knight || flag_black_knight){
								draw_condition=false;
								break;
							}
						}
					}
					else if (piece == CHESS_BLACK_BISHOP){
						bool white_square=this->check_white_square(square);
						if (white_square){
							flag_black_white_bishop=true;
							if (flag_black_black_bishop || flag_white_black_bishop || flag_white_knight || flag_black_knight){
								draw_condition=false;
								break;
							}
						}
						else{
							flag_black_black_bishop=true;
							if (flag_black_white_bishop || flag_white_white_bishop || flag_white_knight || flag_black_knight){
								draw_condition=false;
								break;
							}
						}
					}
					else if (piece == CHESS_WHITE_KNIGHT){
						flag_white_knight=true;
						if (flag_black_black_bishop || flag_black_white_bishop || flag_white_black_bishop || flag_white_white_bishop || flag_black_knight){
							draw_condition=false;
							break;
						}
					}
					else if (piece == CHESS_BLACK_KNIGHT){
						flag_black_knight=true;
						if (flag_black_black_bishop || flag_black_white_bishop || flag_white_black_bishop || flag_white_white_bishop || flag_white_knight){
							draw_condition=false;
							break;
						}
					}
					else if (piece != CHESS_BLACK_KING && piece != CHESS_WHITE_KING){
						draw_condition=false;
						break;
					}
				}
			}
			if (!draw_condition)
				break;
		}
	}

	return ( draw_condition );
}
bool Chess::isWon(){
	return (whoWon() != -1);
}
int Chess::whoWon(){
	if (isWinner(t_players[0]->getTeam())) return t_players[0]->getTeam();
	if (isWinner(t_players[1]->getTeam())) return t_players[1]->getTeam();
	return -1;
}

bool Chess::position_is_under_attack(Coordinates position){
	vector<Coordinates>::iterator playable_moves_it;
	for ( playable_moves_it = t_playable_moves.begin(); playable_moves_it != t_playable_moves.end(); playable_moves_it++){
		if (playable_moves_it->operator [](2) == position[0] && playable_moves_it->operator [](3) == position[1])
			return true;
	}
	return false;
}

bool Chess::isWinner(unsigned int team_id){
	bool result = false;

	Player * opponent=t_players[0];
	if (opponent->getTeam() == team_id){
		opponent=t_players[1];
	}

	if (opponent==t_next_player){
		result=(t_king_checked[t_next_player] && t_playable_moves.empty());
	}

	return result;
}

int Chess::score(unsigned int team_id){
	int winner = whoWon();
	if (winner == (int)team_id) return victoryScore();
	if (winner == -1){
		if (t_players[0]->getTeam() == team_id){
			int score=((int)t_points[t_players[0]] - (int)t_points[t_players[1]]);
			if ( t_castling_kingside_possible[t_players[0]] || t_castling_queenside_possible[t_players[0]] || t_castling_done[t_players[0]] )
				score+=2;
			return score;
		}
		else{
			int score=((int)t_points[t_players[1]] - (int)t_points[t_players[0]]);
			if ( t_castling_kingside_possible[t_players[1]] || t_castling_queenside_possible[t_players[1]] || t_castling_done[t_players[1]] )
				score+=2;
			return score;
		}
	}
	return -victoryScore();
}
int Chess::victoryScore(){
	return CHESS_MAX_SCORE;
}

bool Chess::check_height(int height){
	return (height >=0 && height < CHESS_HEIGHT);
}

bool Chess::check_width(int width){
	return (width >= 0 && width < CHESS_WIDTH);
}

bool Chess::check_existence(Coordinates square){
	return check_height(square[0]) && check_width(square[1]);
}

void Chess::change_player(){
	if (t_next_player == t_players[1]){
		t_next_player = t_players[0];
	}
	else{
		t_next_player = t_players[1];
	}

	update_playable_moves(true);
}

Player * Chess::get_piece_player(Chess_Attributes chess_piece){
	switch (chess_piece)
	{
	case CHESS_BLACK_PAWN:
		return t_players[1];
	case CHESS_WHITE_PAWN:
		return t_players[0];
	case CHESS_BLACK_KNIGHT:
		return t_players[1];
	case CHESS_WHITE_KNIGHT:
		return t_players[0];
	case CHESS_BLACK_BISHOP:
		return t_players[1];
	case CHESS_WHITE_BISHOP:
		return t_players[0];
	case CHESS_BLACK_ROOK:
		return t_players[1];
	case CHESS_WHITE_ROOK:
		return t_players[0];
	case CHESS_BLACK_KING:
		return t_players[1];
	case CHESS_WHITE_KING:
		return t_players[0];
	case CHESS_BLACK_QUEEN:
		return t_players[1];
	case CHESS_WHITE_QUEEN:
		return t_players[0];
	case CHESS_EMPTY:
		return NULL;
	}
	return NULL;
}
void Chess::add_playable_move(Player * player, Coordinates move_start, Coordinates move_end, Chess_Attributes promotion, bool verify_checked_king){
	Coordinates move(5);
	move[0]=move_start[0];
	move[1]=move_start[1];
	move[2]=move_end[0];
	move[3]=move_end[1];
	move[4]=(unsigned int)promotion;

	Player * opponent=t_players[0];
	if (opponent == player){
		opponent=t_players[1];
	}

	if (this->t_king_position[opponent] == move_end){
		this->t_playable_moves.push_back(move);
	}
	else if (verify_checked_king){
		Chess * chess_copy=(Chess *)this->copy();

		Chess_Attributes source_piece = chess_copy->get_piece(chess_copy->t_board->getSquare(move_start));
		chess_copy->set_to_empty(chess_copy->t_board->getSquare(move_start));
		chess_copy->set_piece(move_end, source_piece);

		chess_copy->t_next_player = opponent;

		chess_copy->update_playable_moves(false);

		if (!chess_copy->position_is_under_attack(chess_copy->t_king_position[player])){
			this->t_playable_moves.push_back(move);
		}

		delete chess_copy;
	}
}
void Chess::update_playable_moves(bool verify_checked_king){
	this->t_playable_moves.clear();
	Coordinates square(2);
	for (square[0]=0; square[0]<CHESS_HEIGHT; square[0]++){
		for (square[1]=0; square[1]<CHESS_WIDTH; square[1]++){
			Chess_Attributes piece=this->get_piece(t_board->getSquare(square));
			if (get_piece_player(piece) == t_next_player){
				if ( piece == CHESS_BLACK_PAWN || piece == CHESS_WHITE_PAWN ){
					int move_direction=1;
					if ( t_next_player == t_players[1] )
						move_direction=-1;

					bool already_moved=true;
					if ( (move_direction == 1 && square[0] == 1) || (move_direction == -1 && square[0] == (CHESS_HEIGHT-2)) )
						already_moved=false;

					Coordinates next_square=square;
					next_square[0]=next_square[0]+move_direction;

					if (check_existence(next_square) && t_board->getSquare(next_square)->isAttribute(CHESS_EMPTY)){
						if (move_direction==1 && next_square[0]==CHESS_HEIGHT-1){
							add_playable_move(t_next_player, square, next_square, CHESS_WHITE_QUEEN, verify_checked_king);
							add_playable_move(t_next_player, square, next_square, CHESS_WHITE_KNIGHT, verify_checked_king);
							add_playable_move(t_next_player, square, next_square, CHESS_WHITE_BISHOP, verify_checked_king);
							add_playable_move(t_next_player, square, next_square, CHESS_WHITE_ROOK, verify_checked_king);
						}
						else if (move_direction==-1 && next_square[0]==0){
							add_playable_move(t_next_player, square, next_square, CHESS_BLACK_QUEEN, verify_checked_king);
							add_playable_move(t_next_player, square, next_square, CHESS_BLACK_KNIGHT, verify_checked_king);
							add_playable_move(t_next_player, square, next_square, CHESS_BLACK_BISHOP, verify_checked_king);
							add_playable_move(t_next_player, square, next_square, CHESS_BLACK_ROOK, verify_checked_king);
						}
						else
							add_playable_move(t_next_player, square, next_square, CHESS_EMPTY, verify_checked_king);
						if (!already_moved){
							next_square[0]=next_square[0]+move_direction;
							if (t_board->getSquare(next_square)->isAttribute(CHESS_EMPTY)){
								add_playable_move(t_next_player, square, next_square, CHESS_EMPTY, verify_checked_king);
							}
						}
					}

					next_square[0]=square[0]+move_direction;
					for (int dir=-1;dir<=1;dir+=2){
						next_square[1]=square[1]+dir;
						if (check_existence(next_square)){
							Player * next_square_player=get_piece_player(get_piece(t_board->getSquare(next_square)));
							if (next_square_player != t_next_player && next_square_player!=NULL){
								if (move_direction==1 && next_square[0]==CHESS_HEIGHT-1){
									add_playable_move(t_next_player, square, next_square, CHESS_WHITE_QUEEN, verify_checked_king);
									add_playable_move(t_next_player, square, next_square, CHESS_WHITE_KNIGHT, verify_checked_king);
									add_playable_move(t_next_player, square, next_square, CHESS_WHITE_BISHOP, verify_checked_king);
									add_playable_move(t_next_player, square, next_square, CHESS_WHITE_ROOK, verify_checked_king);
								}
								else if (move_direction==-1 && next_square[0]==0){
									add_playable_move(t_next_player, square, next_square, CHESS_BLACK_QUEEN, verify_checked_king);
									add_playable_move(t_next_player, square, next_square, CHESS_BLACK_KNIGHT, verify_checked_king);
									add_playable_move(t_next_player, square, next_square, CHESS_BLACK_BISHOP, verify_checked_king);
									add_playable_move(t_next_player, square, next_square, CHESS_BLACK_ROOK, verify_checked_king);
								}
								else
									add_playable_move(t_next_player, square, next_square, CHESS_EMPTY, verify_checked_king);
							}
							else if (next_square_player==NULL && !t_last_moves.empty()){
								//En Passant
								Coordinates last_move = t_last_moves.back();
								Coordinates last_move_start(2);
								last_move_start[0]=last_move[0];
								last_move_start[1]=last_move[1];
								Coordinates last_move_end(2);
								last_move_end[0]=last_move[2];
								last_move_end[1]=last_move[3];
								Chess_Attributes last_move_end_piece=get_piece(this->t_board->getSquare(last_move_end));
								Chess_Attributes wished_piece=CHESS_BLACK_PAWN;
								if (piece == CHESS_BLACK_PAWN)
									wished_piece=CHESS_WHITE_PAWN;
								if (last_move_start[0]==(next_square[0]+move_direction) && last_move_start[1]==next_square[1] && last_move_end_piece==wished_piece){
									add_playable_move(t_next_player, square, next_square, CHESS_EMPTY, verify_checked_king);
								}
							}
						}
					}
				}
				else if ( piece == CHESS_BLACK_KNIGHT || piece == CHESS_WHITE_KNIGHT ){
					for (unsigned int two_square_move_on_height=0; two_square_move_on_height<=1; two_square_move_on_height++){
						for (int two_square_move_direction=-1; two_square_move_direction<=1; two_square_move_direction+=2){
							for (int one_square_move_direction=-1; one_square_move_direction<=1; one_square_move_direction+=2){
								Coordinates next_square=square;
								if (two_square_move_on_height==0){
									next_square[0]=next_square[0]+one_square_move_direction;
									next_square[1]=next_square[1]+2*two_square_move_direction;
								}
								else{
									next_square[0]=next_square[0]+2*two_square_move_direction;
									next_square[1]=next_square[1]+one_square_move_direction;
								}
								if (check_existence(next_square) && get_piece_player(get_piece(t_board->getSquare(next_square))) != t_next_player){
									add_playable_move(t_next_player, square, next_square, CHESS_EMPTY, verify_checked_king);
								}
							}
						}
					}
				}
				else if ( piece == CHESS_BLACK_BISHOP || piece == CHESS_WHITE_BISHOP ){
					for (int height_direction=-1; height_direction<=1; height_direction+=2){
						for (int width_direction=-1; width_direction<=1; width_direction+=2){
							Coordinates next_square=square;
							next_square[0]=next_square[0]+height_direction;
							next_square[1]=next_square[1]+width_direction;
							while ( check_existence(next_square) && get_piece(t_board->getSquare(next_square)) == CHESS_EMPTY ){
								add_playable_move(t_next_player, square, next_square, CHESS_EMPTY, verify_checked_king);
								next_square[0]=next_square[0]+height_direction;
								next_square[1]=next_square[1]+width_direction;
							}
							if ( check_existence(next_square) && get_piece_player(get_piece(t_board->getSquare(next_square))) != t_next_player ){
								add_playable_move(t_next_player, square, next_square, CHESS_EMPTY, verify_checked_king);
							}
						}
					}
				}
				else if ( piece == CHESS_BLACK_ROOK || piece == CHESS_WHITE_ROOK ){
					for (int height_direction=-1; height_direction<=1; height_direction++){
						for (int width_direction=-1; width_direction<=1; width_direction++){
							if ( (width_direction==0 || height_direction==0) && (width_direction != height_direction) ){
								Coordinates next_square=square;
								next_square[0]=next_square[0]+height_direction;
								next_square[1]=next_square[1]+width_direction;
								while ( check_existence(next_square) && get_piece(t_board->getSquare(next_square)) == CHESS_EMPTY ){
									add_playable_move(t_next_player, square, next_square, CHESS_EMPTY, verify_checked_king);
									next_square[0]=next_square[0]+height_direction;
									next_square[1]=next_square[1]+width_direction;
								}
								if ( check_existence(next_square) && get_piece_player(get_piece(t_board->getSquare(next_square))) != t_next_player ){
									add_playable_move(t_next_player, square, next_square, CHESS_EMPTY, verify_checked_king);
								}
							}
						}
					}
				}
				else if ( piece == CHESS_BLACK_QUEEN || piece == CHESS_WHITE_QUEEN ){
					for (int height_direction=-1; height_direction<=1; height_direction++){
						for (int width_direction=-1; width_direction<=1; width_direction++){
							if ( ! (width_direction==0 && height_direction==0) ){
								Coordinates next_square=square;
								next_square[0]=next_square[0]+height_direction;
								next_square[1]=next_square[1]+width_direction;
								while ( check_existence(next_square) && get_piece(t_board->getSquare(next_square)) == CHESS_EMPTY ){
									add_playable_move(t_next_player, square, next_square, CHESS_EMPTY, verify_checked_king);
									next_square[0]=next_square[0]+height_direction;
									next_square[1]=next_square[1]+width_direction;
								}
								if ( check_existence(next_square) && get_piece_player(get_piece(t_board->getSquare(next_square))) != t_next_player ){
									add_playable_move(t_next_player, square, next_square, CHESS_EMPTY, verify_checked_king);
								}
							}
						}
					}
				}
				else if ( piece == CHESS_BLACK_KING || piece == CHESS_WHITE_KING ){
					for (int height_direction=-1; height_direction<=1; height_direction++){
						for (int width_direction=-1; width_direction<=1; width_direction++){
							if ( ! (width_direction==0 && height_direction==0) ){
								Coordinates next_square=square;
								next_square[0]=next_square[0]+height_direction;
								next_square[1]=next_square[1]+width_direction;
								if ( check_existence(next_square) && get_piece_player(get_piece(t_board->getSquare(next_square))) != t_next_player ){
									add_playable_move(t_next_player, square, next_square, CHESS_EMPTY, verify_checked_king);
								}
							}
						}
					}
					if (piece == CHESS_WHITE_KING && t_castling_kingside_possible[t_players[0]] && t_castling_kingside_clear[t_players[0]]){
						Coordinates next_square=square;
						next_square[1]=next_square[1]+2;
						add_playable_move(t_next_player, square, next_square, CHESS_EMPTY, verify_checked_king);
					}
					if (piece == CHESS_WHITE_KING && t_castling_queenside_possible[t_players[0]] && t_castling_queenside_clear[t_players[0]]){
						Coordinates next_square=square;
						next_square[1]=next_square[1]-2;
						add_playable_move(t_next_player, square, next_square, CHESS_EMPTY, verify_checked_king);
					}
					if (piece == CHESS_BLACK_KING && t_castling_kingside_possible[t_players[1]] && t_castling_kingside_clear[t_players[1]]){
						Coordinates next_square=square;
						next_square[1]=next_square[1]+2;
						add_playable_move(t_next_player, square, next_square, CHESS_EMPTY, verify_checked_king);
					}
					if (piece == CHESS_BLACK_KING && t_castling_queenside_possible[t_players[1]] && t_castling_queenside_clear[t_players[1]]){
						Coordinates next_square=square;
						next_square[1]=next_square[1]-2;
						add_playable_move(t_next_player, square, next_square, CHESS_EMPTY, verify_checked_king);
					}
				}
			}
		}
	}

}

unsigned int Chess::get_piece_value(Chess_Attributes chess_piece){
	switch (chess_piece)
	{
	case CHESS_BLACK_PAWN:
		return 1;
	case CHESS_WHITE_PAWN:
		return 1;
	case CHESS_BLACK_KNIGHT:
		return 3;
	case CHESS_WHITE_KNIGHT:
		return 3;
	case CHESS_BLACK_BISHOP:
		return 3;
	case CHESS_WHITE_BISHOP:
		return 3;
	case CHESS_BLACK_ROOK:
		return 5;
	case CHESS_WHITE_ROOK:
		return 5;
	case CHESS_BLACK_QUEEN:
		return 10;
	case CHESS_WHITE_QUEEN:
		return 10;
	default:
		return 0;
	}
}

void Chess::set_piece(Coordinates set_square, Chess_Attributes set_piece){
	set_to_empty(t_board->getSquare(set_square));
	t_board->getSquare(set_square)->delAttribute(CHESS_EMPTY);
	t_board->getSquare(set_square)->addAttribute(set_piece);

	Player * player=get_piece_player(set_piece);
	t_points[player]+=get_piece_value(set_piece);
	if ( set_piece == CHESS_WHITE_KING || set_piece == CHESS_BLACK_KING ){
		t_king_position[player]=set_square;
	}
}

void Chess::start(){
	t_last_moves.clear();

	this->t_king_checked[t_players[0]]=false;
	this->t_king_checked[t_players[1]]=false;
	this->t_castling_kingside_clear[t_players[0]]=false;
	this->t_castling_kingside_clear[t_players[1]]=false;
	this->t_castling_kingside_possible[t_players[0]]=true;
	this->t_castling_kingside_possible[t_players[1]]=true;
	this->t_castling_queenside_clear[t_players[0]]=false;
	this->t_castling_queenside_clear[t_players[1]]=false;
	this->t_castling_queenside_possible[t_players[0]]=true;
	this->t_castling_queenside_possible[t_players[1]]=true;
	this->t_castling_done[t_players[0]]=false;
	this->t_castling_done[t_players[1]]=false;

	this->t_pgn.str("");
	this->t_pgn.clear();

	Coordinates square(2);
	for (square[0]=0; square[0]<CHESS_HEIGHT; square[0]++){
		for (square[1]=0; square[1]<CHESS_WIDTH; square[1]++){
			set_to_empty(t_board->getSquare(square));
		}
	}
	Chess_Attributes piece;

	for (square[1]=0; square[1]<CHESS_WIDTH; square[1]++){
		square[0]=1;
		piece=CHESS_WHITE_PAWN;
		set_piece(square, piece);

		square[0]=CHESS_HEIGHT-2;
		piece=CHESS_BLACK_PAWN;
		set_piece(square, piece);
	}

	square[0]=0;
	piece=CHESS_WHITE_ROOK;
	square[1]=0;
	set_piece(square, piece);
	square[1]=CHESS_WIDTH-1;
	set_piece(square, piece);

	piece=CHESS_WHITE_KNIGHT;
	square[1]=1;
	set_piece(square, piece);
	square[1]=CHESS_WIDTH-2;
	set_piece(square, piece);

	piece=CHESS_WHITE_BISHOP;
	square[1]=2;
	set_piece(square, piece);
	square[1]=CHESS_WIDTH-3;
	set_piece(square, piece);

	piece=CHESS_WHITE_QUEEN;
	square[1]=3;
	set_piece(square, piece);
	piece=CHESS_WHITE_KING;
	square[1]=CHESS_WIDTH-4;
	set_piece(square, piece);

	square[0]=CHESS_HEIGHT-1;
	piece=CHESS_BLACK_ROOK;
	square[1]=0;
	set_piece(square, piece);
	square[1]=CHESS_WIDTH-1;
	set_piece(square, piece);

	piece=CHESS_BLACK_KNIGHT;
	square[1]=1;
	set_piece(square, piece);
	square[1]=CHESS_WIDTH-2;
	set_piece(square, piece);

	piece=CHESS_BLACK_BISHOP;
	square[1]=2;
	set_piece(square, piece);
	square[1]=CHESS_WIDTH-3;
	set_piece(square, piece);

	piece=CHESS_BLACK_QUEEN;
	square[1]=3;
	set_piece(square, piece);
	piece=CHESS_BLACK_KING;
	square[1]=CHESS_WIDTH-4;
	set_piece(square, piece);

	this->t_next_player = t_players[0];

	update_playable_moves(true);
}
unsigned int Chess::dimension(){
	return 5;
}

bool Chess::isPlayable(Coordinates coordinates){
	if (isEnded()) return false;

	return (find(this->t_playable_moves.begin(), this->t_playable_moves.end(), coordinates) != this->t_playable_moves.end());
}
vector<Coordinates> Chess::playableCoordinates(){
	if (!isEnded())
		return this->t_playable_moves;
	else
		return vector<Coordinates>();
}

Player * Chess::nextPlayer(){
	return t_next_player;
}

Chess_Attributes Chess::get_piece(Square<Chess_Attributes> * chess_square){
	if (chess_square->isAttribute(CHESS_BLACK_PAWN)){
		return CHESS_BLACK_PAWN;
	}
	else if (chess_square->isAttribute(CHESS_WHITE_PAWN)){
		return CHESS_WHITE_PAWN;
	}
	else if (chess_square->isAttribute(CHESS_BLACK_QUEEN)){
		return CHESS_BLACK_QUEEN;
	}
	else if (chess_square->isAttribute(CHESS_WHITE_QUEEN)){
		return CHESS_WHITE_QUEEN;
	}
	else if (chess_square->isAttribute(CHESS_BLACK_KING)){
		return CHESS_BLACK_KING;
	}
	else if (chess_square->isAttribute(CHESS_WHITE_KING)){
		return CHESS_WHITE_KING;
	}
	else if (chess_square->isAttribute(CHESS_BLACK_KNIGHT)){
		return CHESS_BLACK_KNIGHT;
	}
	else if (chess_square->isAttribute(CHESS_WHITE_KNIGHT)){
		return CHESS_WHITE_KNIGHT;
	}
	else if (chess_square->isAttribute(CHESS_BLACK_BISHOP)){
		return CHESS_BLACK_BISHOP;
	}
	else if (chess_square->isAttribute(CHESS_WHITE_BISHOP)){
		return CHESS_WHITE_BISHOP;
	}
	else if (chess_square->isAttribute(CHESS_BLACK_ROOK)){
		return CHESS_BLACK_ROOK;
	}
	else if (chess_square->isAttribute(CHESS_WHITE_ROOK)){
		return CHESS_WHITE_ROOK;
	}
	else{
		return CHESS_EMPTY;
	}
}

void Chess::remove_piece(Square<Chess_Attributes> * remove_square, Chess_Attributes remove_piece){
	remove_square->delAttribute(remove_piece);
	t_points[get_piece_player(remove_piece)]-=get_piece_value(remove_piece);
}

void Chess::set_to_empty(Square<Chess_Attributes> * chess_square){
	if (chess_square->isAttribute(CHESS_BLACK_PAWN)){
		remove_piece(chess_square, CHESS_BLACK_PAWN);
	}
	else if (chess_square->isAttribute(CHESS_WHITE_PAWN)){
		remove_piece(chess_square, CHESS_WHITE_PAWN);
	}
	else if (chess_square->isAttribute(CHESS_BLACK_QUEEN)){
		remove_piece(chess_square, CHESS_BLACK_QUEEN);
	}
	else if (chess_square->isAttribute(CHESS_WHITE_QUEEN)){
		remove_piece(chess_square, CHESS_WHITE_QUEEN);
	}
	else if (chess_square->isAttribute(CHESS_BLACK_KING)){
		remove_piece(chess_square, CHESS_BLACK_KING);
	}
	else if (chess_square->isAttribute(CHESS_WHITE_KING)){
		remove_piece(chess_square, CHESS_WHITE_KING);
	}
	else if (chess_square->isAttribute(CHESS_BLACK_KNIGHT)){
		remove_piece(chess_square, CHESS_BLACK_KNIGHT);
	}
	else if (chess_square->isAttribute(CHESS_WHITE_KNIGHT)){
		remove_piece(chess_square, CHESS_WHITE_KNIGHT);
	}
	else if (chess_square->isAttribute(CHESS_BLACK_BISHOP)){
		remove_piece(chess_square, CHESS_BLACK_BISHOP);
	}
	else if (chess_square->isAttribute(CHESS_WHITE_BISHOP)){
		remove_piece(chess_square, CHESS_WHITE_BISHOP);
	}
	else if (chess_square->isAttribute(CHESS_BLACK_ROOK)){
		remove_piece(chess_square, CHESS_BLACK_ROOK);
	}
	else if (chess_square->isAttribute(CHESS_WHITE_ROOK)){
		remove_piece(chess_square, CHESS_WHITE_ROOK);
	}
	chess_square->addAttribute(CHESS_EMPTY);
}

const char * Chess::get_pgn_piece(Chess_Attributes piece){
	switch (piece){
	case CHESS_WHITE_PAWN:
	case CHESS_BLACK_PAWN:{
		return "";
	}
	case CHESS_WHITE_ROOK:
	case CHESS_BLACK_ROOK:{
		return "R";
	}
	case CHESS_WHITE_KNIGHT:
	case CHESS_BLACK_KNIGHT:{
		return "N";
	}
	case CHESS_WHITE_BISHOP:
	case CHESS_BLACK_BISHOP:{
		return "B";
	}
	case CHESS_WHITE_QUEEN:
	case CHESS_BLACK_QUEEN:{
		return "Q";
	}
	case CHESS_WHITE_KING:
	case CHESS_BLACK_KING:{
		return "K";
	}
	case CHESS_EMPTY:
		return "P";
	}
	return "";
}

void Chess::update_pgn(Coordinates square_src, Coordinates square_dst, Chess_Attributes piece_src, Chess_Attributes piece_prom, bool is_capture, bool is_check, bool is_kingside_castling, bool is_queenside_castling){
	if ((t_last_moves.size()!=1) && (t_last_moves.size()-1)%6 == 0){
		t_pgn << endl;
	}
	if (t_last_moves.size()%2 != 0){
		t_pgn << t_last_moves.size()/2+1 << ". ";
	}
	if (is_kingside_castling){
		t_pgn << "O-O ";
	}
	else if (is_queenside_castling){
		t_pgn << "O-O-O ";
	}
	else{
		t_pgn<<get_pgn_piece(piece_src);
		t_pgn<<(char)(97+square_src[1])<<square_src[0]+1;
		if (is_capture){
			t_pgn<<"x";
		}
		t_pgn<<(char)(97+square_dst[1])<<square_dst[0]+1;
		if (piece_prom != CHESS_EMPTY){
			t_pgn<<"="<<get_pgn_piece(piece_prom);
		}
		if (isWon()){
			t_pgn<<"#";
		}
		else if (isEnded()){
			t_pgn<<" 1/2-1/2";
		}
		else if (is_check){
			t_pgn<<"+";
		}
		t_pgn<<" ";
	}
}

void Chess::play(Coordinates coordinates){
	if (isPlayable(coordinates)){
		t_last_moves.push_back(coordinates);

		Coordinates source(2);
		Coordinates destination(2);
		source[0] = coordinates[0];
		source[1] = coordinates[1];
		destination[0] = coordinates[2];
		destination[1] = coordinates[3];
		Chess_Attributes promotion_piece=(Chess_Attributes)coordinates[4];

		Chess_Attributes source_piece = get_piece(t_board->getSquare(source));
		Chess_Attributes destination_piece = get_piece(t_board->getSquare(destination));

		bool en_passant=false;
		bool kingside_castling=false;
		bool queenside_castling=false;

		int en_passant_direction;
		if ((source_piece==CHESS_WHITE_PAWN || source_piece==CHESS_BLACK_PAWN) && get_piece(t_board->getSquare(destination))==CHESS_EMPTY && source[1]!=destination[1]){
			en_passant=true;
			en_passant_direction=1;
			if (destination[0]<source[0]){
				en_passant_direction=-1;
			}
		}
		else if (source_piece==CHESS_WHITE_KING){
			t_castling_kingside_possible[t_players[0]]=false;
			t_castling_queenside_possible[t_players[0]]=false;
			if ((int)destination[1]-(int)source[1] == 2){
				kingside_castling=true;
				Coordinates rook_position(2);
				rook_position[0]=0;
				rook_position[1]=CHESS_WIDTH-1;
				set_to_empty(t_board->getSquare(rook_position));
				rook_position[1]=destination[1]-1;
				set_piece(rook_position, CHESS_WHITE_ROOK);
				t_castling_done[t_players[0]]=true;
			}
			else if ((int)destination[1]-(int)source[1] == -2){
				queenside_castling=true;
				Coordinates rook_position(2);
				rook_position[0]=0;
				rook_position[1]=0;
				set_to_empty(t_board->getSquare(rook_position));
				rook_position[1]=destination[1]+1;
				set_piece(rook_position, CHESS_WHITE_ROOK);
				t_castling_done[t_players[0]]=true;
			}
		}
		else if (source_piece==CHESS_BLACK_KING){
			t_castling_kingside_possible[t_players[1]]=false;
			t_castling_queenside_possible[t_players[1]]=false;
			if ((int)destination[1]-(int)source[1] == 2){
				kingside_castling=true;
				Coordinates rook_position(2);
				rook_position[0]=CHESS_HEIGHT-1;
				rook_position[1]=CHESS_WIDTH-1;
				set_to_empty(t_board->getSquare(rook_position));
				rook_position[1]=destination[1]-1;
				set_piece(rook_position, CHESS_BLACK_ROOK);
				t_castling_done[t_players[1]]=true;
			}
			else if ((int)destination[1]-(int)source[1] == -2){
				queenside_castling=true;
				Coordinates rook_position(2);
				rook_position[0]=CHESS_HEIGHT-1;
				rook_position[1]=0;
				set_to_empty(t_board->getSquare(rook_position));
				rook_position[1]=destination[1]+1;
				set_piece(rook_position, CHESS_BLACK_ROOK);
				t_castling_done[t_players[1]]=true;
			}
		}
		else if (source[0]==0 && source[1]==0){
			t_castling_queenside_possible[t_players[0]]=false;
		}
		else if (source[0]==0 && source[1]==CHESS_WIDTH-1){
			t_castling_kingside_possible[t_players[0]]=false;
		}
		else if (source[0]==CHESS_HEIGHT-1 && source[1]==0){
			t_castling_queenside_possible[t_players[1]]=false;
		}
		else if (source[0]==CHESS_HEIGHT-1 && source[1]==CHESS_WIDTH-1){
			t_castling_kingside_possible[t_players[1]]=false;
		}

		set_to_empty(t_board->getSquare(source));
		if (promotion_piece != CHESS_EMPTY)
			set_piece(destination, promotion_piece);
		else{
			set_piece(destination, source_piece);
			if (en_passant){
				Coordinates opponent_pawn_position(2);
				opponent_pawn_position[0]=destination[0]-en_passant_direction;
				opponent_pawn_position[1]=destination[1];
				set_to_empty(t_board->getSquare(opponent_pawn_position));
			}
		}

		update_playable_moves(true);
		Player * player=t_next_player;
		Player * opponent=t_players[0];
		if (player == t_players[0]){
			opponent=t_players[1];
		}
		t_king_checked[opponent]=position_is_under_attack(t_king_position[opponent]);

		if (t_castling_kingside_possible[opponent]){
			t_castling_kingside_clear[opponent]=true;

			Coordinates medium_position=t_king_position[opponent];
			medium_position[1]++;
			if (position_is_under_attack(medium_position)){
				t_castling_kingside_clear[opponent]=false;
			}
			else{
				for (medium_position[1]; medium_position[1]<CHESS_WIDTH-1; medium_position[1]++){
					if (get_piece(t_board->getSquare(medium_position))!=CHESS_EMPTY){
						t_castling_kingside_clear[opponent]=false;
						break;
					}
				}
			}
		}
		if (t_castling_queenside_possible[opponent]){
			t_castling_queenside_clear[opponent]=true;

			Coordinates medium_position=t_king_position[opponent];
			medium_position[1]--;
			if (position_is_under_attack(medium_position)){
				t_castling_queenside_clear[opponent]=false;
			}
			else{
				for (medium_position[1]; medium_position[1]>0; medium_position[1]--){
					if (get_piece(t_board->getSquare(medium_position))!=CHESS_EMPTY){
						t_castling_queenside_clear[opponent]=false;
						break;
					}
				}
			}
		}

		change_player();

		t_king_checked[player]=position_is_under_attack(t_king_position[player]);

		bool capture=(destination_piece!=CHESS_EMPTY);
		update_pgn(source, destination, source_piece, promotion_piece, capture, t_king_checked[opponent], kingside_castling, queenside_castling);
	}
}
vector<Coordinates> Chess::lastMoves(){
	return this->t_last_moves;
}

void Chess::display(std::ostream & out){
	if (this->t_display_board){
		out<<" ";
		for(int column=0; column<CHESS_WIDTH; column++)
			out<<"|  "<<(char)(65+column)<<"  ";
		out<<"|"<<std::endl;
		Coordinates coordinates(2);
		for(int line = CHESS_HEIGHT - 1; line>=0; line--){
			for (unsigned int thick=0; thick<3; thick++){
				if (thick==1)
					out<<line+1;
				else
					out<<" ";
				for(coordinates[1]=0; coordinates[1]<CHESS_WIDTH; coordinates[1]++){
					coordinates[0] = (unsigned int) line;
					Chess_Attributes piece = get_piece(t_board->getSquare(coordinates));

					out<<"| ";
					switch (piece){
					case CHESS_BLACK_PAWN:{
						if (thick == 0)
							out<<" ^ ";
						else if (thick == 1)
							out<<" | ";
						else if (thick == 2)
							out<<"/|\\";
						break;
					}
					case CHESS_WHITE_PAWN:{
						if (thick == 0)
							out<<" ^ ";
						else if (thick == 1)
							out<<" # ";
						else if (thick == 2)
							out<<"/|\\";
						break;
					}
					case CHESS_BLACK_KNIGHT:{
						if (thick == 0)
							out<<"^->";
						else if (thick == 1)
							out<<"|| ";
						else if (thick == 2)
							out<<"/ \\";
						break;
					}
					case CHESS_WHITE_KNIGHT:{
						if (thick == 0)
							out<<"^->";
						else if (thick == 1)
							out<<"|# ";
						else if (thick == 2)
							out<<"/ \\";
						break;
					}
					case CHESS_BLACK_BISHOP:{
						if (thick == 0)
							out<<" ^ ";
						else if (thick == 1)
							out<<"/ \\";
						else if (thick == 2)
							out<<"/ \\";
						break;
					}
					case CHESS_WHITE_BISHOP:{
						if (thick == 0)
							out<<" ^ ";
						else if (thick == 1)
							out<<"/#\\";
						else if (thick == 2)
							out<<"/ \\";
						break;
					}
					case CHESS_BLACK_ROOK:{
						if (thick == 0)
							out<<"+-+";
						else if (thick == 1)
							out<<"| |";
						else if (thick == 2)
							out<<"| |";
						break;
					}
					case CHESS_WHITE_ROOK:{
						if (thick == 0)
							out<<"+-+";
						else if (thick == 1)
							out<<"|#|";
						else if (thick == 2)
							out<<"| |";
						break;
					}
					case CHESS_BLACK_KING:{
						if (thick == 0)
							out<<" + ";
						else if (thick == 1)
							out<<"/ \\";
						else if (thick == 2)
							out<<"/ \\";
						break;
					}
					case CHESS_WHITE_KING:{
						if (thick == 0)
							out<<" + ";
						else if (thick == 1)
							out<<"/#\\";
						else if (thick == 2)
							out<<"/ \\";
						break;
					}
					case CHESS_BLACK_QUEEN:{
						if (thick == 0)
							out<<"YYY";
						else if (thick == 1)
							out<<"/ \\";
						else if (thick == 2)
							out<<"/ \\";
						break;
					}
					case CHESS_WHITE_QUEEN:{
						if (thick == 0)
							out<<"YYY";
						else if (thick == 1)
							out<<"/#\\";
						else if (thick == 2)
							out<<"/ \\";
						break;
					}
					case CHESS_EMPTY:{
						if ( thick != 1 )
							out<<"   ";
						else if (this->t_highlight_playable_squares && position_is_under_attack(coordinates))
							out<<" = ";
						else
							out<<"   ";
					}
					}
					out<<" ";
				}
				if (thick==1)
					out<<"|"<<line+1;
				else
					out<<"| ";

				if (line==0){
					if (thick == 0)
						out<<"    WHITE POINTS "<<t_points[t_players[0]];
					else if (thick==1 && t_king_checked[t_players[0]])
						out<<"    WHITE KING IS CHECKED";
				}
				if (line==CHESS_HEIGHT-1){
					if (thick == 0)
						out<<"    BLACK POINTS "<<t_points[t_players[1]];
					else if (thick==1 && t_king_checked[t_players[1]])
						out<<"    BLACK KING IS CHECKED";
				}

				out<<std::endl;
			}
		}
		out<<" ";
		for(int column=0; column<CHESS_WIDTH; column++)
			out<<"|  "<<(char)(65+column)<<"  ";
		out<<"|"<<std::endl;
	}

	if (this->t_display_playable_coordinates){
		bool add_endl=false;
		out<<"Playable Coordinates :"<<endl;
		for (vector<Coordinates>::iterator playable_moves_it = t_playable_moves.begin(); playable_moves_it != t_playable_moves.end(); playable_moves_it++){
			if (add_endl){
				out<<endl;
				add_endl=false;
			}
			playable_moves_it->display(out);
			if (distance(t_playable_moves.begin(), playable_moves_it) % 3 == 2){
				add_endl=true;
			}
		}
		out<<endl;
	}
	if (this->t_display_pgn){
		out<<t_pgn.str();
		out<<endl;
	}
}

string Chess::getName(){
	return string("Chess");
}


