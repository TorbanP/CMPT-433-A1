#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <stdlib.h>

int main() {

	// Read cpuinfo to show CPU, Cores (assigned to VM) and clock frequency
	FILE *fp1 = fopen("/proc/cpuinfo", "r");
	assert(fp1 != NULL);
	size_t n1 = 0;
	char *line1 = NULL;
	while (getline(&line1, &n1, fp1) > 0) {
		if (strstr(line1, "model name") || strstr(line1, "cpu cores") || strstr(line1, "cpu MHz")) {
			printf("%s", line1);
		}
	}
	free(line1);
	fclose(fp1);

	// read meminfo to show ram available and toral
	FILE *fp2 = fopen("/proc/meminfo", "r");
	assert(fp2 != NULL);
	size_t n2 = 0;
	char *line2 = NULL;
	while (getline(&line2, &n2, fp2) > 0) {
		if (strstr(line2, "MemTotal:") || strstr(line2, "MemFree:")) {
			printf("%s", line2);
		}
	}
	free(line2);
	fclose(fp2);

	// Show OS Version
	FILE *fp3 = fopen("/proc/version", "r");
	assert(fp3 != NULL);
	size_t n3 = 0;
	char *line3 = NULL;
	while (getline(&line3, &n3, fp3) > 0) {
		printf("%s", line3);
	}
	free(line3);
	fclose(fp3);

    return errno;
}