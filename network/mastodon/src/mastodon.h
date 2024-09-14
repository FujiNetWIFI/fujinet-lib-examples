#ifndef MASTODON_H
#define MASTODON_H

void handle_err(char *reason);
void setup();
void new_screen();
void pause(unsigned long time);

static void hline(unsigned char l);
void screen_put_inverse(char c);
void screen_print_inverse(const char *s);

#ifdef __APPLE2__
static void iputc(char c);
#endif

#endif