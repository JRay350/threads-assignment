#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_threads.h"

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

void *mythread(void *arg) {
    FILE *data_fp = fopen("Data.txt", "r");

    // Establish thread's associated text file name
    char file_name[30];
    snprintf(file_name, sizeof(file_name), "Data_Thread_%s.txt", (char *) arg);

    // Create and open thread's associated text file with the established name
    FILE *thread_data_fp = fopen(file_name, "w+");
    if (!thread_data_fp) {
        char err_msg[60];
        snprintf(err_msg, sizeof(err_msg), "failed to create %s", file_name);
        perror(err_msg);
        return NULL;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    while ((nread = getline(&line, &len, data_fp)) != -1) {
        fwrite(line, nread, 1, thread_data_fp);
    }

    free(line);
    fclose(data_fp);
    fclose(thread_data_fp);

    return NULL;
}

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

    pthread_t p1, p2, p3;
    Pthread_create(&p1, NULL, mythread, "01");
    Pthread_create(&p2, NULL, mythread, "02");
    Pthread_create(&p3, NULL, mythread, "03");

    Pthread_join(p1, NULL);
    Pthread_join(p2, NULL);
    Pthread_join(p3, NULL);

    return 0;
}