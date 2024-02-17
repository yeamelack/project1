#include <sys/types.h>
#include <signal.h>
#include "include/utility.h"
#include <stdio.h>
#include <stdint.h> 
#define WORD_LEN 128


// Helper function to see the status of a pid and see if its running
int casecheck(pid_t pid) {
    char ans[255];
    char path[255];
    snprintf(path, sizeof(path), "/proc/%d/status", pid);
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }
    while (fgets(ans, sizeof(ans), file) != NULL) {
        if (strncmp(ans, "State:", 6) == 0) {
            fclose(file);
            if (ans[7] == 'R' || ans[7] == 'r') {
                return 1;
            } else {
                return 0;
            }
        }
    }

    fclose(file);
    return 0;  // If state not found, consider it as not running
}

int main(int argc, char *argv[]) {

    write_filepath_to_submissions("test_solutions", "submission.txt");
    FILE *file = fopen("submission.txt", "r");
      if(!file){
        return -1;
    }

    int total_executables = 0;
    char buff[128];
    // Getting how many executables will be run
    while(fgets(buff, sizeof(buff), file) != NULL){
        total_executables++;
    }
    rewind(file);

    char scores[total_executables][WORD_LEN];
    char students[total_executables][WORD_LEN];
    int batch_size = atoi(argv[1]);
    int iter = 0;

    // Copying the executables strings into a array for the path for exec and
    // storing a seperate one stripped of the beginning path for txt output
    while(fgets(buff, sizeof(buff), file) != NULL){
        buff[strcspn(buff, "\n")] = 0;
        strcpy(students[iter], buff);
        if (strlen(buff) > 15) {
            strcpy(scores[iter], buff + 15);
        }
        iter++;
    }
    int print_count = 0;
    struct timeval start;
    int timer[total_executables];
    pid_t pids[batch_size];
    int count = 0;
    // Iterating through arguments for exec
    for(int i = 2; i < argc; i++){
        int done_executables = 0;
        count = 0;
        print_count = 0;

        // Waiting for all the executables to be finished
        while (done_executables < total_executables){
            // iterating through the list of executables and forking in batch size increments 
            for(int j = 0; j < batch_size; j++){
                strcat(scores[count], " ");
                strcat(scores[count], argv[i]);
                start_timer(&start);
                pid_t pid = fork();
                    if (pid < 0) {
                        perror("fork failed");
                        return 1;
                    }
                    else if(pid == 0){
                        execl(students[count], argv[1], argv[i], NULL);
                        perror("exec failed\n");
                        exit(1);
                    }
                    else {
                        pids[j] = pid;
                    }       
                count++;
            }
            int finished = 0;
            // waiting for the batch of executables to be finished before moving on to the next batch
            while(finished < batch_size){
                sleep(1);
                // Iterating through the baches 
                for(int k = 0; k < batch_size; k++){
                    int status = 0;
                    int pidStatus = waitpid(pids[k], &status, WNOHANG);
                    if(pidStatus == -1){
                        printf("error in process\n");
                        pids[0] = 0;
        
                    }
                    // if pidstatus is greater than 0 it means child terminated 
                    if (pidStatus > 0){//BRANCH GOOD
                    // if what WIFEXITED returns is > 0 it means the child succesfully terminated
                        if(WIFEXITED(status) > 0){ 
                            //checking the return status of the succesfully terminated child
                            int ret = WEXITSTATUS(status);
                            if(ret == 0){ // CORRECT
                                printf("pid: %d %s", pids[k], "correct \n\n");
                                strcat(scores[print_count], " (correct)");
                                sleep(1);
                                pids[k] = 0;
                                finished++;
                                print_count++;
                            }
                            else if(ret == 1){ //INCORRECT
                                printf("pid: %d %s", pids[k] , "incorrect\n\n");
                                strcat(scores[print_count], " (incorrect)");
                                sleep(1);
                                finished++; 
                                print_count++;
                                pids[k] = 0;
                            }
                            
                        }
                        // Checking to see if it terminated because it raised a flag
                        else if (WIFSIGNALED(status) > 0) { //SEGFAULT GOOD
                                printf("Pid %d incorrect seg fault\n\n", pids[k]);
                                strcat(scores[print_count], " (incorrect seg fault)");
                                finished++; 
                                print_count++;
                                pids[k] = 0;
                        }
                    }
                    // If its still waiting for it to finish processing and state is not running
                    // we wait 10 seconds and get it's status if it finished it means its a slow process but if it 
                    // dosn't then we know it's blocked 
                    else if (pidStatus == 0 && casecheck(pids[k]) == 0){ //STUCK BLOCKED 
                        sleep(10);
                        int status2 = 0;
                        int pidStatus2 = waitpid(pids[k], &status2, WNOHANG);
                        if (pidStatus2 > 0){
                            printf("Pid %d slow\n\n", pids[k]);
                            strcat(scores[print_count], " (slow)");
                            finished++;
                            print_count++;

                        }
                        else {
                            printf("Pid %d Stuck/blocked\n\n", pids[k]);
                            strcat(scores[print_count], " (Stuck/blocked)");
                            finished++;
                            print_count++;

                        }
                        
                    }
                    // If the process has not finished and the pid state is running it's infinite
                    else if (pidStatus == 0 && casecheck(pids[k]) == 1){//INFINITE 
                        printf("Pid %d Infinite loop\n\n", pids[k]);
                        strcat(scores[print_count], " (Infinite loop)");
                        kill(pids[k], SIGKILL);
                        finished++;
                        print_count++;

                    }
                    if (pidStatus == 0){
                        sleep(1);
                    }
                }
                sleep(1);
            }
            done_executables += batch_size;
        }
    }

    // Writting our results into a file
    FILE* out_file = fopen("new_print.txt", "w");
    for (int i = 0; i < total_executables; i++){
        if (strlen(scores[i]) > 0) {
            fprintf(out_file, "%s\n",scores[i]);
        }
    } 
    fclose(out_file);
    fclose(file);
}

