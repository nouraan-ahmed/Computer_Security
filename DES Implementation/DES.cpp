#include<intrin.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>
#include <string>

typedef unsigned long long u64;

using namespace std;


/******GLOBAL VARIABLES*******/

/*********** Parity bit drop table *************/

int initial_key_permutaion[] = { 57, 49, 41, 33, 25, 17, 9,
                                1, 58, 50, 42, 34, 26, 18,
                                10, 2, 59, 51, 43, 35, 27,
                                19, 11, 3, 60, 52, 44, 36,
                                63, 55, 47, 39, 31, 23, 15,
                                7, 62, 54, 46, 38, 30, 22,
                                14, 6, 61, 53, 45, 37, 29,
                                21, 13, 5, 28, 20, 12, 4 };

/*********** Initial permutation table ***********/

int initial_message_permutation[] = { 58, 50, 42, 34, 26, 18, 10, 2,
                                        60, 52, 44, 36, 28, 20, 12, 4,
                                        62, 54, 46, 38, 30, 22, 14, 6,
                                        64, 56, 48, 40, 32, 24, 16, 8,
                                        57, 49, 41, 33, 25, 17, 9, 1,
                                        59, 51, 43, 35, 27, 19, 11, 3,
                                        61, 53, 45, 37, 29, 21, 13, 5,
                                        63, 55, 47, 39, 31, 23, 15, 7 };


/*********** Number of bit Shifts ************/

int key_shift_sizes[] = { 1, 1, 2, 2,
                        2, 2, 2, 2,
                        1, 2, 2, 2,
                        2, 2, 2, 1 };

/********** Key compression table ************/

int sub_key_permutation[] = { 14, 17, 11, 24, 1, 5,
                                3, 28, 15, 6, 21, 10,
                                23, 19, 12, 4, 26, 8,
                                16, 7, 27, 20, 13, 2,
                                41, 52, 31, 37, 47, 55,
                                30, 40, 51, 45, 33, 48,
                                44, 49, 39, 56, 34, 53,
                                46, 42, 50, 36, 29, 32 };

/********** Expansion D-box table ************/

int message_expansion[] = { 32, 1, 2, 3, 4, 5, 4, 5,
                      6, 7, 8, 9, 8, 9, 10, 11,
                      12, 13, 12, 13, 14, 15, 16, 17,
                      16, 17, 18, 19, 20, 21, 20, 21,
                      22, 23, 24, 25, 24, 25, 26, 27,
                      28, 29, 28, 29, 30, 31, 32, 1 };

/********** S-box table ************/

int s[512] = {
                        14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
                          0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
                          4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
                          15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13,
                        15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
                          3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
                          0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
                          13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9,
                        10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
                          13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
                          13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
                          1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12,
                        7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
                          13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
                          10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
                          3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14,
                        2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
                          14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
                          4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
                          11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3,
                        12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
                          10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
                          9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
                          4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13,
                        4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
                          13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
                          1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
                          6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12,
                        13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
                          1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
                          7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
                          2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11 
};


/********** Straight permutation table ************/

int right_sub_message_permutation[] = { 16, 7, 20, 21,
                                            29, 12, 28, 17,
                                            1, 15, 23, 26,
                                            5, 18, 31, 10,
                                            2, 8, 24, 14,
                                            32, 27, 3, 9,
                                            19, 13, 30, 6,
                                            22, 11, 4, 25 };

/********** Final permutation table ************/

int final_message_permutation[] = { 40, 8, 48, 16, 56, 24, 64, 32,
                                    39, 7, 47, 15, 55, 23, 63, 31,
                                    38, 6, 46, 14, 54, 22, 62, 30,
                                    37, 5, 45, 13, 53, 21, 61, 29,
                                    36, 4, 44, 12, 52, 20, 60, 28,
                                    35, 3, 43, 11, 51, 19, 59, 27,
                                    34, 2, 42, 10, 50, 18, 58, 26,
                                    33, 1, 41, 9, 49, 17, 57, 25 };


/************* Functions ************/

// Read DES INPUT

