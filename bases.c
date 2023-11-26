#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char to_lowercase(const char chr) {
  if (chr >= 'A' && chr <= 'Z') {
    return chr + 32;
  }

  return chr;
}

int char_to_int(const char chr) {
  const int lc_chr = to_lowercase(chr);

  if (lc_chr >= '0' && lc_chr <= '9') {
    return (int)lc_chr - '0';
  } else if (lc_chr >= 'a' && lc_chr <= 'z') {
    return (int)lc_chr - 'a' + 10;
  } else {
    printf("char %c is not alphanumeric\n", chr);
    exit(1);
  }
}

int str_to_int(const char *str, const int base) {
  int result = 0;
  const int str_len = strlen(str);

  for (int i = 0; i < str_len; ++i) {
    const int reverse_index = str_len - i - 1;
    const int digit = char_to_int(str[reverse_index]);
    result += digit * pow(base, i);
  }

  return result;
}

char num_to_char(int num) {
  assert(num >= 0 && num <= 35);

  if (num <= 9) {
    return (char)num + '0';
  } else {
    return (char)num + 'a' - 10;
  }
}

size_t count_digits(int num) {
  size_t count = 0;

  do {
    num /= 10;
    ++count;
  } while (num != 0);

  return count;
}

char *int_to_str(int num, int base) {
  const size_t digits = count_digits(num);
  const size_t str_len = (int)(ceil((float)digits * log(10) / log(base)));
  char result[str_len];

  for (int i = 0; i < str_len; ++i) {
    const int reverse_index = str_len - i - 1;
    result[reverse_index] = num_to_char(num % base);
    num /= base;
  }

  size_t leading_zeroes = 0;

  while (leading_zeroes < str_len && result[leading_zeroes] == '0') {
    ++leading_zeroes;
  }

  const size_t is_zero = leading_zeroes == str_len;
  const size_t final_str_len = str_len - leading_zeroes + is_zero;
  char *trimmed_result = malloc(final_str_len);

  for (int i = 0; i < final_str_len; ++i) {
    trimmed_result[i] = result[i + leading_zeroes - is_zero];
  }

  return trimmed_result;
}

char *int_to_bin(int num) { return int_to_str(num, 2); }
char *int_to_hex(int num) { return int_to_str(num, 16); }

void pad_left(char *str, char pad_char, size_t max_len) {
  const size_t str_len = strlen(str);

  if (max_len <= str_len)
    return;

  char tmp_str[str_len];
  memcpy(tmp_str, str, str_len);

  char *new_str = realloc(str, max_len);
  assert(new_str != NULL);

  const size_t pad_len = max_len - str_len;

  for (int i = 0; i < pad_len; ++i) {
    new_str[i] = pad_char;
  }

  for (int i = pad_len; i < max_len; ++i) {
    new_str[i] = tmp_str[i - pad_len];
  }
}

void pad_to_byte(char *str) { pad_left(str, '0', 8); }