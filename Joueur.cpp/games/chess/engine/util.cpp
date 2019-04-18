#include "util.hpp"

// Shifts value to the left by shift_amount
// This forces value to be a 64b integer
U64 shift_left(U64 value, int shift_amount) {
    return value << shift_amount;
}

// Shifts value to the right by shift_amount
// This forces value to be a 64b integer
U64 shift_right(U64 value, int shift_amount) {
    return value >> shift_amount;
}

// Ray generation for all directions
U64 n_ray(U64 n_ray, U64 empty) {
    n_ray |= empty & shift_left(n_ray, 8);
    empty &= shift_left(empty, 8);
    n_ray |= empty & shift_left(n_ray, 16);
    empty &= shift_left(empty, 16);
    n_ray |= empty & shift_left(n_ray, 32);
    return shift_left(n_ray, 8);
}

U64 s_ray(U64 s_ray, U64 empty) {
    s_ray |= empty & shift_right(s_ray, 8);
    empty &= shift_right(empty, 8);
    s_ray |= empty & shift_right(s_ray, 16);
    empty &= shift_right(empty, 16);
    s_ray |= empty & shift_right(s_ray, 32);
    return shift_right(s_ray, 8);
}

U64 e_ray(U64 e_ray, U64 empty) {
    empty = empty & NOT_FILE_A;
    e_ray |= empty & shift_left(e_ray, 1);
    empty &= shift_left(empty, 1);
    e_ray |= empty & shift_left(e_ray, 2);
    empty &= shift_left(empty, 2);
    e_ray |= empty & shift_left(e_ray, 4);
    return shift_left(e_ray, 1) & NOT_FILE_A;
}

U64 w_ray(U64 w_ray, U64 empty) {
    empty = empty & NOT_FILE_H;
    w_ray |= empty & shift_right(w_ray, 1);
    empty &= shift_right(empty, 1);
    w_ray |= empty & shift_right(w_ray, 2);
    empty &= shift_right(empty, 2);
    w_ray |= empty & shift_right(w_ray, 4);
    return shift_right(w_ray, 1) & NOT_FILE_H;
}

U64 ne_ray(U64 ne_ray, U64 empty) {
    empty = empty & NOT_FILE_A;
    ne_ray |= empty & shift_left(ne_ray, 9);
    empty &= shift_left(empty, 9);
    ne_ray |= empty & shift_left(ne_ray, 18);
    empty &= shift_left(empty, 18);
    ne_ray |= empty & shift_left(ne_ray, 36);
    return shift_left(ne_ray, 9) & NOT_FILE_A;
}

U64 se_ray(U64 se_ray, U64 empty) {
    empty = empty & NOT_FILE_A;
    se_ray |= empty & shift_right(se_ray, 7);
    empty &= shift_right(empty, 7);
    se_ray |= empty & shift_right(se_ray, 14);
    empty &= shift_right(empty, 14);
    se_ray |= empty & shift_right(se_ray, 28);
    return shift_right(se_ray, 7) & NOT_FILE_A;
}

U64 nw_ray(U64 nw_ray, U64 empty) {
    empty = empty & NOT_FILE_H;
    nw_ray |= empty & shift_left(nw_ray, 7);
    empty &= shift_left(empty, 7);
    nw_ray |= empty & shift_left(nw_ray, 14);
    empty &= shift_left(empty, 14);
    nw_ray |= empty & shift_left(nw_ray, 28);
    return shift_left(nw_ray, 7) & NOT_FILE_H;
}

U64 sw_ray(U64 sw_ray, U64 empty) {
    empty = empty & NOT_FILE_H;
    sw_ray |= empty & shift_right(sw_ray, 9);
    empty &= shift_right(empty, 9);
    sw_ray |= empty & shift_right(sw_ray, 18);
    empty &= shift_right(empty, 18);
    sw_ray |= empty & shift_right(sw_ray, 36);
    return shift_right(sw_ray, 9) & NOT_FILE_H;
}

// Splits the given std::string into a vector delimited by delimeter
std::vector<std::string> split(std::string str_to_split, char delimeter) {
    std::stringstream ss(str_to_split);
    std::string item;
    std::vector<std::string> split_strings;
    while (std::getline(ss, item, delimeter))
    {
        split_strings.push_back(item);
    }

    return split_strings;
}

// Prints a string to the screen
void print(std::string thing_to_print) {
    std::cout << thing_to_print << std::endl;    
}

// Prints a newline to the screen
void print(void) {
    std::cout << std::endl;    
}

