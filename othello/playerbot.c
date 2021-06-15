#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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
    legalmoves_t* greedymoves = malloc(sizeof(legalmoves_t));
	if (greedymoves == NULL) {
		perror("Failed to allocate memory for greedy moves");
		exit(EXIT_FAILURE);
	}
	greedymoves->moves = calloc(HEIGHT*WIDTH, sizeof(move_t));
	if (greedymoves->moves == NULL) {
		perror("Failed to allocate memory for greedy moves array");
		exit(EXIT_FAILURE);
	}
	greedymoves->size = 0;
	
	int most_pieces;
	most_pieces = pieces_earned(legalmoves->moves[0], board);
	greedymoves->moves[0] = legalmoves->moves[0];
	greedymoves->size++;

	for (int i = 0; i < legalmoves->size; i++) {
		if (pieces_earned(legalmoves->moves[i], board) > most_pieces) {
			greedymoves->moves[0] = legalmoves->moves[i];
			greedymoves->size = 1;
		} else if (pieces_earned(legalmoves->moves[i], board) == most_pieces) {
			greedymoves->moves[greedymoves->size] = legalmoves->moves[i];
			greedymoves->size++;
		}
	}

	int r = rand() % (greedymoves->size);
	*move = greedymoves->moves[r];
	free(greedymoves->moves);
	free(greedymoves);
}

// random implementation
void randomize(board_t board, move_t* move, Player player, legalmoves_t* legalmoves) {
    // Initializes random number generator
    srand(time(NULL));  
    
    // Pick a random number legalmove from the array and store it in the move ptr
    int index = rand() % (legalmoves->size);
    *move = legalmoves -> moves[index];
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
	memcpy(dest, src, sizeof(board_t) + 1);
}

// minimax value implementation
int minimax_value(board_t board, Player player, Player current, int search) {
	Player opponent = 1;
	if (current == 1) {
		opponent = 2;
	}			
	if(search == 5 || endgame(board)) {
		return heuristic(board, player);
	}
	legalmoves_t* legalmoves = malloc(sizeof(legalmoves_t));
	if(legalmoves == NULL) {
		perror("failed to allocate memory for minimax values");
		exit(EXIT_FAILURE);
	}
	legalmoves->moves = calloc(HEIGHT*WIDTH, sizeof(move_t));
	if(legalmoves->moves == NULL) {
		perror("failed to allocate memory for minimax array");
		exit(EXIT_FAILURE);
	}
	legalmoves->size = 0;
	legalmove(board, current, legalmoves);
	if(legalmoves->size == 0) {
		free(legalmoves->moves);			
		free(legalmoves);			
		return minimax_value(board, player, opponent, search + 1);
	}		
	int best_move_val = -9999;
	if(player != current) {
		best_move_val = 9999;
	}
	for(int i = 0; i < legalmoves->size; i++) {
		board_t temp_board = calloc(1, sizeof(board_t));
		if(temp_board == NULL) {
			perror("failed to allocate memory for temp in minimax value");
			exit(EXIT_FAILURE);
		}
		copy_board(board, temp_board);
		make_move(legalmoves->moves[i], temp_board);
		int val = minimax_value(temp_board, player, opponent, search + 1);
		if(player == current) {
			if(val > best_move_val) {
				best_move_val = val;
			}
		}
		else {
			if (val < best_move_val) {
				best_move_val = val;
			}
		}
		free(temp_board);
	}	
	free(legalmoves->moves);
	free(legalmoves);
	return best_move_val;		
}				

