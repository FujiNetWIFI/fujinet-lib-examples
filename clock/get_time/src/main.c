#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#include "fujinet-clock.h"
#include "main.h"

char *version = "v1.0.1";

uint8_t buffer[128];
char current_tz[128];
uint8_t i = 0;

void debug() {
}

int main(void)
{
	clrscr();
	printf("FujiNet Clock %s\n\n", version);

	// returns bytes
	prodos_time();
	simple_time();
	ape_tz_time();
	ape_utc_time();

	printf("\n");

	// returns strings
	utc_time();
	iso_time();

	// test getting and setting the current timezone
	test_tz();

	cgetc();
	return 0;
}

void clear_buffer() {
	memset(buffer, 0, 32);
}

void prodos_time() {
	clear_buffer();
	clock_get_time(buffer, PRODOS_BINARY);
	printf("prodos bytes: ");
	for (i = 0; i < 4; i++) {
		printf("%02x ", buffer[i]);
	}
	printf("\n");
}

void simple_time() {
	clear_buffer();
	clock_get_time(buffer, SIMPLE_BINARY);
	printf("simple bytes: ");
	for (i = 0; i < 7; i++) {
		printf("%02x ", buffer[i]);
	}
	printf("\n");
}

void iso_time() {
	clear_buffer();
	clock_get_time(buffer, TZ_ISO_STRING);
	printf(" iso_tz: ");
	printf("%s\n", buffer);
}

void utc_time() {
	clear_buffer();
	clock_get_time(buffer, UTC_ISO_STRING);
	printf("iso_utc: ");
	printf("%s\n", buffer);
}

void ape_tz_time() {
	clear_buffer();
	clock_get_time(buffer, APETIME_TZ_BINARY);
	printf("ape_tz bytes: ");
	for (i = 0; i < 6; i++) {
		printf("%02x ", buffer[i]);
	}
	printf("\n");
}

void ape_utc_time() {
	clear_buffer();
	clock_get_time(buffer, APETIME_BINARY);
	printf("apeutc bytes: ");
	for (i = 0; i < 6; i++) {
		printf("%02x ", buffer[i]);
	}
	printf("\n");
}

void test_tz() {
	// get the current system TZ
	memset(current_tz, 0, 128);

	// debug();
	clock_get_tz(current_tz);
	printf("Current tz: %s\n", current_tz);

	printf("\nSet to W.Australia (+8:45)\n");
	// counterintuitively you have to invert the sign
	clock_set_tz("UTC-8:45");

	// should be offset with new TZ
	iso_time();
	utc_time();

	// restore the old TZ
	printf("\nRestoring TZ\n");
	clock_set_tz(current_tz);

	clock_get_tz(buffer);
	printf("Current tz: %s\n", buffer);

	// and should be back to original iso
	iso_time();
	utc_time();

}