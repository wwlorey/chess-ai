#include "state.hpp"

State::State(ChessBoard board, int depth, int qs_depth, bool max_player_color) {
    this->board = board;
    this->depth = depth;
    this->qs_depth = qs_depth;
    this->max_player_color = max_player_color;
        
    if (this->depth)
       this->board.actions(this->actions);
    else
        // Don't calculate actions if the depth limit is reached    
        this->actions = {0};
}

// Returns the resulting state after applying move to this->board
State State::result(int move) {
    return State(this->board.apply_move(move), this->depth - 1, this->qs_depth, this->max_player_color);
}

// Returns the resulting state after applying move to this->board during a quiescent search
State State::qs_result(int move) {
    return State(this->board.apply_move(move), this->depth, this->qs_depth - 1, this->max_player_color);
}

// Returns the utility value (either actual or material advantage) of this state based on
// the terminal_result.
// The material advantage state evaluation heuristic is the point value summation
// across all pieces for the max player's color.
int State::utility(int terminal_result) {
    if (terminal_result == LOSE_TERMINAL_NODE) {
        // Wins for the max player are good while wins for the min player are bad
        return (this->board.color == this->max_player_color) ? MIN_VALUE : MAX_VALUE;
    }

    else if (terminal_result == DRAW_TERMINAL_NODE) {
        // This state is a terminal node, but we don't necessarily want to be driven here
        return DRAW_VALUE;
    }

    else { // terminal_result == DEPTH_LIMIT_REACHED
        // Calculate the material advantage state evaluation heuristic
        // (the difference in weighted piece values between the friend pieces and foe pieces for the max player)
        int w_sum = 0;
        int b_sum = 0;

        for (int bitboard_index = 0; bitboard_index < NUM_BITBOARDS / 2; bitboard_index++) {
            w_sum += PIECE_WEIGHTS[bitboard_index] * count_set_bits(this->board.bitboards[WHITE_BITBOARD_INDICES[bitboard_index]]);            
            b_sum += PIECE_WEIGHTS[bitboard_index] * count_set_bits(this->board.bitboards[BLACK_BITBOARD_INDICES[bitboard_index]]);            
        }
    
        return (this->max_player_color == WHITE) ? (w_sum - b_sum) : (b_sum - w_sum);
    }
}

bool State::check_3_fold_rep(std::vector<int> history) {
    int last_index = history.size() - 1;    

    return (last_index >= 8) &&
           (history[last_index    ] & history[last_index - 4] & ATTACK_MOVE_MASK & TO_FILE_A_MOVE_MASK & TO_RANK_MOVE_MASK) == 
           (history[last_index    ] &                           ATTACK_MOVE_MASK & TO_FILE_A_MOVE_MASK & TO_RANK_MOVE_MASK) &&

           (history[last_index - 1] & history[last_index - 5] & ATTACK_MOVE_MASK & TO_FILE_A_MOVE_MASK & TO_RANK_MOVE_MASK) == 
           (history[last_index - 1] &                           ATTACK_MOVE_MASK & TO_FILE_A_MOVE_MASK & TO_RANK_MOVE_MASK) &&

           (history[last_index - 2] & history[last_index - 6] & ATTACK_MOVE_MASK & TO_FILE_A_MOVE_MASK & TO_RANK_MOVE_MASK) == 
           (history[last_index - 2] &                           ATTACK_MOVE_MASK & TO_FILE_A_MOVE_MASK & TO_RANK_MOVE_MASK) &&

           (history[last_index - 3] & history[last_index - 7] & ATTACK_MOVE_MASK & TO_FILE_A_MOVE_MASK & TO_RANK_MOVE_MASK) == 
           (history[last_index - 3] &                           ATTACK_MOVE_MASK & TO_FILE_A_MOVE_MASK & TO_RANK_MOVE_MASK);
}

// Returns true if there is insufficient material to continue the game, false otherwise.
// Conditions for insufficient material (only one must be satisfied):/
// source: https://en.wikipedia.org/wiki/Draw_(chess)
//     * king versus king
//     * king and bishop versus king
//     * king and knight versus king
//     * king and bishop versus king and bishop with the bishops on the same color
bool State::insufficient_material(void) {
    // Initially assume all cases are valid, then disprove them
    bool king_versus_king = true;
    bool king_and_bishop_versus_king = true;
    bool king_and_knight_versus_king = true;
    bool king_and_bishop_versus_king_and_bishop_with_bishops_on_same_color = true;

    // Get which pieces are still in play
    std::vector<int> piece_counts;
    piece_counts.reserve(NUM_BITBOARDS);
    int piece_count;
    int bb_index = 0;
    while (bb_index < NUM_BITBOARDS && (
        king_versus_king ||
        king_and_bishop_versus_king ||
        king_and_knight_versus_king ||
        king_and_bishop_versus_king_and_bishop_with_bishops_on_same_color)) {
        
        piece_count = count_set_bits(this->board.bitboards[bb_index]);
        
        if (piece_count && (bb_index == WQ || bb_index == BQ || bb_index == WR || bb_index == BR || bb_index == WP || bb_index == BP)) {
            // There is sufficient material to continue the game
            return false;
        }
        
        if (piece_count) {
            // There is a piece on this board 

            // Only permit king versus king if the piece is a king
            king_versus_king = bb_index == WK || bb_index == BK;
            
            // Similar for the rest
            king_and_bishop_versus_king = king_versus_king || bb_index == WB || bb_index == BB;
            king_and_knight_versus_king = king_versus_king || bb_index == WN || bb_index == BN;
            king_and_bishop_versus_king_and_bishop_with_bishops_on_same_color = king_and_bishop_versus_king;
        }
        
        piece_counts.push_back(piece_count);
        bb_index++;
    }
            
    if (bb_index != NUM_BITBOARDS) {
        // We broke out of the loop with no insufficient material conditions met
        return false;
    }
        
    // There is still hope for insufficient material
    king_and_bishop_versus_king = (piece_counts[WB] && !piece_counts[BB]) || (piece_counts[BB] && !piece_counts[WB]);
    king_and_knight_versus_king = (piece_counts[WN] && !piece_counts[BN]) || (piece_counts[BN] && !piece_counts[WN]);
    
    king_and_bishop_versus_king_and_bishop_with_bishops_on_same_color = (piece_counts[WB] && piece_counts[BB]) &&
        count_set_bits(this->board.bitboards[WB]) == 1 && count_set_bits(this->board.bitboards[BB]) == 1 &&
        DIAG_RAYS[this->board.bitboards[WB]] & this->board.bitboards[BB];
    
    return  king_versus_king ||
            king_and_bishop_versus_king ||
            king_and_knight_versus_king ||
            king_and_bishop_versus_king_and_bishop_with_bishops_on_same_color;
}
