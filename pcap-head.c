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
	int n = atoi(argv[1]);
	FILE *fp = fopen(argv[2], "r");
	char buf[512];

	copy_bytes(stdout, fp, 24);

	for (int i = 0; i < n; i++) {
		uint32_t packet_length;
		fread(buf, 1, 16, fp);
		fwrite(buf, 1, 16, stdout);
		packet_length = *((uint32_t *)(buf + 8));
		if (copy_bytes(stdout, fp, packet_length) < packet_length)
			return 1;
	}
}
