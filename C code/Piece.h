
#pragma once
#include <stdint.h>
#include <stdbool.h>

#define MAX_MOVE_COUNT 30
#define KING_VAL 10
#define QUEEN_VAL 9
#define ROOK_VAL 5
#define BISHOP_VAL 3
#define KNIGHT_VAL 4
#define PAWN_VAL 1
#define EMPTY 0
#define WHITE 1
#define BLACK 2

static char piece_names[11] = {' ', 'P', '-', 'B', 'N', 'R', '-', '-', '-', 'Q', 'K'};
static char color_names[3] = {' ', 'W','B'};

struct Piece {
	int value;
	int color;
};

struct Position {
	int x;
	int y;
};

struct Move {
	struct Position start;
	struct Position end;
	bool capture;
};

struct MoveList {
	struct Move moves[MAX_MOVE_COUNT];
	int count;
};

bool printMove(struct Move);

struct Board {
	struct Piece positions[8][8];
	bool WHITE_CHECK;
	bool BLACK_CHECK;
};

bool reset_move_list(struct MoveList*);

struct Piece Pawn(int);
bool generateMoves_Pawn(struct Position, struct Board, struct MoveList*);
bool validateMove_Pawn(struct Move, struct Board);

struct Piece Knight(int);
bool generateMoves_Knight(struct Position, struct Board, struct MoveList*);
bool validateMove_Knight(struct Move, struct Board);

struct Piece Bishop(int);
bool generateMoves_Bishop(struct Position, struct Board, struct MoveList*);
bool validateMove_Bishop(struct Move, struct Board);

struct Piece Rook(int);
bool generateMoves_Rook(struct Position, struct Board, struct MoveList*);
bool validateMove_Rook(struct Move, struct Board);

struct Piece Queen(int);
bool generateMoves_Queen(struct Position, struct Board, struct MoveList*);
bool validateMove_Queen(struct Move, struct Board);

struct Piece King(int);
bool generateMoves_King(struct Position, struct Board, struct MoveList*);
bool validateMove_King(struct Move, struct Board);

bool validateMove(struct Piece, struct Move, struct Board);
bool generateMoves(struct Piece, struct Position, struct Board, struct MoveList*);

// int board_at(struct Position);
// void board_set(struct Position, struct Piece new_piece);

bool reset_board(struct Board*);
bool board_copy(struct Board, struct Board*);
bool movePiece(struct Move, struct Board*);
struct Move find_move(struct Board, struct Board);
bool check(struct Board, int);
bool check_mate(struct Board, int);
bool check_mate_help(int, struct Position, struct Board);
