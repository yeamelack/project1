#include "include/utility.h"
#include <stdio.h>
#include <stdint.h> 
#define WORD_LEN 128


int main(int argc, char *argv[]) {

    write_filepath_to_submissions("/home/erist003/4061/p1/test", "/home/erist003/4061/p1/submission.txt");
    FILE *file = fopen("submission.txt", "r");
      if(!file){
        return -1;
    }

    pid_t pids[argc];
    struct timeval timer[argc];
    char buff[128];
    int numStudents = atoi(argv[1]);
    char students[numStudents][WORD_LEN];
    int finishedpid = 0;
    int count = 0;
    int index = 0;
    
    

    for(int i=0; i < 3; i++){
        for(int j=0; j<atoi(argv[1]); j++){
            pid_t pid = fork();
            if(pid>0){
                    pid_t terminated_pid = wait(NULL);
                }
                else if(pid == 0){
                    pids[j] = getpid();
                    printf("%s%d", "\n", pids[j]);
                    execl(students[i], argv[1], argv[j+2], NULL);
                }
                while(finishedpid < numStudents){
                    //printf("%d", finishedpid);
                    int status = 0;
                    //printf("%s%ld", "\n", );
                    int finished = waitpid(pids[j], &status, WNOHANG);
                    if(WIFEXITED(status)){
                        int ret = WEXITSTATUS(status);
                        if(ret == 0){
                            printf("%s", "correct \n");
                            finishedpid++;
                            count++;
                            index++;
                            //printf("%d%s", ret, "\n");
                        }
                        else if(ret == 1){
                            printf("%s", "incorrect\n");
                            finishedpid++;
                            index++;
                        }
                        else if(ret == 3){
                            printf("%s", "seg fault\n");
                            finishedpid++;
                            index++;
                        }
                        else{
                            break;
                        }
                    }
                    
                }
            }
            finishedpid = 0;
            if(i<3){
                memset(pids, 0, sizeof(pids)); 
                printf("%s%d%s","\nnew\n", pids[1], "\n");

                break;
            }

        }
    

    fclose(file);
    return 0;
}  
