#ifndef FLAG
#include <stdint.h>
#include <stddef.h>
#define FLAG
#endif
void generateRoundKeys(int64_t secret_key1, int64_t secret_key2, int64_t * round_keys, size_t rounds);
int64_t removeParity(int64_t starting_key);
int32_t leftShift(int32_t register, size_t shift);
int64_t secondPerm(int32_t reg1, int32_t reg2);