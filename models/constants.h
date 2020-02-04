#ifndef MY_MODEL_CONSTANTS_H
#define MY_MODEL_CONSTANTS_H

const uint8_t s_a[4] = {1, 2, 3, 4};
const uint8_t s_b[4] = {5, 6, 7, 8};
const uint8_t s_c[4] = {5, 6, 7, 8};
const uint8_t s_d[4] = {1, 1, 1, 1};
// out = a*b + c + d

const uint8_t s_out_ref[4] = {19 + 5 + 1, 22 + 6 + 1, 43 + 7 + 1, 50 + 8 + 1};
#endif
