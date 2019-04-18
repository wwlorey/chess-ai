#ifndef PIECEMOVES_HPP
#define PIECEMOVES_HPP

#include "constants.hpp"
#include "util.hpp"
#include <vector>

class PieceMoves {
public:
    int move_type; // Type of move this piece can make
    U64 piece;     // The bitboard representing this piece
    U64 moves;     // Moves (on a single bitboard) this piece can make

    PieceMoves(int move_type, U64 piece, U64 moves) {
        this->move_type = move_type;
        this->piece = piece;
        this->moves = moves;
    }
};

#endif // PIECEMOVES_HPP
