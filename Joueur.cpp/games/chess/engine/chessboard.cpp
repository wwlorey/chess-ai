#include "chessboard.hpp"

// Unpacks & interprets the given FEN string
ChessBoard::ChessBoard(std::string fen) {
    // Initialize the bitboards with 12 empty 64b integers
    this->bitboards = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    this->stalemate = false;

    if (fen != "") {
        // Parse the given FEN string
        std::vector<std::string> split_fen;
        split_fen.reserve(6);
        split_fen = split(fen, ' ');

        // Determine piece locations
        int bit_index = 0;  // Index of the bit to set in a bitboard
        bool inc_bit_index; // The bit index is conditionally incremented

        for (int piece_index = 0; piece_index < split_fen[PIECE_LOCATIONS].size(); piece_index++) {
            inc_bit_index = true;

            switch (split_fen[PIECE_LOCATIONS][piece_index]) {
            case WK_FEN:
                this->bitboards[WK] |= shift_left(1, bit_index);
                break;

            case WQ_FEN:
                this->bitboards[WQ] |= shift_left(1, bit_index);
                break;

            case WB_FEN:
                this->bitboards[WB] |= shift_left(1, bit_index);
                break;

            case WR_FEN:
                this->bitboards[WR] |= shift_left(1, bit_index);
                break;

            case WN_FEN:
                this->bitboards[WN] |= shift_left(1, bit_index);
                break;

            case WP_FEN:
                this->bitboards[WP] |= shift_left(1, bit_index);
                break;

            case BK_FEN:
                this->bitboards[BK] |= shift_left(1, bit_index);
                break;

            case BQ_FEN:
                this->bitboards[BQ] |= shift_left(1, bit_index);
                break;

            case BB_FEN:
                this->bitboards[BB] |= shift_left(1, bit_index);
                break;

            case BR_FEN:
                this->bitboards[BR] |= shift_left(1, bit_index);
                break;

            case BN_FEN:
                this->bitboards[BN] |= shift_left(1, bit_index);
                break;

            case BP_FEN:
                this->bitboards[BP] |= shift_left(1, bit_index);
                break;

            case DELIMETER_FEN:
                // Nothing to do here
                inc_bit_index = false;
                break;

            default:
                // This char is an integer representing the number of empty squares
                // Increment bit index by this amount
                bit_index += atoi(&split_fen[PIECE_LOCATIONS][piece_index]);
                inc_bit_index = false;

            } // switch case on FEN piece chars

            if (inc_bit_index)
                bit_index++;

        } // for loop through FEN piece placements

        // Castling
        this->white_can_castle_kingside = str_contains(split_fen[CASTLING], WHITE_CASTLE_KINGSIDE_FEN);
        this->white_can_castle_queenside = str_contains(split_fen[CASTLING], WHITE_CASTLE_QUEENSIDE_FEN);
        this->black_can_castle_kingside = str_contains(split_fen[CASTLING], BLACK_CASTLE_KINGSIDE_FEN);
        this->black_can_castle_queenside = str_contains(split_fen[CASTLING], BLACK_CASTLE_QUEENSIDE_FEN);

        // En passant
        this->en_passant_str = split_fen[EN_PASSANT];

        // Half moves & whole moves
        this->half_moves = stoi(split_fen[HALF_MOVES]);
        this->whole_moves = stoi(split_fen[WHOLE_MOVES]);
        
        // Active color
        this->color = (split_fen[ACTIVE_COLOR] == "w") ? WHITE : BLACK;
    } else {
        // No fen was provided, use default values
        // Castling
        this->white_can_castle_kingside = false;
        this->white_can_castle_queenside = false;
        this->black_can_castle_kingside = false;
        this->black_can_castle_queenside = false;

        // En passant
        this->en_passant_str = NO_EN_PASSANT_STR;

        // Half moves & whole moves
        this->half_moves = 0;
        this->whole_moves = 0;
        
        // Active color
        this->color = 0;
    }
}

