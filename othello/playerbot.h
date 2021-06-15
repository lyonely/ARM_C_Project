#ifndef __PLAYERBOT_H__
#define __PLAYERBOT_H__
#include "othello.h"

// greedy implementation
void greedy(board_t board, move_t* move, Player player, legalmoves_t* legalmoves);

// random implementation
void randomize(board_t board, move_t* move, Player player, legalmoves_t* legalmoves);

// minimax implementation
void minimax(board_t board, move_t* move, Player player, legalmoves_t* legalmoves);

// minimax_1 implementation
void minimax_1(board_t board, move_t* move, Player player, legalmoves_t* legalmoves); 

//lousy implementation
void lousy(board_t board, move_t* moev, Player player, legalmoves_t* legalmoves);

#endif
