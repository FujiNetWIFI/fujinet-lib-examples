#include <conio.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>

#include "get_line.h"

void get_line(char* buf, uint8_t max_len) {
	uint8_t c;
	uint16_t i = 0;
	uint8_t init_x = wherex();

	cursor(1);

	do {
		gotox(i + init_x);

		c = cgetc();

		if (isprint(c)) {
			putchar(c);
			buf[i] = c;
			if (i < max_len - 1) i++;
		}
		else if ((c == CH_CURS_LEFT) || (c == CH_DEL)) {
			if (i) {
				putchar(CH_CURS_LEFT);
				putchar(' ');
				putchar(CH_CURS_LEFT);
				--i;
			}
		}
	} while (c != CH_ENTER);
	putchar('\n');
	buf[i] = '\0';

	cursor(0);
}