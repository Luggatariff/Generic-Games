/*!
 * \file Draughts.cpp
 * \brief Draughts code
 * \author Anaël Petit
 */

#include <Draughts/Draughts.hpp>
#include <algorithm>

Draughts::Draughts(Player * player_one, Player * player_two){
	Coordinates board_size(2);
	board_size[0] = DRAUGHTS_HEIGHT;
	board_size[1] = DRAUGHTS_WIDTH;
	this->t_board = new Board<Draughts_Attributes>(board_size);

	this->t_players.push_back(player_one);
	this->t_players.push_back(player_two);
	this->t_next_player = player_one;

	this->t_black_pawns = 0;
	this->t_black_queens = 0;
	this->t_black_total = 0;
	this->t_white_pawns = 0;
	this->t_white_queens = 0;
	this->t_white_total = 0;

	this->t_last_move_was_null=false;
	this->t_count_null_moves = 0;
	this->t_count_last_moves_started = false;
	this->t_count_last_moves = 0;

	this->t_current_board_occurence = 1;

	this->t_in_a_taking_row = false;

	Coordinates square(2);
	for (square[0]=0; square[0]<DRAUGHTS_HEIGHT; square[0]++){
		for (square[1]=0; square[1]<DRAUGHTS_WIDTH; square[1]++){
			if ((square[0] + square[1]) % 2 == 0 ){
				t_board->getSquare(square)->addAttribute(DRAUGHTS_EMPTY);
			}
			else{
				t_board->getSquare(square)->addAttribute(DRAUGHTS_UNPLAYABLE);
			}
		}
	}
}

Draughts::~Draughts(){
	delete t_board;
}

Game * Draughts::copy(){
	Draughts * result = new Draughts(this->t_players[0], this->t_players[1]);
	delete result->t_board;

	result->t_board = this->t_board->copy();
	result->t_next_player = this->t_next_player;
	result->t_playable_moves = this->t_playable_moves;
	result->t_black_pawns = this->t_black_pawns;
	result->t_black_queens = this->t_black_queens;
	result->t_black_total = this->t_black_total;
	result->t_white_pawns = this->t_white_pawns;
	result->t_white_queens = this->t_white_queens;
	result->t_white_total = this->t_white_total;

	result->t_last_move_was_null = this->t_last_move_was_null;
	result->t_count_null_moves = this->t_count_null_moves;
	result->t_count_last_moves = this->t_count_last_moves;
	result->t_count_last_moves_started = this->t_count_last_moves_started;

	result->t_recurring_boards = this->t_recurring_boards;
	result->t_current_board_occurence = this->t_current_board_occurence;

	result->t_in_a_taking_row=this->t_in_a_taking_row;

	return (Game *)result;
}

vector<Player *> Draughts::players(){
	return t_players;
}

bool Draughts::isEnded(){
	if (isWon()) return true;

	bool draw_condition = (t_count_null_moves >= 25);
	if (!draw_condition)
		draw_condition = (!t_in_a_taking_row && (t_white_pawns == 0 && t_black_pawns == 0 && (t_white_queens + t_black_queens) <= 3));
	if (!draw_condition)
		draw_condition = (t_count_last_moves_started && t_count_last_moves>=32);
	if (!draw_condition){
		draw_condition = (t_current_board_occurence >= 3);
	}

	return ( draw_condition );
}
bool Draughts::isWon(){
	return (whoWon() != -1);
}
int Draughts::whoWon(){
	if (isWinner(t_players[0]->getTeam())) return t_players[0]->getTeam();
	if (isWinner(t_players[1]->getTeam())) return t_players[1]->getTeam();
	return -1;
}
bool Draughts::isWinner(unsigned int team_id){
	bool result = false;

	if (team_id == t_players[0]->getTeam()){
		result = (this->t_black_total == 0 || (this->t_next_player == t_players[1] && this->t_playable_moves.empty()));
	}
	else{
		result = (this->t_white_total == 0 || (this->t_next_player == t_players[0] && this->t_playable_moves.empty()));
	}

	return result;
}

