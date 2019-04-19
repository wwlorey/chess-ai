#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <climits>
#include <cstdint>
#include <limits>
#include <string>
#include <unordered_map>
#include <vector>

typedef uint64_t U64;
constexpr int BITBOARD_SIZE = 64;
constexpr U64 UNIVERSAL_SET = 0xFFFFFFFFFFFFFFFF;
const std::string NO_EN_PASSANT_STR = "-";
constexpr bool WHITE = 0;
constexpr bool BLACK = 1;
constexpr int MAX_NUM_MOVES = 100;
constexpr int ESTIMATED_REMAINING_MOVES = 40;
constexpr int MAX_QS_DEPTH = 3;

constexpr U64 FILE_A = 0x0101010101010101;
constexpr U64 FILE_B = 0x0202020202020202;
constexpr U64 FILE_C = 0x0404040404040404;
constexpr U64 FILE_D = 0x0808080808080808;
constexpr U64 FILE_E = 0x1010101010101010;
constexpr U64 FILE_F = 0x2020202020202020;
constexpr U64 FILE_G = 0x4040404040404040;
constexpr U64 FILE_H = 0x8080808080808080;
constexpr U64 NOT_FILE_A = ~FILE_A;
constexpr U64 NOT_FILE_H = ~FILE_H;
constexpr char FILE_A_CHAR = 'a';
constexpr char FILE_B_CHAR = 'b';
constexpr char FILE_C_CHAR = 'c';
constexpr char FILE_D_CHAR = 'd';
constexpr char FILE_E_CHAR = 'e';
constexpr char FILE_F_CHAR = 'f';
constexpr char FILE_G_CHAR = 'g';
constexpr char FILE_H_CHAR = 'h';

constexpr U64 RANK_1 = 0xFF00000000000000;
constexpr U64 RANK_2 = 0x00FF000000000000;
constexpr U64 RANK_3 = 0x0000FF0000000000;
constexpr U64 RANK_4 = 0x000000FF00000000;
constexpr U64 RANK_5 = 0x00000000FF000000;
constexpr U64 RANK_6 = 0x0000000000FF0000;
constexpr U64 RANK_7 = 0x000000000000FF00;
constexpr U64 RANK_8 = 0x00000000000000FF;
constexpr char RANK_1_CHAR = '1';
constexpr char RANK_2_CHAR = '2';
constexpr char RANK_3_CHAR = '3';
constexpr char RANK_4_CHAR = '4';
constexpr char RANK_5_CHAR = '5';
constexpr char RANK_6_CHAR = '6';
constexpr char RANK_7_CHAR = '7';
constexpr char RANK_8_CHAR = '8';

constexpr U64 WHITE_CASTLE_KINGSIDE_MASK = 0x9000000000000000;
constexpr U64 WHITE_CASTLE_KINGSIDE_INVALID = 0x6000000000000000;
constexpr U64 WHITE_CASTLE_KINGSIDE_KING_MOVE = 0x2000000000000000;
constexpr U64 WHITE_CASTLE_KINGSIDE_ROOK_MOVE = 0x1000000000000000;

constexpr U64 WHITE_CASTLE_QUEENSIDE_MASK = 0x1100000000000000;
constexpr U64 WHITE_CASTLE_QUEENSIDE_INVALID = 0x0E00000000000000;
constexpr U64 WHITE_CASTLE_QUEENSIDE_KING_MOVE = 0x0800000000000000;
constexpr U64 WHITE_CASTLE_QUEENSIDE_ROOK_MOVE = 0x1000000000000000;

constexpr U64 BLACK_CASTLE_KINGSIDE_MASK = 0x0000000000000090;
constexpr U64 BLACK_CASTLE_KINGSIDE_INVALID = 0x0000000000000060;
constexpr U64 BLACK_CASTLE_KINGSIDE_KING_MOVE = 0x0000000000000020;
constexpr U64 BLACK_CASTLE_KINGSIDE_ROOK_MOVE = 0x0000000000000010;

