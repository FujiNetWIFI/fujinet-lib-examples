#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "multinet.h"
#include "fujinet-network.h"


char u1[50]; // e.g. becomes "n1:http://localhost:8080/text/n1";
char u2[50]; // e.g. becomes "n2:http://localhost:8080/text/n2";
char u3[50];
char u4[50];
char u5[50];
// char u6[50];
// char u7[50];
// char u8[50];

char result[120];
uint8_t err = 0;

uint16_t conn_bw;
uint8_t connected;
uint8_t conn_err;

bool sure = false;

char *version = "v1.0.0";

int main(void) {
    int i = 0;
    setup();

    printf("multinet %s\n", version);

    createUrl(u1, 1, "n1");
    createUrl(u2, 2, "n2");
    createUrl(u3, 3, "n3");
    // createUrl(u4, 4, "n4");
    // createUrl(u5, 5, "n5");
    // createUrl(u6, 6, "n6");
    // createUrl(u7, 7, "n7");
    // createUrl(u8, 8, "n8");

    // hit the endpoints hard so we can test memory
    for (i = 0; i < 20; i++) {
        err = network_open(u1, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE); handle_err("open u1");
        err = network_open(u2, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE); handle_err("open u2");
        err = network_open(u3, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE); handle_err("open u3");
        // err = network_open(u4, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE); handle_err("open u4");
        // err = network_open(u5, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE); handle_err("open u5");
        // FN runs out of memory on about 6, and 5 is stable.
        // err = network_open(u6, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE); handle_err("open u6");
        // err = network_open(u7, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE); handle_err("open u7");
        // err = network_open(u8, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE); handle_err("open u8");

        // 
        get(u1); printf("%s", result);
        get(u2); printf("%s", result);
        get(u3); printf("%s", result);
        // get(u4); printf("%s", result);
        // get(u5); printf("%s", result);
        // get(u6); printf("6");
        // get(u7); printf("7");
        // get(u8); printf("8");

        // the fujinet does handle reusing the network subdevice by manually closing an open connection
        close(u1);
        close(u2);
        close(u3);
        // close(u4);
        // close(u5);

        // close(u6);
        // close(u7);
        // close(u8);

    }
    // we do, however, want to close the open connections at the end to free up all the used memory.
    // if it's done above instead, it isn't needed here.
    // This way of doing it at the end is slightly quicker for the overall time, as there's less FN commands being sent.
    close(u1);
    close(u2);
    close(u3);
    // close(u4);
    // close(u5);

    // close(u6);
    // close(u7);
    // close(u8);

    printf("\npress a key to exit.");
    cgetc();

    return 0;
}


void handle_err(char *reason) {
    if (err) {
        printf("Error: %d (d: %d) %s\n", err, fn_device_error, reason);
        printf("press a key to exit.");
        cgetc();
        exit(1);
    }
}

void setup() {
    clrscr();
    gotox(0);
    bzero(result, 120);
}

void open(char *u) {
    err = network_open(u, OPEN_MODE_HTTP_GET, OPEN_TRANS_NONE);
    handle_err("open");
}

void close(char *u) {
    err = network_close(u);
    handle_err("close");
}

void get(char *u) {
    int count = 0;
    bzero(result, 5);
    count = network_read(u, (uint8_t *) result, 5);
    if (count < 0) {
        err = -count;
        handle_err("read");
    }
}

void createUrl(char *u, uint8_t n, char *text) {
  sprintf(u, "n%d:http://%s:%s/text/%s", n, REST_SERVER_ADDRESS, REST_SERVER_PORT, text);
}

