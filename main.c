#include "md5.c"

int main() {
  const char *calculated_md5 = calculate_md5("Hello, world!");

  printf("%s\n", calculated_md5);

  free((char *)calculated_md5);
}