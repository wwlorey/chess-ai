#ifndef UTIL_HPP
#define UTIL_HPP

#include "constants.hpp"
#include <chrono>
#include <iostream>
#include <set>
#include <sstream>
#include <unordered_map>
#include <vector>

#define GET_TIME_NS() ((double)(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count()))

U64 shift_left(U64 value, int shift_amount);
U64 shift_right(U64 value, int shift_amount);
U64 n_ray(U64 n_ray, U64 empty=UNIVERSAL_SET);
U64 s_ray(U64 s_ray, U64 empty=UNIVERSAL_SET);
U64 e_ray(U64 e_ray, U64 empty=UNIVERSAL_SET);
U64 w_ray(U64 w_ray, U64 empty=UNIVERSAL_SET);
U64 ne_ray(U64 ne_ray, U64 empty=UNIVERSAL_SET);
U64 se_ray(U64 se_ray, U64 empty=UNIVERSAL_SET);
U64 nw_ray(U64 nw_ray, U64 empty=UNIVERSAL_SET);
U64 sw_ray(U64 sw_ray, U64 empty=UNIVERSAL_SET);
std::vector<std::string> split(std::string str_to_split, char delimeter);
void print(std::string thing_to_print);
void print(void);
void pretty_print(U64 bitboard);
bool str_contains(std::string str1, char str2);
bool map_contains(std::unordered_map<U64, U64> map, U64 item);
bool set_contains(std::set<U64> set, U64 item);
bool ht_contains(std::unordered_map<int, int> ht, int move);
std::vector<U64> split_bitboard(U64 bitboard);

std::unordered_map<std::string, U64> get_file_rank_to_piece_str(void);
extern std::unordered_map<std::string, U64> FILE_RANK_TO_PIECE;

std::unordered_map<U64, std::string> get_piece_to_file_rank(void);
extern std::unordered_map<U64, std::string> PIECE_TO_FILE_RANK;

std::unordered_map<U64, int> get_piece_to_file_rank_move_mask__from(void);
std::unordered_map<U64, int> get_piece_to_file_rank_move_mask__to(void);
extern std::unordered_map<U64, int> PIECE_TO_FILE_RANK_MOVE_MASK__FROM;
extern std::unordered_map<U64, int> PIECE_TO_FILE_RANK_MOVE_MASK__TO;

std::unordered_map<int, U64> get_file_rank_move_mask_to_piece__from(void);
std::unordered_map<int, U64> get_file_rank_move_mask_to_piece__to(void);
extern std::unordered_map<int, U64> FILE_RANK_MOVE_MASK_TO_PIECE__FROM;
extern std::unordered_map<int, U64> FILE_RANK_MOVE_MASK_TO_PIECE__TO;

std::unordered_map<U64, std::vector<U64>> gen_rays_list(void);
extern std::unordered_map<U64, std::vector<U64>> RAYS_LIST;

std::unordered_map<U64, U64> gen_rays(void);
extern std::unordered_map<U64, U64> RAYS;

std::unordered_map<U64, U64> gen_diag_rays(void);
extern std::unordered_map<U64, U64> DIAG_RAYS;

std::set<U64> gen_rays_set(void);
extern std::set<U64> RAYS_SET;

std::set<U64> gen_hv_rays_set(void);
extern std::set<U64> HV_RAYS_SET;

std::set<U64> gen_diag_rays_set(void);
extern std::set<U64> DIAG_RAYS_SET;

std::unordered_map<U64, U64> gen_knight_moves(void);
extern std::unordered_map<U64, U64> KNIGHT_MOVES;

std::unordered_map<U64, U64> gen_king_moves(void);
extern std::unordered_map<U64, U64> KING_MOVES;

std::string get_move_str(int move);
int count_set_bits(U64 bits);

int get_to_file_rank_mask(std::string san, int starting_index);
int server_san_to_move(std::string san);

std::vector<int> ht_sort(std::vector<int> actions, std::unordered_map<int, int> history_table);

#endif // UTIL_HPP
