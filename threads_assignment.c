/**
Name: Jacob Ray
Course Number: CPE3350/CSC3350
Assignment Title: Threads Assignment
Professor: Andy Cameron
Date: 5/30/2026
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "my_threads.h"

void *thread_routine(void *arg) {
    // Track time in seconds at beginning of thread execution
    double start_time = Time_GetSeconds();

    FILE *data_fp = fopen("Data.txt", "r");
    if (!data_fp) {
        perror("Data.txt fopen failed\n");
        return NULL;
    }    

    // Establish thread's associated text file name
    char file_name[30];
    snprintf(file_name, sizeof(file_name), "Data_Thread_%s.txt", (char *) arg);

    // Create and open thread's associated text file with the established name
    FILE *thread_data_fp = fopen(file_name, "w+");
    if (!thread_data_fp) {
        char err_msg[60];
        snprintf(err_msg, sizeof(err_msg), "failed to create %s\n", file_name);
        fclose(data_fp); // Close Data.txt if thread text file creation fails
        perror(err_msg);
        return NULL;
    }

    // For use in getline() call
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    // Continually grab each line in the original data file, apply to updating sum, and copy it over to the thread's associated file
    int i = 0;
    double avg = 0;

    while ((nread = getline(&line, &len, data_fp)) != -1) {
        fwrite(line, nread, 1, thread_data_fp);
        i++;

        double value_read = strtod(line, NULL); // No end pointer is used, so NULL is passed
        avg += value_read;
    }

    // As long as i is non-zero, use it as the number of values read to calculate average
    if (i != 0) {
        avg /= i;
    } else {
        char err_msg[60];
        snprintf(err_msg, sizeof(err_msg), "failed to calculate average for thread %s\n", (char *) arg);
        perror(err_msg);

        // Free dynamically allocated line data
        free(line);

        // Close Data.txt and Data_Thread_NN.txt files
        fclose(data_fp);
        fclose(thread_data_fp);

        return NULL;
    }
    printf("Thread %s Average: %.2f\n", (char *) arg, avg);

    // Free dynamically allocated line data
    free(line);

    // Close Data.txt and Data_Thread_NN.txt files
    fclose(data_fp);
    fclose(thread_data_fp);

    // Output the difference between end of execution and start
    printf("Thread %s Execution Time (ms): %f\n", (char *) arg, (Time_GetSeconds() - start_time) * 1000); // Convert difference from seconds to milliseconds within call

    return NULL;
}

int main (int argc, char *argv[]) {
    // Track time at beginning of program
    double start_time = Time_GetSeconds();

    // First open Data.txt
    FILE *data_fp = fopen("Data.txt", "w+");
    if (!data_fp) {
        perror("Data.txt fopen failed\n");
        return EXIT_FAILURE;
    }

    // Establish 1,000,000 integers to write in range [0, 100]
    srandom(time(NULL)); // Set seed for random() call

    for (int i = 0; i < 1000000; i++) {
        long random_number = random() % 101;
        if (i == 999999) { // Don't print a new line after the last number
            fprintf(data_fp, "%ld", random_number);
        } else {
            fprintf(data_fp, "%ld\n", random_number);
        }
    }

    fclose(data_fp);

    // Creation of threads
    pthread_t p1, p2, p3;
    Pthread_create(&p1, NULL, thread_routine, "01");
    Pthread_create(&p2, NULL, thread_routine, "02");
    Pthread_create(&p3, NULL, thread_routine, "03");

    // Wait for threads to finish before calculating execution time
    Pthread_join(p1, NULL);
    Pthread_join(p2, NULL);
    Pthread_join(p3, NULL);

    // Output the difference between end of execution and start
    printf("Total Program Execution Time (ms): %f\n", (Time_GetSeconds() - start_time) * 1000); // Convert difference from seconds to milliseconds within call

    return 0;
}