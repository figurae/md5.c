#include <stdint.h>
#include <stdio.h>

#include "bases.c"
#include "endianness.c"

// #define STRING "They are deterministic"
// #define STRING "Hello, world!"
#define STRING "test"
#define INPUT_TXT_SIZE 56
#define INPUT_LEN_SIZE 8
#define BLOCK_SIZE (INPUT_TXT_SIZE + INPUT_LEN_SIZE)
#define WORD_COUNT 16
#define WORD_SIZE (BLOCK_SIZE / WORD_COUNT)
#define OP_COUNT 64

// #define A 0x01234567
// #define B 0x89abcdef
// #define C 0xfedcba98
// #define D 0x76543210

#define A 0x67452301
#define B 0xefcdab89
#define C 0x98badcfe
#define D 0x10325476

#define F(X, Y, Z) ((X & Y) | (~X & Z))
#define G(X, Y, Z) ((X & Z) | (Y & ~Z))
#define H(X, Y, Z) (X ^ Y ^ Z)
#define I(X, Y, Z) (Y ^ (X | ~Z))

#define ROTATE_LEFT(X, N) (X << N) | (X >> (32 - N))

static uint32_t S[] = {7,  12, 17, 22, 7,  12, 17, 22, 7,  12, 17, 22, 7,
                       12, 17, 22, 5,  9,  14, 20, 5,  9,  14, 20, 5,  9,
                       14, 20, 5,  9,  14, 20, 4,  11, 16, 23, 4,  11, 16,
                       23, 4,  11, 16, 23, 4,  11, 16, 23, 6,  10, 15, 21,
                       6,  10, 15, 21, 6,  10, 15, 21, 6,  10, 15, 21};

static uint32_t K[OP_COUNT];

void initialize_k(uint32_t *arr) {
  for (int i = 0; i < OP_COUNT; ++i) {
    arr[i] = (uint32_t)floor(fabs(sin((double)i + 1.0)) * pow(2.0, 32.0));
  }
}

int main() {
  const size_t str_len = strlen(STRING);

  initialize_k(K);

  if (BLOCK_SIZE * 8 - str_len * 8 <= INPUT_LEN_SIZE * 8) {
    printf("support for 2+ blocks to be implemented\n");
    exit(1);
  }

  uint8_t block[BLOCK_SIZE] = {0};
  memcpy(block, STRING, str_len);

  // append bit 1
  block[str_len] = 0x80;

  // append 64-bit input length
  uint64_t input_len = str_len * 8;
  memcpy(&block[BLOCK_SIZE - INPUT_LEN_SIZE], &input_len, INPUT_LEN_SIZE);

  // split message into 16 32-byte words
  uint32_t M[WORD_COUNT] = {0};
  for (int i = 0; i < WORD_COUNT; ++i) {
    memcpy(&M[i], &block[i * WORD_SIZE], WORD_SIZE);
  }

  uint32_t R[] = {A, B, C, D};

  uint32_t AA = A;
  uint32_t BB = B;
  uint32_t CC = C;
  uint32_t DD = D;

  for (uint32_t i = 0; i < OP_COUNT; ++i) {
    uint32_t function_result;
    uint32_t message_index;

    switch (i / 16) {
    case 0:
      function_result = F(BB, CC, DD);
      message_index = i;
      break;
    case 1:
      function_result = G(BB, CC, DD);
      message_index = (5 * i + 1) % 16;
      break;
    case 2:
      function_result = H(BB, CC, DD);
      message_index = (3 * i + 5) % 16;
      break;
    default:
      function_result = I(BB, CC, DD);
      message_index = (7 * i) % 16;
      break;
    }

    function_result += AA + K[i] + M[message_index];
    AA = DD;
    DD = CC;
    CC = BB;
    BB += ROTATE_LEFT(function_result, S[i]);
  }

  R[0] += AA;
  R[1] += BB;
  R[2] += CC;
  R[3] += DD;

  uint32_t final_md5[] = {reverse_endianness(R[0]), reverse_endianness(R[1]),
                          reverse_endianness(R[2]), reverse_endianness(R[3])};

  printf("calculated md5: %.8x%.8x%.8x%.8x\n", final_md5[0], final_md5[1],
         final_md5[2], final_md5[3]);

  // for (int i = 0; i < BLOCK_SIZE; ++i) {
  //   // char *bin_chr = int_to_bin(block[i]);
  //   // pad_to_byte(bin_chr);
  //   printf("0x%.2x\n", K[i]);
  //   // free(bin_chr);
  // }
  // printf("\n");
}