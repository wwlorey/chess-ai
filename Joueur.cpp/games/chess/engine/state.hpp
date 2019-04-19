#ifndef STATE_HPP
#define STATE_HPP

#include "chessboard.hpp"
#include <vector>

class State {
public:
    ChessBoard board;
    int depth;
    int qs_depth;
    bool max_player_color;
    bool is_quiescent;
    std::vector<int> actions;

    State(ChessBoard board, int depth, int qs_depth, bool max_player_color, bool is_quiescent=true);
    State result(int move);
    int utility(int terminal_result);
    bool check_3_fold_rep(std::vector<int> history);
    bool insufficient_material(void);
};

#endif // STATE_HPP