// Prints a pretty bitboard to the screen
void pretty_print(U64 bitboard) {
    int bit_count = 0;
    std::string str_bitboard = "";

    while (bit_count < BITBOARD_SIZE) {
        if (bit_count % 8 == 0 && bit_count != 0) {
            // End of row
            str_bitboard += "\n";
        } 
        
        if (bitboard & 1) {
            // This bit is set high
            str_bitboard += "1 ";
        } 
        else {
            // This bit is set low
            str_bitboard += "0 ";
        }
        
        bitboard = bitboard >> 1;
        bit_count++;
    }

    print(str_bitboard + "\n");
}

// Returns true if str1 contains str2, false otherwise
bool str_contains(std::string str1, char str2) {
    return (str1.find(str2) != std::string::npos);
}

// Returns true if item is in map, false otherwise
bool map_contains(std::unordered_map<U64, U64> map, U64 item) {
    return (map.find(item) != map.end());
}

// Returns true if item is in set, false otherwise
bool set_contains(std::set<U64> set, U64 item) {
    return (set.find(item) != set.end());
}

std::vector<U64> split_bitboard(U64 bitboard) {
    std::vector<U64> set_bit_indices;
    int bit_index = 0;
    
    while (bitboard) {
        if (bitboard & 1)
            set_bit_indices.push_back(bit_index);
        
        bit_index++;
        bitboard >>= 1;
    }
    
    for (int i = 0; i < set_bit_indices.size(); i++)
        set_bit_indices[i] = shift_left(1, set_bit_indices[i]);

    return set_bit_indices;
}

// Generate the piece for all ranks & files
std::unordered_map<std::string, U64> get_file_rank_to_piece_str(void) {
    std::string file_rank;
    U64 mask;
    std::unordered_map<std::string, U64> file_rank_to_piece;
    
    for (int bit_index = 0; bit_index < BITBOARD_SIZE; bit_index++) {
        mask = shift_left(1, bit_index);

        // Determine file
        if (mask & FILE_A)
            file_rank += FILE_A_CHAR;
        else if (mask & FILE_B)
            file_rank += FILE_B_CHAR;
        else if (mask & FILE_C)
            file_rank += FILE_C_CHAR;
        else if (mask & FILE_D)
            file_rank += FILE_D_CHAR;
        else if (mask & FILE_E)
            file_rank += FILE_E_CHAR;
        else if (mask & FILE_F)
            file_rank += FILE_F_CHAR;
        else if (mask & FILE_G)
            file_rank += FILE_G_CHAR;
        else // if (mask & FILE_H)
            file_rank += FILE_H_CHAR;
        
        // Determine rank
        if (mask & RANK_1)
            file_rank += RANK_1_CHAR;
        else if (mask & RANK_2)
            file_rank += RANK_2_CHAR;
        else if (mask & RANK_3)
            file_rank += RANK_3_CHAR;
        else if (mask & RANK_4)
            file_rank += RANK_4_CHAR;
        else if (mask & RANK_5)
            file_rank += RANK_5_CHAR;
        else if (mask & RANK_6)
            file_rank += RANK_6_CHAR;
        else if (mask & RANK_7)
            file_rank += RANK_7_CHAR;
        else // if (mask & RANK_8)
            file_rank += RANK_8_CHAR;

        file_rank_to_piece[file_rank] = mask;
    }

    // Add en passant
    file_rank_to_piece[NO_EN_PASSANT_STR] = 0;
    
    return file_rank_to_piece;
}

std::unordered_map<std::string, U64> FILE_RANK_TO_PIECE = get_file_rank_to_piece_str();

// Generate the file & rank for all pieces
std::unordered_map<U64, std::string> get_piece_to_file_rank(void) {
    std::string file_rank;
    U64 mask;
    std::unordered_map<U64, std::string> piece_to_file_rank;
    
    for (int bit_index = 0; bit_index < BITBOARD_SIZE; bit_index++) {
        file_rank = "";
        mask = shift_left(1, bit_index);

        // Determine file
        if (mask & FILE_A)
            file_rank += FILE_A_CHAR;
        else if (mask & FILE_B)
            file_rank += FILE_B_CHAR;
        else if (mask & FILE_C)
            file_rank += FILE_C_CHAR;
        else if (mask & FILE_D)
            file_rank += FILE_D_CHAR;
        else if (mask & FILE_E)
            file_rank += FILE_E_CHAR;
        else if (mask & FILE_F)
            file_rank += FILE_F_CHAR;
        else if (mask & FILE_G)
            file_rank += FILE_G_CHAR;
        else // if (mask & FILE_H)
            file_rank += FILE_H_CHAR;
        
        // Determine rank
        if (mask & RANK_1)
            file_rank += RANK_1_CHAR;
        else if (mask & RANK_2)
            file_rank += RANK_2_CHAR;
        else if (mask & RANK_3)
            file_rank += RANK_3_CHAR;
        else if (mask & RANK_4)
            file_rank += RANK_4_CHAR;
        else if (mask & RANK_5)
            file_rank += RANK_5_CHAR;
        else if (mask & RANK_6)
            file_rank += RANK_6_CHAR;
        else if (mask & RANK_7)
            file_rank += RANK_7_CHAR;
        else // if (mask & RANK_8)
            file_rank += RANK_8_CHAR;
            
        piece_to_file_rank[mask] = file_rank;
    }

    return piece_to_file_rank;
}

