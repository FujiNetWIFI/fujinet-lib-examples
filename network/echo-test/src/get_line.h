#ifndef BWC_GETLINE_H
#define BWC_GETLINE_H

#if defined(__APPLE2__)
#include <apple2.h>

// apple2.h is missing CH_DEL
#ifndef CH_DEL
#define CH_DEL 0x7F
#endif

#elif defined(__ATARI__)
#include <atari.h>
#elif defined(__CBM__)
#include <cbm.h>
#endif

#include <stdint.h>

void get_line(char* buf, uint8_t max_len);

#endif // BWC_GETLINE_H
