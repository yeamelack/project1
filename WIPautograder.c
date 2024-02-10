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
    pid_t pids[numStudents];
    int finishedpid = 0;
    int i, j;
    for(i=2; i < argc; i++){//running through arguments 
        int finishedpid = 0;  
        for(j=0 ; j < numStudents; j++){    
            int pid = fork();
            if(pid > 0){
                pids[j] = pid;
                }
                else if(pid == 0){
                    //printf("%d%s", pids[j],"\n\n");
                    execl(students[j], argv[1], argv[i], NULL);
                    perror("exec failed");
                    exit(1);
                }
                else {
                    fprintf(stderr, "Fork failed\n");
                    return 1;
                }
                printf("\n%d\n", finishedpid);
                printf("%d\n", finishedpid);
                while(finishedpid <= numStudents){

                    pids[j] = getpid();
                    //printf("%d%s",pids[j] ,"\n\n");
                    
                    int status = 0;
                    //printf("%s%d", "\nhi", pids[j]);

                    int finished = waitpid(-1, &status, WNOHANG);
                    //printf("%d", finished);
                    if(finished == -1){
                        printf("%s", "lala");
                        kill(pids[j], SIGKILL);
                        finishedpid++;
                       
                    }
                    
                    if(finished > 0){
                        if(WIFEXITED(status)){
                            int ret = WEXITSTATUS(status);
                            if(ret == 0){
                            
                                printf("%s", "correct \n");
                                printf("%s\n", argv[i]);
                                finishedpid++;
                                break;
                            }
                            else if(ret == 1){
                                printf("%s", "incorrect\n");
                                finishedpid++; 
                                printf("%s\n", argv[i]);
                                 
                            }
                            else if(ret == 3){
                                printf("%s", "seg fault\n");
                                finishedpid++;
                                printf("%s\n", argv[i]);
                            }
                            else if(ret == 6){
                                printf("%s", "seg fault\n");
                                finishedpid++;
                                printf("%s\n", argv[i]);
                            }
                        }
                    }
                }
            }
            
        }
        
        return 0;
    }
            