std::unordered_map<U64, std::string> PIECE_TO_FILE_RANK = get_piece_to_file_rank();

// Generate the piece move mask for all ranks & files
std::unordered_map<U64, int> get_piece_to_file_rank_move_mask__from(void) {
    int file_rank;
    U64 mask;
    std::unordered_map<U64, int> file_rank_to_piece;
    
    for (int bit_index = 0; bit_index < BITBOARD_SIZE; bit_index++) {
        mask = shift_left(1, bit_index);
        file_rank = 0;

        // Determine file
        if (mask & FILE_A)
            file_rank |= FROM_FILE_A_MOVE_MASK;
        else if (mask & FILE_B)
            file_rank |= FROM_FILE_B_MOVE_MASK;
        else if (mask & FILE_C)
            file_rank |= FROM_FILE_C_MOVE_MASK;
        else if (mask & FILE_D)
            file_rank |= FROM_FILE_D_MOVE_MASK;
        else if (mask & FILE_E)
            file_rank |= FROM_FILE_E_MOVE_MASK;
        else if (mask & FILE_F)
            file_rank |= FROM_FILE_F_MOVE_MASK;
        else if (mask & FILE_G)
            file_rank |= FROM_FILE_G_MOVE_MASK;
        else // if (mask & FILE_H)
            file_rank |= FROM_FILE_H_MOVE_MASK;
        
        // Determine rank
        if (mask & RANK_1)
            file_rank |= FROM_RANK_1_MOVE_MASK;
        else if (mask & RANK_2)
            file_rank |= FROM_RANK_2_MOVE_MASK;
        else if (mask & RANK_3)
            file_rank |= FROM_RANK_3_MOVE_MASK;
        else if (mask & RANK_4)
            file_rank |= FROM_RANK_4_MOVE_MASK;
        else if (mask & RANK_5)
            file_rank |= FROM_RANK_5_MOVE_MASK;
        else if (mask & RANK_6)
            file_rank |= FROM_RANK_6_MOVE_MASK;
        else if (mask & RANK_7)
            file_rank |= FROM_RANK_7_MOVE_MASK;
        else // if (mask & RANK_8)
            file_rank |= FROM_RANK_8_MOVE_MASK;

        file_rank_to_piece[mask] = file_rank;
    }

    return file_rank_to_piece;
}

std::unordered_map<U64, int> PIECE_TO_FILE_RANK_MOVE_MASK__FROM = get_piece_to_file_rank_move_mask__from();

std::unordered_map<U64, int> get_piece_to_file_rank_move_mask__to(void) {
    int file_rank;
    U64 mask;
    std::unordered_map<U64, int> file_rank_to_piece;
    
    for (int bit_index = 0; bit_index < BITBOARD_SIZE; bit_index++) {
        mask = shift_left(1, bit_index);
        file_rank = 0;

        // Determine file
        if (mask & FILE_A)
            file_rank |= TO_FILE_A_MOVE_MASK;
        else if (mask & FILE_B)
            file_rank |= TO_FILE_B_MOVE_MASK;
        else if (mask & FILE_C)
            file_rank |= TO_FILE_C_MOVE_MASK;
        else if (mask & FILE_D)
            file_rank |= TO_FILE_D_MOVE_MASK;
        else if (mask & FILE_E)
            file_rank |= TO_FILE_E_MOVE_MASK;
        else if (mask & FILE_F)
            file_rank |= TO_FILE_F_MOVE_MASK;
        else if (mask & FILE_G)
            file_rank |= TO_FILE_G_MOVE_MASK;
        else // if (mask & FILE_H)
            file_rank |= TO_FILE_H_MOVE_MASK;
        
        // Determine rank
        if (mask & RANK_1)
            file_rank |= TO_RANK_1_MOVE_MASK;
        else if (mask & RANK_2)
            file_rank |= TO_RANK_2_MOVE_MASK;
        else if (mask & RANK_3)
            file_rank |= TO_RANK_3_MOVE_MASK;
        else if (mask & RANK_4)
            file_rank |= TO_RANK_4_MOVE_MASK;
        else if (mask & RANK_5)
            file_rank |= TO_RANK_5_MOVE_MASK;
        else if (mask & RANK_6)
            file_rank |= TO_RANK_6_MOVE_MASK;
        else if (mask & RANK_7)
            file_rank |= TO_RANK_7_MOVE_MASK;
        else // if (mask & RANK_8)
            file_rank |= TO_RANK_8_MOVE_MASK;

        file_rank_to_piece[mask] = file_rank;
    }

    return file_rank_to_piece;   
}

