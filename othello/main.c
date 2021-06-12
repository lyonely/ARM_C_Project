#include <stdio.h>
#include <stdlib.h>
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
    printf("- Your aim is to own more pieces than your opponent when the game is over.\n\n");

    board_t board = initial_board();
    print_board(board);

    printf("Player1 please choose the position to place your piece: \n");

    char p1_pos[2];
    scanf("%s", p1_pos);
    printf("%s\n", p1_pos);

}