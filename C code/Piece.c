#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "Piece.h"

bool reset_move_list(struct MoveList* returnMoves) {
	for( int i = 0; i < MAX_MOVE_COUNT; i++) {
		(*returnMoves).moves->capture = false;
		(*returnMoves).moves->start.x = 0;
		(*returnMoves).moves->start.y = 0;
		(*returnMoves).moves->end.x   = 0;
		(*returnMoves).moves->end.y   = 0;
	}
	(*returnMoves).count = 0;
	return true;
}

bool printMove(struct Move move) {
	printf("Start X: %d\tY: %d\n", move.start.x, move.start.y);
	printf("End   X: %d\tY: %d\n", move.end.x, move.end.y);
	// printf("Capture: %d\n", move.capture);
	return true;
}

/******************************PAWN******************************/
struct Piece Pawn(int new_color) {
	struct Piece pawn;
	pawn.value = PAWN_VAL;
	pawn.color = new_color;
	return pawn;
}
bool validateMove_Pawn(struct Move move, struct Board board) {
	// Out of bounds
	if ((move.end.x < 0) || (move.end.x > 7) || (move.end.y < 0) || (move.end.y > 7)) {
		// printf("LOG: Validating move Pawn Out of Bounds\n");
		return false;
	}

	// printf("LOG: Validating move Pawn\n");

	int direction;
	if (board.positions[move.start.y][move.start.x].color == WHITE) {
		direction = 1;
		//Moving the wrong direction
		if (move.end.y > move.start.y) {
			// printf("LOG: Validating move Pawn Wrong direction\n");
			return false;
		}
	}
	else {
		direction = -1;
		//Moving the wrong direction
		if (move.end.y < move.start.y) {
			// printf("LOG: Validating move Pawn Wrong direction\n");
			return false;
		}
	}

	struct Piece endpos = board.positions[move.end.y][move.end.x];

	//Moving straight forward
	if (move.end.x == move.start.x) {	
		if (abs(move.end.y - move.start.y) == 1) {
			if (endpos.value == EMPTY) {
				return true;
			}
			else {
				// printf("LOG: Validating move Pawn Can't capture moving forward\n");
				return false;
			}
		}
		else if(abs(move.end.y - move.start.y) == 2) {
			if (((move.start.y == 6) && (board.positions[move.start.y][move.start.x].color == WHITE)) || ((move.start.y == 1) && (board.positions[move.start.y][move.start.x].color == BLACK))) {
				if ((board.positions[move.start.y - direction][move.start.x].value == EMPTY) && (endpos.value == EMPTY)) {
					return true;
				}
				else {
					// printf("LOG: Validating move Pawn Double move can't capture\n");
					return false;
				}
			}
			else {
				// printf("LOG: Validating move Pawn can't double move off first row\n");
				return false;
			}			
		}
		else {
			// printf("LOG: Validating move Pawn can't move X: %d\tY: %d\n", move.end.x-move.start.x, move.end.y-move.start.y);
			return false;
		}
	}
	//Moving diagnoally
	else if ((abs(move.end.x - move.start.x) == 1) && (abs(move.end.y - move.start.y) == 1)) {
		//Must be capturing a piece
		if (endpos.value != EMPTY) {
			return true;
		}
		else {
			// printf("LOG: Validating move Pawn can't move diagonal and not capture\n");
			return false;
		}
	}
	else {
		// printf("LOG: Invalid Move X: %d\tY: %d\n", move.end.x, move.end.y);
		return false;
	}
}

