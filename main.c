#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "RoundFunc.h"
#include "KeySchedule.h"

int main(void){
    char input[16], output[16];
    size_t size = 16;

    printf("Please enter plaintext to encrypt:\n");
    while (fgets(input, size, stdin) == NULL){
        printf("ERROR - no input seen.  Try again!\n");
    }
    printf("\nInput taken as:\t%s",input);

    //printf("Plaintext:\t%s\t\t", input);

    size_t length = strlen(input);
    int64_t right_half = 0;
    int64_t left_half = 0;
    int64_t i, mask;
    for(i = 0;i < length; i++){
        if(input[i] == 0x0a)
            input[i] =0;
        mask = input[i];
        if(i <= 7){
            left_half |= mask << (56 - i*8);
        }else{
            right_half |= mask << (56 - (i-8)*8);
        }
    }

    for(i = 0;i < 16; i++){
        if(input[i] == 0x00)
            input[i] =0x5F;
    }
    printf("Plaintext:\t%s\t\t", input);

    //printf("\nInput as Hex is:\t%lx%lx\n\n",left_half, right_half);
    // secret key is "big chungus 4/!7"
    int64_t secret_key1 = 0x626967206368756E;
    int64_t secret_key2 = 0x67757320342F2137;
    int64_t round_Keys[32] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32};
    generateRoundKeys(secret_key1, secret_key2, round_Keys, 16);

    size_t round;
    int32_t funciton_in_1, function_in_2, function_out_1, function_out_2;
    int64_t new_right_half, new_left_half, rkey1, rkey2;

    /*Encryption Rounds*/
    for(round = 0;round < 16; round++){
        //printf("\nRound %lu Input as Hex is:\t%lx %lx\n\n",round, left_half, right_half);
        rkey1 = round_Keys[2*round]; 
        rkey2 = round_Keys[2*round + 1];
        new_left_half = right_half;
        // if(round == 15)
        //     printf("\nEncryption Round %lu Round Keys as Hex is:\t%014lx %014lx\n\n",round, rkey1, rkey2);
        funciton_in_1 = (right_half & 0xFFFFFFFF00000000) >> 32;
        function_in_2 = (right_half & 0xFFFFFFFF);

        function_out_1 = totalRound(funciton_in_1, rkey1);
        function_out_2 = totalRound(function_in_2, rkey2);
        // if(round == 15)
        //     printf("\nEn Round %lu Function Output as Hex is:\t%x %x\n\n",round, function_out_1, function_out_2);
        new_right_half = function_out_1;
        new_right_half &= 0x00000000FFFFFFFF;
        // if(round == 10){
        //     printf("\nDe Round %lu 1/2 Function Output as Hex is:\t%lx\n\n",round, new_right_half);
        // }
        new_right_half = (new_right_half << 32) | ((int64_t)function_out_2 & 0x00000000FFFFFFFF);;
        // if(round == 10){
        //     printf("\nEn Round %lu Function Output as Hex is:\t%lx\n\n",round, new_right_half);
        // }
        new_right_half ^= left_half;
        right_half = swapHalves(new_right_half);
        left_half = new_left_half;
        // if(round == 10){
        //      printf("\nRound %lu Output as Hex is:\t%lx %lx\n\n",round, left_half, right_half);
        // }
    }

    //printf("\nEncrypted Output as Hex is:\t%lx %lx\n\n",left_half, right_half);

    for(i = 0;i < length; i++){
        if(i <= 7){
            mask = (left_half  >> (56 - i*8)) & 0xFF;
            output[i] = mask;
        }else{
            mask = (right_half  >> (56 - (i-8)*8)) & 0xFF;
            output[i] = mask;
        }
    }

    // printf("\nOutput given as:\t%s\n",output);
    // printf("\nOutput given as:\t%d\n",output[0]);

    for(i = 0;i < 16; i++){
        if(output[i] == 0x00)
            output[i] =0x5F;
    }
    printf("Ciphertext:\t%s\n", output);

    //Swap left and right to start decryption
    new_left_half = swapHalves(right_half);
    right_half = left_half;
    left_half = new_left_half;


    /*Decryption Rounds - running encryption backwards*/
    for(round = 16;round > 0; round--){
        // if(round == 11){
        //     printf("\nDecryption Round %lu Input as Hex is:\t%016lx %016lx \n\n",round - 1, left_half, right_half);
        // }
        if(round % 2 == 0){
            rkey1 = round_Keys[2*round - 1]; 
            rkey2 = round_Keys[2*round - 2]; 
        }else{
            rkey2 = round_Keys[2*round - 1]; 
            rkey1 = round_Keys[2*round - 2]; 
        }
        new_left_half = right_half;
        // if(round == 16)
        //     printf("\nDecryption Round %lu Round Keys as Hex is:\t%014lx %014lx\n\n",round - 1, rkey1, rkey2);

        funciton_in_1 = (right_half & 0xFFFFFFFF00000000) >> 32;
        function_in_2 = (right_half & 0xFFFFFFFF);

        function_out_1 = totalRound(funciton_in_1, rkey2);
        function_out_2 = totalRound(function_in_2, rkey1);

        // if(round == 16){
        //     printf("\nDe Round %lu Function Output as Hex is:\t%x %x\n\n",round - 1, function_out_1, function_out_2);
        // }
        new_right_half = function_out_1;
        new_right_half &= 0x00000000FFFFFFFF;
        // if(round == 11){
        //     printf("\nDe Round %lu 1/2 Function Output as Hex is:\t%lx\n\n",round - 1, new_right_half);
        // }
        new_right_half = (new_right_half << 32) | ((int64_t)function_out_2 & 0x00000000FFFFFFFF);
        // if(round == 11){
        //     printf("\nDe Round %lu Function Output as Hex is:\t%lx\n\n",round - 1, new_right_half);
        // }
        new_right_half ^= left_half;
        right_half = swapHalves(new_right_half);
        left_half = new_left_half;

        
        //printf("\nDecryption Round %lu Output as Hex is:\t%016lx %016lx\n\n",round - 1, left_half, right_half);
    }

    //Swap left and right to end decryption
    new_left_half = swapHalves(left_half);
    left_half = right_half;
    right_half = new_left_half;
    

    //printf("\nDecrypted Output as Hex is:\t%lx%lx\n\n",left_half, right_half);

    for(i = 0;i < length; i++){
        if(i <= 7){
            mask = (left_half  >> (56 - i*8)) & 0xFF;
            output[i] = mask;
        }else{
            mask = (right_half  >> (56 - (i-8)*8)) & 0xFF;
            output[i] = mask;
        }
    }

    // printf("\nOutput given as:\t%s\n",output);
    // printf("\nOutput given as:\t%d\n",output[0]);
}