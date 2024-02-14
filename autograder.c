#include <sys/types.h>
#include <signal.h>
#include "include/utility.h"
#include <stdio.h>
#include <stdint.h> 
#define WORD_LEN 128

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
            printf("State: %s\n", ans);
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

    int k = 0;
    int numStudents = atoi(argv[1]);
    char scores[numStudents][WORD_LEN];
    char students[numStudents][WORD_LEN];
    int count = 0;
    int inc = 0;
    pid_t pids[numStudents];
    int finishedpid = 0;
    int batch = 1;
    int i, j;
    
    
    char buff[128];
    while(fgets(buff, sizeof(buff), file) != NULL){
        buff[strcspn(buff, "\n")] = 0;
        strcpy(scores[k], buff);
        strcpy(students[k], buff);
        k++;
    }
    fclose(file);





    for(i = 2; i < argc; i++){//running through parameters
        int finishedpid = 0;  
        pid_t pid;    
        // printf("\nbatch %d\n", batch);
        inc = 0;
        for (j = 0; j < atoi(argv[1]); j++){// RUNNING THROUGH EXECUTABLES
            // strcat(scores[j], " ");
            // strcat(scores[j], argv[i]);
            pid = fork();
            if (pid < 0) {
                fprintf(stderr, "Fork failed\n");
                return 1;
            }
            else if(pid == 0){
                // printf("student: %s, answer%s, pid: %d\n", students[j], argv[i], getpid());
                printf("get pid %d\n", getpid());

                execl(students[j], argv[1], argv[i], NULL);
                perror("exec failed\n");
                exit(1);
            }
            else {
                pids[inc] = pid;
                // printf("%s %d %d\n", "child ", inc, pids[inc]);
                inc++;
            }           
        }
        printf("\n");
        inc = 0;
        int done = 0;
        int i = 0;
        int iter = 0;


        while(finishedpid < atoi(argv[1])){
            sleep(1);        
            while (done == 0){
                // printf("\niter is; %d", iter);
                int status = 0;
                printf("\npid getting checked %d\n", pids[i]);
                int pidStatus = waitpid(pids[i], &status, WNOHANG);
                printf("pid status %d\n", pidStatus);
                printf("Exit stat %d\n", WIFEXITED(status));
                printf("iter is %d\n", iter);
                if(pidStatus == -1){ //ERROR 
                    printf("error in process\n");
                    pids[i] = 0;
                    finishedpid++;       
                    // done = 1;
                }
                //casecheck(pids[i]) == -1
                else if (pidStatus == 0 && casecheck(pids[i]) == 0){ //STUCK BLOCKED NEEDS CHECK
                    printf("child %d finished\n", pidStatus);
                    printf("Stuck/blocked\n\n");
                    strcat(scores[i], " stuck");
                    finishedpid++;
                    done = 1;
                }
                //!(WIFSIGNALED(status) > 0)   
                else if (pidStatus == 0 && casecheck(pids[i]) == 1){//INFINITE NEEDS CHECK
                    // printf("get pid %d\n", getpid());
                    printf("child %d finished\n", pidStatus);
                    printf("Infinite loop\n");
                    strcat(scores[i], " infinite");
                    kill(pids[i], SIGKILL);
                    iter = 0;
                    finishedpid++;
                    done = 1;
                }
                if (pidStatus == 0){ // NO CHILD HAS COMPLETE
                    printf("no child proccess has complete\n\n");
                    printf("iter %d", iter);
                    sleep(1);
                    iter++;
                }
                //SLOW PROCESS NEEDS WORK         *WIFEXITED returns 0 bad termination 0 > for normal
                else if (pidStatus > 0 && iter > S && casecheck(pids[i] == 1) && WIFEXITED(status) > 0){ //&& WIFEXITED(status) > 0)
                    printf("child %d finished\n", pidStatus);
                    printf("slow process\n");
                    strcat(scores[i], " slow process");
                    finishedpid++;
                    done = 1;
                }
                // if pidstatus is greater than 0 it means child terminated 
                else if (pidStatus > 0){//BRANCH GOOD
                    // if what WIFEXITED returns is > 0 it means the child succesfully terminated
                    if(WIFEXITED(status) > 0){ 
                        //checking the return status of the succesfully terminated child
                        int ret = WEXITSTATUS(status);
                        if(ret == 0){ // CORRECT
                            printf("%s", "correct \n");
                            strcat(scores[i], " correct");
                            pids[i] = 0;
                            finishedpid++;
                            done = 1;
                        }
                        else if(ret == 1){ //INCORRECT
                            printf("%s", "incorrect\n");
                            strcat(scores[i], " incorrect");
                            finishedpid++; 
                            pids[i] = 0;
                            done = 1;
                        }
                    }
                    else if (WIFSIGNALED(status) > 0) { //SEGFAULT GOOD
                        printf("%s", "incorrect seg fault\n");
                        strcat(scores[i], " seg fault");
                        finishedpid++; 
                        pids[i] = 0;
                        done = 1;
                    }
                }
            }
            // printf("\ni is; %d\n", i);
            done = 0;
            printf("\ndead kids: %d num of kids %d\n\n", finishedpid, atoi(argv[1]));
            i++;
            sleep(1);
        }
        batch++;
        inc = 0;    
    } 
    // for (i = 0; i < numStudents; i++){
    //     if (strlen(scores[i]) > 0) {
    //         printf("\n%s\n",scores[i]);
    //     }
    // }       
    return 0;
 }        

            
