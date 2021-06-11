#ifndef __OTHELLO_H__
#define __OTHELLO_H__

#include <stdbool.h>
#define HEIGHT 8
#define WIDTH 8

typedef char board_t[HEIGHT][WIDTH];

typedef struct {
	int row;
	int col;
	int player;
} move_t;

typedef struct {
	move_t* moves;
	int size;
} legalmoves_t;

// prints the current state of the board
void print_board(board_t* board);

// creates an initial empty board
board_t* initial_board(void);

// determines if the move is legal
bool is_legal(move_t move, board_t* board);

// updates the board with the result of the move
void make_move(move_t move, board_t* board);

// determines an array of legal moves
legalmoves_t* legalmoves(board_t* board);

// determines if this is a full board 
bool is_full_board(board_t* board);

// determines the winning player or if it is a draw
int outcome(board_t* board);

#endif