u64 read_DES_input(const char* data)
{
    assert(strlen(data) == 16);
    u64 value = 0;
    for (int i = 0; i < 16; ++i)
    {
        char c = data[i];
        if (c >= '0' && c <= '9')
            value |= (u64)(c - '0') << ((15 - i) << 2);
        else if (c >= 'A' && c <= 'F')
            value |= (u64)(c - 'A' + 10) << ((15 - i) << 2);
        else if (c >= 'a' && c <= 'f')
            value |= (u64)(c - 'a' + 10) << ((15 - i) << 2);
    }
    return value;
}

u64 permutation(int* permutation, int m, int n, u64 in) {

    u64 out = 0;
    for (int i = 0; i < n; ++i) {
        out |= (in >> (m - permutation[n - 1 - i]) & 1) << i;
    }
    return out;
}

u64 des_encryption(u64 plain, vector<u64>& roundkeys) {

    //initial permutation

    u64 ptt = permutation(initial_message_permutation, 64,64,plain);

    //Splitting the plain text left and right

    u64 leftplaintext = (ptt>>32)&(0xFFFFFFFF);
    u64 rightplaintext = ptt & (0xFFFFFFFF);


    for (int i = 0; i < 16; i++) {

        //Expansion permutation

        u64 right_expanded = permutation(message_expansion,32, 48, rightplaintext);

        //XOR Unique Key With Right plain Text

        u64 x = roundkeys[i] ^ right_expanded;

        //substitution/Choice (S-box)

        u64 idx = 0;
        u64 op=0;

        for (int k = 0; k < 8; k++) {
            auto box = s + (k << 6);
            idx = x >> (7 - k) * 6 & 0x3F;
            idx = idx >> (u64)1 & 15 | (idx & 1) << 4 | idx & 0x20;
            op |= box[idx] << (7 - k) * 4;
       }

        //permutation (P)

        op = permutation( right_sub_message_permutation,32,32, op);

        x = op^leftplaintext;

        leftplaintext = x;

        //swap
        if (i != 15) {
            swap(leftplaintext, rightplaintext);
        }
    }

    //combination
    u64 combinee = (leftplaintext<<32) | rightplaintext;

    //Inverse Initial Permutation
    u64 cipher = permutation(final_message_permutation, 64,64, combinee);
    return cipher;

}


int main(int argc, char** argv)
{

    u64 plain = read_DES_input(argv[2]);
    u64 keyy = read_DES_input(argv[3]);

    vector<u64> roundkeys;
    u64 leftkey = 0;
    u64 rightkey = 0;

    u64 combine = 0;
    u64 roundkey = 0;

    //key split
    keyy = permutation(initial_key_permutaion, 64, 56, keyy);

    //split the key to left part and right part
    leftkey = (keyy >> 28) & (0xFFFFFFF);
    rightkey = keyy & (0xFFFFFFF);

    for (int i = 0; i < 16; i++) {
        //left circular shift
        leftkey = ((leftkey << key_shift_sizes[i]) | (leftkey >> 28 - key_shift_sizes[i])) & 0xFFFFFFF;
        rightkey = ((rightkey << key_shift_sizes[i]) | (rightkey >> 28 - key_shift_sizes[i])) & 0xFFFFFFF;

        //combining

        combine = (leftkey << 28) | rightkey;

        //key compression
        roundkey = permutation(sub_key_permutation, 56, 48, combine);

       // if (encrypt) {
            //round key in hexadecimal
            roundkeys.push_back(roundkey);
       // }

    }
    string str = argv[1];

    if (str== "encrypt") {       
        long long t1 = __rdtsc();
        u64 result = des_encryption(plain, roundkeys);
        long long t2 = __rdtsc();
        printf("cipher: %016llX\n", result);
        printf("cycles: %lld\n", t2 - t1);
    }
    else if(str== "decrypt"){
        reverse(roundkeys.begin(), roundkeys.end());
        long long t1 = __rdtsc();
        u64 result = des_encryption(plain, roundkeys);
        long long t2 = __rdtsc();
        printf("cipher: %016llX\n", result);
        printf("cycles: %lld\n", t2 - t1);
    }
    return 0;
}