// Returns a list of all valid moves (SAN strings) for pieces of given color on the board
void ChessBoard::actions(std::vector<int> &moves) {
    bool enemy_color = this->get_enemy_color(this->color);
    std::vector<PieceMoves> enemy_piecemoves;
    enemy_piecemoves.reserve(MAX_NUM_MOVES); 
    this->get_moves_for_color(enemy_piecemoves, enemy_color, true);

    // Determine if our king is in check and if so, what moves put it in check
    U64 king = (this->color == WHITE) ? this->bitboards[WK] : this->bitboards[BK];
    std::vector<PieceMoves> moves_attacking_king;
    moves_attacking_king.reserve(10);
    std::set<U64> pieces_attacking_king;
    U64 all_enemy_moves = 0;
    PieceMoves new_piecemoves = PieceMoves(0, 0, 0);
    this->in_check = false;

    for (int p = 0; p < enemy_piecemoves.size(); p++) {
        if (enemy_piecemoves[p].moves & king) {
            // The king is in check
            this->in_check = true;

            // Find which moves attack the king for this piece
            U64 killshot_moves = this->get_king_killshots(king, enemy_piecemoves[p]);
            new_piecemoves = PieceMoves(enemy_piecemoves[p].move_type, enemy_piecemoves[p].piece, killshot_moves);
            moves_attacking_king.emplace_back(new_piecemoves);
            pieces_attacking_king.insert(new_piecemoves.piece);
        }

        // Add these moves to the enemy's possible moves
        all_enemy_moves |= enemy_piecemoves[p].moves;
    }

    // Get moves for our color
    std::vector<PieceMoves> piecemoves;
    piecemoves.reserve(MAX_NUM_MOVES);
    this->get_moves_for_color(piecemoves, this->color);

    // Find where the king piecemove is stored
    int king_index = this->get_king_index_from_piecemoves_list(piecemoves);

    // Don't allow friendly king to move into attacked squares
    piecemoves[king_index].moves &= ~all_enemy_moves;

    // Find pinned pieces (pieces that expose the king if moved)
    std::unordered_map<U64, U64> pinned_pieces_to_pinners;
    U64 pinner;
    for (int p = 0; p < piecemoves.size(); p++) {
        pinner = get_pinner(king, piecemoves[p].piece, enemy_piecemoves);
        if (pinner)
            pinned_pieces_to_pinners[piecemoves[p].piece] = pinner;
    }

    std::vector<int> castling_moves;
    castling_moves.reserve(2);
    std::vector<PieceMoves> valid_piecemoves;
    valid_piecemoves.reserve(MAX_NUM_MOVES);
    U64 canceling_moves;
    if (this->in_check) {
        // The king is in check, get out of check
        // Make sure the king doesn't move into the path of a sliding piece
        piecemoves[king_index].moves = this->remove_sliding_path_moves(king, piecemoves[king_index].moves, enemy_piecemoves);

        // Initialize the valid moves with moves for the king
        valid_piecemoves.emplace_back(piecemoves[king_index]);

        for (int p = 0; p < piecemoves.size(); p++) {
            // Get moves that cancel out enemy moves attacking the king
            canceling_moves = this->get_canceling_moves(piecemoves[p], moves_attacking_king);

            if (canceling_moves) {
                // These moves cancel the attacking moves
                // Add them to the moves to return
                piecemoves[p].moves = canceling_moves;
                valid_piecemoves.emplace_back(piecemoves[p]);

                // This piece isn't pinned if it can get the king out of check
                if (map_contains(pinned_pieces_to_pinners, piecemoves[p].piece) && set_contains(pieces_attacking_king, pinned_pieces_to_pinners[piecemoves[p].piece]))
                    pinned_pieces_to_pinners.erase(pinned_pieces_to_pinners.find(piecemoves[p].piece));
            }
        }
    }
    else {
        // The king is not in check so check avoidance is not necessary
        // Address castling
        U64 rooks = (this->color == WHITE) ? this->bitboards[WR] : this->bitboards[BR];
        this->get_castling_moves(castling_moves, this->color, all_enemy_moves, king, rooks);

        valid_piecemoves = piecemoves;
    }
    
    // Get the vector of moves
    this->get_moves(moves, valid_piecemoves, castling_moves, pinned_pieces_to_pinners);
    
    // Mix 'em up
    std::random_shuffle(moves.begin(), moves.end());
}