bool generateMoves_Pawn(struct Position start, struct Board board, struct MoveList* returnMoves) {
	int step;
	struct MoveList potentialMoves;
	reset_move_list(&potentialMoves);

	// printf("LOG: Generating Move Pawn\n");
	
	//Check for double move forward
	if (board.positions[start.y][start.x].color == BLACK) {
		if (start.y == 0) {										//check if we're at the edge of the board, no valid moves
			return true;
		}
		if (start.y == 6) {
			//double move if the pawn is still on the starting line
			potentialMoves.moves[potentialMoves.count].start.x = start.x;
			potentialMoves.moves[potentialMoves.count].start.y = start.y;
			potentialMoves.moves[potentialMoves.count].end.x = start.x;
			potentialMoves.moves[potentialMoves.count].end.y = start.y - 2;
			potentialMoves.moves[potentialMoves.count].capture = false;
			// printf("LOG: Adding potential move double step forward\n");
			potentialMoves.count++;
		}
		step = 1;
	}
	else {
		if (start.y == 7) {										//check if we're at the edge of the board, no valid moves
			return true;
		}
		if (start.y == 1) {
			//double move if the pawn is still on the starting line	
			potentialMoves.moves[potentialMoves.count].start.x = start.x;
			potentialMoves.moves[potentialMoves.count].start.y = start.y;
			potentialMoves.moves[potentialMoves.count].end.x = start.x;
			potentialMoves.moves[potentialMoves.count].end.y = start.y + 2;
			potentialMoves.moves[potentialMoves.count].capture = false;
			// printf("LOG: Adding potential move double step forward\n");			
			potentialMoves.count++;
		}
		step = -1;
	}
	
	//single step forward
	struct Move newmove;
	potentialMoves.moves[potentialMoves.count].start.x = start.x;
	potentialMoves.moves[potentialMoves.count].start.y = start.y;
	potentialMoves.moves[potentialMoves.count].end.x = start.x;
	potentialMoves.moves[potentialMoves.count].end.y = start.y + step;
	potentialMoves.moves[potentialMoves.count].capture = false;
	// printf("LOG: Adding potential move single step forward\n");
	potentialMoves.count++;
	
	//check if can take a piece
	if (start.x > 0) {			//make sure we're not going to fall off the side of the board
		if (board.positions[start.y + step][start.x - 1].value != EMPTY) {		//this is confusing but it's checking the diagonal for a blank space or not
			potentialMoves.moves[potentialMoves.count].start.x = start.x;
			potentialMoves.moves[potentialMoves.count].start.y = start.y;
			potentialMoves.moves[potentialMoves.count].end.x = start.x - 1;
			potentialMoves.moves[potentialMoves.count].end.y = start.y + step;
			potentialMoves.moves[potentialMoves.count].capture = true;
			// printf("LOG: Adding potential move sideways step, capturing: %c\n", piece_names[board.positions[start.y + step][start.x - 1].value]);
			potentialMoves.count++;
		}
	}
	if (start.x < 7) {
		if (board.positions[start.y + step][start.x + 1].value != EMPTY) {		//this is confusing but it's checking the diagonal for a blank space or not
			potentialMoves.moves[potentialMoves.count].start.x = start.x;
			potentialMoves.moves[potentialMoves.count].start.y = start.y;
			potentialMoves.moves[potentialMoves.count].end.x = start.x + 1;
			potentialMoves.moves[potentialMoves.count].end.y = start.y + step;
			potentialMoves.moves[potentialMoves.count].capture = true;
			// printf("LOG: Adding potential move sideways step, capturing: %c\n", piece_names[board.positions[start.y + step][start.x + 1].value]);
			potentialMoves.count++;
		}
	}

	for (int m = 0; m < potentialMoves.count; m++) {
		if (validateMove_Pawn(potentialMoves.moves[m], board)){
			returnMoves->moves[returnMoves->count] = potentialMoves.moves[m];
			returnMoves->count++;
		}
	}
	
	// printf("LOG: Number of moves generated Pawn: %d\n", returnMoves->count);
	
	return returnMoves->count;
}

/******************************KNIGHT******************************/
struct Piece Knight(int new_color) {
	struct Piece knight;
	knight.value = KNIGHT_VAL;
	knight.color = new_color;
	return knight;
}

bool validateMove_Knight(struct Move move, struct Board board) {
	if ((move.end.x < 0) || (move.end.x > 7) || (move.end.y < 0) || (move.end.y > 7)) {
		// printf("LOG: Validating Knight Out of Bounds\n");
		return false;		//out of bounds
	}

	// printf("LOG: Validating move Knight\n");
	
	int xdiff = abs(move.start.x - move.end.x);
	int ydiff = abs(move.start.y - move.end.y);
	
	if ((xdiff == 0) || (ydiff == 0)) {					//both need to be 1 or 2
		// printf("LOG: Validating move Knigth: Invalid move X: %d\t Y: %d\n", xdiff, ydiff);
		return false;
	}
	else {
		if (xdiff == 2) {														//one must be two, and the other must be one
			if (ydiff == 1) {
				if (board.positions[move.start.y][move.start.x].color == board.positions[move.end.y][move.end.x].color) {
					// printf("LOG: Validing Move Knight ending on same color\n");
					// printMove(move);
					return false;
				}
				else {
					return true;
				}
			}
			else {
			// printf("LOG: Validating move Knigth: Invalid move X: %d\t Y: %d\n", xdiff, ydiff);
				return false;
			}
		}
		else if (ydiff == 2) {
			if (xdiff == 1) {
				if (board.positions[move.start.y][move.start.x].color == board.positions[move.end.y][move.end.x].color) {
					// printf("LOG: Validing Move Knight ending on same color\n");
					// printMove(move);
					return false;
				}
				else {
					return true;
				}
			}
			else {
			// printf("LOG: Validating move Knigth: Invalid move X: %d\t Y: %d\n", xdiff, ydiff);
				return false;
			}
		}
		else {
			// printf("LOG: Validating move Knigth: Invalid move X: %d\t Y: %d\n", xdiff, ydiff);
			return false;
		}
	}
	
}

