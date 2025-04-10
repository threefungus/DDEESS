#include "KeySchedule.h"

unsigned char PC1_perm[56] = {57, 49, 41, 33, 25, 17,  9,  1,
                              58, 50, 42, 34, 26, 18, 10,  2,
                              59, 51, 43, 35, 27, 19, 11,  3,
                              60, 52, 44, 36, 63, 55, 47, 39,
                              31, 23, 15,  7, 62, 54, 46, 38,
                              30, 22, 14,  6, 61, 53, 45, 37,
                              29, 21, 13,  5, 28, 20, 12,  4};
     

unsigned char PC2_perm[48] = {14, 17, 11, 24,  1,  5,  3, 28,
                              15,  6, 21, 10, 23, 19, 12,  4,
                              26,  8, 16,  7, 27, 20, 13,  2,
                              41, 52, 31, 37, 47, 55, 30, 40,
                              51, 45, 33, 48, 44, 49, 39, 56,
                              34, 53, 46, 42, 50, 36, 29, 32};

/**
 * Generate 2 56-bit round key halves, placing them in pairs into the array round_keys
 * Start using the secret_key split into 2 halves, remove parity bits
 * Then shift to left, and pair down 56-bits to create 48-bit round key
 */
void generateRoundKeys(int64_t secret_key1, int64_t secret_key2, int64_t * round_keys, size_t rounds){
    int64_t temp1, temp2;
    size_t i;
    int64_t no_parity1 = removeParity(secret_key1);
    int64_t no_parity2 = removeParity(secret_key2);
    int32_t regA = (int32_t)((no_parity1 & 0x00FFFFFFF0000000) >> 28) & 0x0FFFFFFF;
    int32_t regB = (int32_t)(no_parity1 & 0x000000000FFFFFFF) & 0x0FFFFFFF;
    int32_t regC = (int32_t)((no_parity2 & 0x00FFFFFFF0000000) >> 28) & 0x0FFFFFFF;
    int32_t regD = (int32_t)(no_parity2 & 0x000000000FFFFFFF) & 0x0FFFFFFF;
    for(i = 1;i <= rounds;i++){
        if(i == 1|| i == 2 || i == 9 || i == 16){
            regA = leftShift(regA, 1);
            regB = leftShift(regB, 1);
            regC = leftShift(regC, 1);
            regD = leftShift(regD, 1);
        }else{
            regA = leftShift(regA, 2);
            regB = leftShift(regB, 2);
            regC = leftShift(regC, 2);
            regD = leftShift(regD, 2);
        }
        temp1 = secondPerm(regA, regB);
        temp2 = secondPerm(regC, regD);
        *(round_keys + (2 * i - 2)) = temp1;
        *(round_keys + (2 * i - 1)) = temp2;
    }
}

/**
 * Apply PC-1 to remove parity bits from each half of secret key
 * Assembles bits into 56-bit register
 */
int64_t removeParity(int64_t starting_key){
    int64_t mask = 0;
    int64_t result = 0;
    size_t i;
    for(i = 0;i < 56;i++){
        mask = (starting_key & (1 << (63 - PC1_perm[i]))) >> (63 - PC1_perm[i]);
        result |= (mask & 1) << (55 - i);
    }
    return result;
}

/**
 * Wrapping left shift of 28 bit register
 * @param shift - number of bits to left to left
 */
int32_t leftShift(int32_t reg, size_t shift){
    size_t i;
    int32_t bit27_mask;
    for(i = 0;i < shift;i++){
        bit27_mask = (reg & (1 << 27));
        reg = (reg << 1) & 0x0FFFFFFF;
        reg |= bit27_mask  >> 27;
    }
    return reg;
}

/**
 * Apply PC-2 to isolate a half of a round key
 * Assembles 2 28-bit registers into one 56-bit register
 * Pair down into a 48-bit round key
 */
int64_t secondPerm(int32_t reg1, int32_t reg2){
    int64_t start = reg1;
    start = (start & 0x000000000FFFFFFF) << 27;
    start |= ((int64_t) reg2) & 0x000000000FFFFFFF;

    int64_t mask = 0;
    int64_t result = 0;
    size_t i;
    for(i = 0;i < 48; i++){
        mask = (start & (1 << (55 - PC2_perm[i]))) >> (55 - PC2_perm[i]);
        result |= (mask & 1) << (47 - i);
    }
    return result;
}