// Returns all valid PieceMoves for a given color
void ChessBoard::get_moves_for_color(std::vector<PieceMoves> &moves_for_color, bool color, bool hypothetical) {
    U64 friendly_pieces = (color == WHITE) ? this->get_white() : this->get_black();
    U64 enemy_pieces    = (color == WHITE) ? this->get_black() : this->get_white();
    U64 all_pieces = this->get_all();
    std::vector<int> bitboard_indices;
    bitboard_indices.reserve(6);
    bitboard_indices = (color == WHITE) ? WHITE_BITBOARD_INDICES : BLACK_BITBOARD_INDICES;

    int bitboard_index;
    for (int k = 0; k < bitboard_indices.size(); k++) {
        bitboard_index = bitboard_indices[k];

        std::vector<U64> pieces;
        pieces.reserve(2);
        pieces = split_bitboard(this->bitboards[bitboard_index]);

        for (int piece_index = 0; piece_index < pieces.size(); piece_index++) {
            U64 piece = pieces[piece_index];
            U64 valid_moves_for_piece = 0;

            // Generate moves for this piece
            // Handle sliding pieces
            if (bitboard_index == WR || bitboard_index == BR) {
                valid_moves_for_piece = get_rook_moves(piece, color, hypothetical);
                moves_for_color.emplace_back(PieceMoves(ROOK, piece, valid_moves_for_piece));
            }
            else if (bitboard_index == WB || bitboard_index == BB) {
                valid_moves_for_piece = get_bishop_moves(piece, color, hypothetical);
                moves_for_color.emplace_back(PieceMoves(BISHOP, piece, valid_moves_for_piece));
            }
            else if (bitboard_index == WQ || bitboard_index == BQ) {
                // The queen can make both rook and bishop moves
                valid_moves_for_piece = get_rook_moves(piece, color, hypothetical) | get_bishop_moves(piece, color, hypothetical);
                moves_for_color.emplace_back(PieceMoves(QUEEN, piece, valid_moves_for_piece));
            }

            // Handle non-sliding pieces
            else if (bitboard_index == WK || bitboard_index == BK) {
                // King moves
                valid_moves_for_piece |= KING_MOVES[piece];

                if (!hypothetical) {
                    // Mask away moves landing on friendly pieces
                    valid_moves_for_piece &= ~friendly_pieces;
                }

                moves_for_color.emplace_back(PieceMoves(KING, piece, valid_moves_for_piece));
            }
            else if (bitboard_index == WN || bitboard_index == BN) {
                // Knight moves
                valid_moves_for_piece |= KNIGHT_MOVES[piece];

                if (!hypothetical) {
                    valid_moves_for_piece &= ~friendly_pieces;
                }

                moves_for_color.emplace_back(PieceMoves(KNIGHT, piece, valid_moves_for_piece));
            }
            else if (bitboard_index == WP || bitboard_index == BP) {
                // Pawn moves
                // Attack moves
                if (bitboard_index == BP) {
                    valid_moves_for_piece |= shift_left(piece, 7);
                    valid_moves_for_piece |= shift_left(piece, 9);
                }
                else { // if (bitboard_index == WP)
                    valid_moves_for_piece |= shift_right(piece, 7);
                    valid_moves_for_piece |= shift_right(piece, 9);
                }

                // Clean up moves if necessary
                if (piece & FILE_A) {
                    valid_moves_for_piece &= FILE_A | FILE_B;
                }
                else if (piece & FILE_H) {
                    valid_moves_for_piece &= FILE_G | FILE_H;
                }

                if (!hypothetical) {
                    valid_moves_for_piece &= ~friendly_pieces;

                    // Generate en passant move if it exists
                    U64 en_passant_move = valid_moves_for_piece & FILE_RANK_TO_PIECE[this->en_passant_str];

                    // Ensure pawn attack moves collide with an enemy piece
                    if ((valid_moves_for_piece & enemy_pieces) == 0) {
                        // No overlap with enemies so no valid moves can be made
                        valid_moves_for_piece = 0;
                    }
                    else {
                        // There is overlap with enemies
                        valid_moves_for_piece = valid_moves_for_piece & enemy_pieces;
                    }

                    // Save the en passant move
                    valid_moves_for_piece |= en_passant_move;
                }

                moves_for_color.emplace_back(PieceMoves(PAWN_ATTACK, piece, valid_moves_for_piece));

                if (hypothetical) {
                    // We don't care about standard pawn moves, they can't attack pieces
                    continue;
                }

                // Reset and calculate standard pawn moves
                valid_moves_for_piece = 0;

                if (bitboard_index == BP) {
                    valid_moves_for_piece |= shift_left(piece, 8);

                    // Check if a double move is possible
                    if (piece & RANK_7 && !(all_pieces & shift_left(piece, 8))) {
                        valid_moves_for_piece |= shift_left(piece, 16);
                    }
                }
                else { // if (bitboard_index == WP)
                    valid_moves_for_piece |= shift_right(piece, 8);

                    // Check if a double move is possible
                    if (piece & RANK_2 && !(all_pieces & shift_right(piece, 8))) {
                        valid_moves_for_piece |= shift_right(piece, 16);
                    }
                }

                // Clean up moves if necessary
                if (piece & FILE_A) {
                    valid_moves_for_piece &= FILE_A | FILE_B;
                }
                else if (piece & FILE_H) {
                    valid_moves_for_piece &= FILE_G | FILE_H;
                }

                if (!hypothetical) {
                    valid_moves_for_piece &= ~friendly_pieces;

                    // These moves cannot be made in attack
                    valid_moves_for_piece &= ~enemy_pieces;
                }

                moves_for_color.emplace_back(PieceMoves(PAWN, piece, valid_moves_for_piece));
            }
        }
    }
}

// Returns the rook's possible moves as a single bitboard
U64 ChessBoard::get_rook_moves(U64 rook, bool color, bool hypothetical) {
    U64 empty = this->get_empty();

    U64 rook_moves = e_ray(rook, empty) | w_ray(rook, empty) | n_ray(rook, empty) | s_ray(rook, empty);

    if (!hypothetical) {
        // Make sure friend-occupied spaces can't be moved to
        rook_moves &= ~((color == WHITE) ? this->get_white() : this->get_black());
    }

    return rook_moves;
}