bool generateMoves_Knight(struct Position start, struct Board board, struct MoveList* returnMoves){
	// printf("LOG: Generating Moves Knight\n");
	struct MoveList potentialMoves;
	reset_move_list(&potentialMoves);

	//move up 2
	if (start.y < 6) {
		//Move right 1
		if (start.x < 7) {
			struct Move newmove;
			newmove.start.x = start.x;
			newmove.start.y = start.y;
			newmove.end.x = start.x + 1;
			newmove.end.y = start.y - 2;
			if (board.positions[newmove.end.y][newmove.end.x].value != EMPTY) {
				newmove.capture = true;
			}
			else {
				newmove.capture = false;
			}
			// printf("LOG: Add potential move KNIGHT UP2 RIGHT1\n");
			potentialMoves.moves[potentialMoves.count] = newmove;
			potentialMoves.count++;
		}
		//move left 1
		if (start.x > 1) {
			struct Move newmove;
			newmove.start.x = start.x;
			newmove.start.y = start.y;
			newmove.end.x = start.x - 1;
			newmove.end.y = start.y - 2;
			if (board.positions[newmove.end.y][newmove.end.x].value != EMPTY) {
				newmove.capture = true;
			}
			else {
				newmove.capture = false;
			}
			// printf("LOG: Add potential move KNIGHT UP2 LEFT1\n");
			potentialMoves.moves[potentialMoves.count] = newmove;
			potentialMoves.count++;		
		}
	}	
	
	//move down 2
	if (start.y > 2) {
		//Move right 1
		if (start.x < 7) {
			struct Move newmove;
			newmove.start.x = start.x;
			newmove.start.y = start.y;
			newmove.end.x = start.x + 1;
			newmove.end.y = start.y + 2;
			if (board.positions[newmove.end.y][newmove.end.x].value != EMPTY) {
				newmove.capture = true;
			}
			else {
				newmove.capture = false;
			}
			// printf("LOG: Add potential move KNIGHT DOWN2 RIGHT1\n");
			potentialMoves.moves[potentialMoves.count] = newmove;
			potentialMoves.count++;
		}
		//move left 1
		if (start.x > 1) {
			struct Move newmove;
			newmove.start.x = start.x;
			newmove.start.y = start.y;
			newmove.end.x = start.x - 1;
			newmove.end.y = start.y + 2;
			if (board.positions[newmove.end.y][newmove.end.x].value != EMPTY) {
				newmove.capture = true;
			}
			else {
				newmove.capture = false;
			}
			// printf("LOG: Add potential move KNIGHT UP2 LEFT1\n");
			potentialMoves.moves[potentialMoves.count] = newmove;
			potentialMoves.count++;			
		}
	}
	
	//move right 2
	if (start.x < 6) {
		//move up 1
		if (start.y < 7) {
			struct Move newmove;
			newmove.start.x = start.x;
			newmove.start.y = start.y;
			newmove.end.x = start.x + 2;
			newmove.end.y = start.y - 1;
			if (board.positions[newmove.end.y][newmove.end.x].value != EMPTY) {
				newmove.capture = true;
			}
			else {
				newmove.capture = false;
			}
			// printf("LOG: Add potential move KNIGHT RIGHT2 UP1\n");
			potentialMoves.moves[potentialMoves.count] = newmove;
			potentialMoves.count++;
		}
		//move down 1
		if (start.y > 1) {
			struct Move newmove;
			newmove.start.x = start.x;
			newmove.start.y = start.y;
			newmove.end.x = start.x + 2;
			newmove.end.y = start.y + 1;
			if (board.positions[newmove.end.y][newmove.end.x].value != EMPTY) {
				newmove.capture = true;
			}
			else {
				newmove.capture = false;
			}
			// printf("LOG: Add potential move KNIGHT RIGHT2 DOWN1\n");
			potentialMoves.moves[potentialMoves.count] = newmove;
			potentialMoves.count++;
		}
	}
	
	//move left 2
	if (start.x > 2) {
		//move up 1
		if (start.y < 7) {
			struct Move newmove;
			newmove.start.x = start.x;
			newmove.start.y = start.y;
			newmove.end.x = start.x - 2;
			newmove.end.y = start.y - 1;
			if (board.positions[newmove.end.y][newmove.end.x].value != EMPTY) {
				newmove.capture = true;
			}
			else {
				newmove.capture = false;
			}
			// printf("LOG: Add potential move KNIGHT LEFT2 UP1\n");
			potentialMoves.moves[potentialMoves.count] = newmove;
			potentialMoves.count++;
		}
		//move down 1
		if (start.y > 1) {
			struct Move newmove;
			newmove.start.x = start.x;
			newmove.start.y = start.y;
			newmove.end.x = start.x - 2;
			newmove.end.y = start.y + 1;
			if (board.positions[newmove.end.y][newmove.end.x].value != EMPTY) {
				newmove.capture = true;
			}
			else {
				newmove.capture = false;
			}
			// printf("LOG: Add potential move KNIGHT LEFT2 DOWN1\n");
			potentialMoves.moves[potentialMoves.count] = newmove;
			potentialMoves.count++;
		}
	}

	for (int m = 0; m < potentialMoves.count; m++) {
		if (validateMove_Knight(potentialMoves.moves[m], board)){
			returnMoves->moves[returnMoves->count] = potentialMoves.moves[m];
			returnMoves->count++;
		}
	}
	
	// printf("LOG: Number of moves generated Knight: %d\n", returnMoves->count);
	
	return returnMoves->count;
}

/******************************BISHOP******************************/
struct Piece Bishop(int new_color) {
	struct Piece bishop;
	bishop.value = BISHOP_VAL;
	bishop.color = new_color;
	return bishop;
}

