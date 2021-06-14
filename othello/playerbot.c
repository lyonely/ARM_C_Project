#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "playerbot.h"
#include "othello.h"

int pieces_earned(move_t move, board_t board) {

	char player_piece;
	char opponent_piece;
	int counter = 0;

    if(move.player == 1){
        player_piece = 'X';
		opponent_piece = 'O';
    } else if (move.player == 2){
        player_piece = 'O';
		opponent_piece = 'X';
    }

    // checks the N direction
	if (!(move.row == 0) && ((*board)[move.row - 1][move.col] == opponent_piece)) {
		for (int r = move.row - 1; r >= 0; r--) {
			if ((*board)[r][move.col] == '-') {
				break;
			} else if ((*board)[r][move.col] == player_piece) {
				counter += (move.row - r);
				break;
			}
		}
	}

	// checks the NE direction
	if (!(move.row == 0) && !(move.col == 7) \
			&& ((*board)[move.row - 1][move.col + 1] == opponent_piece)) {
		for (int i = 1; (move.row - i) >= 0 && (move.col + i) <= 7; i++) {
			if ((*board)[move.row - i][move.col + i] == '-') {
				break;
			} else if ((*board)[move.row - i][move.col + i] == player_piece) {
				counter += i;
				break;
			}
		}
	}

	// checks the E direction
	if (!(move.col == 7) && ((*board)[move.row][move.col + 1] == opponent_piece)) {
		for (int c = move.col + 1; c <= 7; c++) {
			if ((*board)[move.row][c] == '-') {
				break;
			} else if ((*board)[move.row][c] == player_piece) {
				counter += (c - move.col);
				break;
			}
		}
	}

	// checks the SE direction
	if (!(move.row == 7) && !(move.col == 7) \
			&& ((*board)[move.row + 1][move.col + 1] == opponent_piece)) {
		for (int i = 1; (move.row + i) <= 7 && (move.col + i) <= 7; i++) {
			if ((*board)[move.row + i][move.col + i] == '-') {
				break;
			} else if ((*board)[move.row + i][move.col + i] == player_piece) {
				counter += i;
				break;
			}
		}
	}

	// checks the S direction
	if (!(move.row == 7) && ((*board)[move.row + 1][move.col] == opponent_piece)) {
		for (int r = move.row + 1; r <= 7; r++) {
			if ((*board)[r][move.col] == '-') {
				break;
			} else if ((*board)[r][move.col] == player_piece) {
				counter += (r - move.row);
				break;
			}
		}
	}

	// checks the SW direction
	if (!(move.row == 7) && !(move.col == 0) \
			&& ((*board)[move.row + 1][move.col - 1] == opponent_piece)) {
		for (int i = 1; (move.row + i) <= 7 && (move.col - i) >= 0; i++) {
			if ((*board)[move.row + i][move.col - i] == '-') {
				break;
			} else if ((*board)[move.row + i][move.col - i] == player_piece) {
				counter += i;
				break;
			}
		}
	}

	// checks the W direction
	if (!(move.col == 0) && ((*board)[move.row][move.col - 1] == opponent_piece)) {
		for (int c = move.col - 1; c >= 0; c--) {
			if ((*board)[move.row][c] == '-') {
				break;
			} else if ((*board)[move.row][c] == player_piece) {
				counter += (move.col - c);
				break;
			}
		}
	}

	// checks the NW direction
	if (!(move.col == 0) && !(move.row == 0) \
			&& ((*board)[move.row - 1][move.col - 1] == opponent_piece)) {
		for (int i = 1; (move.row - i) >= 0 && (move.col - i) >= 0; i++) {
			if ((*board)[move.row - i][move.col - i] == '-') {
				break;
			} else if ((*board)[move.row - i][move.col - i] == player_piece) {
				counter += i;
				break;
			}
		}
	}
	return counter;
}

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

