#ifndef CHESSBOARD_HPP
#define CHESSBOARD_HPP

#include "constants.hpp"
#include "piecemoves.hpp"
#include "util.hpp"
#include <algorithm>
#include <iostream>
#include <set>
#include <stdlib.h>
#include <vector>

class ChessBoard {
private:
    U64 get_rook_moves(U64 rook, bool color, bool hypothetical);
    U64 get_bishop_moves(U64 bishop, bool color, bool hypothetical);
    bool get_enemy_color(bool color);
    void get_moves_for_color(std::vector<PieceMoves> &moves_for_color, bool color, bool hypothetical = false);
    U64 get_white(void);
    U64 get_black(void);
    U64 get_empty(void);
    U64 get_king_killshots(U64 king, PieceMoves enemy_piecemoves);
    bool is_sliding_move(int move_type);
    int get_king_index_from_piecemoves_list(std::vector<PieceMoves> piecemoves);
    U64 get_pinner(U64 king, U64 piece, std::vector<PieceMoves> enemy_piecemoves);
    U64 remove_sliding_path_moves(U64 king, U64 king_moves, std::vector<PieceMoves> enemy_piecemoves);
    U64 get_canceling_moves(PieceMoves piecemove, std::vector<PieceMoves> moves_attacking_king);
    void get_castling_moves(std::vector<int> &castling_moves, bool color, U64 all_enemy_moves, U64 king, U64 rooks);

public:
    std::vector<U64> bitboards;
    bool white_can_castle_kingside;
    bool white_can_castle_queenside;
    bool black_can_castle_kingside;
    bool black_can_castle_queenside;
    bool stalemate;
    bool in_check;
    std::string en_passant_str;
    int half_moves;
    int whole_moves;
    bool color;

    ChessBoard(std::string fen="");
    void actions(std::vector<int> &moves);
    U64 get_bitboard(int bitboard_index);
    U64 get_all(void);
    void get_moves(std::vector<int> &moves, std::vector<PieceMoves> piecemoves, std::vector<int> castling_moves, std::unordered_map<U64, U64> blacklist);
    ChessBoard apply_move(int move);
};

#endif // CHESSBOARD_HPP