bool validateMove_Bishop(struct Move move, struct Board board) {
	if ((move.end.x < 0) || (move.end.x > 7) || (move.end.y < 0) || (move.end.y > 7)) {
		// printf("LOG: Validating move Bishop Out of Bounds\n");
		return false;		//out of bounds
	}
	
	int xdiff = abs(move.start.x - move.end.x);
	int ydiff = abs(move.start.y - move.end.y);
	int ydir = 1; 
	int xdir = 1;
	if (move.start.y > move.end.y) {
		ydir = -1;
	}
	if (move.start.x > move.end.x) {
		xdir = -1;
	}

	// printf("LOG: Validating move Bishop Ydir: %d\t Xdir: %d\n", ydir, xdir);
	
	if (xdiff == ydiff) {
		//Not passing through pieces
		for (int d = 1; d < ydiff; d++) {
			if (board.positions[move.start.y + (d * ydir)][move.start.x + (d * xdir)].value != EMPTY) {
				// printf("LOG: Validating move Bishop INVALID: passing through piece at X: %d\tY: %d\n",move.end.x + (d * xdir), move.end.y + (d * ydir));
				// printMove(move);
				return false;
			}
		}
		if (board.positions[move.start.y][move.start.x].color == board.positions[move.end.y][move.end.x].color) {
			// printf("LOG: Validating move Bishop ending on same color\n");
			// printMove(move);
			return false;
		}
		else {
			return true;
		}
	}
	else {
		// printf("LOG: Validating move Bishop not moving diagonally\n");
		return false;
	}
}
bool generateMoves_Bishop(struct Position start, struct Board board, struct MoveList* returnMoves) {
	struct MoveList potentialMoves;
	reset_move_list(&potentialMoves);

	//Down Left
	int dist = start.x;
	if (start.y < start.x) {
		dist = start.y;
	}
	for (int j = 1; j <= dist; j++) {
		struct Move newmove;
		newmove.start.x = start.x;
		newmove.start.y = start.y;
		newmove.end.x = start.x - j;
		newmove.end.y = start.y + j;
		if (board.positions[newmove.end.y][newmove.end.x].value != EMPTY) {
			newmove.capture = true;
		}
		else {
			newmove.capture = false;
		}
		// printf("LOG: Added potential move Bishop: DOWN LEFT %d\n", j);
		potentialMoves.moves[potentialMoves.count] = newmove;
		potentialMoves.count++;
		if (board.positions[newmove.end.y][newmove.end.x].value != EMPTY) {
			break;
		}
	}
	
	//Down Right
	dist = 7 - start.x;
	if (start.y < dist) {
		dist = start.y;
	}
	for (int j = 1; j <= dist; j++) {
		struct Move newmove;
		newmove.start.x = start.x;
		newmove.start.y = start.y;
		newmove.end.x = start.x + j;
		newmove.end.y = start.y + j;
		if (board.positions[newmove.end.y][newmove.end.x].value != EMPTY) {
			newmove.capture = true;
		}
		else {
			newmove.capture = false;
		}
		// printf("LOG: Added potential move Bishop: DOWN RIGHT %d\n", j);
		potentialMoves.moves[potentialMoves.count] = newmove;
		potentialMoves.count++;
		if (board.positions[newmove.end.y][newmove.end.x].value != EMPTY) {
			break;
		}			
	}
	
	//Up Left
	dist = start.x;
	if ((7 - start.y) < start.x) {
		dist = 7 - start.y;
	}
	for (int j = 1; j <= dist; j++) {
		struct Move newmove;
		newmove.start.x = start.x;
		newmove.start.y = start.y;
		newmove.end.x = start.x - j;
		newmove.end.y = start.y - j;
		if (board.positions[newmove.end.y][newmove.end.x].value != EMPTY) {
			newmove.capture = true;
		}
		else {
			newmove.capture = false;
		}
		// printf("LOG: Added potential move Bishop: UP LEFT %d\n", j);
		potentialMoves.moves[potentialMoves.count] = newmove;
		potentialMoves.count++;
		if (board.positions[newmove.end.y][newmove.end.x].value != EMPTY) {
			break;
		}
	}
	
	//Up Right
	dist = 7 - start.x;
	if ((7 - start.y) < dist) {
		dist = 7 - start.y;
	}
	for (int j = 1; j <= dist; j++) {
		struct Move newmove;
		newmove.start.x = start.x;
		newmove.start.y = start.y;
		newmove.end.x = start.x + j;
		newmove.end.y = start.y - j;
		if (board.positions[newmove.end.y][newmove.end.x].value != EMPTY) {
			newmove.capture = true;
		}
		else {
			newmove.capture = false;
		}
		// printf("LOG: Added potential move Bishop: UP RIGHT %d\n", j);
		potentialMoves.moves[potentialMoves.count] = newmove;
		potentialMoves.count++;
		if (board.positions[newmove.end.y][newmove.end.x].value != EMPTY) {
			break;
		}
	}

	for (int m = 0; m < potentialMoves.count; m++) {
		if (validateMove_Bishop(potentialMoves.moves[m], board)){
			returnMoves->moves[returnMoves->count] = potentialMoves.moves[m];
			returnMoves->count++;
		}
	}
	
	// printf("LOG: Number of moves generated Bishop: %d\n", returnMoves->count);
	
	return returnMoves->count;
}

