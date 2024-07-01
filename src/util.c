#include <stdio.h>
#include <string.h>

#include "util.h"

char *strtok_s_consume(char *str, const char *delim, char **ptr) {
  char *token = strtok_s(str, delim, ptr);
  printf("found: %s\n", token);

  if (token) {
    size_t delim_length = sizeof(delim);
    str = (char *)strcspn(str + strlen(token), delim) + delim_length;
  }

  printf("now: %s\n", str);

  return token;
}
