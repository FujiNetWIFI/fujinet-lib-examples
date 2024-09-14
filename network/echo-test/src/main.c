#include <cc65.h>
#include <conio.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fujinet-network.h"
#include "get_line.h"
#include "main.h"

uint8_t err = 0;
char endpoint[80];
char url_buffer[128];
char *echo_url = "/echo";

int main() {
    uint8_t hxp = 6;
    uint8_t txp = 2;
    uint8_t yps = 1;
    uint16_t i;
    uint8_t r;
    uint8_t i8;
    uint8_t err_count = 0;
    uint8_t x, y;

    clrscr();
    chlinexy(2, yps - 1, 36);
    revers(1);
    cputsxy(hxp, yps + 0, "                            ");
    cputsxy(hxp, yps + 1, " ECHO POST/READ Binary Test ");
    cputsxy(hxp, yps + 2, "                     v1.0.2 ");
    revers(0);

    chlinexy(2, yps + 3, 36);

    cursor(1);
    cputsxy(txp, yps + 4, "Please enter ECHO server URL:");
    cputsxy(txp, yps + 5, "> ");

    memset(endpoint, 0, 80);
    get_line(endpoint, 60);
    memmove(endpoint + 3, endpoint, 76);
    endpoint[0] = 'n';
    endpoint[1] = '1';
    endpoint[2] = ':';

    memset(url_buffer, 0, sizeof(url_buffer));
    strcat(url_buffer, endpoint);
    strcat(url_buffer, echo_url);

    cursor(0);
    cputsxy(txp, yps + 6, "Testing. Please wait.");

    for (i = 0; i < 256;  i++) {
        x = i % 32;
        y = i / 32;
        gotoxy(txp + x, yps + 7 + y);

        i8 = (uint8_t) (i & 0xFF);
        r = do_post(i8);
        if (r != i8) {
            cputc('E');
            gotoxy(txp + (err_count % 5) * 7, yps + 16 + (err_count / 5));
            printf("%02x->%02x\n", i8, r);
            err_count++;
        } else {
            cputc('.');
        }
    }
    gotoxy(txp, 22);
    printf("Completed.");
    cgetc();

    return 0;
}

void handle_err(char *reason) {
    if (err) {
        gotoxy(0, 20);
        cputs("Error: ");
        cputs(reason);

        if (doesclrscrafterexit()) {
            cgetc();
        }
        exit(1);
    }
}

uint8_t do_post(uint8_t v) {
    int n;
    uint8_t tries = 0;
    uint8_t buf[1];
    uint8_t res[1];

    buf[0] = v;

    // open the endpoint, try up to 5 times
    err = 1;
    while (err != 0) {
        err = network_open(url_buffer, OPEN_MODE_HTTP_POST, OPEN_TRANS_NONE);
        if (err != 0 && tries >= 5) {
            handle_err("post:open [5]");
        }
        tries++;
    }

    err = network_http_post_bin(url_buffer, buf, 1);
    handle_err("post:data");

    // read the response, this is just 1 byte. Try up to 5 times.
    n = -1;
    tries = 0;
    while (n < 0) {
        n = network_read(url_buffer, (uint8_t *)res, 1);
        if (n < 0 && tries >= 5) {
            err = -n;
            handle_err("read");
        }
        tries++;
    }

    network_close(url_buffer);
    return res[0];

}