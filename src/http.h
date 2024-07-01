#pragma once

#include <string.h>

typedef enum HttpRequestMethod {
  HTTP_GET,
  HTTP_POST,
  HTTP_INVALID_METHOD,
} HttpRequestMethod;

typedef struct HttpRequest {
  HttpRequestMethod method;
  const char *path;
} HttpRequest;

HttpRequestMethod string_to_http_method(const char *string);

HttpRequest *parse_request(char *buffer);