std::unordered_map<U64, int> PIECE_TO_FILE_RANK_MOVE_MASK__TO = get_piece_to_file_rank_move_mask__to();

std::unordered_map<int, U64> get_file_rank_move_mask_to_piece__from(void) {
    U64 from;
    int move;
    std::unordered_map<int, U64> file_rank_to_piece;
    
    for (int file = FROM_FILE_A_MOVE_MASK; file <= FROM_FILE_H_MOVE_MASK; file+=0x10) {
        for (int rank = FROM_RANK_1_MOVE_MASK; rank <= FROM_RANK_8_MOVE_MASK; rank+=0x100) {
            move = file | rank;
            from = 0;
            
            if ((move & FROM_FILE_MOVE_MASK) == FROM_FILE_A_MOVE_MASK)
                from |= FILE_A;
            else if ((move & FROM_FILE_MOVE_MASK) == FROM_FILE_B_MOVE_MASK)
                from |= FILE_B;
            else if ((move & FROM_FILE_MOVE_MASK) == FROM_FILE_C_MOVE_MASK)
                from |= FILE_C;
            else if ((move & FROM_FILE_MOVE_MASK) == FROM_FILE_D_MOVE_MASK)
                from |= FILE_D;
            else if ((move & FROM_FILE_MOVE_MASK) == FROM_FILE_E_MOVE_MASK)
                from |= FILE_E;
            else if ((move & FROM_FILE_MOVE_MASK) == FROM_FILE_F_MOVE_MASK)
                from |= FILE_F;
            else if ((move & FROM_FILE_MOVE_MASK) == FROM_FILE_G_MOVE_MASK)
                from |= FILE_G;
            else // if ((move & FROM_FILE_MOVE_MASK) == FROM_FILE_H_MOVE_MASK)
                from |= FILE_H;
                
            if ((move & FROM_RANK_MOVE_MASK) == FROM_RANK_1_MOVE_MASK)
                from &= RANK_1;
            else if ((move & FROM_RANK_MOVE_MASK) == FROM_RANK_2_MOVE_MASK)
                from &= RANK_2;
            else if ((move & FROM_RANK_MOVE_MASK) == FROM_RANK_3_MOVE_MASK)
                from &= RANK_3;
            else if ((move & FROM_RANK_MOVE_MASK) == FROM_RANK_4_MOVE_MASK)
                from &= RANK_4;
            else if ((move & FROM_RANK_MOVE_MASK) == FROM_RANK_5_MOVE_MASK)
                from &= RANK_5;
            else if ((move & FROM_RANK_MOVE_MASK) == FROM_RANK_6_MOVE_MASK)
                from &= RANK_6;
            else if ((move & FROM_RANK_MOVE_MASK) == FROM_RANK_7_MOVE_MASK)
                from &= RANK_7;
            else // if ((move & FROM_RANK_MOVE_MASK) == FROM_RANK_8_MOVE_MASK)
                from &= RANK_8;
            
            file_rank_to_piece[move] = from;
        }
    }
    
    return file_rank_to_piece;
}
    
std::unordered_map<int, U64> FILE_RANK_MOVE_MASK_TO_PIECE__FROM = get_file_rank_move_mask_to_piece__from();

std::unordered_map<int, U64> get_file_rank_move_mask_to_piece__to(void) {
    U64 to;
    int move;
    std::unordered_map<int, U64> file_rank_to_piece;
    
    for (int file = TO_FILE_A_MOVE_MASK; file <= TO_FILE_H_MOVE_MASK; file+=0x1000) {
        for (int rank = TO_RANK_1_MOVE_MASK; rank <= TO_RANK_8_MOVE_MASK; rank+=0x10000) {
            move = file | rank;
            to = 0;

            if ((move & TO_FILE_MOVE_MASK) == TO_FILE_A_MOVE_MASK)
                to |= FILE_A;
            else if ((move & TO_FILE_MOVE_MASK) == TO_FILE_B_MOVE_MASK) 
                to |= FILE_B;
            else if ((move & TO_FILE_MOVE_MASK) == TO_FILE_C_MOVE_MASK)
                to |= FILE_C;
            else if ((move & TO_FILE_MOVE_MASK) == TO_FILE_D_MOVE_MASK)
                to |= FILE_D;
            else if ((move & TO_FILE_MOVE_MASK) == TO_FILE_E_MOVE_MASK)
                to |= FILE_E;
            else if ((move & TO_FILE_MOVE_MASK) == TO_FILE_F_MOVE_MASK)
                to |= FILE_F;
            else if ((move & TO_FILE_MOVE_MASK) == TO_FILE_G_MOVE_MASK)
                to |= FILE_G;
            else // if ((move & TO_FILE_MOVE_MASK) == TO_FILE_H_MOVE_MASK)
                to |= FILE_H;
                
            if ((move & TO_RANK_MOVE_MASK) == TO_RANK_1_MOVE_MASK)
                to &= RANK_1;
            else if ((move & TO_RANK_MOVE_MASK) == TO_RANK_2_MOVE_MASK)
                to &= RANK_2;
            else if ((move & TO_RANK_MOVE_MASK) == TO_RANK_3_MOVE_MASK)
                to &= RANK_3;
            else if ((move & TO_RANK_MOVE_MASK) == TO_RANK_4_MOVE_MASK)
                to &= RANK_4;
            else if ((move & TO_RANK_MOVE_MASK) == TO_RANK_5_MOVE_MASK)
                to &= RANK_5;
            else if ((move & TO_RANK_MOVE_MASK) == TO_RANK_6_MOVE_MASK)
                to &= RANK_6;
            else if ((move & TO_RANK_MOVE_MASK) == TO_RANK_7_MOVE_MASK)
                to &= RANK_7;
            else // if ((move & TO_RANK_MOVE_MASK) == TO_RANK_8_MOVE_MASK)
                to &= RANK_8;
                
            file_rank_to_piece[move] = to;
        }
    }
    
    return file_rank_to_piece;
}