constexpr U64 BLACK_CASTLE_QUEENSIDE_MASK = 0x0000000000000011;
constexpr U64 BLACK_CASTLE_QUEENSIDE_INVALID = 0x000000000000000E;
constexpr U64 BLACK_CASTLE_QUEENSIDE_KING_MOVE = 0x0000000000000008;
constexpr U64 BLACK_CASTLE_QUEENSIDE_ROOK_MOVE = 0x0000000000000010;

// Move encoding
constexpr int KING_MOVE_MASK   = 0x00000001;
constexpr int QUEEN_MOVE_MASK  = 0x00000002;
constexpr int BISHOP_MOVE_MASK = 0x00000003;
constexpr int ROOK_MOVE_MASK   = 0x00000004;
constexpr int KNIGHT_MOVE_MASK = 0x00000005;
constexpr int PAWN_MOVE_MASK   = 0x00000006;

constexpr int PIECE_MOVE_MASK  = 0x00000007;

constexpr int FROM_FILE_A_MOVE_MASK = 0x00000010;
constexpr int FROM_FILE_B_MOVE_MASK = 0x00000020;
constexpr int FROM_FILE_C_MOVE_MASK = 0x00000030;
constexpr int FROM_FILE_D_MOVE_MASK = 0x00000040;
constexpr int FROM_FILE_E_MOVE_MASK = 0x00000050;
constexpr int FROM_FILE_F_MOVE_MASK = 0x00000060;
constexpr int FROM_FILE_G_MOVE_MASK = 0x00000070;
constexpr int FROM_FILE_H_MOVE_MASK = 0x00000080;

constexpr int FROM_FILE_MOVE_MASK   = 0x000000F0;

constexpr int FROM_RANK_1_MOVE_MASK = 0x00000100;
constexpr int FROM_RANK_2_MOVE_MASK = 0x00000200;
constexpr int FROM_RANK_3_MOVE_MASK = 0x00000300;
constexpr int FROM_RANK_4_MOVE_MASK = 0x00000400;
constexpr int FROM_RANK_5_MOVE_MASK = 0x00000500;
constexpr int FROM_RANK_6_MOVE_MASK = 0x00000600;
constexpr int FROM_RANK_7_MOVE_MASK = 0x00000700;
constexpr int FROM_RANK_8_MOVE_MASK = 0x00000800;

constexpr int FROM_RANK_MOVE_MASK   = 0x00000F00;

constexpr int TO_FILE_A_MOVE_MASK = 0x00001000;
constexpr int TO_FILE_B_MOVE_MASK = 0x00002000;
constexpr int TO_FILE_C_MOVE_MASK = 0x00003000;
constexpr int TO_FILE_D_MOVE_MASK = 0x00004000;
constexpr int TO_FILE_E_MOVE_MASK = 0x00005000;
constexpr int TO_FILE_F_MOVE_MASK = 0x00006000;
constexpr int TO_FILE_G_MOVE_MASK = 0x00007000;
constexpr int TO_FILE_H_MOVE_MASK = 0x00008000;

constexpr int TO_FILE_MOVE_MASK   = 0x0000F000;

constexpr int TO_RANK_1_MOVE_MASK = 0x00010000;
constexpr int TO_RANK_2_MOVE_MASK = 0x00020000;
constexpr int TO_RANK_3_MOVE_MASK = 0x00030000;
constexpr int TO_RANK_4_MOVE_MASK = 0x00040000;
constexpr int TO_RANK_5_MOVE_MASK = 0x00050000;
constexpr int TO_RANK_6_MOVE_MASK = 0x00060000;
constexpr int TO_RANK_7_MOVE_MASK = 0x00070000;
constexpr int TO_RANK_8_MOVE_MASK = 0x00080000;

constexpr int TO_RANK_MOVE_MASK   = 0x000F0000;

constexpr int KINGSIDE_CASTLE_MOVE_MASK  = 0x00100000;
constexpr int QUEENSIDE_CASTLE_MOVE_MASK = 0x00200000;
constexpr int CASTLE_MOVE_MASK           = 0x00300000;

constexpr int ATTACK_MOVE_MASK = 0x00400000;

