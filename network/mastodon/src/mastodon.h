#ifndef MASTODON_H
#define MASTODON_H

void setup(void);
void handle_err(uint8_t err, char *reason);
void filter_buf(void);
void line(char type);
void pause(unsigned long time);

#endif