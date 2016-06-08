/*
 * pcap-head - truncate packet captures
 *
 * Written in 2015 by Jakob Kramer <jakob.kramer@gmx.de>
 *
 * To the extent possible under law, the author(s) have dedicated all
 * copyright and related and neighboring rights to this software to the
 * public domain worldwide. This software is distributed without any
 * warranty.
 *
 * You should have received a copy of the CC0 Public Domain Dedication
 * along with this software.
 * If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
 */

#include <arpa/inet.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int min(int a, int b) {
	return a < b ? a : b;
}

uint32_t copy_bytes(FILE *dest, FILE *src, uint32_t n) {
	char buf[BUFSIZ];
	uint32_t i = 0;

	while (i < n) {
		size_t r = fread(buf, 1, min(BUFSIZ, n - i), src);
		fwrite(buf, 1, r, dest);
		i += r;
		if (r < BUFSIZ)
			break;
	}
	return i;
}

int main(int argc, char *argv[]) {
	int n;
	char buf[16];

	if (argc != 2) {
		fprintf(stderr, "usage: %s packets\n", argv[0]);
		return 1;
	}
	n = atoi(argv[1]);

	if(copy_bytes(stdout, stdin, 24) < 24)
		return 1;

	for (int i = 0; i < n; i++) {
		uint32_t packet_length;
		size_t r = fread(buf, 1, 16, stdin);
		if (r < 16) {
			if (feof(stdin) && r == 0)
				return 0;
			else
				return 1;
		}
		fwrite(buf, 1, 16, stdout);
		packet_length = *((uint32_t *)(buf + 8));
		if (copy_bytes(stdout, stdin, packet_length) < packet_length)
			return 1;
	}
}