// Returns the bishop's possible moves as a single bitboard
U64 ChessBoard::get_bishop_moves(U64 bishop, bool color, bool hypothetical) {
    U64 empty = this->get_empty();

    U64 bishop_moves = ne_ray(bishop, empty) | nw_ray(bishop, empty) | sw_ray(bishop, empty) | se_ray(bishop, empty);

    if (!hypothetical) {
        // Make sure friend-occupied spaces can't be moved to
        bishop_moves &= ~((color == WHITE) ? this->get_white() : this->get_black());
    }

    return bishop_moves;
}

// Returns the bitboard at given bitboard index
U64 ChessBoard::get_bitboard(int bitboard_index) {
    return this->bitboards[bitboard_index];
}

// Returns the enemy's color given the friendly color
bool ChessBoard::get_enemy_color(bool color) {
    return (color == WHITE) ? BLACK : WHITE;
}

U64 ChessBoard::get_white(void) {
    return this->bitboards[WK] | this->bitboards[WQ] | this->bitboards[WR] |
           this->bitboards[WB] | this->bitboards[WN] | this->bitboards[WP];
}

U64 ChessBoard::get_black(void) {
    return this->bitboards[BK] | this->bitboards[BQ] | this->bitboards[BR] |
           this->bitboards[BB] | this->bitboards[BN] | this->bitboards[BP];
}

U64 ChessBoard::get_all(void) {
    return this->get_white() | this->get_black();
}

U64 ChessBoard::get_empty(void) {
    return ~(this->get_all());
}

// Returns a bitboard of moves that attack the king from enemy_piecemoves
U64 ChessBoard::get_king_killshots(U64 king, PieceMoves enemy_piecemoves) {
    // First deal with sliding moves
    if (this->is_sliding_move(enemy_piecemoves.move_type)) {
        // Check if any ray from enemy collides with the king
        std::vector<U64> rays;
        rays.reserve(8);
        rays = RAYS_LIST[enemy_piecemoves.piece];

        for (int r = 0; r < rays.size(); r++) {
            if (king & enemy_piecemoves.moves & rays[r]) {
                // This ray contains an attack on the king
                return enemy_piecemoves.moves & rays[r];
            }
        }

        // No killshots from this enemy
        return 0;
    }
    else {
        // this is a non-sliding piece
        // Only enemy moves that intersect with the king are killshots
        return enemy_piecemoves.moves & king;
    }
}

// Returns true if the move type is sliding, false otherwise
bool ChessBoard::is_sliding_move(int move_type) {
    return move_type == QUEEN || move_type == BISHOP || move_type == ROOK;
}

// Returns the index of the king in piecemoves
int ChessBoard::get_king_index_from_piecemoves_list(std::vector<PieceMoves> piecemoves) {
    for (int i = 0; i < piecemoves.size(); i++) {
        if (piecemoves[i].move_type == KING)
            return i;
    }

    return -1;
}