/******************************ROOK******************************/
struct Piece Rook(int new_color) {
	struct Piece rook;
	rook.value = ROOK_VAL;
	rook.color = new_color;
	return rook;
}

bool validateMove_Rook(struct Move move, struct Board board) {
	if ((move.end.x < 0) || (move.end.x > 7) || (move.end.y < 0) || (move.end.y > 7)) {
		// printf("LOG: Validing Move Rook Out of Bounds\n");
		return false;		//out of bounds
	}
	
	int xdiff = abs(move.start.x - move.end.x);
	int ydiff = abs(move.start.y - move.end.y);
	int ydir = 1;
	int xdir = 1;
	if (move.start.y > move.end.y) {
		ydir = -1;
	}
	if (move.start.x > move.end.x) {
		xdir = -1;
	}

	// printf("LOG: Validing Move Rook ydir: %d\txdir: %d\n", ydir, xdir);
	
	//The piece has to move
	if ((xdiff == 0) && (ydiff == 0)) {
		return false;
	}
	//Moving up and down
	else if (xdiff == 0) {
		for (int y = 1; y < ydiff; y++) {
			if (board.positions[move.start.y + (y * ydir)][move.start.x].value != EMPTY) {
				// printf("LOG: Validating Move Rook Passing through piece vertical Y: %d\tX: %d\n", move.start.y + (y * ydir), move.end.x);
				return false;
			}
			// else {
			// 	printf("LOG: Validating Move Rook value at intermediate value Y: %d X: %d Piece: %c%c\n", move.start.y + (y * ydir), move.start.x, color_names[board.positions[move.start.y + (y * ydir)][move.start.x].color], piece_names[board.positions[move.start.y + (y * ydir)][move.start.x].value]);
			// }
		}
		if (board.positions[move.end.y][move.end.x].color == board.positions[move.start.y][move.start.x].color) {
			// printf("LOG: Validating Move Rook Capturing piece of same color\n");
			// printMove(move);
			return false;
		}
		else {
			return true;
		}
	}
	//Moving side to side
	else if (ydiff == 0) {
		for (int x = 1; x < xdiff; x++) {
			if (board.positions[move.end.y][move.end.x + (x * xdir)].value != EMPTY) {
				// printf("LOG: Validating Move Rook Passing through piece horizontal Y: %d\tX: %d\n", move.end.y, move.start.x + (x * xdir));
				return false;
			}
		}
		if (board.positions[move.end.y][move.end.x].color == board.positions[move.start.y][move.start.x].color) {
			// printf("LOG: Validating Move Rook Capturing piece of same color\n");
			// printMove(move);
			return false;
		}
		else {
			return true;
		}
	}
	else {
		// printf("LOG: Validating Move Rook X and Y both non zero\n");
		return false;
	}
}

bool generateMoves_Rook(struct Position start, struct Board board, struct MoveList* returnMoves) {
	struct MoveList potentialMoves;
	reset_move_list(&potentialMoves);

	//Up
	int dist = start.y;
	for (int j = 1; j <= dist; j++) {
		struct Move newmove;
		newmove.start.x = start.x;
		newmove.start.y = start.y;
		newmove.end.x = start.x;
		newmove.end.y = start.y - j;
		if (board.positions[newmove.end.y][newmove.end.x].value != EMPTY) {
			newmove.capture = true;
		}
		else {
			newmove.capture = false;
		}
		// printf("LOG: Add potential move: ROOK UP %d\n", j);
		potentialMoves.moves[potentialMoves.count] = newmove;
		potentialMoves.count++;
		if (board.positions[newmove.end.x][newmove.end.y].value != EMPTY) {
			break;
		}
	}
	
	//Down
	dist = 7 - start.y;
	for (int j = 1; j <= dist; j++) {
		struct Move newmove;
		newmove.start.x = start.x;
		newmove.start.y = start.y;
		newmove.end.x = start.x;
		newmove.end.y = start.y + j;
		if (board.positions[newmove.end.y][newmove.end.x].value != EMPTY) {
			newmove.capture = true;
		}
		else {
			newmove.capture = false;
		}
		// printf("LOG: Add potential move: ROOK DOWN %d\n", j);
		potentialMoves.moves[potentialMoves.count] = newmove;
		potentialMoves.count++;
		if (board.positions[newmove.end.y][newmove.end.x].value != EMPTY) {
			break;
		}
	}
	
	//Right
	dist = 7 - start.x;
	for (int j = 1; j <= dist; j++) {
		struct Move newmove;
		newmove.start.x = start.x;
		newmove.start.y = start.y;
		newmove.end.x = start.x + j;
		newmove.end.y = start.y;
		if (board.positions[newmove.end.y][newmove.end.x].value != EMPTY) {
			newmove.capture = true;
		}
		else {
			newmove.capture = false;
		}
		// printf("LOG: Add potential move: ROOK RIGHT %d\n", j);
		potentialMoves.moves[potentialMoves.count] = newmove;
		potentialMoves.count++;
		if (board.positions[newmove.end.y][newmove.end.x].value != EMPTY) {
			break;
		}
	}
	
	//Left
	dist = start.x;
	for (int j = 1; j <= dist; j++) {
		struct Move newmove;
		newmove.start.x = start.x;
		newmove.start.y = start.y;
		newmove.end.x = start.x - j;
		newmove.end.y = start.y;
		if (board.positions[newmove.end.y][newmove.end.x].value != EMPTY) {
			newmove.capture = true;
		}
		else {
			newmove.capture = false;
		}
		// printf("LOG: Add potential move: ROOK LEFT %d\n", j);
		potentialMoves.moves[potentialMoves.count] = newmove;
		potentialMoves.count++;
		if (board.positions[newmove.end.y][newmove.end.x].value != EMPTY) {
			break;
		}
	}

	for (int m = 0; m < potentialMoves.count; m++) {
		if (validateMove_Rook(potentialMoves.moves[m], board)){
			returnMoves->moves[returnMoves->count] = potentialMoves.moves[m];
			returnMoves->count++;
		}
	}
	
	// printf("LOG: Number of moves generated Rook: %d\n", returnMoves->count);
	
	return returnMoves->count;
}