int minimax_value_1(board_t board, Player player, Player current, int search) {
	Player opponent = 1;
	if (current == 1) {
		opponent = 2;
	}			
	if(search == 15 || endgame(board)) {
		return heuristic(board, player);
	}
	legalmoves_t* legalmoves = malloc(sizeof(legalmoves_t));
	if(legalmoves == NULL) {
		perror("failed to allocate memory for minimax values");
		exit(EXIT_FAILURE);
	}
	legalmoves->moves = calloc(HEIGHT*WIDTH, sizeof(move_t));
	if(legalmoves->moves == NULL) {
		perror("failed to allocate memory for minimax array");
		exit(EXIT_FAILURE);
	}
	legalmoves->size = 0;
	legalmove(board, current, legalmoves);
	if(legalmoves->size == 0) {
		free(legalmoves->moves);			
		free(legalmoves);			
		return minimax_value(board, player, opponent, search + 1);
	}		
	int best_move_val = -9999;
	if(player != current) {
		best_move_val = 9999;
	}
	for(int i = 0; i < legalmoves->size; i++) {
		board_t temp_board = calloc(1, sizeof(board_t));
		if(temp_board == NULL) {
			perror("failed to allocate memory for temp in minimax value");
			exit(EXIT_FAILURE);
		}
		copy_board(board, temp_board);
		make_move(legalmoves->moves[i], temp_board);
		int val = minimax_value(temp_board, player, opponent, search + 1);
		if(player == current) {
			if(val > best_move_val) {
				best_move_val = val;
			}
		}
		else {
			if (val < best_move_val) {
				best_move_val = val;
			}
		}
		free(temp_board);
	}	
	free(legalmoves->moves);
	free(legalmoves);
	return best_move_val;		
}				



// minimax implementation
void minimax(board_t board, move_t* move, Player player, legalmoves_t* legalmoves) {
	Player opponent = 1;
	if(player == 1) {
		opponent = 2;
	}
	move_t best_move = legalmoves->moves[0];
	int best_move_val = -9000;
	for(int i = 0; i < legalmoves->size; i++) {
		board_t temp_board = calloc(1, sizeof(board_t));
		if(temp_board == NULL) {
			perror("failed to allocate memory for temp_board in minimax");
			exit(EXIT_FAILURE);
		}	
		copy_board(board, temp_board);
		make_move(legalmoves->moves[i], temp_board);
		int val = minimax_value(temp_board, player, opponent, 1);
		if(val > best_move_val) {
			best_move_val = val;
			best_move = legalmoves->moves[i];
		}
		free(temp_board);
	}
	*move = best_move;	
}


void minimax_1(board_t board, move_t* move, Player player, legalmoves_t* legalmoves) {
	Player opponent = 1;
	if(player == 1) {
		opponent = 2;
	}
	move_t best_move = legalmoves->moves[0];
	int best_move_val = -9000;
	for(int i = 0; i < legalmoves->size; i++) {
		board_t temp_board = calloc(1, sizeof(board_t));
		if(temp_board == NULL) {
			perror("failed to allocate memory for temp_board in minimax");
			exit(EXIT_FAILURE);
		}	
		copy_board(board, temp_board);
		make_move(legalmoves->moves[i], temp_board);
		int val = minimax_value_1(temp_board, player, opponent, 1);
		if(val > best_move_val) {
			best_move_val = val;
			best_move = legalmoves->moves[i];
		}
		free(temp_board);
	}
	*move = best_move;	
}

//lousy implementation
void lousy(board_t board, move_t* move, Player player, legalmoves_t* legalmoves) {
    legalmoves_t* lousymoves = malloc(sizeof(legalmoves_t));
	if (lousymoves == NULL) {
		perror("Failed to allocate memory for greedy moves");
		exit(EXIT_FAILURE);
	}
	lousymoves->moves = calloc(HEIGHT*WIDTH, sizeof(move_t));
	if (lousymoves->moves == NULL) {
		perror("Failed to allocate memory for greedy moves array");
		exit(EXIT_FAILURE);
	}
	lousymoves->size = 0;
	
	int least_pieces;
	least_pieces = pieces_earned(legalmoves->moves[0], board);
	lousymoves->moves[0] = legalmoves->moves[0];
	lousymoves->size++;

	for (int i = 0; i < legalmoves->size; i++) {
		if (pieces_earned(legalmoves->moves[i], board) < least_pieces) {
			lousymoves->moves[0] = legalmoves->moves[i];
			lousymoves->size = 1;
		} else if (pieces_earned(legalmoves->moves[i], board) == least_pieces) {
			lousymoves->moves[lousymoves->size] = legalmoves->moves[i];
			lousymoves->size++;
		}
	}

	int r = rand() % (lousymoves->size);
	*move = lousymoves->moves[r];
	free(lousymoves->moves);
	free(lousymoves);
}

