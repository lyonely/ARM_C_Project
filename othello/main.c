#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "othello.h"

/*  FLOW
    - Print welcome message
    - Print initial board
    - Get player1 input
    - Check if legal move, if not get player input again
    - Update board & print new board
    - Check if game is ended
    - Repeat same steps for player2
*/

int main(void) {
    printf("***************** Welcome to our game of OTHELLO! *****************\n\n");
    printf("RULES:\n");
    printf("- Flip your opponent's pieces by trapping them between 2 of your own.\n");
    printf("- Your aim is to own more pieces than your opponent when the game is over.\n");
	printf("- Player 1 is represented by X while player 2 is represented by O.\n\n");

    board_t board = initial_board();
    	
	Player curr = 1;
	legalmoves_t* legalmoves = malloc(sizeof(legalmoves_t));
	if (legalmoves == NULL) {
		perror("failed to allocate memory for legalmoves");
		exit(EXIT_FAILURE);
	}
	legalmoves->moves = calloc(HEIGHT*WIDTH, sizeof(move_t));
	if (legalmoves->moves == NULL) {
		perror("Failed to allocate memory for moves array");
		exit(EXIT_FAILURE);
	}
	move_t* move = malloc(sizeof(move_t));
	if (move == NULL) {
		perror("failed to allocate memory for move");
		exit(EXIT_FAILURE);
	}
	int tracker = 0; //keeps track of number of players who do not have legal moves left
	bool human_player = true; //true if you're playing against a human player, false if you're against a bot
	Player turn; // 1 if you're playing first, 2 if you're playing second (applicable only for playing against bots)

	printf("Would you like to play against an AI? (y/n)\n");
	char ans;
	scanf("%c", &ans);

	void (*generate_move)(board_t board, move_t*, Player, legalmoves_t*);

	if (ans == 'y') {
		human_player = false;
		printf("Choose your opponent!\n");
		printf("1 - Greedy Gregory, 2 - Random Ralph, 3 - Minimax Matt, 4 - Lousy Louis\n");
		int choice;
		scanf("%d", &choice);
		if (choice == 1) {
			generate_move = &greedy;
		} else if (choice == 2) {
			generate_move = &randomize;
		} else if (choice == 3) {
			generate_move = &minimax;
		} else if (choice == 4) {
			generate_move = &lousy;
		}
		printf("Would you like to be the first or second player?\n");
		printf("1 - first player, 2 - second player\n");
		scanf("%d", &choice);
		turn = choice;
	} else if (ans == 'n') {
		printf("Proceeding with normal 2-player game\n");
	}
	
	while (1) {
		print_board(board);
    	legalmove(board, curr, legalmoves);
		if (legalmoves->size > 0) {
			tracker = 0;
			if (!human_player && turn != curr) {
				printf("Bot is now making its move.\n");
				generate_move(board, move, curr, legalmoves);
			} else {
				do {
					get_move(curr, move);
					if (!is_legal(*move, board)) {
						printf("Illegal move detected.\n");
					}
				} while (!is_legal(*move, board));
			}
			make_move(*move, board);
		} else {
			printf("No available moves, passing on the next player...\n");
			tracker++;
		}
		curr = abs(3 - curr);
		if (endgame(board) || tracker == 2) {
			break;
		}
	}
	print_board(board);
	int result = outcome(board);
	if (result == 0) {
		printf("The game ended in a draw...\n");
	} else {
		printf("Player %d has won!\n", result);
	}
	free(board);
	free(legalmoves->moves);
	free(legalmoves);
	free(move);
}