/******************************QUEEN******************************/
struct Piece Queen(int new_color) {
	struct Piece queen;
	queen.value = QUEEN_VAL;
	queen.color = new_color;
	return queen;
}
bool validateMove_Queen(struct Move move, struct Board board) {
	// printf("LOG: Validating Queen\n");
	return ((validateMove_Bishop(move, board)) || (validateMove_Rook(move, board)));
}

bool generateMoves_Queen(struct Position start, struct Board board, struct MoveList* returnMoves) {
	
	// printf("LOG: Generating Move Queen: Rook\n");
	generateMoves_Rook(start, board, returnMoves);
	// printf("LOG: Number of moves generated for Queen from Rook: %d\n", returnMoves->count);

	// printf("LOG: Generating Move Queen: Bishop\n");
	generateMoves_Bishop(start, board, returnMoves);
	
	// printf("LOG: Number of moves generated Queen: %d\n", returnMoves->count);
	
	return returnMoves->count;
}

/******************************KING******************************/
struct Piece King(int new_color) {
	struct Piece king;
	king.value = KING_VAL;
	king.color = new_color;
	return king;
}

bool validateMove_King(struct Move move, struct Board board) {
	if ((move.end.x < 0) || (move.end.x > 7) || (move.end.y < 0) || (move.end.y > 7)) {
		// printf("LOG: Validating King: Out of Bounds\n");
		return false;		//out of bounds
	}
	
	int xdiff = abs(move.start.x - move.end.x);
	int ydiff = abs(move.start.y - move.end.y);
	
	if (((xdiff == 0) || (xdiff == 1)) && ((ydiff == 0) || (ydiff == 1))) {
		// printf("LOG: Validating King: Good distance, need to check check\n");
		// return !check(board, board.positions[move.start.y][move.start.x].color);
		if (board.positions[move.start.y][move.start.x].color == board.positions[move.end.y][move.end.x].color) {
			// printf("LOG: Validing King ending at same color\n");
			// printMove(move);
			return false;
		}
		else {
			return true;
		}
	}
	else {
		return false;
	}
}

