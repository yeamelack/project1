#include "include/utility.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define WORD_LEN 128
#define S 1  // Time limit for slow process
#define L 5  // Time limit for stuck process

void handle_child(int index, pid_t pid) {
    int status;
    waitpid(pid, &status, 0);

    if (WIFEXITED(status)) {
        int exit_status = WEXITSTATUS(status);
        if (exit_status == 0) {
            printf("Exit status of child %d was incorrect\n", index);
            // Handle event 3: Incorrect answer
        } else {
            printf("Exit status of child %d was correct\n", index);
            // Handle event 1: Correct answer
        }
    } else if (WIFSIGNALED(status)) {
        printf("Child %d terminated by signal %d\n", index, WTERMSIG(status));
        // Handle event 4: Child process crashed
    }
}

void handle_stuck_child(int index, pid_t pid) {
    sleep(S);  // Allow some time to see if it completes
    int status;
    int result = waitpid(pid, &status, WNOHANG);

    if (result == 0) {
        // Child process still running after S seconds
        kill(pid, SIGTERM);  // Attempt graceful termination
        sleep(1);  // Wait for the process to exit gracefully
        result = waitpid(pid, &status, WNOHANG);

        if (result == 0) {
            kill(pid, SIGKILL);  // Forcefully terminate if not terminated gracefully
            printf("Child %d killed: Stuck in an infinite loop or blocked\n", index);
            // Handle event 5 or 6: Stuck in an infinite loop or blocked
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <num_students> <program1> <program2> ... <programN>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    write_filepath_to_submissions("/home/sakha019/CSCI4061/p1/solutions", "/home/sakha019/CSCI4061/p1/submission.txt");
    FILE *file = fopen("/home/sakha019/CSCI4061/p1/submission.txt", "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int numStudents = atoi(argv[1]);
    pid_t pids[numStudents];
    char buff[WORD_LEN];
    char students[numStudents][WORD_LEN];

    for (int i = 0; i < numStudents && fgets(buff, sizeof(buff), file) != NULL; i++) {
        buff[strcspn(buff, "\n")] = 0;
        strncpy(students[i], buff, sizeof(students[i]) - 1);
    }

    for (int k = 2; k < argc; k++) {
        for (int j = 0; j < numStudents; j++) {
            pid_t pid = fork();
            if (pid < 0) {
                perror("Fork failed");
                exit(EXIT_FAILURE);
            } else if (pid == 0) {
                // Child process code
                printf("Executing: %s %s\n", students[j], argv[k]);
                execl(students[j], students[j], argv[k], NULL);

                // If execl fails, print an error message
                perror("Exec failed");
                exit(EXIT_FAILURE);
            } else {
                // Parent process
                pids[j] = pid;
            }
        }

        // Parent process waits for all child processes
        sleep(L);  // Wait for L seconds to allow processes to finish
        for (int i = 0; i < numStudents; i++) {
            handle_child(i + 1, pids[i]);
            handle_stuck_child(i + 1, pids[i]);
        }
    }

    fclose(file);
    return 0;
}

