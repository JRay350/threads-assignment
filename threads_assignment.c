#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

int main (int argc, char *argv[]) {
    // First open Data.txt
    FILE *data_fp = fopen("Data.txt", "w+");
    if (!data_fp) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    // Establish 1,000,000 integers to write in range [0, 100]
    for (int i = 0; i < 1000000; i++) {
        long random_number = random() % 101;
        char buffer[20];
        if (i == 999999) { // Don't print a new line after the last number
            fprintf(data_fp, "%ld", random_number);
        } else {
            fprintf(data_fp, "%ld\n", random_number);
        }
    }

    fclose(data_fp);

    return 0;
}