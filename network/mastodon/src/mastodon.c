#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#ifdef __APPLE2__
#include <apple2.h>
#include <peekpoke.h>
#include <6502.h>
#endif

#include "fujinet-network.h"

#include "mastodon.h"

uint8_t buffer[1024];
char url[] = "n1:https://oldbytes.space/api/v1/timelines/public?limit=1";
char display_name_query[] = "/0/account/display_name";
char created_at_query[] = "/0/created_at";
char content_query[] = "/0/content";
char version[] = "v1.1.0";
uint8_t err = 0;

#ifdef __APPLE2__
static bool lowercase;
#endif

void main(void)
{
	int count = 0;
	char line[41];
	setup();
	new_screen();
	while (1)
	{
		network_open(url, OPEN_MODE_READ, OPEN_TRANS_NONE);
		err = network_json_parse(url);
		handle_err("parse");

		count = network_json_query(url, display_name_query, (char *) buffer);
		if (count < 0) {
			err = -count;
			handle_err("query");
		}

		clrscr();
		hline(40);
		sprintf(line, "%40s", buffer);
		screen_print_inverse(line);

		count = network_json_query(url, created_at_query, (char *) buffer);
		if (count < 0) {
			err = -count;
			handle_err("query");
		}
		sprintf(line, "%40s", buffer);
		screen_print_inverse(line);
		hline(40);

		count = network_json_query(url, content_query, (char *) buffer);
		if (count < 0) {
			err = -count;
			handle_err("query");
		}
		printf("%s\n", buffer);
		hline(40);

		network_close(url);

		pause(32000);
	}
}

void setup()
{
	uint8_t init_r = 0;

#ifdef __APPLE2__
    if (get_ostype() >= APPLE_IIE)
    {
      POKE(0xC00F,0); // ALTCHAR
      allow_lowercase(true);
      lowercase = true;
    }
#endif


	init_r = network_init();
	bzero(buffer, 1024);

	if (init_r != 0)
	{
		err = init_r;
		handle_err("network_init failed");
	}


}

void new_screen()
{
	clrscr();
	gotoxy(0, 0);
	printf("mastodon %s\n", version);
}

void handle_err(char *reason)
{
	if (err)
	{
		printf("Error: %d (d: %d) %s\n", err, fn_device_error, reason);
		cgetc();
		exit(1);
	}
}

void pause(unsigned long time)
{
	unsigned long i;
	for (i = 0; i < time; i++)
		;
}

#ifdef __APPLE2__
static void iputc(char c)
{
  if (c >= 0x40 && c <= 0x5F /* uppercase */)
  {
    c += 0x40;
  }
  else if (c >= 0x20 /* printable */)
  {
    c += 0x80;
  }
  cputc(c);
}
#endif

static void hline(unsigned char l)
{
#ifdef __APPLE2__
  if (lowercase)
  {
    while (l--)
    {
      cputc(0xD3);
    }
  }
  else
  {
    chline(l);
  }
#else
	chline(l);
#endif

}

void screen_put_inverse(char c)
{
#ifdef __APPLE2__
  if (lowercase)
  {
    iputc(c);
  }
  else
  {
    revers(true);
    cputc(c);
    revers(false);
  }
#else
    revers(true);
    cputc(c);
    revers(false);
#endif

}

void screen_print_inverse(const char *s)
{
#ifdef __APPLE2__
  if (lowercase)
  {
    char c;
    while (c = *s++)
    {
      iputc(c);
    }
  }
  else
  {
    revers(true);
    cputs(s);
    revers(false);
  }
#else
    revers(true);
    cputs(s);
    revers(false);
#endif

}
