#include <stdio.h>
#include <stdlib.h>
#include "othello.h"

// creates an initial empty board
board_t* initial_board(void) {
    board_t* board = (board_t*) malloc(HEIGHT * WIDTH * sizeof(char));
    if(board == NULL){
        perror("Could not allocate memory for a new board, initial_board failed");
        exit(EXIT_FAILURE);
    }

    for(int row = 0; row < HEIGHT; row ++) {
        for(int col = 0; col < WIDTH; col ++) {
            *board[row][col] = '-';
        }
    }    

    return board;
}

// prints the current state of the board
void print_board(board_t* board) {
    for(int row = 0; row < HEIGHT; row ++) {
        for(int col = 0; col < WIDTH; col ++) {

            if(col == 7) {
                printf(" %c ", *board[row][col]);
            } else {
                printf(" %c |", *board[row][col]);
            }
        }
        printf("\n");
    }
};

int main(void){
    board_t* board = initial_board();
    print_board(board);
}