std::unordered_map<int, U64> FILE_RANK_MOVE_MASK_TO_PIECE__TO = get_file_rank_move_mask_to_piece__to();

// Generate ray list map
std::unordered_map<U64, std::vector<U64>> gen_rays_list(void) {
    std::unordered_map<U64, std::vector<U64>> rays_list;
    U64 mask;

    for (int bit_index = 0; bit_index < BITBOARD_SIZE; bit_index++) {
        mask = shift_left(1, bit_index);        
        rays_list[mask] = std::vector<U64>();

        rays_list[mask].push_back(n_ray(mask));
        rays_list[mask].push_back(s_ray(mask));
        rays_list[mask].push_back(e_ray(mask));
        rays_list[mask].push_back(w_ray(mask));
        rays_list[mask].push_back(ne_ray(mask));
        rays_list[mask].push_back(nw_ray(mask));
        rays_list[mask].push_back(se_ray(mask));
        rays_list[mask].push_back(sw_ray(mask));
    }
    
    return rays_list;
}

std::unordered_map<U64, std::vector<U64>> RAYS_LIST = gen_rays_list();

// Returns a map of piece to all rays emanating from that piece
std::unordered_map<U64, U64> gen_rays(void) {
    std::unordered_map<U64, U64> rays;
    U64 mask;
    
    for (int bit_index = 0; bit_index < BITBOARD_SIZE; bit_index++) {
        mask = shift_left(1, bit_index);
        rays[mask] = 0;
        
        rays[mask] |= n_ray(mask);
        rays[mask] |= s_ray(mask);
        rays[mask] |= e_ray(mask);
        rays[mask] |= w_ray(mask);
        rays[mask] |= ne_ray(mask);
        rays[mask] |= nw_ray(mask);
        rays[mask] |= se_ray(mask);
        rays[mask] |= sw_ray(mask);
    }
    
    return rays;
}

std::unordered_map<U64, U64> RAYS = gen_rays();

std::unordered_map<U64, U64> gen_diag_rays(void) {
    std::unordered_map<U64, U64> rays;
    U64 mask;

    for (int bit_index = 0; bit_index < BITBOARD_SIZE; bit_index++) {
        mask = shift_left(1, bit_index);        
        rays[mask] = 0;

        rays[mask] |= ne_ray(mask);
        rays[mask] |= nw_ray(mask);
        rays[mask] |= se_ray(mask);
        rays[mask] |= sw_ray(mask);
    }
    
    return rays;
}

std::unordered_map<U64, U64> DIAG_RAYS = gen_diag_rays();

std::set<U64> gen_rays_set(void) {
    std::set<U64> rays;
    U64 mask;

    for (int bit_index = 0; bit_index < BITBOARD_SIZE; bit_index++) {
        mask = shift_left(1, bit_index);        
        
        rays.insert(n_ray(mask) | s_ray(mask) | mask);
        rays.insert(e_ray(mask) | w_ray(mask) | mask);
        rays.insert(ne_ray(mask) | sw_ray(mask) | mask);
        rays.insert(nw_ray(mask) | se_ray(mask) | mask);
    }
    
    return rays;
}

std::set<U64> RAYS_SET = gen_rays_set();

// hv: horizontal/vertical
std::set<U64> gen_hv_rays_set(void) {
    std::set<U64> rays;
    U64 mask;

    for (int bit_index = 0; bit_index < BITBOARD_SIZE; bit_index++) {
        mask = shift_left(1, bit_index);        
        
        rays.insert(n_ray(mask) | s_ray(mask) | mask);
        rays.insert(e_ray(mask) | w_ray(mask) | mask);
    }
    
    return rays;
}