bool generateMoves_King(struct Position start, struct Board board, struct MoveList* returnMoves) {
	// printf("LOG: Generating Random Move King\n");
	struct MoveList potentialMoves;
	reset_move_list(&potentialMoves);
	
	//Up
	if (start.x < 7) {
		//Up
		struct Move newmove;
		newmove.start.x = start.x;
		newmove.start.y = start.y;
		newmove.end.x = start.x + 1;
		newmove.end.y = start.y;
		if (board.positions[newmove.end.y][newmove.end.x].value != EMPTY) {
			newmove.capture = true;
		}
		else {
			newmove.capture = false;			
		}
		// printf("LOG: Added potential move King: UP UP\n");
		potentialMoves.moves[potentialMoves.count] = newmove;
		potentialMoves.count++;
		//Left
		if (start.y > 0) {
			struct Move diagmove;
			diagmove.start.x = start.x;
			diagmove.start.y = start.y;
			diagmove.end.x = start.x + 1;
			diagmove.end.y = start.y + 1;
			if (board.positions[diagmove.end.y][diagmove.end.x].value != EMPTY) {
				diagmove.capture = true;
			}
			else {
				diagmove.capture = false;
			}
			// printf("LOG: Added potential move King: UP Left\n");
			potentialMoves.moves[potentialMoves.count] = newmove;
			potentialMoves.count++;
		}
		//Right
		if (start.y < 7) {
			struct Move diagmove;
			diagmove.start.x = start.x;
			diagmove.start.y = start.y;
			diagmove.end.x = start.x + 1;
			diagmove.end.y = start.y - 1;
			if (board.positions[diagmove.end.y][diagmove.end.x].value != EMPTY) {
				diagmove.capture = true;
			}
			else {
				diagmove.capture = false;
			}
			// printf("LOG: Added potential move King: UP RIGHT\n");
			potentialMoves.moves[potentialMoves.count] = newmove;
			potentialMoves.count++;
		}
	}
	//Down
	if (start.x > 0) {
		//Down
		struct Move newmove;
		newmove.start.x = start.x;
		newmove.start.y = start.y;
		newmove.end.x = start.x - 1;
		newmove.end.y = start.y;
		if (board.positions[newmove.end.y][newmove.end.x].value != EMPTY) {
			newmove.capture = true;
		}
		else {
			newmove.capture = false;			
		}
		// printf("LOG: Added potential move King: DOWN DOWN\n");
		potentialMoves.moves[potentialMoves.count] = newmove;
		potentialMoves.count++;
		//Left
		if (start.y > 0) {
			struct Move diagmove;
			diagmove.start.x = start.x;
			diagmove.start.y = start.y;
			diagmove.end.x = start.x + 1;
			diagmove.end.y = start.y - 1;
			if (board.positions[diagmove.end.y][diagmove.end.x].value != EMPTY) {
				diagmove.capture = true;
			}
			else {
				diagmove.capture = false;
			}
			// printf("LOG: Added potential move King: DOWN LEFT\n");
			potentialMoves.moves[potentialMoves.count] = newmove;
			potentialMoves.count++;
		}
		//Right
		if (start.y < 7) {
		struct Move diagmove;
		diagmove.start.x = start.x;
		diagmove.start.y = start.y;
		diagmove.end.x = start.x - 1;
		diagmove.end.y = start.y - 1;
		if (board.positions[diagmove.end.y][diagmove.end.x].value != EMPTY) {
			diagmove.capture = true;
		}
		else {
			diagmove.capture = false;
		}
			// printf("LOG: Added potential move King: DOWN RIGHT\n");
			potentialMoves.moves[potentialMoves.count] = newmove;
			potentialMoves.count++;
		}
	}
	//Left
	if (start.y > 0) {
		struct Move newmove;
		newmove.start.x = start.x;
		newmove.start.y = start.y;
		newmove.end.x = start.x;
		newmove.end.y = start.y + 1;
		if (board.positions[newmove.end.y][newmove.end.x].value != EMPTY) {
			newmove.capture = true;
		}
		else {
			newmove.capture = false;			
		}
		// printf("LOG: Added potential move King: LEFT\n");
		potentialMoves.moves[potentialMoves.count] = newmove;
		potentialMoves.count++;
	}
	//Right
	if (start.y < 7) {
		struct Move newmove;
		newmove.start.x = start.x;
		newmove.start.y = start.y;
		newmove.end.x = start.x;
		newmove.end.y = start.y - 1;
		if (board.positions[newmove.end.y][newmove.end.x].value != EMPTY) {
			newmove.capture = true;
		}
		else {
			newmove.capture = false;			
		}
		// printf("LOG: Added potential move King: RIGHT\n");
		potentialMoves.moves[potentialMoves.count] = newmove;
		potentialMoves.count++;
	}

	for (int m = 0; m < potentialMoves.count; m++) {
		if (validateMove_King(potentialMoves.moves[m], board)){
			returnMoves->moves[returnMoves->count] = potentialMoves.moves[m];
			returnMoves->count++;
		}
	}

	// printf("LOG: Number of moves generated King: %d\n", returnMoves->count);
	
	return returnMoves->count;
}

/**************************Wrapper Functions for ease of use*****************************/
bool validateMove(struct Piece piece, struct Move move, struct Board board) {
	switch(piece.value) {
		case PAWN_VAL:
			// printf("LOG: Validating Move Pawn\n");
			return validateMove_Pawn(move, board);
		case ROOK_VAL:
			// printf("LOG: Validating Move Rook\n");
			return validateMove_Rook(move, board);
		case KNIGHT_VAL:
			// printf("LOG: Validating Move Knight\n");
			return validateMove_Knight(move, board);
		case BISHOP_VAL:
			// printf("LOG: Validating Move Bishop\n");
			return validateMove_Bishop(move, board);
		case KING_VAL:
			// printf("LOG: Validating Move King\n");
			return validateMove_King(move, board);
		case QUEEN_VAL:
			// printf("LOG: Validating Move Queen\n");
			return validateMove_Queen(move, board);
		default:
			return false;
	}
}

bool generateMoves(struct Piece piece, struct Position start, struct Board board, struct MoveList* possible_moves) {
	switch (piece.value) {
		case PAWN_VAL:
			// printf("LOG: Generate Move Case Pawn\n");
			return generateMoves_Pawn(start, board, possible_moves);
		case ROOK_VAL:
			// printf("LOG: Generate Move Case Rook\n");
			return generateMoves_Rook(start, board, possible_moves);
		case KNIGHT_VAL:
			// printf("LOG: Generate Move Case Knight\n");
			return generateMoves_Knight(start, board, possible_moves);
		case BISHOP_VAL:
			// printf("LOG: Generate Move Case Bishop\n");
			return generateMoves_Bishop(start, board, possible_moves);
		case KING_VAL:
			// printf("LOG: Generate Move Case King\n");
			return generateMoves_King(start, board, possible_moves);
		case QUEEN_VAL:
			// printf("LOG: Generate Move Case Queen\n");
			return generateMoves_Queen(start, board, possible_moves);
		default:
			return false;
	}
}

