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

    int k = 0;
    struct timeval timer[argc];
    char buff[128];
    int numStudents = atoi(argv[1]);
    char students[numStudents][WORD_LEN];
    
    while(fgets(buff, sizeof(buff), file) != NULL){
        buff[strcspn(buff, "\n")] = 0;
        strcpy(students[k], buff);
        k++;
    }
    fclose(file);
    pid_t terminated_pid;

    pid_t pids[argc];
   
    int finishedpid = 0;

    for(int i=2; i < argc; i++){
        for(int j=0 ; j < numStudents; j++){
            pid_t pid = fork();
            if(pid > 0){
                    pid_t terminated_pid = wait(NULL);
                }
                else if(pid == 0){
                    //printf("%d", pids[j]);
                    pids[j] = getpid();
                    //printf("%s%s%s", "\nmake", students[j], "\n");
                    execl(students[i], argv[1], argv[j+2], NULL);
                    
                }

                while(finishedpid < numStudents){
                    break;
                    int status = 0;
                    //printf("%s%d", "\n", pids[j]);
                    int finished = waitpid(pids[j], &status, WNOHANG);
                    printf("%s%d", "\n", finished);

                    if(finished > 0){
                        if(WIFEXITED(status)){
                            int ret = WEXITSTATUS(status);
                            if(ret == 0){
                                printf("%s", "correct \n");
                                finishedpid++;
                            }
                            else if(ret == 1){
                                printf("%s", "incorrect\n");
                                finishedpid++;   
                            }
                            else if(ret == 3){
                                printf("%s", "seg fault\n");
                                finishedpid++;
                            }
                            else{
                                break;
                            }
                        }
                    }
                }
            }
         break;
        }
    

    return 0;
}  