std::set<U64> HV_RAYS_SET = gen_hv_rays_set();

std::set<U64> gen_diag_rays_set(void) {
    std::set<U64> rays;
    U64 mask;

    for (int bit_index = 0; bit_index < BITBOARD_SIZE; bit_index++) {
        mask = shift_left(1, bit_index);        
        
        rays.insert(ne_ray(mask) | sw_ray(mask) | mask);
        rays.insert(nw_ray(mask) | se_ray(mask) | mask);
    }
    
    return rays;
}

std::set<U64> DIAG_RAYS_SET = gen_diag_rays_set();

std::unordered_map<U64, U64> gen_knight_moves(void) {
    std::unordered_map<U64, U64> knight_moves;
    U64 mask;
    
    for (int bit_index = 0; bit_index < BITBOARD_SIZE; bit_index++) {
        mask = shift_left(1, bit_index);
        knight_moves[mask] = 0;

        knight_moves[mask] |= shift_left(mask, 6);
        knight_moves[mask] |= shift_left(mask, 10);
        knight_moves[mask] |= shift_left(mask, 15);
        knight_moves[mask] |= shift_left(mask, 17);
        knight_moves[mask] |= shift_right(mask, 6);
        knight_moves[mask] |= shift_right(mask, 10);
        knight_moves[mask] |= shift_right(mask, 15);
        knight_moves[mask] |= shift_right(mask, 17);

        // Clean up moves if necessary
        if (mask & (FILE_G | FILE_H)) {
            knight_moves[mask] &= 0xF0F0F0F0F0F0F0F0;
        }
        else if (mask & (FILE_A | FILE_B)) {
            knight_moves[mask] &= 0x0F0F0F0F0F0F0F0F;
        }
    }

    return knight_moves;;
}

std::unordered_map<U64, U64> KNIGHT_MOVES = gen_knight_moves();

std::unordered_map<U64, U64> gen_king_moves(void) {
    std::unordered_map<U64, U64> king_moves;
    U64 mask;
    
    for (int bit_index = 0; bit_index < BITBOARD_SIZE; bit_index++) {
        mask = shift_left(1, bit_index);
        king_moves[mask] = 0;

        king_moves[mask] |= shift_left(mask, 1);
        king_moves[mask] |= shift_right(mask, 1);
        king_moves[mask] |= shift_left(mask, 7);
        king_moves[mask] |= shift_right(mask, 7);
        king_moves[mask] |= shift_left(mask, 8);
        king_moves[mask] |= shift_right(mask, 8);
        king_moves[mask] |= shift_left(mask, 9);
        king_moves[mask] |= shift_right(mask, 9);
        
        // Clean up moves if necessary
        if (mask & FILE_A) {
            king_moves[mask] &= FILE_A | FILE_B;
        }
        else if (mask & FILE_H) {
            king_moves[mask] &= FILE_G | FILE_H;
        }
    }

    return king_moves;
}

std::unordered_map<U64, U64> KING_MOVES = gen_king_moves();

