#include "chessboard.hpp"
#include "constants.hpp"
#include "search.hpp"
#include "state.hpp"
#include "util.hpp"
#include <algorithm>
#include <unordered_map>

std::unordered_map<int, int> dummy_map = {{0,0}};

// Returns the terminal node type if state is a terminal node, INTERNAL_NODE otherwise.
int terminal_test(State state, std::vector<int> move_history) {
    if (state.board.stalemate)
        return DRAW_TERMINAL_NODE;

    if (!state.actions.size()) {
        // Checkmate
        return LOSE_TERMINAL_NODE;
    }

    if (state.board.half_moves == 100) {
        // 100 half moves = 50 moves (50 move rule)
        return DRAW_TERMINAL_NODE;
    }

    if (state.board.half_moves == 8 && state.check_3_fold_rep(move_history)) {
        // 3-fold repetition
        return DRAW_TERMINAL_NODE;
    }

    if (state.insufficient_material()) {
        return DRAW_TERMINAL_NODE;
    }

    if (state.depth <= 0) {
        if (!state.is_quiescent && state.qs_depth > 0) {
            // Continue with quiescent search
            return INTERNAL_NODE;
        }

        // No more regular depth or quiescent depth to exhaust
        return DEPTH_LIMIT_REACHED;
    }

    // This node is not a terminal node
    return INTERNAL_NODE;
}

// Depth-Limited MiniMax where the current depth is encoded (and decremented) 
// as part of the State class.
// Returns an action. 
int depth_limited_minimax(State initial_state, std::vector<int> history) {
    // Return the action with maximum utility value
    int best_action;
    int best_value = MIN_VALUE;    
    int curr_value;
    
    for (int a = 0; a < initial_state.actions.size(); a++) {
        curr_value = min_value(initial_state.result(initial_state.actions[a]), history);
        
        if (curr_value >= best_value) {
            best_value = curr_value;
            best_action = initial_state.actions[a];
        }
    }
    
    return best_action;
}

// Returns a utility value
int max_value(State state, std::vector<int> history) {
    int terminal_result = terminal_test(state, history);

    if (terminal_result != INTERNAL_NODE) {
        // This is a terminal node
        return state.utility(terminal_result);
    }

    int value = MIN_VALUE;

    for (int a = 0; a < state.actions.size(); a++) {
        history.push_back(state.actions[a]);
        value = std::max(value, min_value(state.result(state.actions[a]), history));
        history.pop_back();
    }

    return value;
}

int min_value(State state, std::vector<int> history) {
    int terminal_result = terminal_test(state, history);

    if (terminal_result != INTERNAL_NODE) {
        // This is a terminal node
        return state.utility(terminal_result);
    }

    int value = MAX_VALUE;

    for (int a = 0; a < state.actions.size(); a++) {
        history.push_back(state.actions[a]);
        value = std::min(value, max_value(state.result(state.actions[a]), history));
        history.pop_back();
    }

    return value;
}

// Returns an action
int iterative_deepening_depth_limited_minimax(std::string initial_fen, int max_depth_limit, bool max_player_color, std::vector<int> history) {
    int action;    

    for (int depth_limit = 1; depth_limit <= max_depth_limit; depth_limit++) {
        print("Depth" + std::to_string(depth_limit));
        action = depth_limited_minimax(State(ChessBoard(initial_fen), depth_limit, 0, max_player_color), history);
    }

    print();

    return action;
}

// Time-Limited Iterative-Deepening Depth-Limited MiniMax with alpha-beta pruning max value. Returns a utility value
int tliddlmmwabp_max_value(State state, int alpha, int beta, std::vector<int> history) {
    int terminal_result = terminal_test(state, history);

    if (terminal_result != INTERNAL_NODE) {
        // This is a terminal node
        return state.utility(terminal_result);
    }
    
    int value = MIN_VALUE;
    
    for (auto &action : state.actions) {
        history.push_back(action);
        value = std::max(value, tliddlmmwabp_min_value(state.result(action), alpha, beta, history));
        history.pop_back();
        
        if (value >= beta)
            // Fail high, prune
            return value;
        
        alpha = std::max(alpha, value);
    }
    
    return value;
}

// Time-Limited Iterative-Deepening Depth-Limited MiniMax with alpha-beta pruning min value. Returns a utility value
int tliddlmmwabp_min_value(State state, int alpha, int beta, std::vector<int> history) {
    int terminal_result = terminal_test(state, history);

    if (terminal_result != INTERNAL_NODE) {
        // This is a terminal node
        return state.utility(terminal_result);
    }
    
    int value = MAX_VALUE;
    
    for (auto &action : state.actions) {
        history.push_back(action);
        value = std::min(value, tliddlmmwabp_max_value(state.result(action), alpha, beta, history));
        history.pop_back();

        if (value <= alpha)
            // Fail low, prune
            return value;
        
        beta = std::min(beta, value);
    }
    
    return value;
}

