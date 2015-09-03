/*!
 * \file Chess.cpp
 * \brief Chess code
 * \author Anaël Petit
 */

#include <Chess/Chess.hpp>
#include <algorithm>

Chess::Chess(Player * player_one, Player * player_two){
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

	this->t_playable_moves.insert(pair<Player *, vector<Coordinates> >(player_one, vector<Coordinates>()));
	this->t_playable_moves.insert(pair<Player *, vector<Coordinates> >(player_two, vector<Coordinates>()));

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

	result->t_points = this->t_points;
	result->t_king_position = this->t_king_position;

	return (Game *)result;
}

vector<Player *> Chess::players(){
	return t_players;
}

bool Chess::isEnded(){
	if (isWon()) return true;

	bool draw_condition = this->t_playable_moves[this->t_next_player].empty();
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
					else{
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
bool Chess::isWinner(unsigned int team_id){
	bool result = false;

	if (team_id == t_players[0]->getTeam()){
		result = (this->t_next_player == t_players[1] && position_is_under_attack(this->t_king_position[this->t_next_player], t_players[0]) && this->t_playable_moves[this->t_next_player].empty());
	}
	else{
		result = (this->t_next_player == t_players[0] && position_is_under_attack(this->t_king_position[this->t_next_player], t_players[1]) && this->t_playable_moves[this->t_next_player].empty());
	}

	return result;
}

int Chess::score(unsigned int team_id){
	int winner = whoWon();
	if (winner == (int)team_id) return victoryScore();
	if (winner == -1){
		if (t_players[0]->getTeam() == team_id){
			return ((int)t_points[t_players[0]] - (int)t_points[t_players[1]]);
		}
		else{
			return ((int)t_points[t_players[1]] - (int)t_points[t_players[0]]);
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

	update_playable_moves();
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
}
void Chess::add_playable_move(Player * player, Coordinates move_start, Coordinates move_end){
	Coordinates move(4);
	move[0]=move_start[0];
	move[1]=move_start[1];
	move[2]=move_end[0];
	move[3]=move_end[1];
	this->t_playable_moves[player].push_back(move);
}
void Chess::update_playable_moves(){
	Coordinates square(2);
	for (square[0]=0; square[0]<CHESS_HEIGHT; square[0]++){
		for (square[1]=0; square[1]<CHESS_WIDTH; square[1]++){
			Square<Chess_Attributes> * board_square = t_board->getSquare(square);
			// First, check if the square represents a playable piece
			if (! (board_square->isAttribute(CHESS_EMPTY))){
				Chess_Attributes piece = this->get_piece(board_square);

				Player * player=get_piece_player(piece);
				if ( piece == CHESS_BLACK_PAWN || piece == CHESS_WHITE_PAWN ){
					int move_direction=1;
					if ( player == t_players[1] )
						move_direction=-1;

					bool already_moved=true;
					if ( (move_direction == 1 && square[0] == 1) || (move_direction == -1 && square[0] == (CHESS_HEIGHT-2)) )
						already_moved=false;

					Coordinates next_square=square;
					next_square[0]=next_square[0]+move_direction;

					if (check_existence(next_square) && t_board->getSquare(next_square)->isAttribute(CHESS_EMPTY)){
						add_playable_move(player, square, next_square);
						if (!already_moved){
							next_square[0]=next_square[0]+move_direction;
							if (t_board->getSquare(next_square)->isAttribute(CHESS_EMPTY)){
								add_playable_move(player, square, next_square);
							}
						}
					}

					next_square=square;
					next_square[0]=next_square[0]+move_direction;
					next_square[1]=next_square[1]-1;
					if (check_existence(next_square) && get_piece_player(get_piece(t_board->getSquare(next_square))) != player){
						add_playable_move(player, square, next_square);
					}
					next_square[1]=next_square[1]+2;
					if (check_existence(next_square) && get_piece_player(get_piece(t_board->getSquare(next_square))) != player){
						add_playable_move(player, square, next_square);
					}
				}
				else if ( piece == CHESS_BLACK_KNIGHT || piece == CHESS_WHITE_KNIGHT ){

				}
				else if ( piece == CHESS_BLACK_BISHOP || piece == CHESS_WHITE_BISHOP ){

				}
				else if ( piece == CHESS_BLACK_ROOK || piece == CHESS_WHITE_ROOK ){

				}
				else if ( piece == CHESS_BLACK_QUEEN || piece == CHESS_WHITE_QUEEN ){

				}
				else if ( piece == CHESS_BLACK_KING || piece == CHESS_WHITE_KING ){

				}
			}
		}
	}
}

void Chess::start(){
	t_last_moves.clear();

	t_last_move_was_null = false;
	t_count_null_moves = 0;
	t_count_last_moves_started = false;
	t_count_last_moves = 0;
	t_in_a_taking_row = false;

	t_recurring_boards.clear();
	t_current_board_occurence = 1;

	Coordinates square(2);
	for (square[0]=0; square[0]<CHESS_HEIGHT; square[0]++){
		for (square[1]=0; square[1]<CHESS_WIDTH; square[1]++){
			if (!t_board->getSquare(square)->isAttribute(CHESS_EMPTY) && !t_board->getSquare(square)->isAttribute(CHESS_UNPLAYABLE)){
				set_to_empty(t_board->getSquare(square));
			}
		}
	}

	this->t_white_pawns=0;
	this->t_black_pawns=0;

	for (square[0]=0; square[0]<CHESS_HEIGHT/2-1; square[0]++){
		for (square[1]=0; square[1]<CHESS_WIDTH; square[1]++){
			if (t_board->getSquare(square)->isAttribute(CHESS_EMPTY)){
				t_board->getSquare(square)->delAttribute(CHESS_EMPTY);
				t_board->getSquare(square)->addAttribute(CHESS_WHITE_PAWN);
				this->t_white_pawns++;
			}
		}
	}
	for (square[0]=CHESS_HEIGHT/2+1; square[0]<CHESS_HEIGHT; square[0]++){
		for (square[1]=0; square[1]<CHESS_WIDTH; square[1]++){
			if (t_board->getSquare(square)->isAttribute(CHESS_EMPTY)){
				t_board->getSquare(square)->delAttribute(CHESS_EMPTY);
				t_board->getSquare(square)->addAttribute(CHESS_BLACK_PAWN);
				this->t_black_pawns++;
			}
		}
	}

	this->t_black_total = this->t_black_pawns;
	this->t_white_total = this->t_white_pawns;
	this->t_black_queens = 0;
	this->t_white_queens = 0;
	this->t_next_player = t_players[0];

	update_playable_moves();
}
unsigned int Chess::dimension(){
	return 4;
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
	else if (chess_square->isAttribute(CHESS_WHITE_QUEEN)){
		return CHESS_WHITE_QUEEN;
	}
	else{
		return CHESS_BLACK_QUEEN;
	}
}

void Chess::set_to_empty(Square<Chess_Attributes> * chess_square){
	if (chess_square->isAttribute(CHESS_BLACK_PAWN)){
		chess_square->delAttribute(CHESS_BLACK_PAWN);
		this->t_black_pawns--;
		this->t_black_total--;
	}
	else if (chess_square->isAttribute(CHESS_WHITE_PAWN)){
		chess_square->delAttribute(CHESS_WHITE_PAWN);
		this->t_white_pawns--;
		this->t_white_total--;
	}
	else if (chess_square->isAttribute(CHESS_WHITE_QUEEN)){
		chess_square->delAttribute(CHESS_WHITE_QUEEN);
		this->t_white_queens--;
		this->t_white_total--;
	}
	else{
		chess_square->delAttribute(CHESS_BLACK_QUEEN);
		this->t_black_queens--;
		this->t_black_total--;
	}
	chess_square->addAttribute(CHESS_EMPTY);
}

void Chess::play(Coordinates coordinates){
	if (isPlayable(coordinates)){
		t_last_moves.push_back(coordinates);

		t_count_null_moves++;
		t_last_move_was_null = true;

		Coordinates source(2);
		Coordinates destination(2);
		source[0] = coordinates[0];
		source[1] = coordinates[1];
		destination[0] = coordinates[2];
		destination[1] = coordinates[3];

		Chess_Attributes source_piece = get_piece(t_board->getSquare(source));
		t_board->getSquare(source)->delAttribute(source_piece);
		t_board->getSquare(source)->addAttribute(CHESS_EMPTY);

		bool became_a_queen=false;
		Chess_Attributes destination_piece = source_piece;
		if ( source_piece == CHESS_WHITE_PAWN || source_piece == CHESS_BLACK_PAWN){
			this->t_count_null_moves = 0;
			t_last_move_was_null = false;
			if ( source_piece == CHESS_WHITE_PAWN && destination[0] == CHESS_HEIGHT - 1){
				destination_piece = CHESS_WHITE_QUEEN;
				this->t_white_queens++;
				this->t_white_pawns--;
				became_a_queen = true;
			}
			else if ( source_piece == CHESS_BLACK_PAWN && destination[0] == 0){
				destination_piece = CHESS_BLACK_QUEEN;
				this->t_black_queens++;
				this->t_black_pawns--;
				became_a_queen = true;
			}
		}
		t_board->getSquare(destination)->delAttribute(CHESS_EMPTY);
		t_board->getSquare(destination)->addAttribute(destination_piece);

		int height_direction = (source[0] < destination[0]) ? 1 : -1;
		int width_direction = (source[1] < destination[1]) ? 1 : -1;

		bool took_a_piece = false;
		Coordinates square(2);
		unsigned int offset = 1;
		square[0] = source[0] + offset*height_direction;
		square[1] = source[1] + offset*width_direction;
		while (! (square == destination)){
			if (!t_board->getSquare(square)->isAttribute(CHESS_EMPTY)){
				set_to_empty(t_board->getSquare(square));
				this->t_count_null_moves = 0;
				t_last_move_was_null = false;
				took_a_piece = true;
				break;
			}
			offset++;
			square[0] = source[0] + offset*height_direction;
			square[1] = source[1] + offset*width_direction;
		}

		if ((!t_count_last_moves_started) &&
				((t_white_queens == 1 && t_white_total == 1 && t_black_queens >= 1 && t_black_total == 3) ||
				(t_black_queens == 1 && t_black_total == 1 && t_white_queens >= 1 && t_white_total == 3))){
			t_count_last_moves_started=true;
			t_count_last_moves=0;
		}

		if (took_a_piece && !became_a_queen)
			update_playable_moves(true);
		else{
			change_player();
		}
	}
}
vector<Coordinates> Chess::lastMoves(){
	return this->t_last_moves;
}

void Chess::display(std::ostream & out){
	out<<" ";
	for(int column=0; column<CHESS_WIDTH; column++)
		out<<"|"<<column;
	out<<"|"<<std::endl;
	Coordinates coordinates(2);
	for(int line = CHESS_HEIGHT - 1; line>=0; line--){
		out<<line;
		for(coordinates[1]=0; coordinates[1]<CHESS_WIDTH; coordinates[1]++){
			coordinates[0] = (unsigned int) line;

			out<<"|";
			if (t_board->getSquare(coordinates)->isAttribute(CHESS_WHITE_PAWN))
				out<<'o';
			else if (t_board->getSquare(coordinates)->isAttribute(CHESS_BLACK_PAWN))
				out<<'x';
			else if (t_board->getSquare(coordinates)->isAttribute(CHESS_BLACK_QUEEN))
				out<<'X';
			else if (t_board->getSquare(coordinates)->isAttribute(CHESS_WHITE_QUEEN))
				out<<'O';
			else{
				vector<Coordinates>::iterator playable_moves_it;
				for ( playable_moves_it = t_playable_moves.begin(); playable_moves_it != t_playable_moves.end(); playable_moves_it++){
					if (playable_moves_it->operator [](2) == coordinates[0] && playable_moves_it->operator [](3) == coordinates[1])
						break;
				}
				if (playable_moves_it == t_playable_moves.end())
					out<<' ';
				else
					out<<"=";
			}
		}
		out<<"|"<<line<<"   ";
		if (line == CHESS_HEIGHT - 3){
			switch (t_current_board_occurence)
			{
				case 1:{
					out<<"First ";
					break;
				}
				case 2:{
					out<<"Second ";
					break;
				}
				case 3:{
					out<<"Third ";
				}
			}
			out<<"Occurrence(s) of this configuration";
		}
		else if (line == CHESS_HEIGHT - 4){
			out<<t_count_null_moves<<"/25 Null moves";
		}
		else if(t_count_last_moves_started && line == CHESS_HEIGHT - 5){
			out<<t_count_last_moves<<"/32 Last moves";
		}
		out<<std::endl;
	}
	out<<" ";
	for(int column=0; column<CHESS_WIDTH; column++)
		out<<"|"<<column;
	out<<"|"<<std::endl;

	out<<"Playable Coordinates :"<<endl;
	for (vector<Coordinates>::iterator playable_moves_it = t_playable_moves.begin(); playable_moves_it != t_playable_moves.end(); playable_moves_it++){
		playable_moves_it->display(out);
		if (distance(t_playable_moves.begin(), playable_moves_it) % 3 == 2){
			out<<endl;
		}
	}
	out<<endl;
}

string Chess::getName(){
	return string("Chess");
}


