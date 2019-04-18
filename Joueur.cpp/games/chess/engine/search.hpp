#ifndef SEARCH_HPP
#define SEARCH_HPP

#include "constants.hpp"
#include "util.hpp"
#include "state.hpp"

int terminal_test(State state, std::vector<int> history);

int depth_limited_minimax(State initial_state, std::vector<int> history);
int iterative_deepening_depth_limited_minimax(std::string initial_fen, int max_depth_limit, bool max_player_color, std::vector<int> history);
int max_value(State state, std::vector<int> history);
int min_value(State state, std::vector<int> history);

int time_limited_iterative_deepening_depth_limited_minimax_with_alpha_beta_pruning(std::string initial_fen, bool max_player_color, std::vector<int> history, double time_remaining_ns);
int tliddlmmwabp_max_value(State state, int alpha, int beta, std::vector<int> history);
int tliddlmmwabp_min_value(State state, int alpha, int beta, std::vector<int> history);

int time_limited_iterative_deepening_depth_limited_minimax_alpha_beta_pruning_quiescence_search_history_table(std::string initial_fen, bool max_player_color, std::vector<int> history, double time_remaining_ns); 
int tliddlmabpqsht_max_value(State state, int alpha, int beta, std::vector<int> history, std::unordered_map<int, int> &history_table);
int tliddlmabpqsht_min_value(State state, int alpha, int beta, std::vector<int> history, std::unordered_map<int, int> &history_table);

#endif // SEARCH_HPP