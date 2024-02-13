#include <sys/types.h>
#include <signal.h>
#include "include/utility.h"
#include <stdio.h>
#include <stdint.h> 
#define WORD_LEN 128


int main(int argc, char *argv[]) {
        int casecheck(pid_t pid){
            char ans[255];
            char path[255];
            
            snprintf(path, sizeof(path), "/proc/%d/status", pid);
            FILE *file = fopen(path, "r");
    
            if (file == NULL) {
                perror("Error opening file");
                return -1;
            }
    
            fgets(ans, sizeof(ans), file);
            fclose(file);
    
            return strcmp(ans[1], "r");
    }
    write_filepath_to_submissions("/home/lexxx668/4061/p1/test", "/home/lexxx668/4061/p1/submission.txt");
    FILE *file = fopen("submission.txt", "r");
      if(!file){
        return -1;
    }
    int k = 0;
    struct timeval timer[argc];
    char buff[128];
    int numStudents = atoi(argv[1]);
    char scores[numStudents][WORD_LEN];
    char students[numStudents][WORD_LEN];
    int count = 0;
    int inc = 0;
    pid_t pids[numStudents];
    int finishedpid = 0;
    int batch = 1;
    int i, j;
    
    while(fgets(buff, sizeof(buff), file) != NULL){
        buff[strcspn(buff, "\n")] = 0;
        strcpy(scores[k], buff);
        strcpy(students[k], buff);
        k++;
    }
    fclose(file);
    for(i = 2; i < argc; i++){//running through arguments 

        int finishedpid = 0;  
        pid_t pid;    
        printf("\nbatch %d\n", batch);
        inc = 0;
        for (j = 0; j < numStudents; j++){
            strcat(scores[j], " ");
            strcat(scores[j], argv[i]);

            pid = fork();
            if (pid < 0) {
                fprintf(stderr, "Fork failed\n");
                return 1;
            }
            else if(pid == 0){
                // printf("student: %s, answer%s, pid: %d\n", students[j], argv[i], getpid());
                execl(students[j], argv[1], argv[i], NULL);
                perror("exec failed\n");
                exit(1);
            }
            else {
                pids[inc] = pid;
                printf("%s %d %d\n", "child ", inc, pids[inc]);
                inc++;
            }           
        }
        printf("\n");
        inc = 0;
        int done = 0;
        int i = 0;
        int iter = 0;
        while(finishedpid < numStudents){
            sleep(1);        
            while (done == 0){
                // printf("\niter is; %d", iter);
                int status = 0;
                // printf("\npid getting checked %d\n", pids[i]);
                int pidStatus = waitpid(pids[i], &status, WNOHANG);
                printf("pid status %d\n", pidStatus);
                if (pidStatus == 0){
                    printf("no child proccess has complete\n\n");
                    sleep(1);
                    iter++;
                }
                if(pidStatus == -1){
                    printf("error in process\n");
                    pids[i] = 0;
                    finishedpid++;       
                    done = 1;
                }
                else if (iter > L){
                    printf("child %d finished\n", pidStatus);
                    printf("Stuck/blocked\n");
                    strcat(scores[i], " stuck");
                    finishedpid++;
                    done = 1;
                }
                else if (pidStatus > 0 && iter == L){
                    printf("child %d finished\n", pidStatus);
                    printf("Infinite loop\n");
                    strcat(scores[i], " infinite");
                    // kill(getpid(), SIGKILL);
                    finishedpid++;
                    done = 1;
                }
                else if (pidStatus > 0 && iter > S){
                    printf("child %d finished\n", pidStatus);
                    printf("slow process\n");
                    strcat(scores[i], " slow process");
                    finishedpid++;
                    done = 1;
                }
                // if pidstatus is greater than 0 it means child terminated 
                else if (pidStatus > 0){
                    // if what WIFEXITED returns is > 0 it means the child succesfully terminated
                    if(WIFEXITED(status) > 0){
                        //checking the return status of the succesfully terminated child
                        int ret = WEXITSTATUS(status);
                        if(ret == 0){
                            printf("%s", "correct \n");
                            strcat(scores[i], " correct");
                            pids[i] = 0;
                            finishedpid++;
                            done = 1;
                        }
                        else if(ret == 1){
                            printf("%s", "incorrect\n");
                            strcat(scores[i], " incorrect");
                            finishedpid++; 
                            pids[i] = 0;
                            done = 1;
                        }
                    }
                    else if (WIFSIGNALED(status) > 0) {
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
            printf("dead kids: %d num of kids %d\n\n", finishedpid, numStudents);
            i++;
            sleep(1);
        }
        batch++;
        inc = 0;    
    } 
    for (i = 0; i < numStudents; i++){
        if (strlen(scores[i]) > 0) {
            printf("\n%s\n",scores[i]);
        }

    }       
    return 0;
 }        

            
