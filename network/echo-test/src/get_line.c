#include <conio.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>

#include "get_line.h"

#ifndef CH_DEL
#define CH_DEL 0x7F
#endif

void get_line(char* buf, uint8_t max_len) {
	uint8_t c;
	uint16_t i = 0;
	uint8_t init_x = wherex();

	cursor(1);

	--max_len;
	do {
		gotox(init_x + i);

		c = cgetc();

		if (isprint(c)) {
			if (i < max_len) {
				putchar(c);
				buf[i++] = c;
			}
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