bool reset_board(struct Board* board) {
	//set up an empty board
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			(*board).positions[y][x].value = EMPTY;
			(*board).positions[y][x].color = EMPTY;
		}
	}
	
	//set Pawns
	for (int x = 0; x < 8; x++) {
		(*board).positions[1][x] = Pawn(BLACK);
		(*board).positions[6][x] = Pawn(WHITE);
	}
	//set Rooks
	(*board).positions[0][0] = Rook(BLACK);
	(*board).positions[0][7] = Rook(BLACK);
	(*board).positions[7][0] = Rook(WHITE);
	(*board).positions[7][7] = Rook(WHITE);
	
	//set Knights
	(*board).positions[0][1] = Knight(BLACK);
	(*board).positions[0][6] = Knight(BLACK);
	(*board).positions[7][1] = Knight(WHITE);
	(*board).positions[7][6] = Knight(WHITE);
	
	//set Bishops
	(*board).positions[0][2] = Bishop(BLACK);
	(*board).positions[0][5] = Bishop(BLACK);
	(*board).positions[7][2] = Bishop(WHITE);
	(*board).positions[7][5] = Bishop(WHITE);
	
	//set Queens
	(*board).positions[0][3] = Queen(BLACK);
	(*board).positions[7][3] = Queen(WHITE);
	
	//set Kings
	(*board).positions[0][4] = King(BLACK);
	(*board).positions[7][4] = King(WHITE);
	
	return true;
}

bool board_copy(struct Board oldboard, struct Board *newboard) {
	(*newboard).BLACK_CHECK = oldboard.BLACK_CHECK;
	(*newboard).WHITE_CHECK = oldboard.WHITE_CHECK;
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			(*newboard).positions[y][x].value = oldboard.positions[y][x].value;
			(*newboard).positions[y][x].color = oldboard.positions[y][x].color;
		}
	}
	return true;
}

bool movePiece(struct Move move, struct Board *board) {
	struct Piece blank;
	blank.color = EMPTY;
	blank.value = EMPTY;
	
	(*board).positions[move.end.y][move.end.x] = (*board).positions[move.start.y][move.start.x];
	(*board).positions[move.start.y][move.start.x] = blank;

	if ((move.end.y == 0) && (board->positions[move.end.y][move.end.x].color == WHITE) && (board->positions[move.end.y][move.end.x].value == PAWN_VAL)) {
		board->positions[move.end.y][move.end.x].value = QUEEN_VAL;
	}
	else if ((move.end.y == 7) && (board->positions[move.end.y][move.end.x].color == BLACK) && (board->positions[move.end.y][move.end.x].value == PAWN_VAL)) {
		board->positions[move.end.y][move.end.x].value = QUEEN_VAL;
	}
	
	return true;
}

bool check(struct Board board, int in_color) {
	//TODO: actually check if the <color> king is in check
	// Doesn't check for checkmate
	struct MoveList possible_moves;
	reset_move_list(&possible_moves);
	struct Position pos;
	struct Piece currpiece;
	struct Position stop;
	struct Piece endpiece;
	for (pos.y = 0; pos.y < 8; pos.y++) {
		for (pos.x = 0; pos.x < 8; pos.x++) {
			currpiece = board.positions[pos.y][pos.x];
			if (currpiece.color == in_color) {
				generateMoves(currpiece, pos, board, &possible_moves);
				// if ((currpiece.color == WHITE) && (currpiece.value == PAWN_VAL)) {
				// 	printf("Move count: %d\n", possible_moves.count);
				// 	for (int i = 0; i < possible_moves.count; i++) {
				// 		printMove(possible_moves.moves[i]);
				// 	}
				// }
				int move_count = possible_moves.count;
				for (int m = 0; m < move_count; m++) {
					stop = possible_moves.moves[m].end;
					endpiece = board.positions[stop.y][stop.x];
					// if ((currpiece.color == WHITE) && (currpiece.value == ROOK_VAL)) {
					// 	printf("Potential piece: %c\n", piece_names[endpiece.value]);
					// }
					// printf("Here\n");
					if ((endpiece.value == KING_VAL) && (endpiece.color != in_color)) {		//If it's possible for any piece to move to the King
						struct Position start = possible_moves.moves[m].start;
						struct Piece startpiece = board.positions[start.y][start.x];
						// printf("%i King in check by %i %i", in_color, startpiece.value);
						return true;																																										//	Check
					}
				}
			}
		}
	}
	// printf("Not check\n");
	return false;
}

bool check_mate(struct Board board, int color) {
	int x;
	int y;
	for (x = 0; x < 8; x++) {
		for (y = 0; y < 8; y++) {
			if ((board.positions[x][y].value == KING_VAL) && (board.positions[x][y].color == color)) {
				struct Position kingpos;
				kingpos.x = x;
				kingpos.y = y;
				return check_mate_help(color, kingpos, board);
			}
		}
	}
	return true;
}

bool check_mate_help(int color, struct Position king_pos, struct Board board) {
	struct MoveList possible_moves;
	reset_move_list(&possible_moves);
	struct Board newboard;
	generateMoves_King(king_pos, board, &possible_moves);
	for (int n = 0; n < MAX_MOVE_COUNT; n++) {
		board_copy(board, &newboard);
		movePiece(possible_moves.moves[n], &board);
		if (!check(newboard, color)) {													//If there's ever a point where I'm not in check, its not check mate
			return false;
		}
	}
	return false;
}