std::string get_move_str(int move) {
    // Check castling    
    if ((move & KINGSIDE_CASTLE_MOVE_MASK) == KINGSIDE_CASTLE_MOVE_MASK)
        return KINGSIDE_CASTLE_SAN;
    else if ((move & QUEENSIDE_CASTLE_MOVE_MASK) == QUEENSIDE_CASTLE_MOVE_MASK)
        return QUEENSIDE_CASTLE_SAN;
    

    char san_char;
    std::string san = "";

    // Get a SAN char for this move
    if ((move & PIECE_MOVE_MASK) == KING_MOVE_MASK)
        san_char = K_SAN;
    else if ((move & PIECE_MOVE_MASK) == QUEEN_MOVE_MASK)
        san_char = Q_SAN;
    else if ((move & PIECE_MOVE_MASK) == BISHOP_MOVE_MASK)
        san_char = B_SAN;
    else if ((move & PIECE_MOVE_MASK) == ROOK_MOVE_MASK)
        san_char = R_SAN;
    else if ((move & PIECE_MOVE_MASK) == KNIGHT_MOVE_MASK)
        san_char = N_SAN;
    else // if ((move & PIECE_MOVE_MASK) == PAWN_MOVE_MASK)
        san_char = P_SAN;

    // Don't allow the empty char to be written
    if (san_char != (char)0)
        san += san_char;
    
    // Get the from file & rank
    if ((move & FROM_FILE_MOVE_MASK) == FROM_FILE_A_MOVE_MASK)
        san += FILE_A_CHAR;
    else if ((move & FROM_FILE_MOVE_MASK) == FROM_FILE_B_MOVE_MASK)
        san += FILE_B_CHAR;
    else if ((move & FROM_FILE_MOVE_MASK) == FROM_FILE_C_MOVE_MASK)
        san += FILE_C_CHAR;
    else if ((move & FROM_FILE_MOVE_MASK) == FROM_FILE_D_MOVE_MASK)
        san += FILE_D_CHAR;
    else if ((move & FROM_FILE_MOVE_MASK) == FROM_FILE_E_MOVE_MASK)
        san += FILE_E_CHAR;
    else if ((move & FROM_FILE_MOVE_MASK) == FROM_FILE_F_MOVE_MASK)
        san += FILE_F_CHAR;
    else if ((move & FROM_FILE_MOVE_MASK) == FROM_FILE_G_MOVE_MASK)
        san += FILE_G_CHAR;
    else // if ((move & FROM_FILE_MOVE_MASK) == FROM_FILE_H_MOVE_MASK)
        san += FILE_H_CHAR;
        
    if ((move & FROM_RANK_MOVE_MASK) == FROM_RANK_1_MOVE_MASK)
        san += RANK_1_CHAR;
    else if ((move & FROM_RANK_MOVE_MASK) == FROM_RANK_2_MOVE_MASK)
        san += RANK_2_CHAR;
    else if ((move & FROM_RANK_MOVE_MASK) == FROM_RANK_3_MOVE_MASK)
        san += RANK_3_CHAR;
    else if ((move & FROM_RANK_MOVE_MASK) == FROM_RANK_4_MOVE_MASK)
        san += RANK_4_CHAR;
    else if ((move & FROM_RANK_MOVE_MASK) == FROM_RANK_5_MOVE_MASK)
        san += RANK_5_CHAR;
    else if ((move & FROM_RANK_MOVE_MASK) == FROM_RANK_6_MOVE_MASK)
        san += RANK_6_CHAR;
    else if ((move & FROM_RANK_MOVE_MASK) == FROM_RANK_7_MOVE_MASK)
        san += RANK_7_CHAR;
    else // if ((move & FROM_RANK_MOVE_MASK) == FROM_RANK_8_MOVE_MASK)
        san += RANK_8_CHAR;
        
    // Add attack character if necessary
    if ((move & ATTACK_MOVE_MASK) == ATTACK_MOVE_MASK)
        san += ATTACK_SAN;

    // Get the to file & rank
    if ((move & TO_FILE_MOVE_MASK) == TO_FILE_A_MOVE_MASK)
        san += FILE_A_CHAR;
    else if ((move & TO_FILE_MOVE_MASK) == TO_FILE_B_MOVE_MASK)
        san += FILE_B_CHAR;
    else if ((move & TO_FILE_MOVE_MASK) == TO_FILE_C_MOVE_MASK)
        san += FILE_C_CHAR;
    else if ((move & TO_FILE_MOVE_MASK) == TO_FILE_D_MOVE_MASK)
        san += FILE_D_CHAR;
    else if ((move & TO_FILE_MOVE_MASK) == TO_FILE_E_MOVE_MASK)
        san += FILE_E_CHAR;
    else if ((move & TO_FILE_MOVE_MASK) == TO_FILE_F_MOVE_MASK)
        san += FILE_F_CHAR;
    else if ((move & TO_FILE_MOVE_MASK) == TO_FILE_G_MOVE_MASK)
        san += FILE_G_CHAR;
    else // if ((move & TO_FILE_MOVE_MASK) == TO_FILE_H_MOVE_MASK)
        san += FILE_H_CHAR;
        
    if ((move & TO_RANK_MOVE_MASK) == TO_RANK_1_MOVE_MASK)
        san += RANK_1_CHAR;
    else if ((move & TO_RANK_MOVE_MASK) == TO_RANK_2_MOVE_MASK)
        san += RANK_2_CHAR;
    else if ((move & TO_RANK_MOVE_MASK) == TO_RANK_3_MOVE_MASK)
        san += RANK_3_CHAR;
    else if ((move & TO_RANK_MOVE_MASK) == TO_RANK_4_MOVE_MASK)
        san += RANK_4_CHAR;
    else if ((move & TO_RANK_MOVE_MASK) == TO_RANK_5_MOVE_MASK)
        san += RANK_5_CHAR;
    else if ((move & TO_RANK_MOVE_MASK) == TO_RANK_6_MOVE_MASK)
        san += RANK_6_CHAR;
    else if ((move & TO_RANK_MOVE_MASK) == TO_RANK_7_MOVE_MASK)
        san += RANK_7_CHAR;
    else // if ((move & TO_RANK_MOVE_MASK) == TO_RANK_8_MOVE_MASK)
        san += RANK_8_CHAR;
    
    // Add promotion
    if ((move & PROMO_MOVE_MASK) == QUEEN_PROMO_MOVE_MASK)
        san += Q_SAN;
    else if ((move & PROMO_MOVE_MASK) == BISHOP_PROMO_MOVE_MASK)
        san += B_SAN;
    else if ((move & PROMO_MOVE_MASK) == ROOK_PROMO_MOVE_MASK)
        san += R_SAN;
    else if ((move & PROMO_MOVE_MASK) == KNIGHT_PROMO_MOVE_MASK)
        san += N_SAN;
    
    return san;
}

