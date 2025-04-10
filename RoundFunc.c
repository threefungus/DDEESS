#include "RoundFunc.h"

unsigned char expansion_perm[48] ={32,  1,  2,  3,  4,  5, 
                                    4,  5,  6,  7,  8,  9,
                                    8,  9, 10, 11, 12, 13,
                                   12, 13, 14, 15, 16, 17,
                                   16, 17, 18, 19, 20, 21,
                                   20, 21, 22, 23, 24, 25,
                                   24, 25, 26, 27, 28, 29,
                                   28, 29, 30, 31, 32,  1};

unsigned char s_box1[4][16] = {{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
                               {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
                               {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
                               {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}};

unsigned char s_box2[4][16] = {{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
                               {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
                               {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
                               {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}};

unsigned char s_box3[4][16] = {{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
                               {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
                               {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
                               {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}};

unsigned char s_box4[4][16] = {{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
                               {13, 8, 11, 5, 6, 15, 0, 3, 4, 6, 2, 12, 1, 10, 14, 9},
                               {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
                               {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}};

unsigned char s_box5[4][16] = {{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
                               {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
                               {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
                               {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}};

unsigned char s_box6[4][16] = {{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
                               {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
                               {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
                               {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}};

unsigned char s_box7[4][16] = {{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
                               {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
                               {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
                               {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}};

unsigned char s_box8[4][16] = {{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
                               {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
                               {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
                               {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}};

unsigned char f_perm[32] = {16,  7, 20, 21, 29, 12, 28, 17,
                            1, 15, 23, 26,  5, 18, 31, 10,
                            2,  8, 24, 14, 32, 27,  3,  9,
                            19, 13, 30,  6, 22, 11,  4, 25};

                        



int32_t totalRound(int32_t input_state, int32_t round_key){
    return permutation(sBoxSubst(roundKeyXOR(expansion(input_state),round_key)));
}


/**
 * Take 32 bit round function input and expand to 48 bits
 * Using Expansion array
 */
int64_t expansion(int32_t round_input){
    int64_t mask = 0;
    int64_t i, target;
    for(i = 0;i < 48; i++){
        target = 32 - expansion_perm[i];
        mask |= ((round_input & (1 << target)) << (47 - i));
    } 
    return mask;
}

/**
 * Just XORs the round key to the current state
 * Doesn't really need to be a new function but I make the rules
 */
int64_t roundKeyXOR(int64_t expanded_input, int64_t round_key){
    return expanded_input ^ round_key;
}


int32_t sBoxSubst(int64_t s_box_input){
    int8_t first6   = (s_box_input & 0x0000FC0000000000) >> 42;
    int8_t second6  = (s_box_input & 0x000003F000000000) >> 36;
    int8_t thrid6   = (s_box_input & 0x0000000FC0000000) >> 30;
    int8_t fourth6  = (s_box_input & 0x000000003F000000) >> 24;
    int8_t fifth6   = (s_box_input & 0x0000000000FC0000) >> 18;
    int8_t sixth6   = (s_box_input & 0x000000000003F000) >> 12;
    int8_t seventh6 = (s_box_input & 0x0000000000000FC0) >> 6;
    int8_t eighth6  = (s_box_input & 0x000000000000003F);

    int8_t box1_row     = (first6 & 0x20 >> 4) | (first6 & 1);
    int8_t box1_column  = (first6 & 0x1E) >> 1;

    int8_t box2_row     = (second6 & 0x20 >> 4) | (second6 & 1);
    int8_t box2_column  = (second6 & 0x1E) >> 1;

    int8_t box3_row     = (thrid6 & 0x20 >> 4) | (thrid6 & 1);
    int8_t box3_column  = (thrid6 & 0x1E) >> 1;

    int8_t box4_row     = (fourth6 & 0x20 >> 4) | (fourth6 & 1);
    int8_t box4_column  = (fourth6 & 0x1E) >> 1;

    int8_t box5_row     = (fifth6 & 0x20 >> 4) | (fifth6 & 1);
    int8_t box5_column  = (fifth6 & 0x1E) >> 1;

    int8_t box6_row     = (sixth6 & 0x20 >> 4) | (sixth6 & 1);
    int8_t box6_column  = (sixth6 & 0x1E) >> 1;

    int8_t box7_row     = (seventh6 & 0x20 >> 4) | (seventh6 & 1);
    int8_t box7_column  = (seventh6 & 0x1E) >> 1;

    int8_t box8_row     = (eighth6 & 0x20 >> 4) | (eighth6 & 1);
    int8_t box8_column  = (eighth6 & 0x1E) >> 1;

    int32_t mask = 0;
    mask |= (s_box1[box1_row][box1_column]) << 28;
    mask |= (s_box2[box2_row][box2_column]) << 24;
    mask |= (s_box3[box3_row][box3_column]) << 20;
    mask |= (s_box4[box4_row][box4_column]) << 16;
    mask |= (s_box5[box5_row][box5_column]) << 12;
    mask |= (s_box6[box6_row][box6_column]) << 8;
    mask |= (s_box7[box7_row][box7_column]) << 4;
    mask |= (s_box8[box8_row][box8_column]);

    return mask;
}

int32_t permutation(int32_t s_box_output){
    int32_t mask = 0;
    int32_t i, target;
    for(i = 0;i < 32; i++){
        target = 32 - f_perm[i];
        mask |= ((s_box_output & (1 << target)) << (31 - i));
    } 
    return mask;    
}

int64_t swapHalves(int64_t new_right_half){
    int64_t mask = 0;
    mask |= (new_right_half >> 32) & 0x00000000FFFFFFFF;
    mask |= new_right_half << 32;
    return mask;
}