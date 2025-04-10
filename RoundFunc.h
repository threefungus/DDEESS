#ifndef FLAG
#include <stdint.h>
#include <stddef.h>
#define FLAG
#endif
int32_t totalRound(int32_t input_state, int32_t round_key);
int64_t expansion(int32_t round_input);
int64_t roundKeyXOR(int64_t expanded_input, int64_t round_key);
int32_t sBoxSubst(int64_t s_box_input);
int32_t permutation(int32_t s_box_output);
int64_t swapHalves(int64_t new_right_half);