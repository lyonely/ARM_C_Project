#include <stdio.h>
#include <stdlib.h>
#include "playerbot.h"
#include "othello.h"

void make_move(move_t move, board_t board) {

	char player_piece;
	char opponent_piece;

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
				for (int i = move.row; i >= r; i--) {
					(*board)[i][move.col] = player_piece;
				}
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
				for (int j = 0; j <= i; j++) {
					(*board)[move.row - j][move.col + j] = player_piece;
				}
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
				for (int i = move.col; i <= c; i++) {
					(*board)[move.row][i] = player_piece;
				}
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
				for (int j = 0; j <= i; j++) {
					(*board)[move.row + j][move.col + j] = player_piece;
				}
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
				for (int i = move.row; i <= r; i++) {
					(*board)[i][move.col] = player_piece;
				}
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
				for (int j = 0; j <= i; j++) {
					(*board)[move.row + j][move.col - j] = player_piece;
				}
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
				for (int i = move.col; i >= c; i--) {
					(*board)[move.row][i] = player_piece;
				}
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
				for (int j = 0; j <= i; j++) {
					(*board)[move.row - j][move.col - j] = player_piece;
				}
				break;
			}
		}
	}
}

// greedy implementation
void greedy(board_t board, move_t* move, Player player, legalmoves_t* legalmoves) {
    // TODO
}

// random implementation
void randomize(board_t board, move_t* move, Player player, legalmoves_t* legalmoves) {
    // TODO
}

// minimax implementation
void minimax(board_t board, move_t* move, Player player, legalmoves_t* legalmoves) {
    // TODO
}

//lousy implementation
void lousy(board_t board, move_t* moev, Player player, legalmoves_t* legalmoves) {
    // TODO
}

