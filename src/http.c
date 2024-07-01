#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "http.h"

HttpRequestMethod string_to_http_method(const char *string) {
  if (strcmp(string, "GET") == 0) {
    return HTTP_GET;
  } else if (strcmp(string, "POST") == 0) {
    return HTTP_POST;
  }

  return HTTP_INVALID_METHOD;
}

HttpRequest *parse_request(char *buffer) {
  // NOTE: there is absolutely a memory leak here since i'm incrementing the
  // string buffer, maybe fix it idk
  size_t buffer_length = strlen(buffer);
  char *buffer_context;
  char *first_line = strtok_s(buffer, "\r\n", &buffer_context);

  char *first_line_context;
  char *method_string = strtok_s(first_line, " ", &first_line_context);
  first_line = first_line + strlen(method_string) + 1;
  HttpRequestMethod method = string_to_http_method(method_string);

  char *path_string = strtok_s(first_line, " ", &first_line_context) + 1;

  HttpRequest *request = malloc(sizeof(HttpRequest));
  request->method = method;
  request->path = path_string;

  return request;
}
