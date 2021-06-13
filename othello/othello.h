#ifndef __OTHELLO_H__
#define __OTHELLO_H__
#include <stdbool.h>

#define HEIGHT 8
#define WIDTH 8

// Player1 = X and Player2 = O

// board_t is a pointer to an 8x8 array of char
typedef char (*board_t)[HEIGHT][WIDTH];

typedef enum Player {BLACK = 1, WHITE = 2} Player;

typedef struct {
	int row;
	int col;
	Player player;
} move_t;

typedef struct {
	move_t* moves;
	int size;
} legalmoves_t;

// prints the current state of the board
void print_board(board_t board);

// creates an initial empty board
board_t initial_board(void);

// determines if the move is legal
bool is_legal(move_t move, board_t board);

// updates the board with the result of the move
void make_move(move_t move, board_t board);

// determines an array of legal moves
void legalmove(board_t board, Player player, legalmoves_t* legalmoves);

// returns 1 if player1 (X) wins, 2 if player2 (O) wins, or 0 otherwise
int outcome(board_t board);

// returns 1 if game has ended (i.e. if all spaces are either an X or O)
bool endgame(board_t board);

// returns the move from player input
void get_move(Player player, move_t* move);

#endif