constexpr int QUEEN_PROMO_MOVE_MASK  = 0x01000000;
constexpr int ROOK_PROMO_MOVE_MASK   = 0x02000000;
constexpr int BISHOP_PROMO_MOVE_MASK = 0x03000000;
constexpr int KNIGHT_PROMO_MOVE_MASK = 0x04000000;

constexpr int PROMO_MOVE_MASK        = 0x0F000000;

constexpr int LEN_PAWN_PROMOTION_CHOICES = 4;
constexpr int PAWN_PROMOTION_MOVE_MASK_CHOICES[LEN_PAWN_PROMOTION_CHOICES] = {QUEEN_PROMO_MOVE_MASK, ROOK_PROMO_MOVE_MASK, BISHOP_PROMO_MOVE_MASK, KNIGHT_PROMO_MOVE_MASK};

enum BitboardIndices {
    WK, // White king
    WQ, // White queen
    WB, // White bishop
    WN, // White (k)night
    WR, // White rook
    WP, // White pawn
    BK, // Black king
    BQ, // Black queen
    BB, // Black bishop
    BN, // Black (k)night
    BR, // Black rook
    BP, // Black pawn
    NUM_BITBOARDS
};

const std::vector<int> WHITE_BITBOARD_INDICES = {WK, WQ, WB, WN, WR, WP};
const std::vector<int> BLACK_BITBOARD_INDICES = {BK, BQ, BB, BN, BR, BP};

// Search parameters
enum TerminalTestResults {
    LOSE_TERMINAL_NODE,
    DRAW_TERMINAL_NODE,
    INTERNAL_NODE,
    DEPTH_LIMIT_REACHED,
};

#define INIT_ALPHA INT_MIN
#define INIT_BETA INT_MAX
#define MAX_VALUE (INT_MAX - 1)
#define MIN_VALUE (INT_MIN + 1)
#define DRAW_VALUE 0

// Piece weight indices match the *_BITBOARD_INDICES vectors
// Source: https://en.wikipedia.org/wiki/Chess_piece_relative_value
const std::vector<int> PIECE_WEIGHTS = {0,9,3,3,5,1};

enum FENCharacterMappings {
    // White pieces are uppercase, black pieces are lowercase
    WK_FEN = 'K',
    WQ_FEN = 'Q',
    WB_FEN = 'B',
    WN_FEN = 'N',
    WR_FEN = 'R',
    WP_FEN = 'P',
    WHITE_CASTLE_KINGSIDE_FEN = 'K',
    WHITE_CASTLE_QUEENSIDE_FEN = 'Q',
    BK_FEN = 'k',
    BQ_FEN = 'q',
    BB_FEN = 'b',
    BN_FEN = 'n',
    BR_FEN = 'r',
    BP_FEN = 'p',
    BLACK_CASTLE_KINGSIDE_FEN = 'k',
    BLACK_CASTLE_QUEENSIDE_FEN = 'q',
    DELIMETER_FEN = '/'
};

enum SANCharacterMappings {
    K_SAN = 'K',
    Q_SAN = 'Q',
    B_SAN = 'B',
    N_SAN = 'N',
    R_SAN = 'R',
    P_SAN = (char)0, // Empty character
    ATTACK_SAN = 'x'
};

// Enums don't play nice with strings
const std::string KINGSIDE_CASTLE_SAN = "O-O";
const std::string QUEENSIDE_CASTLE_SAN = "O-O-O";

constexpr char PAWN_PROMOTION_CHOICES[LEN_PAWN_PROMOTION_CHOICES] = {Q_SAN, R_SAN, B_SAN, N_SAN};

enum SplitFenIndices {
    PIECE_LOCATIONS,
    ACTIVE_COLOR,
    CASTLING,
    EN_PASSANT,
    HALF_MOVES,
    WHOLE_MOVES
};

enum MoveTypes {
    KING,
    QUEEN,
    BISHOP,
    ROOK,
    KNIGHT,
    PAWN,
    PAWN_ATTACK,
};

#endif // CONSTANTS_HPP