int Draughts::score(unsigned int team_id){
	int winner = whoWon();
	if (winner == (int)team_id) return victoryScore();
	if (winner == -1){
		int player_pawns;
		int player_queens;
		int adverse_pawns;
		int adverse_queens;
		if (t_players[0]->getTeam() == team_id){
			player_pawns=(int)t_white_pawns;
			player_queens=(int)t_white_queens;
			adverse_pawns=(int)t_black_pawns;
			adverse_queens=(int)t_black_queens;
		}
		else{
			player_pawns=(int)t_black_pawns;
			player_queens=(int)t_black_queens;
			adverse_pawns=(int)t_white_pawns;
			adverse_queens=(int)t_white_queens;
		}
		int pure_score = player_pawns*25 + player_queens*50 - adverse_pawns*25 - adverse_queens*50;

		int draw_modifier = t_count_null_moves*2;
		if ((int)t_count_last_moves > draw_modifier){
			draw_modifier = t_count_last_moves;
		}
		if ( isEnded() ){
			draw_modifier = victoryScore()/10;
		}
		//player in a bad situation will try to reach draw game
		if (pure_score >= 0){
			return pure_score - draw_modifier;
		}
		else{
			return pure_score + draw_modifier;
		}
	}
	return -victoryScore();
}
int Draughts::victoryScore(){
	return DRAUGHTS_MAX_SCORE;
}

bool Draughts::check_height(int height){
	return (height >=0 && height < DRAUGHTS_HEIGHT);
}

bool Draughts::check_width(int width){
	return (width >= 0 && width < DRAUGHTS_WIDTH);
}

bool Draughts::check_existence(Coordinates square){
	return check_height(square[0]) && check_width(square[1]);
}

void Draughts::change_player(){
	if (t_next_player == t_players[1]){
		t_next_player = t_players[0];
	}
	else{
		t_next_player = t_players[1];
	}

	if (t_count_last_moves_started){
		t_count_last_moves++;
	}

	if (t_last_move_was_null){
		pair<Board<Draughts_Attributes>, Player *> recurring_boards_key(*t_board, t_next_player);
		vector<pair< pair<Board<Draughts_Attributes>, Player *>, unsigned int > >::iterator t_recurring_boards_it;
		for (t_recurring_boards_it = t_recurring_boards.begin(); t_recurring_boards_it != t_recurring_boards.end(); t_recurring_boards_it++){
			if (t_recurring_boards_it->first == recurring_boards_key){
				t_recurring_boards_it->second++;
				break;
			}
		}
		if (t_recurring_boards_it == t_recurring_boards.end()){
			pair< pair<Board<Draughts_Attributes>, Player *>, unsigned int > new_recurring_board(recurring_boards_key, 1);
			t_recurring_boards.push_back(new_recurring_board);
			t_current_board_occurence = 1;
		}
		else{
			t_current_board_occurence = t_recurring_boards_it->second;
		}
	}
	else{
		t_current_board_occurence = 1;
	}
	update_playable_moves();
}

unsigned int Draughts::compute_takes(Coordinates taking_move){
	unsigned int result = 1;
	Draughts * game_copy = (Draughts *)this->copy();

	game_copy->play(taking_move);
	if ( !game_copy->isEnded() && this->t_next_player == game_copy->t_next_player){
		vector<Coordinates> next_taking_moves = game_copy->playableCoordinates();
		vector<Coordinates>::iterator next_taking_moves_it = next_taking_moves.begin();
		unsigned int max_takes = game_copy->compute_takes(*next_taking_moves_it);
		for (next_taking_moves_it++; next_taking_moves_it != next_taking_moves.end(); next_taking_moves_it++){
			unsigned int takes = game_copy->compute_takes(*next_taking_moves_it);
			if (takes > max_takes){
				max_takes = takes;
			}
		}
		result = max_takes + 1;
	}

	delete game_copy;
	return result;
}

