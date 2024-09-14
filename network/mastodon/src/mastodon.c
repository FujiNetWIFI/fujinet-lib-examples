#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#ifdef __APPLE2__
#include <apple2.h>
#endif

#ifdef __ATARI__
#include <atari.h>
#endif

#include "fujinet-network.h"

#include "mastodon.h"

unsigned char screen_width;
uint8_t buffer[1024];
char url[] = "n1:https://oldbytes.space/api/v1/timelines/public?limit=1";
char display_name_query[] = "/0/account/display_name";
char created_at_query[] = "/0/created_at";
char content_query[] = "/0/content";
char version[] = "v1.1.0";

void main(void)
{
	uint8_t err;
	int16_t count;

	setup();
	clrscr();
	printf("mastodon %s\n", version);

	while (1) {
		printf("\n\n");

		network_open(url, OPEN_MODE_READ, OPEN_TRANS_NONE);
		err = network_json_parse(url);
		if (err != FN_ERR_OK) {
			  handle_err(err, "parse");
		}

		line(HEADING_START_CHAR);

		query_and_filter(display_name_query);
		printf("%*s", screen_width, buffer);

		query_and_filter(created_at_query);
		printf("%*s", screen_width, buffer);

		line(MESSAGE_START_CHAR);

		query_and_filter(content_query);
		printf("%s\n", buffer);

		line(MESSAGE_END_CHAR);

		network_close(url);

		pause(52000);
	}
}

void setup(void)
{
	uint8_t init_r, screen_height;

#ifdef __APPLE2__
#ifdef __APPLE2ENH__
	videomode(VIDEOMODE_80COL);
#else
	if (get_ostype() >= APPLE_IIIEM) {
		allow_lowercase(true);
	}
#endif
#endif

#ifdef __ATARI__
	OS.color2 = 0;
#endif

	screensize(&screen_width, &screen_height);

	init_r = network_init();
	if (init_r != FN_ERR_OK) {
		handle_err(init_r, "network_init failed");
	}
}

void handle_err(uint8_t err, char *reason)
{
	printf("Error: %d (d: %d) %s\n", err, fn_device_error, reason);
	cgetc();
	exit(1);
}

void filter_buf(void)
{
	register char *c;

	for (c = buffer; *c != '\0'; ++c) {
		if (!isascii(*c) || !isprint(*c)) {
			*c = '?';
		}
	}
}

void query_and_filter(char *query) {
	int16_t count;
	uint16_t i;
	count = network_json_query(url, query, (char *) buffer);
	if (count < 0) {
			handle_err(-count, "query");
	}
	filter_buf();
}

void line(char type)
{
	uint8_t x = screen_width;

	while (x--) {
		putchar(type);
	}
}

void pause(unsigned long time)
{
	unsigned long i;

	for (i = 0; i < time; ++i)
		;
}
