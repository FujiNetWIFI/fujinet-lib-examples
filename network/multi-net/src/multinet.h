#ifndef JSONTEST_H
#define JSONTEST_H

#include <stdint.h>
#include <stdbool.h>

// these will be overridden by make
#ifndef REST_SERVER_ADDRESS
#define REST_SERVER_ADDRESS "localhost"
#endif

// these will be overridden by make
#ifndef REST_SERVER_PORT
#define REST_SERVER_PORT "8080"
#endif

void createUrl(char *u, uint8_t n, char *text);
void handle_err(char *reason);
void setup();

void open(char *u);
void close(char *u);

void get(char *u);

#endif