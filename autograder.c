#include <sys/types.h>
#include <signal.h>
#include "include/utility.h"
#include <stdio.h>
#include <stdint.h> 
#define WORD_LEN 128


int main(int argc, char *argv[]) {

    write_filepath_to_submissions("/home/lexxx668/4061/p1/test", "/home/lexxx668/4061/p1/submission.txt");
    FILE *file = fopen("submission.txt", "r");
      if(!file){
        return -1;
    }

    int k = 0;
    struct timeval timer[argc];
    char buff[128];
    int numStudents = atoi(argv[1]);
    char students[numStudents][WORD_LEN];
    int count = 0;
    int inc = 0;
    
    while(fgets(buff, sizeof(buff), file) != NULL){
        buff[strcspn(buff, "\n")] = 0;
        strcpy(students[k], buff);
        k++;
    }
    fclose(file);
    
    pid_t pids[numStudents];
    int finishedpid = 0;
    int batch = 1;
    int i, j;

    for(i=2; i < argc; i++){//running through arguments 
        int finishedpid = 0;  
        pid_t pid;
    
        printf("\nbatch %d\n", batch);
        inc = 0;
        for(j=0 ; j < numStudents; j++){    
            pid = fork();
            if (pid < 0) {
                fprintf(stderr, "Fork failed\n");
                return 1;
            }
            else if(pid == 0){
                printf("student: %s, answer%s, pid: %d\n", students[j], argv[i], getpid());
                execl(students[j], argv[1], argv[i], NULL);
                perror("exec failed\n");
                exit(1);
            }
            else {
                pids[inc] = pid;
                printf("%s %d %d\n", "child ", inc, pids[inc]);
                // sleep(1);
                inc++;
            }
            
        }
        inc = 0;
        while(finishedpid < numStudents){
            int status = 0;
            int pidStatus = waitpid(-1, &status, WNOHANG);
            printf("\npid status %d\n", pidStatus);
            if (pidStatus == 0){
                printf("child processes still running\n");
            }
            if(pidStatus == -1){
                printf("\n%d\n", 9090);
                pids[0] = 0;
                finishedpid++;             
            }

            if(pidStatus > 0){
                printf("child %d finished\n", pidStatus);
                if(WIFEXITED(status) > 0){
                    int ret = WEXITSTATUS(status);
                    printf("return status %d\n", ret);
                    if(ret == 0){
                        printf("%s", "correct \n");
                        pids[0] = 0;
                        finishedpid++;
                        
                    }
                    else if(ret == 1){
                        printf("%s", "incorrect\n");
                        finishedpid++; 
                        pids[0] = 0;
                        
                    }
                }
                else {
                    int ret = WEXITSTATUS(status);
                    printf("return status %d\n", ret);
                    if (ret == 0){
                        printf("%s", "incorrect seg fault\n");
                        finishedpid++; 
                        pids[0] = 0;
                    }
                }
            }
            printf("dead kids: %d num of kids %d\n\n", finishedpid, numStudents);
            sleep(1);
        }
        batch++;
        inc = 0;    
    }        
    return 0;
}
            