int count_set_bits(U64 bits) {
    int count = 0;

    while (bits) {
        count += bits & 1;
        bits >>= 1;
    }

    return count;
}

// Returns the file & rank integer encoding for the given SAN, with the file starting at starting_index
// and the rank following immediately after
int get_to_file_rank_mask(std::string san, int starting_index) {
    int file_rank_mask = 0;
    
    // Determine file
    switch (san[starting_index]) {
        case FILE_A_CHAR:
            file_rank_mask |= TO_FILE_A_MOVE_MASK;
            break;
        case FILE_B_CHAR:
            file_rank_mask |= TO_FILE_B_MOVE_MASK;
            break;
        case FILE_C_CHAR:
            file_rank_mask |= TO_FILE_C_MOVE_MASK;
            break;
        case FILE_D_CHAR:
            file_rank_mask |= TO_FILE_D_MOVE_MASK;
            break;
        case FILE_E_CHAR:
            file_rank_mask |= TO_FILE_E_MOVE_MASK;
            break;
        case FILE_F_CHAR:
            file_rank_mask |= TO_FILE_F_MOVE_MASK;
            break;
        case FILE_G_CHAR:
            file_rank_mask |= TO_FILE_G_MOVE_MASK;
            break;
        default: // FILE_H_CHAR
            file_rank_mask |= TO_FILE_H_MOVE_MASK;
    }

    // Determine rank
    switch (san[starting_index + 1]) {
        case RANK_1_CHAR:
            file_rank_mask |= TO_RANK_1_MOVE_MASK;
            break;
        case RANK_2_CHAR:
            file_rank_mask |= TO_RANK_2_MOVE_MASK;
            break;
        case RANK_3_CHAR:
            file_rank_mask |= TO_RANK_3_MOVE_MASK;
            break;
        case RANK_4_CHAR:
            file_rank_mask |= TO_RANK_4_MOVE_MASK;
            break;
        case RANK_5_CHAR:
            file_rank_mask |= TO_RANK_5_MOVE_MASK;
            break;
        case RANK_6_CHAR:
            file_rank_mask |= TO_RANK_6_MOVE_MASK;
            break;
        case RANK_7_CHAR:
            file_rank_mask |= TO_RANK_7_MOVE_MASK;
            break;
        default: // RANK_8_CHAR
            file_rank_mask |= TO_RANK_8_MOVE_MASK;
    }
    
    return file_rank_mask;
}

// Returns the move bitfield equal to the given SAN string (in the format that the server uses)
int server_san_to_move(std::string san) {
    if (san[0] == 'O') {
        // Castling    
        return (san == KINGSIDE_CASTLE_SAN) ? KINGSIDE_CASTLE_MOVE_MASK : QUEENSIDE_CASTLE_MOVE_MASK;
    }
    
    int length = san.length();

    if (san[san.length() - 1] == '+')
        // Ignore the 'in check' character
        length -= 1;
    
    if (length == 2) {
        // Pawn move, no capture
        return PAWN_MOVE_MASK | get_to_file_rank_mask(san, 0);
    }
    
    int move = 0;
    
    if (san.find(ATTACK_SAN)) {
        move |= ATTACK_MOVE_MASK;
    }

    if ((san[length - 2]) == '=') {
        // Pawn promotion
        move |= PAWN_MOVE_MASK;

        // Get where the pawn is moving to
        move |= get_to_file_rank_mask(san, length - 4);

        char promo_san = san.back(); 
        
        switch (promo_san) {
            case Q_SAN:
                return move | QUEEN_PROMO_MOVE_MASK;
            case B_SAN:
                return move | BISHOP_PROMO_MOVE_MASK;
            case R_SAN:
                return move | ROOK_PROMO_MOVE_MASK;
            default: // N_SAN
                return move | KNIGHT_PROMO_MOVE_MASK;
        }
    }
    
    // Handle non-pawn pieces
    switch (san[0]) {
        case K_SAN:
            move |= KING_MOVE_MASK;
            break;
        case Q_SAN:
            move |= QUEEN_MOVE_MASK;
            break;
        case B_SAN:
            move |= BISHOP_MOVE_MASK;
            break;
        case R_SAN:
            move |= ROOK_MOVE_MASK;
            break;
        default: // N_SAN
            move |= KNIGHT_MOVE_MASK;
    }

    return move | get_to_file_rank_mask(san, length - 2);
}
