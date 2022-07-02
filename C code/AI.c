#include <stdlib.h>
#include "AI.h"
#include "Piece.h"
#include <stdlib.h>
#include <stdio.h>

bool accept_positions(struct Piece current_best[8][8], struct Piece possible_best[8][8]) {
	int current_sum = 0;
	int possible_sum = 0;
	
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			current_sum += current_best[x][y].value;
		}
	}
	
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			possible_sum += possible_best[x][y].value;
		}
	}
	
	if (possible_sum > current_sum) {
		// If we can take a piece, do it
		return true;
	}
	else {
		return false;
	}
}

struct Move get_best_move(struct Board board) {
	struct Piece rand_piece;
	struct Board changed_board = board;
	struct Move best_move;
	best_move.start.y = 0;
	best_move.start.x = 0;
	best_move.end.y = 0;
	best_move.end.x = 0;
	struct Move rand_move;
	struct Position rand_pos;
	rand_pos.y = 0;
	rand_pos.x = 0;
	struct MoveList possible_moves;
	reset_move_list(&possible_moves);
	bool accept;
	bool valid_move;
	bool valid_piece;
	int rand_val;
	
	for (int i = 0; i < RUN_LEN; i++) {

		rand_piece.value = EMPTY;
		rand_piece.color = EMPTY;
		board_copy(board, &changed_board);
		// printf("Iteration: %d\n",i);

		valid_move = false;
		while(!valid_move) {
		valid_piece = false;
			while(!valid_piece) {
				rand_pos.x = rand() % 8;										//generate random number [0,7]
				rand_pos.y = rand() % 8;
				// printf("TEMP: X: %d\tY: %d\n", rand_pos.x, rand_pos.y);
				rand_piece = board.positions[rand_pos.y][rand_pos.x];
				// printf("i: %d\tX: %d\tY: %d\tPiece: %c\tPiece Color: %c\n", i, rand_pos.x, rand_pos.y, piece_names[rand_piece.value], color_names[rand_piece.color]);
				valid_piece = ((rand_piece.color == BLACK) && (rand_piece.value != EMPTY));
			}
			// printf("LOG: Rand Piece: %c%c\n", color_names[rand_piece.color] ,piece_names[rand_piece.value]);
			valid_move = generateMoves(rand_piece, rand_pos, board, &possible_moves);
		}
		// printf("LOG: Generated Random Move\n");
		
		rand_val = rand();
		rand_move = possible_moves.moves[rand_val % possible_moves.count];
		// printf("LOG: Get Move randval: %d\tModed: %d\n", rand_val, rand_val % possible_moves.count);

		movePiece(rand_move, &changed_board);
		// printf("LOG: Piece moved\n");

		accept = accept_positions(board.positions, changed_board.positions);
		// printf("LOG: Acceptance determined\n");
		if ((accept) || (i == 0)) {		//Make sure best_move has at least one move put into it
			//If we can take a piece, do it
			best_move = rand_move;
		}
		else {
			//Otherwise move to a good place
			int curr_pos_score;
			int new_pos_score;
			switch(rand_piece.value) {
				case PAWN_VAL:
					curr_pos_score = pawnTable[rand_move.start.y][rand_move.start.x];
					new_pos_score  = pawnTable[rand_move.end.y][rand_move.end.x];
					break;
				case ROOK_VAL:
					curr_pos_score = rookTable[rand_move.start.y][rand_move.start.x];
					new_pos_score  = rookTable[rand_move.end.y][rand_move.end.x];
					break;
				case KNIGHT_VAL:
					curr_pos_score = knightTable[rand_move.start.y][rand_move.start.x];
					new_pos_score  = knightTable[rand_move.end.y][rand_move.end.x];
					break;
				case BISHOP_VAL:
					curr_pos_score = bishopTable[rand_move.start.y][rand_move.start.x];
					new_pos_score  = bishopTable[rand_move.end.y][rand_move.end.x];
					break;
				case KING_VAL:
					curr_pos_score = kingTable[rand_move.start.y][rand_move.start.x];
					new_pos_score  = kingTable[rand_move.end.y][rand_move.end.x];
					break;
				case QUEEN_VAL:
					curr_pos_score = queenTable[rand_move.start.y][rand_move.start.x];
					new_pos_score  = queenTable[rand_move.end.y][rand_move.end.x];
					break;
				default:
					curr_pos_score = 0;
					new_pos_score  = 0;
					break;
			}
			if (new_pos_score > curr_pos_score) {
				best_move = rand_move;
			}
		}
		// printf("LOG: End Loop\n");
	}

	// printf("Start:\n\tX: %d\tY: %d\nEnd:\n\tX: %d\tY: %d\nPiece Val: %d\n", best_move.start.x, best_move.start.y, best_move.end.x, best_move.end.y, rand_piece.value);
	return best_move;
	
}

struct Move eval_move(struct Board old, struct Board new) {
	bool both_piece;
	bool both_empty;
	int x;
	int y;
	struct Move move;
	int first_x = 0;
	int second_x = 0;
	int first_y = 0;
	int second_y = 0;
	//Find first wrong position
	for (y = 0; y < 8; y++) {
		for (x = 0; x < 8; x++) {
			both_piece = ((old.positions[y][x].value > 0) && (new.positions[y][x].value > 0));
			both_empty = ((old.positions[y][x].value == 0) && (new.positions[y][x].value == 0));
			if (!(both_piece || both_empty)) {
				first_x = x;
				first_y = y;
				break;
			}
		}
		if (!(both_piece || both_empty)) {
			break;
		}
	}

	for (y = y; y < 8; y++) {
		for (x = x; x < 8; x++) {
			both_piece = ((old.positions[y][x].value > 0) && (new.positions[y][x].value > 0));
			both_empty = ((old.positions[y][x].value == 0) && (new.positions[y][x].value == 0));
			if (!(both_piece || both_empty)) {
				second_x = x;
				second_y = y;
				break;
			}
		}
		if (!(both_piece || both_empty)) {
			break;
		}
	}

	if (new.positions[first_y][first_x].value != EMPTY) {
		move.start.x = first_x;
		move.start.y = first_y;
		move.end.x = second_x;
		move.end.y = second_y;
	}
	else {
		move.start.x = second_x;
		move.start.y = second_y;
		move.end.x = first_x;
		move.end.y = first_y;
	}

	return move;
}