// Returns the enemy that pins piece and zero if no piece is pinned
// Where a pinned piece cannot move because if it does, it exposes the king to an enemy
U64 ChessBoard::get_pinner(U64 king, U64 piece, std::vector<PieceMoves> enemy_piecemoves) {
    if (piece == king)
        // The king can't be pinned
        return 0;

    std::set<U64>::iterator ray; 
    U64 possible_blockers;
    std::vector<U64> split_possible_blockers;
    split_possible_blockers.reserve(5);
    bool pinned;
    
    for (int p = 0; p < enemy_piecemoves.size(); p++) {
        // Only deal with sliding enemies
        // They are the only piece types that can pin other pieces
        if (this->is_sliding_move(enemy_piecemoves[p].move_type)) {
            // Check for the possibility of a pinned piece
            if (RAYS[enemy_piecemoves[p].piece] & RAYS[king] & piece) {
                // This piece may be pinned
                if (enemy_piecemoves[p].move_type == ROOK) {
                    for (ray = HV_RAYS_SET.begin(); ray != HV_RAYS_SET.end(); ++ray) {
                        if ((*ray | enemy_piecemoves[p].piece | king | piece) == *ray) {
                            // These pieces occupy the same ray
                            possible_blockers = (*ray & ~(enemy_piecemoves[p].piece | king | piece)) & this->get_all();
                            
                            if (possible_blockers) {                            
                                split_possible_blockers = split_bitboard(possible_blockers);
                                
                                pinned = true;

                                for (int b = 0; b < split_possible_blockers.size(); b++) {
                                    // Ensure the order is as expected for pinned pieces
                                    if (!((split_possible_blockers[b] > king && king > piece && piece > enemy_piecemoves[p].piece) || 
                                        (split_possible_blockers[b] < king && king < piece && piece < enemy_piecemoves[p].piece) ||
                                        (king > piece && piece > enemy_piecemoves[p].piece && enemy_piecemoves[p].piece > split_possible_blockers[b]) || 
                                        (king < piece && piece < enemy_piecemoves[p].piece && enemy_piecemoves[p].piece < split_possible_blockers[b]))) {
                                        pinned = false;
                                        break;
                                    }
                                }

                                if (pinned)
                                    // This piece is pinned, return the enemy pinner
                                    return enemy_piecemoves[p].piece;
                            } else {
                                if ((king > piece && piece > enemy_piecemoves[p].piece) || (king < piece && piece < enemy_piecemoves[p].piece)) {
                                    // This piece is pinned, return the enemy pinner
                                    return enemy_piecemoves[p].piece;
                                }
                            }
                        }
                    }
                } else if (enemy_piecemoves[p].move_type == BISHOP) {
                    for (ray = DIAG_RAYS_SET.begin(); ray != DIAG_RAYS_SET.end(); ++ray) {
                        if ((*ray | enemy_piecemoves[p].piece | king | piece) == *ray) {
                            // These pieces occupy the same ray
                            possible_blockers = (*ray & ~(enemy_piecemoves[p].piece | king | piece)) & this->get_all();
                            
                            if (possible_blockers) {                            
                                split_possible_blockers = split_bitboard(possible_blockers);
                                
                                pinned = true;

                                for (int b = 0; b < split_possible_blockers.size(); b++) {
                                    // Ensure the order is as expected for pinned pieces
                                    if (!((split_possible_blockers[b] > king && king > piece && piece > enemy_piecemoves[p].piece) || 
                                        (split_possible_blockers[b] < king && king < piece && piece < enemy_piecemoves[p].piece) ||
                                        (king > piece && piece > enemy_piecemoves[p].piece && enemy_piecemoves[p].piece > split_possible_blockers[b]) || 
                                        (king < piece && piece < enemy_piecemoves[p].piece && enemy_piecemoves[p].piece < split_possible_blockers[b]))) {
                                        pinned = false;
                                        break;
                                    }
                                }

                                if (pinned)
                                    // This piece is pinned, return the enemy pinner
                                    return enemy_piecemoves[p].piece;
                            } else {
                                if ((king > piece && piece > enemy_piecemoves[p].piece) || (king < piece && piece < enemy_piecemoves[p].piece)) {
                                    // This piece is pinned, return the enemy pinner
                                    return enemy_piecemoves[p].piece;
                                }
                            }
                        }
                    }
                } else  { // enemy_piecemoves[p].move_type == QUEEN
                    for (ray = RAYS_SET.begin(); ray != RAYS_SET.end(); ++ray) {
                        if ((*ray | enemy_piecemoves[p].piece | king | piece) == *ray) {
                            // These pieces occupy the same ray
                            possible_blockers = (*ray & ~(enemy_piecemoves[p].piece | king | piece)) & this->get_all();
                            
                            if (possible_blockers) {                            
                                split_possible_blockers = split_bitboard(possible_blockers);
                                
                                pinned = true;

                                for (int b = 0; b < split_possible_blockers.size(); b++) {
                                    // Ensure the order is as expected for pinned pieces
                                    if (!((split_possible_blockers[b] > king && king > piece && piece > enemy_piecemoves[p].piece) || 
                                        (split_possible_blockers[b] < king && king < piece && piece < enemy_piecemoves[p].piece) ||
                                        (king > piece && piece > enemy_piecemoves[p].piece && enemy_piecemoves[p].piece > split_possible_blockers[b]) || 
                                        (king < piece && piece < enemy_piecemoves[p].piece && enemy_piecemoves[p].piece < split_possible_blockers[b]))) {
                                        pinned = false;
                                        break;
                                    }
                                }

                                if (pinned)
                                    // This piece is pinned, return the enemy pinner
                                    return enemy_piecemoves[p].piece;
                            } else {
                                if ((king > piece && piece > enemy_piecemoves[p].piece) || (king < piece && piece < enemy_piecemoves[p].piece)) {
                                    // This piece is pinned, return the enemy pinner
                                    return enemy_piecemoves[p].piece;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return 0;
}

// Returns moves for the king that are not in the path of sliding enemy pieces
U64 ChessBoard::remove_sliding_path_moves(U64 king, U64 king_moves, std::vector<PieceMoves> enemy_piecemoves) {
    for (int e = 0; e < enemy_piecemoves.size(); e++) {
        // Only deal with sliding piece moves
        if (this->is_sliding_move(enemy_piecemoves[e].move_type)) {
            // Get the rays starting from this piece
            std::vector<U64> rays;
            rays.reserve(8);
            rays = RAYS_LIST[enemy_piecemoves[e].piece];

            for (int r = 0; r < rays.size(); r++) {
                if (king & enemy_piecemoves[e].moves & rays[r]) {
                    // The king may attempt to move along this ray. Prevent that
                    king_moves &= ~rays[r];
                }
            }
        }
    }

    return king_moves;
}

// Returns a bitboard of moves that cancel all moves attacking the king (if they exist)
// 0 is returned otherwise
U64 ChessBoard::get_canceling_moves(PieceMoves piecemoves, std::vector<PieceMoves> moves_attacking_king) {
    U64 canceling_moves = 0;
    U64 local_canceling_moves;

    for (int i = 0; i < moves_attacking_king.size(); i++) {
        local_canceling_moves = 0;

        // Save moves that take out this attacking piece or that block it from moving
        local_canceling_moves |= (piecemoves.moves & moves_attacking_king[i].piece) |
                                 (piecemoves.moves & moves_attacking_king[i].moves);

        if (!local_canceling_moves) {
            // This attacker could not be neutralized
            return 0;
        }

        canceling_moves |= local_canceling_moves;
    }

    return canceling_moves;
}

// Returns a list of possible castling moves for color if they can be made
void ChessBoard::get_castling_moves(std::vector<int> &castling_moves, bool color, U64 all_enemy_moves, U64 king, U64 rooks) {
    if (color == WHITE) {
        if (this->white_can_castle_kingside &&
            WHITE_CASTLE_KINGSIDE_MASK & king &&
            WHITE_CASTLE_KINGSIDE_MASK & rooks &&
            !(WHITE_CASTLE_KINGSIDE_INVALID & this->get_all()) &&
            !(WHITE_CASTLE_KINGSIDE_INVALID & all_enemy_moves) &&
            !(WHITE_CASTLE_KINGSIDE_KING_MOVE & all_enemy_moves)) {
            // White can castle kingside
            castling_moves.emplace_back(KINGSIDE_CASTLE_MOVE_MASK);
        }

        if (this->white_can_castle_queenside &&
            WHITE_CASTLE_QUEENSIDE_MASK & king &&
            WHITE_CASTLE_QUEENSIDE_MASK & rooks &&
            !(WHITE_CASTLE_QUEENSIDE_INVALID & this->get_all()) &&
            !(WHITE_CASTLE_QUEENSIDE_INVALID & all_enemy_moves) &&
            !(WHITE_CASTLE_QUEENSIDE_KING_MOVE & all_enemy_moves)) {
            // White can castle queenside
            castling_moves.emplace_back(QUEENSIDE_CASTLE_MOVE_MASK);
        }
    }
    else { // color == BLACK 
        if (this->black_can_castle_kingside &&
            BLACK_CASTLE_KINGSIDE_MASK & king &&
            BLACK_CASTLE_KINGSIDE_MASK & rooks &&
            !(BLACK_CASTLE_KINGSIDE_INVALID & this->get_all()) &&
            !(BLACK_CASTLE_KINGSIDE_INVALID & all_enemy_moves) &&
            !(BLACK_CASTLE_KINGSIDE_KING_MOVE & all_enemy_moves)) {
            // Black can castle kingside
            castling_moves.emplace_back(KINGSIDE_CASTLE_MOVE_MASK);
        }

        if (this->black_can_castle_queenside &&
            BLACK_CASTLE_QUEENSIDE_MASK & king &&
            BLACK_CASTLE_QUEENSIDE_MASK & rooks &&
            !(BLACK_CASTLE_QUEENSIDE_INVALID & this->get_all()) &&
            !(BLACK_CASTLE_QUEENSIDE_INVALID & all_enemy_moves) &&
            !(BLACK_CASTLE_QUEENSIDE_KING_MOVE & all_enemy_moves)) {
            // Black can castle queenside
            castling_moves.emplace_back(QUEENSIDE_CASTLE_MOVE_MASK);
        }
    }
}

void ChessBoard::get_moves(std::vector<int> &moves, std::vector<PieceMoves> piecemoves, std::vector<int> castling_moves, std::unordered_map<U64, U64> blacklist) {
    int move, piece_move_mask;
    U64 enemy_pieces = (this->color == WHITE) ? this->get_black() : this->get_white();
    std::vector<U64> moves_for_piece;
    moves_for_piece.reserve(MAX_NUM_MOVES / 3);

    for (int p = 0; p < piecemoves.size(); p++) {
        if (map_contains(blacklist, piecemoves[p].piece)) {
            // Don't generate moves for blacklisted pieces
            continue;
        }

        // Separate out possible moves
        moves_for_piece = split_bitboard(piecemoves[p].moves);

        if (!moves_for_piece.size()) {
            // No valid moves for this piece
            continue;
        }

        // Set the piece type mask
        piece_move_mask = 0;
        switch (piecemoves[p].move_type) {
            case KING:
                piece_move_mask |= KING_MOVE_MASK;
                break;
            case QUEEN:
                piece_move_mask |= QUEEN_MOVE_MASK;
                break;
            case BISHOP:
                piece_move_mask |= BISHOP_MOVE_MASK;
                break;
            case ROOK:
                piece_move_mask |= ROOK_MOVE_MASK;
                break;
            case KNIGHT:
                piece_move_mask |= KNIGHT_MOVE_MASK;
                break;
            default: // Default to pawn
                piece_move_mask |= PAWN_MOVE_MASK;
                break;
        }

        // Generate moves
        int rand_promotion_index;
        for (int m = 0; m < moves_for_piece.size(); m++) {
            if (moves_for_piece[m]) {
                move = piece_move_mask;
                move |= PIECE_TO_FILE_RANK_MOVE_MASK__FROM[piecemoves[p].piece];
                move |= (moves_for_piece[m] & enemy_pieces) ? ATTACK_MOVE_MASK : 0;
                move |= PIECE_TO_FILE_RANK_MOVE_MASK__TO[moves_for_piece[m]];

                if (((piece_move_mask & PAWN_MOVE_MASK) == PAWN_MOVE_MASK) && ((piecemoves[p].piece & RANK_2 && this->color == BLACK) || (piecemoves[p].piece & RANK_7 && this->color == WHITE))) {
                    // This pawn can be promoted. Choose promotion type randomly
                    rand_promotion_index = rand() % LEN_PAWN_PROMOTION_CHOICES;
                    move |= PAWN_PROMOTION_MOVE_MASK_CHOICES[rand_promotion_index];
                }

                moves.emplace_back(move);
            }
        }
    }

    for (int c = 0; c < castling_moves.size(); c++)
        moves.emplace_back(castling_moves[c]);

    this->stalemate = !moves.size() && !this->in_check;
}

// Returns a new ChessBoard object with move applied
ChessBoard ChessBoard::apply_move(int move) {
    bool attack = false;
    bool pawn_promotion = false;
    bool pawn_moving = false;

    int pawn_promotion_bitboard_index;
    int new_king_bitboard_index = -1;
    int new_rook_bitboard_index = -1;

    U64 old_king_rook = 0;
    U64 new_king = 0;
    U64 new_rook = 0; 
    U64 from = 0;
    U64 to = 0; 

    ChessBoard new_board = ChessBoard();
    
    if (!(move & CASTLE_MOVE_MASK)) { 
        // This move does not involve castling
        attack = move & ATTACK_MOVE_MASK;
        
        // Get start (from) & end (to) piece masks
        from = FILE_RANK_MOVE_MASK_TO_PIECE__FROM[move & (FROM_FILE_MOVE_MASK | FROM_RANK_MOVE_MASK)];
        to   = FILE_RANK_MOVE_MASK_TO_PIECE__TO[move & (TO_FILE_MOVE_MASK | TO_RANK_MOVE_MASK)];
            
        pawn_moving = (move & PIECE_MOVE_MASK) == PAWN_MOVE_MASK;
        pawn_promotion = pawn_moving && (move & PROMO_MOVE_MASK);
        
        if (pawn_promotion) {
            if ((move & PROMO_MOVE_MASK) == QUEEN_PROMO_MOVE_MASK)
                pawn_promotion_bitboard_index = (this->color == WHITE) ? WQ : BQ;
            else if ((move & PROMO_MOVE_MASK) == BISHOP_PROMO_MOVE_MASK)
                pawn_promotion_bitboard_index = (this->color == WHITE) ? WB : BB;
            else if ((move & PROMO_MOVE_MASK) == ROOK_PROMO_MOVE_MASK)
                pawn_promotion_bitboard_index = (this->color == WHITE) ? WR : BR;
            else // (move & PROMO_MOVE_MASK) == KNIGHT_PROMO_MOVE_MASK
                pawn_promotion_bitboard_index = (this->color == WHITE) ? WN : BN;
        }
        
        // Check for en passant opportunity
        if ((this->color == WHITE) && (from == (to << 16)))
            new_board.en_passant_str = PIECE_TO_FILE_RANK[to << 8];
        
        else if ((this->color == WHITE) && (from == (to >> 16)))
            new_board.en_passant_str = PIECE_TO_FILE_RANK[to >> 8];
        
        else
            new_board.en_passant_str = NO_EN_PASSANT_STR;
        
        // Castling did not occur
        new_board.white_can_castle_kingside = this->white_can_castle_kingside;
        new_board.white_can_castle_queenside = this->white_can_castle_queenside;
        new_board.black_can_castle_kingside = this->black_can_castle_kingside;
        new_board.black_can_castle_queenside = this->black_can_castle_queenside;
    } else {
        // Castling ocurred
        if ((move & CASTLE_MOVE_MASK) == KINGSIDE_CASTLE_MOVE_MASK) {
            if (this->color == WHITE) {
                new_board.white_can_castle_kingside = false;
                new_board.white_can_castle_queenside = false;

                old_king_rook = WHITE_CASTLE_KINGSIDE_MASK;
                new_king = WHITE_CASTLE_KINGSIDE_KING_MOVE;
                new_rook = WHITE_CASTLE_KINGSIDE_ROOK_MOVE;
                new_king_bitboard_index = WK;
                new_rook_bitboard_index = WR;
            } else { // this->color == BLACK 
                new_board.black_can_castle_kingside = false;
                new_board.black_can_castle_queenside = false;

                old_king_rook = BLACK_CASTLE_KINGSIDE_MASK;
                new_king = BLACK_CASTLE_KINGSIDE_KING_MOVE;
                new_rook = BLACK_CASTLE_KINGSIDE_ROOK_MOVE;
                new_king_bitboard_index = BK;
                new_rook_bitboard_index = BR;
            }
        } else { // (move & CASTLE_MOVE_MASK) == QUEENSIDE_CASTLE_MOVE_MASK
            if (this->color == WHITE) {
                new_board.white_can_castle_kingside = false;
                new_board.white_can_castle_queenside = false;

                old_king_rook = WHITE_CASTLE_QUEENSIDE_MASK;
                new_king = WHITE_CASTLE_QUEENSIDE_KING_MOVE;
                new_rook = WHITE_CASTLE_QUEENSIDE_ROOK_MOVE;
                new_king_bitboard_index = WK;
                new_rook_bitboard_index = WR;
            } else { // this->color == BLACK
                new_board.black_can_castle_kingside = false;
                new_board.black_can_castle_queenside = false;

                old_king_rook = BLACK_CASTLE_QUEENSIDE_MASK;
                new_king = BLACK_CASTLE_QUEENSIDE_KING_MOVE;
                new_rook = BLACK_CASTLE_QUEENSIDE_ROOK_MOVE;
                new_king_bitboard_index = BK;
                new_rook_bitboard_index = BR;
            }
        }
            
        // No en passant possible after this move
        new_board.en_passant_str = NO_EN_PASSANT_STR;
    }
    
    // Update half moves & whole moves
    if (pawn_moving || attack)
        new_board.half_moves = 0;

    else
        new_board.half_moves = this->half_moves + 1;
    
    new_board.whole_moves = (this->color == BLACK) ? this->whole_moves + 1 : this->whole_moves;

    // Update active color
    new_board.color = (this->color == WHITE) ? BLACK : WHITE;
    
    // Update piece locations
    for (int bitboard_index = 0; bitboard_index < NUM_BITBOARDS; bitboard_index++) {
        // Check castling
        if (old_king_rook & this->bitboards[bitboard_index]) {
            new_board.bitboards[bitboard_index] = this->bitboards[bitboard_index] & (~old_king_rook);
            
            if (bitboard_index == new_king_bitboard_index) {
                new_board.bitboards[bitboard_index] = this->bitboards[bitboard_index] | new_king;
            } else { // bitboard_index == new_rook_bitboard_index
                new_board.bitboards[bitboard_index] = this->bitboards[bitboard_index] | new_rook;
            }
        }
        
        if (attack && (to & this->bitboards[bitboard_index])) {
            // Remove attacked piece
            new_board.bitboards[bitboard_index] = this->bitboards[bitboard_index] & ~to;
        } else if (pawn_promotion) {
            if (bitboard_index == pawn_promotion_bitboard_index) {
                // Promotion bitboard
                new_board.bitboards[bitboard_index] = this->bitboards[bitboard_index] | to;
            } else {
                // Pawn bitboard    
                new_board.bitboards[bitboard_index] = this->bitboards[bitboard_index] & ~from;
            }
        } else if (from & this->bitboards[bitboard_index]) {
            // Move this piece on its bitboard
            new_board.bitboards[bitboard_index] = (this->bitboards[bitboard_index] | to) & ~from;
        } else {
            // No moves made on these bitboards
            new_board.bitboards[bitboard_index] = this->bitboards[bitboard_index];
        }
        
        // if (from & this->bitboards[bitboard_index]) {
        //     // A piece is moving on this bitboard
        //     new_board.bitboards[bitboard_index] = (this->bitboards[bitboard_index] & ~from) | to;
        // } else if (to & this->bitboards[bitboard_index]) {
        //     // A piece is being *captured* (and removed) from this bitboard
        //     new_board.bitboards[bitboard_index] = this->bitboards[bitboard_index] & ~to;
        // }
        // else {
        //     new_board.bitboards[bitboard_index] = this->bitboards[bitboard_index];    
        // }
    }    
    
    return new_board;
}
