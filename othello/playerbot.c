#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "playerbot.h"
#include "othello.h"

// greedy implementation
void greedy(board_t board, move_t* move, Player player, legalmoves_t* legalmoves) {
    // TODO
}

// random implementation
void randomize(board_t board, move_t* move, Player player, legalmoves_t* legalmoves) {
    // TODO
}

// heuristic function for minimax
int heuristic(board_t board, Player player) {
	char opponent ='X';
	char me = 'O';
	if(player == 1) {
		opponent = 'O';
		me = 'X';
	}	
	return score(board, me) - score(board, opponent);
}

// a copy board function
void copy_board(board_t src, board_t dest) {
	memcpy(dest, src, 8 * 8 * sizeof(char));
}	

// minimax implementation
void minimax(board_t board, move_t* move, Player player, legalmoves_t* legalmoves) {
    // TODO
}

//lousy implementation
void lousy(board_t board, move_t* moev, Player player, legalmoves_t* legalmoves) {
    // TODO
}

