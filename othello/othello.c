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

    *board[3][3] = 'O';
    *board[3][4] = 'X';
    *board[4][3] = 'X';
    *board[4][4] = 'O';


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

// determines if the move is legal 
// (ie. if move if within bounds, and the position is empty)
// (and if there is another piece of the same type horizontally, vertically or diagonally)
bool is_legal(move_t move, board_t* board){
    // check if move is within bounds
    if(move.row < 0 || move.row > 7 || move.col < 0 || move.col > 7){
        return false;
    }

    // check if position is empty
    if (*board[move.row][move.col] != '-'){
        return false;
    }

    char player_piece;

    if(move.player == BLACK){
        player_piece = 'X';
    } else if (move.player == WHITE){
        player_piece = 'O';
    }
    
    // check if there is another piece of same type vertically
    for(int r = 0; r < HEIGHT; r ++){
        if(*board[r][move.col] == player_piece){
            return true;
        }
    }

    // check if there is another piece of same type horitzontally
    for(int c = 0; c < WIDTH; c ++){
        if(*board[move.row][c] == player_piece){
            return true;
        }
    }

    // check if there is another piece of same type diagonally
    for(int r = 0; r < HEIGHT; r ++) {
        for(int c = 0; c < WIDTH; c ++) {
            if(move.row + r < 8){
                if(move.col + c < 8) {
                    if(*board[move.row + r][move.col + c] == player_piece) {
                        return true;
                    }
                }
            }

            if(move.row - r < 8){
                if(move.col - c < 8) {
                    if(*board[move.row + r][move.col + c] == player_piece) {
                        return true;
                    }
                }
            }
        }
    }



    return true;
}


// checks if game has ended (i.e. if all spaces are either an X or O)
bool endgame(board_t* board){
    for(int row = 0; row < HEIGHT; row ++) {
        for(int col = 0; col < WIDTH; col ++) {
            if (*board[row][col] == '-') {
                return false;
            };
        }
    }   
    return true;
}


// determines the winning player or if it is a draw
int outcome(board_t* board){
    // player 1 == X and player 2 == O
    int player1 = 0;
    int player2 = 0;

    for(int row = 0; row < HEIGHT; row ++) {
        for(int col = 0; col < WIDTH; col ++) {
            if (*board[row][col] == 'X') {
                player1 ++;
            } else if (*board[row][col] == 'O') {
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

// temporary main function for testing
int main(void){
    board_t* board = initial_board();
    print_board(board);
}
