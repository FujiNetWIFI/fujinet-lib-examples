#include <cc65.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#include "fujinet-clock.h"

char *version = "v1.0.0";

uint8_t buffer[32];
char clock_buffer[20];

void debug() {
}

void format_time(const uint8_t *buffer) {
    // Convert to "YYYY-MM-DD HH:mm:SS\0"
    
    uint8_t thous;
    uint8_t years;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;

    thous  = buffer[0];
    years  = buffer[1];
    month  = buffer[2];
    day    = buffer[3];
    hour   = buffer[4];
    minute = buffer[5];
    second = buffer[6];
    
    snprintf(clock_buffer, 20, "%02d%02d-%02d-%02d %02d:%02d:%02d", 
             thous, years, month, day, hour, minute, second);
}

int main(void)
{
	long clock_calls = 1;
	uint8_t i = 0;
	uint8_t current_sec, target_sec, last_sec;
	clrscr();
	printf("FujiNet Clock Looper %s\n\n", version);
	memset(buffer, 0, 32);
	printf("Counting to 30s...\n");

	clock_get_time(buffer, SIMPLE_BINARY);
	format_time(buffer);
	printf("Start: %s\n", clock_buffer);

	// capture the seconds, and keep looping getting the time until it's this value + 30 (mod 60)
	current_sec = buffer[6];
	last_sec = current_sec;
	target_sec = (current_sec + 30) % 60;
	printf("%d ", current_sec);

	while (current_sec != target_sec) {
		clock_get_time(buffer, SIMPLE_BINARY);
		clock_calls++;
		current_sec = buffer[6];
		if (current_sec != last_sec) {
			printf("%d ", current_sec);
			last_sec = current_sec;
		}
	}

	format_time(buffer);
	printf("\nFinish: %s\n", clock_buffer);

	printf("Called clock %ld times.\n", clock_calls);

	if (doesclrscrafterexit()) {
		printf("Press a key to exit");
		cgetc();
	}
	return 0;
}

void clear_buffer() {
	memset(buffer, 0, 128);
}