// Returns an action
int time_limited_iterative_deepening_depth_limited_minimax_with_alpha_beta_pruning(std::string initial_fen, bool max_player_color, std::vector<int> history, double time_remaining_ns) {
    int value = MIN_VALUE;
    int best_value = MIN_VALUE;
    int best_action = 0;
    int prev_depth_best_action = 0;
    int alpha;
    int beta;
    int terminal_result;

    // Determine allocated time for this move
    double end_time = GET_TIME_NS() + (time_remaining_ns / ESTIMATED_REMAINING_MOVES);

    int depth_limit = 1;
    while (true) {
        print("Depth" + std::to_string(depth_limit));

        State state = State(ChessBoard(initial_fen), depth_limit, 0, max_player_color);
        terminal_result = terminal_test(state, history);

        // Return this state's action with value found from the max value function
        if (terminal_result != INTERNAL_NODE) {
            // The search cannot start in a terminal node
            print("The search cannot start in a terminal node.");
            return 0;
        } else {
            alpha = INIT_ALPHA;
            beta  = INIT_BETA;

            for (auto &action : state.actions) {
                history.push_back(action);
                value = tliddlmmwabp_min_value(state.result(action), alpha, beta, history);
                history.pop_back();

                if (value > best_value) {
                    best_value = value;
                    best_action = action;
                }
                
                if (value >= beta)
                    // Fail high, prune
                    return action;
                
                alpha = std::max(alpha, value);
                
                // Check for timeout
                if (GET_TIME_NS() > (end_time)) {
                    print("TIMEOUT");
                    return prev_depth_best_action;
                }
            }
        }

        prev_depth_best_action = best_action;
        depth_limit++;
    }
    
    print();
    
    return best_action;
}

// Time-Limited Iterative-Deepening Depth-Limited MiniMax with alpha-beta pruning and Quiescence Search and History Table
// Returns a utility value
int tliddlmabpqsht_max_value(State state, int alpha, int beta, std::vector<int> move_history, std::unordered_map<int, int> &history_table) {
    int terminal_result = terminal_test(state, move_history);

    if (terminal_result != INTERNAL_NODE) {
        // This is a terminal node
        return state.utility(terminal_result);
    }
    
    int value = MIN_VALUE;
    int best_action = 0;
    int new_value;

    state.actions = ht_sort(state.actions, history_table);
    
    for (auto &action : state.actions) {
        move_history.push_back(action);
        new_value = tliddlmabpqsht_min_value(state.result(action), alpha, beta, move_history, history_table);
        move_history.pop_back();
        
        // Get the new max value and best action
        if (new_value > value) {
            value = new_value;
            best_action = action;
        }

        if (value >= beta) {
            // Fail high, prune

            // Update the history table
            if (ht_contains(history_table, action))
                history_table[action]++;
            else
                history_table[action] = 1;

            return value;
        }
        
        alpha = std::max(alpha, value);
    }
    
    // Update the history table
    if (ht_contains(history_table, best_action))
        history_table[best_action]++;
    else
        history_table[best_action] = 1;

    return value;
}

// Time-Limited Iterative-Deepening Depth-Limited MiniMax with alpha-beta pruning and Quiescence Search and History Table
// Returns a utility value
int tliddlmabpqsht_min_value(State state, int alpha, int beta, std::vector<int> move_history, std::unordered_map<int, int> &history_table) {
    int terminal_result = terminal_test(state, move_history);

    if (terminal_result != INTERNAL_NODE) {
        // This is a terminal node
        return state.utility(terminal_result);
    }

    int value = MAX_VALUE;
    int best_action = 0;
    int new_value;
    
    state.actions = ht_sort(state.actions, history_table);

    for (auto &action : state.actions) {
        move_history.push_back(action);
        new_value = tliddlmabpqsht_max_value(state.result(action), alpha, beta, move_history, history_table);
        move_history.pop_back();

        // Get the new min value and best action
        if (new_value < value) {
            value = new_value;
            best_action = action;
        }

        if (value <= alpha) {
            // Fail low, prune

            // Update the history table
            if (ht_contains(history_table, action))
                history_table[action]++;
            else
                history_table[action] = 1;

            return value;
        }
        
        beta = std::min(beta, value);
    }

    // Update the history table
    if (ht_contains(history_table, best_action))
        history_table[best_action]++;
    else
        history_table[best_action] = 1;
    
    return value;
}

// Returns an action
int time_limited_iterative_deepening_depth_limited_minimax_alpha_beta_pruning_quiescence_search_history_table(std::string initial_fen, bool max_player_color, std::vector<int> move_history, double time_remaining_ns) {
    int value = MIN_VALUE;
    int best_value = MIN_VALUE;
    int best_action = 0;
    int prev_depth_best_action = 0;
    int alpha;
    int beta;
    int terminal_result;
    std::unordered_map<int, int> history_table;

    // Determine allocated time for this move
    double end_time = GET_TIME_NS() + (time_remaining_ns / ESTIMATED_REMAINING_MOVES);

    int depth_limit = 1;
    while (true) {
        print("Depth" + std::to_string(depth_limit));

        // NOTE: Depth information (both for regular and quiescent depth) is encoded in the State class
        State state = State(ChessBoard(initial_fen), depth_limit, MAX_QS_DEPTH, max_player_color);
        terminal_result = terminal_test(state, move_history);

        // Return this state's action with value found from the max value function
        if (terminal_result != INTERNAL_NODE) {
            // The search cannot start in a terminal node
            print("The search cannot start in a terminal node.");
            return 0;
        } else {
            alpha = INIT_ALPHA;
            beta  = INIT_BETA;

            for (auto &action : state.actions) {
                move_history.push_back(action);
                value = tliddlmabpqsht_min_value(state.result(action), alpha, beta, move_history, history_table);
                move_history.pop_back();

                if (value > best_value) {
                    best_value = value;
                    best_action = action;
                }
                
                if (value >= beta)
                    // Fail high, prune
                    return action;
                
                alpha = std::max(alpha, value);
                
                // Check for timeout
                if (GET_TIME_NS() > (end_time)) {
                    print("TIMEOUT");
                    return prev_depth_best_action;
                }
            }
        }

        prev_depth_best_action = best_action;

        // Update the history table
        if (ht_contains(history_table, best_action))
            history_table[best_action]++;
        else
            history_table[best_action] = 1;

        depth_limit++;
    }
    
    // Just in case the while loop is broken out of
    return best_action;
}
