#include <stdio.h>
#include <stdlib.h>
#include "othello.h"

// creates an initial empty board, returns pointer to it
board_t initial_board(void) {
    board_t board = malloc(sizeof(*board));
    if(board == NULL){
        perror("Could not allocate memory for a new board, initial_board failed");
        exit(EXIT_FAILURE);
    }

    for(int row = 0; row < HEIGHT; row ++) {
        for(int col = 0; col < WIDTH; col ++) {
            (*board)[row][col] = '-';
        }
    }    

    (*board)[3][3] = 'O';
    (*board)[3][4] = 'X';
    (*board)[4][3] = 'X';
    (*board)[4][4] = 'O';

    return board;
}

// prints the current state of the board
void print_board(board_t board) {
    printf("  A   B   C   D   E   F   G   H\n");
    for(int row = 0; row < HEIGHT; row ++) {
        printf("%d", row + 1);
        for(int col = 0; col < WIDTH; col ++) {
            if(col == 7) {
                printf(" %c \n", (*board)[row][col]);
            } else {
                printf(" %c |", (*board)[row][col]);
            }
        }
    }
};

// determines if the move is legal 
// (ie. if move if within bounds, and the position is empty)
// (and if there is another piece of the same type horizontally, vertically or diagonally)
bool is_legal(move_t move, board_t board){
    // check if move is within bounds
    if(move.row < 0 || move.row > 7 || move.col < 0 || move.col > 7){
        return false;
    }

    // check if position is empty
    if ((*board)[move.row][move.col] != '-'){
        return false;
    }
	
//	printf("checking row: %d, col %d\n", move.row, move.col);

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
				printf("row: %d, col: %d is legal\n", move.row, move.col);
				return true;
			}
		}
	}

	// checks the NE direction
	if (!(move.row == 0) && !(move.col == 7) \
			&& ((*board)[move.row - 1][move.col + 1] == opponent_piece)) {
		for (int i = 1; ((move.row - i) >= 0) && ((move.col + i) <= 7); i++) {
			if ((*board)[move.row - i][move.col + i] == '-') {
				break;
			} else if ((*board)[move.row - i][move.col + i] == player_piece) {
				printf("row: %d, col: %d is legal\n", move.row, move.col);
				return true;
			}
		}
	}

	// checks the E direction
	if (!(move.col == 7) && ((*board)[move.row][move.col + 1] == opponent_piece)) {
		for (int c = move.col + 1; c <= 7; c++) {
			if ((*board)[move.row][c] == '-') {
				break;
			} else if ((*board)[move.row][c] == player_piece) {
				printf("row: %d, col: %d is legal\n", move.row, move.col);
				return true;
			}
		}
	}

	// checks the SE direction
	if (!(move.row == 7) && !(move.col == 7) \
			&& ((*board)[move.row + 1][move.col + 1] == opponent_piece)) {
		for (int i = 1; ((move.row + i) <= 7) && ((move.col + i) <= 7); i++) {
			if ((*board)[move.row + i][move.col + i] == '-') {
				break;
			} else if ((*board)[move.row + i][move.col + i] == player_piece) {
				printf("row: %d, col: %d is legal\n", move.row, move.col);
				return true;
			}
		}
	}

	// checks the S direction
	if (!(move.row == 7) && ((*board)[move.row + 1][move.col] == opponent_piece)) {
		for (int r = move.row + 1; r <= 7; r++) {
			if ((*board)[r][move.col] == '-') {
				break;
			} else if ((*board)[r][move.col] == player_piece) {
				printf("row: %d, col: %d is legal\n", move.row, move.col);
				return true;
			}
		}
	}

	// checks the SW direction
	if (!(move.row == 7) && !(move.col == 0) \
			&& ((*board)[move.row + 1][move.col - 1] == opponent_piece)) {
		for (int i = 1; ((move.row + i) <= 7) && ((move.col - i) >= 0); i++) {
			if ((*board)[move.row + i][move.col - i] == '-') {
				break;
			} else if ((*board)[move.row + i][move.col - i] == player_piece) {
				printf("row: %d, col: %d is legal\n", move.row, move.col);
				return true;
			}
		}
	}

	// checks the W direction
	if (!(move.col == 0) && ((*board)[move.row][move.col - 1] == opponent_piece)) {
		for (int c = move.col - 1; c >= 0; c--) {
			if ((*board)[move.row][c] == '-') {
				break;
			} else if ((*board)[move.row][c] == player_piece) {
				printf("row: %d, col: %d is legal\n", move.row, move.col);
				return true;
			}
		}
	}

	// checks the NW direction
	if (!(move.col == 0) && !(move.row == 0) \
			&& ((*board)[move.row - 1][move.col - 1] == opponent_piece)) {
		for (int i = 1; ((move.row - i) >= 0) && ((move.col - i) >= 0); i++) {
			if ((*board)[move.row - i][move.col - i] == '-') {
				break;
			} else if ((*board)[move.row - i][move.col - i] == player_piece) {
				printf("row: %d, col: %d is legal\n", move.row, move.col);
				return true;
			}
		}
	}
	
    return false;
}


// checks if game has ended (i.e. if all spaces are either an X or O)
bool endgame(board_t board){
    for(int row = 0; row < HEIGHT; row ++) {
        for(int col = 0; col < WIDTH; col ++) {
            if ((*board)[row][col] == '-') {
                return false;
            };
        }
    }   
    return true;
}

void legalmove(board_t board, Player player, legalmoves_t* legalmoves) {
	legalmoves->size = 0;
	move_t move;
	(&move)->player = player;
	
	for (int r = 0; r < HEIGHT; r++) {
		for (int c = 0; c < WIDTH; c++) {
			(&move)->row = r;
			(&move)->col = c;
			if (is_legal(move, board)) {
				legalmoves->moves[legalmoves->size] = move;
				legalmoves->size++;
			}
		}
	}
}

// PRE: input move is legal
// updates the board with the move
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

// determines the winning player or if it is a draw
int outcome(board_t board){
    // player 1 == X and player 2 == O
    int player1 = 0;
    int player2 = 0;

    for(int row = 0; row < HEIGHT; row ++) {
        for(int col = 0; col < WIDTH; col ++) {
            if ((*board)[row][col] == 'X') {
                player1 ++;
            } else if ((*board)[row][col] == 'O') {
                player2 ++;
            }
        }
    }   

    if (player1 > player2){
        return 1;
    } if (player1 < player2){
        return 2;
    }

    return 0;
}

// returns a move from player input
void get_move(Player player, move_t* move) {
	printf("Player %d please choose the position to place your piece: \n", player);
	printf("(Enter row then column)\n");
	int row;
	char col;
	scanf("%d%c", &row, &col);
	move->row = row - 1;
	move->col = col - 'A';
	move->player = player;
	if (move->row < 0 || move->row > 7 || move->col < 0 || move->col > 7) {
		printf("Incorrect input. Please try again.\n");
		get_move(player, move);
	}
}