void Draughts::update_playable_moves(bool use_last_taking_move){
	t_in_a_taking_row=use_last_taking_move;

	// It's necessary to discriminate taking moves from simple moves (taking moves have to be played)
	vector<Coordinates> simple_moves;
	vector<Coordinates> taking_moves;

	//White pawns move to the up of the board
	int move_direction = 1;
	Draughts_Attributes player_pawn = DRAUGHTS_WHITE_PAWN;
	Draughts_Attributes player_queen = DRAUGHTS_WHITE_QUEEN;
	Draughts_Attributes adverse_pawn = DRAUGHTS_BLACK_PAWN;
	Draughts_Attributes adverse_queen = DRAUGHTS_BLACK_QUEEN;
	if (t_next_player == t_players[1]){
		//Black pawns move backward
		move_direction = -1;
		player_pawn = DRAUGHTS_BLACK_PAWN;
		player_queen = DRAUGHTS_BLACK_QUEEN;
		adverse_pawn = DRAUGHTS_WHITE_PAWN;
		adverse_queen = DRAUGHTS_WHITE_QUEEN;
	}

	unsigned int first_height = 0;
	unsigned int last_height = DRAUGHTS_HEIGHT;
	unsigned int first_width = 0;
	unsigned int last_width = DRAUGHTS_WIDTH;
	//If we may be in a row of taking moves
	if (use_last_taking_move){
		//Then we only care about last moved piece, so it's no really a loop on the board
		first_height = t_last_moves.back()[2];
		first_width = t_last_moves.back()[3];
		last_height = first_height + 1;
		last_width = first_width + 1;
	}

	Coordinates square(2);
	for (square[0]=first_height; square[0]<last_height; square[0]++){
		for (square[1]=first_width; square[1]<last_width; square[1]++){
			Square<Draughts_Attributes> * board_square = t_board->getSquare(square);
			// First, check if the square represents a playable piece
			if (! (board_square->isAttribute(DRAUGHTS_UNPLAYABLE) || board_square->isAttribute(DRAUGHTS_EMPTY))){
				// Then it could be the source of a move
				Coordinates potential_move(4);
				potential_move[0] = square[0];
				potential_move[1] = square[1];

				// differentiate a pawn from a queen
				if (board_square->isAttribute(player_pawn)){
					for (int height_direction = -1; height_direction <= 1; height_direction += 2){
						for (int width_direction = -1; width_direction <= 1; width_direction += 2){
							if (check_height(square[0] + 2*height_direction)){
								if (check_width(square[1] + 2*width_direction)){
									Coordinates potential_destination(2);
									potential_destination[0] = square[0] + 2*height_direction;
									potential_destination[1] = square[1] + 2*width_direction;
									if (t_board->getSquare(potential_destination)->isAttribute(DRAUGHTS_EMPTY)){
										Coordinates potential_take(2);
										potential_take[0] = square[0] + height_direction;
										potential_take[1] = square[1] + width_direction;
										if (t_board->getSquare(potential_take)->isAttribute(adverse_pawn) || t_board->getSquare(potential_take)->isAttribute(adverse_queen)){
											potential_move[2] = potential_destination[0];
											potential_move[3] = potential_destination[1];
											taking_moves.push_back(potential_move);
										}
									}
								}
							}
						}
					}

					//if there is already taking moves, simple_moves are no longer needed to compute
					if (taking_moves.empty()){
						int height_direction = move_direction;
						for (int width_direction = -1; width_direction <= 1; width_direction += 2){
							if (check_height(square[0] + height_direction)){
								if (check_width(square[1] + width_direction)){
									Coordinates potential_destination(2);
									potential_destination[0] = square[0] + height_direction;
									potential_destination[1] = square[1] + width_direction;
									if (t_board->getSquare(potential_destination)->isAttribute(DRAUGHTS_EMPTY)){
										potential_move[2] = potential_destination[0];
										potential_move[3] = potential_destination[1];
										simple_moves.push_back(potential_move);
									}
								}
							}
						}
					}
				}
				else if (board_square->isAttribute(player_queen)){
					for (int height_direction = -1; height_direction <= 1; height_direction += 2){
						for (int width_direction = -1; width_direction <= 1; width_direction += 2){
							bool found_adverse_piece = false;
							for (unsigned int offset = 1; offset < DRAUGHTS_HEIGHT; offset++){
								if (check_height(square[0] + offset*height_direction)){
									if (check_width(square[1] + offset*width_direction)){
										Coordinates potential_destination(2);
										potential_destination[0] = square[0] + offset*height_direction;
										potential_destination[1] = square[1] + offset*width_direction;
										if (t_board->getSquare(potential_destination)->isAttribute(DRAUGHTS_EMPTY)){
											potential_move[2] = potential_destination[0];
											potential_move[3] = potential_destination[1];
											if (!found_adverse_piece){
												simple_moves.push_back(potential_move);
											}
											else{
												taking_moves.push_back(potential_move);
											}
										}
										else if (t_board->getSquare(potential_destination)->isAttribute(adverse_pawn) || t_board->getSquare(potential_destination)->isAttribute(adverse_queen)){
											if (found_adverse_piece){
												break;
											}
											found_adverse_piece=true;
										}
										else{
											break;
										}
									}
									else{
										break;
									}
								}
								else{
									break;
								}
							}
						}
					}
				}
			}
		}
	}

	if (use_last_taking_move && taking_moves.empty()){
		change_player();
	}
	else{
		if (!taking_moves.empty()){
			t_playable_moves=taking_moves;

			vector<Coordinates> final_taking_moves;
			unsigned int max_taken_pieces;
			vector<Coordinates>::iterator taking_moves_it = taking_moves.begin();

			max_taken_pieces = compute_takes(*taking_moves_it);
			final_taking_moves.push_back(*taking_moves_it);
			for (taking_moves_it++; taking_moves_it != taking_moves.end(); taking_moves_it++){
				unsigned int taken_pieces = compute_takes(*taking_moves_it);
				if (taken_pieces > max_taken_pieces){
					final_taking_moves.clear();
					max_taken_pieces = taken_pieces;
					final_taking_moves.push_back(*taking_moves_it);
				}
				else if(taken_pieces == max_taken_pieces){
					final_taking_moves.push_back(*taking_moves_it);
				}
			}
			t_playable_moves=final_taking_moves;
		}
		else{
			t_playable_moves=simple_moves;
		}
	}
}

