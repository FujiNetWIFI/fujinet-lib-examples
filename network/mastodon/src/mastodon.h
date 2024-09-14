#ifndef MASTODON_H
#define MASTODON_H

void setup(void);
void handle_err(uint8_t err, char *reason);
void filter_buf(void);
void line(char type);
void pause(unsigned long time);
void query_and_filter(char *query);

#if defined(__ATARI__)

#define HEADING_START_CHAR 0x15
#define MESSAGE_START_CHAR 0x12
#define MESSAGE_END_CHAR 0x95

#elif defined(__APPLE2__)

#define HEADING_START_CHAR '='
#define MESSAGE_START_CHAR '-'
#define MESSAGE_END_CHAR '-'

#else

#define HEADING_START_CHAR '='
#define MESSAGE_START_CHAR '-'
#define MESSAGE_END_CHAR '-'

#endif

#endif