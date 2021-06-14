#include <stdio.h>
#include <stdlib.h>
#include "playerbot.h"
#include "othello.h"

// greedy implementation
void greedy(board_t board, move_t* move, Player player, legalmoves_t* legalmoves) {
    // TODO
}

// random implementation
void randomize(board_t board, move_t* move, Player player, legalmoves_t* legalmoves) {
    // Initializes random number generator
    srand(time(NULL));  
    
    // Pick a random number legalmove from the array and store it in the move ptr
    int index = rand() % sizeof(legalmoves -> moves);
    *move = legalmoves -> moves[index];
}



// minimax implementation
void minimax(board_t board, move_t* move, Player player, legalmoves_t* legalmoves) {
    // TODO
}

//lousy implementation
void lousy(board_t board, move_t* moev, Player player, legalmoves_t* legalmoves) {
    // TODO
}