void Draughts::start(){
	t_last_moves.clear();

	t_last_move_was_null = false;
	t_count_null_moves = 0;
	t_count_last_moves_started = false;
	t_count_last_moves = 0;
	t_in_a_taking_row = false;

	t_recurring_boards.clear();
	t_current_board_occurence = 1;

	Coordinates square(2);
	for (square[0]=0; square[0]<DRAUGHTS_HEIGHT; square[0]++){
		for (square[1]=0; square[1]<DRAUGHTS_WIDTH; square[1]++){
			if (!t_board->getSquare(square)->isAttribute(DRAUGHTS_EMPTY) && !t_board->getSquare(square)->isAttribute(DRAUGHTS_UNPLAYABLE)){
				set_to_empty(t_board->getSquare(square));
			}
		}
	}

	this->t_white_pawns=0;
	this->t_black_pawns=0;

	for (square[0]=0; square[0]<DRAUGHTS_HEIGHT/2-1; square[0]++){
		for (square[1]=0; square[1]<DRAUGHTS_WIDTH; square[1]++){
			if (t_board->getSquare(square)->isAttribute(DRAUGHTS_EMPTY)){
				t_board->getSquare(square)->delAttribute(DRAUGHTS_EMPTY);
				t_board->getSquare(square)->addAttribute(DRAUGHTS_WHITE_PAWN);
				this->t_white_pawns++;
			}
		}
	}
	for (square[0]=DRAUGHTS_HEIGHT/2+1; square[0]<DRAUGHTS_HEIGHT; square[0]++){
		for (square[1]=0; square[1]<DRAUGHTS_WIDTH; square[1]++){
			if (t_board->getSquare(square)->isAttribute(DRAUGHTS_EMPTY)){
				t_board->getSquare(square)->delAttribute(DRAUGHTS_EMPTY);
				t_board->getSquare(square)->addAttribute(DRAUGHTS_BLACK_PAWN);
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
unsigned int Draughts::dimension(){
	return 4;
}

bool Draughts::isPlayable(Coordinates coordinates){
	if (isEnded()) return false;

	return (find(this->t_playable_moves.begin(), this->t_playable_moves.end(), coordinates) != this->t_playable_moves.end());
}
vector<Coordinates> Draughts::playableCoordinates(){
	if (!isEnded())
		return this->t_playable_moves;
	else
		return vector<Coordinates>();
}

Player * Draughts::nextPlayer(){
	return t_next_player;
}

Draughts_Attributes Draughts::get_piece(Square<Draughts_Attributes> * draughts_square){
	if (draughts_square->isAttribute(DRAUGHTS_BLACK_PAWN)){
		return DRAUGHTS_BLACK_PAWN;
	}
	else if (draughts_square->isAttribute(DRAUGHTS_WHITE_PAWN)){
		return DRAUGHTS_WHITE_PAWN;
	}
	else if (draughts_square->isAttribute(DRAUGHTS_WHITE_QUEEN)){
		return DRAUGHTS_WHITE_QUEEN;
	}
	else{
		return DRAUGHTS_BLACK_QUEEN;
	}
}

void Draughts::set_to_empty(Square<Draughts_Attributes> * draughts_square){
	if (draughts_square->isAttribute(DRAUGHTS_BLACK_PAWN)){
		draughts_square->delAttribute(DRAUGHTS_BLACK_PAWN);
		this->t_black_pawns--;
		this->t_black_total--;
	}
	else if (draughts_square->isAttribute(DRAUGHTS_WHITE_PAWN)){
		draughts_square->delAttribute(DRAUGHTS_WHITE_PAWN);
		this->t_white_pawns--;
		this->t_white_total--;
	}
	else if (draughts_square->isAttribute(DRAUGHTS_WHITE_QUEEN)){
		draughts_square->delAttribute(DRAUGHTS_WHITE_QUEEN);
		this->t_white_queens--;
		this->t_white_total--;
	}
	else{
		draughts_square->delAttribute(DRAUGHTS_BLACK_QUEEN);
		this->t_black_queens--;
		this->t_black_total--;
	}
	draughts_square->addAttribute(DRAUGHTS_EMPTY);
}

void Draughts::play(Coordinates coordinates){
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

		Draughts_Attributes source_piece = get_piece(t_board->getSquare(source));
		t_board->getSquare(source)->delAttribute(source_piece);
		t_board->getSquare(source)->addAttribute(DRAUGHTS_EMPTY);

		bool became_a_queen=false;
		Draughts_Attributes destination_piece = source_piece;
		if ( source_piece == DRAUGHTS_WHITE_PAWN || source_piece == DRAUGHTS_BLACK_PAWN){
			this->t_count_null_moves = 0;
			t_last_move_was_null = false;
			if ( source_piece == DRAUGHTS_WHITE_PAWN && destination[0] == DRAUGHTS_HEIGHT - 1){
				destination_piece = DRAUGHTS_WHITE_QUEEN;
				this->t_white_queens++;
				this->t_white_pawns--;
				became_a_queen = true;
			}
			else if ( source_piece == DRAUGHTS_BLACK_PAWN && destination[0] == 0){
				destination_piece = DRAUGHTS_BLACK_QUEEN;
				this->t_black_queens++;
				this->t_black_pawns--;
				became_a_queen = true;
			}
		}
		t_board->getSquare(destination)->delAttribute(DRAUGHTS_EMPTY);
		t_board->getSquare(destination)->addAttribute(destination_piece);

		int height_direction = (source[0] < destination[0]) ? 1 : -1;
		int width_direction = (source[1] < destination[1]) ? 1 : -1;

		bool took_a_piece = false;
		Coordinates square(2);
		unsigned int offset = 1;
		square[0] = source[0] + offset*height_direction;
		square[1] = source[1] + offset*width_direction;
		while (! (square == destination)){
			if (!t_board->getSquare(square)->isAttribute(DRAUGHTS_EMPTY)){
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
vector<Coordinates> Draughts::lastMoves(){
	return this->t_last_moves;
}

void Draughts::display(std::ostream & out){
	out<<" ";
	for(int column=0; column<DRAUGHTS_WIDTH; column++)
		out<<"|"<<column;
	out<<"|"<<std::endl;
	Coordinates coordinates(2);
	for(int line = DRAUGHTS_HEIGHT - 1; line>=0; line--){
		out<<line;
		for(coordinates[1]=0; coordinates[1]<DRAUGHTS_WIDTH; coordinates[1]++){
			coordinates[0] = (unsigned int) line;

			out<<"|";
			if (t_board->getSquare(coordinates)->isAttribute(DRAUGHTS_WHITE_PAWN))
				out<<'o';
			else if (t_board->getSquare(coordinates)->isAttribute(DRAUGHTS_BLACK_PAWN))
				out<<'x';
			else if (t_board->getSquare(coordinates)->isAttribute(DRAUGHTS_BLACK_QUEEN))
				out<<'X';
			else if (t_board->getSquare(coordinates)->isAttribute(DRAUGHTS_WHITE_QUEEN))
				out<<'O';
			else
				out<<" ";
		}
		out<<"|"<<line<<"   ";
		if (line == DRAUGHTS_HEIGHT - 3){
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
		else if (line == DRAUGHTS_HEIGHT - 4){
			out<<t_count_null_moves<<"/25 Null moves";
		}
		else if(t_count_last_moves_started && line == DRAUGHTS_HEIGHT - 5){
			out<<t_count_last_moves<<"/32 Last moves";
		}
		out<<std::endl;
	}
	out<<" ";
	for(int column=0; column<DRAUGHTS_WIDTH; column++)
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

string Draughts::getName(){
	return string("Draughts");
}


